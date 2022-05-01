#pragma once

#include <cstdint>
#include <array>
#include <cassert>
#include <Windows.h>
#include <string>

namespace simp {

#pragma pack(push, 1)
  struct PluginHostMessage {
    enum MessageCode : uint8_t {
      None = 0,
      Plugin = 16,
      Host = 32,
      Error = 64,
      PluginStart = Plugin | 1,
      PluginFinalize,
      PluginAccessVariable,
      PluginAccessSystemVariable,
      PluginAccessStringVariable,
      PluginAccessTrigger,
      HostShutdown = Host | 1,
      HostLoadPlugin,
      ErrorUnknownCommand = Error | 1,
      ErrorInvalidIndex,
      ErrorLoadLibraryFailed,
      ErrorMissingSymbol
    };

    MessageCode Code = None;
    uint32_t PluginIndex = 0;
    uint16_t VarIndex = 0;
    uint8_t WriteFlag = 0;
    float Value = 0.f;
    PluginHostMessage() = default;
    PluginHostMessage(
      MessageCode _code) 
      : Code(_code) 
    {};
    PluginHostMessage(
      MessageCode _code,
      uint32_t _pluginIndex) 
      : Code(_code),
      PluginIndex(_pluginIndex) 
    {};
    PluginHostMessage(
      MessageCode _code,
      uint32_t _pluginIndex,
      uint32_t _varIndex,
      bool _writeFlag) 
      : Code(_code), 
      PluginIndex(_pluginIndex), 
      VarIndex(_varIndex),
      WriteFlag(_writeFlag)
    {};
    PluginHostMessage(
      MessageCode _code,
      uint32_t _pluginIndex,
      uint32_t _varIndex,
      float _value,
      bool _writeFlag)
      : Code(_code), 
      PluginIndex(_pluginIndex), 
      VarIndex(_varIndex), 
      Value(_value), 
      WriteFlag(_writeFlag) 
    {};
  };
#pragma pack(pop)

#define SPH_MESSAGE_PIPE_ID "\\\\.\\pipe\\simp_pluginhost_message_pipe"
#define SPH_STRING_PIPE_ID "\\\\.\\pipe\\simp_pluginhost_string_pipe"

  struct PluginHostInterface {

    HANDLE MessagePipeHandle;
    HANDLE StringPipeHandle;

    inline PluginHostInterface() {
#ifdef SPH_IS_PLUGIN
      MessagePipeHandle = CreateFile(
        SPH_MESSAGE_PIPE_ID,
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);
      StringPipeHandle = CreateFile(
        SPH_STRING_PIPE_ID,
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);
#else
      MessagePipeHandle =
        CreateNamedPipeA(
          SPH_MESSAGE_PIPE_ID,
          PIPE_ACCESS_DUPLEX, 
          PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
          1,
          sizeof(PluginHostMessage),
          sizeof(PluginHostMessage),
          NMPWAIT_WAIT_FOREVER,
          nullptr);
      StringPipeHandle =
        CreateNamedPipeA(
          SPH_STRING_PIPE_ID,
          PIPE_ACCESS_OUTBOUND,
          PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
          1,
          0,
          0,
          NMPWAIT_WAIT_FOREVER,
          nullptr);
#endif
    }

    inline ~PluginHostInterface() {
      CloseHandle(MessagePipeHandle);
      CloseHandle(StringPipeHandle);
    }

    inline void Post(const PluginHostMessage& msg) {
      if (!WriteFile(MessagePipeHandle, &msg, sizeof(msg), nullptr, nullptr)) {
        auto err = GetLastError();
        assert(false);
      }
    }

    inline bool Get(PluginHostMessage& msg) {
      if (!ReadFile(MessagePipeHandle, &msg, sizeof(msg), nullptr, nullptr)) {
        auto err = GetLastError();
        if (err == ERROR_BROKEN_PIPE) return false;
        assert(false);
      }
      return true;
    }
#ifdef SPH_IS_PLUGIN
    inline std::wstring GetString() {
      std::wstring result{};
      wchar_t buf[128];
      DWORD bytesRead;
      bool success = false;
      do {
        success = ReadFile(StringPipeHandle, buf, sizeof(buf), &bytesRead, nullptr);
        if (!success && GetLastError() != ERROR_MORE_DATA) {
          assert(false);
        }
        result.append(buf, bytesRead / sizeof(buf[0]));
      } while (!success);
      return result;
    }
#endif

#ifndef SPH_IS_PLUGIN
    inline void PostString(const std::wstring& wstr) {
      if (!WriteFile(StringPipeHandle, wstr.data(), wstr.size() * sizeof(wstr[0]), nullptr, nullptr)) {
        auto err = GetLastError();
        assert(false);
      }
    }

    inline void Shutdown() {
      Post({ PluginHostMessage::HostShutdown });
    }

    inline void Connect() {
      ConnectNamedPipe(MessagePipeHandle, nullptr);
      ConnectNamedPipe(StringPipeHandle, nullptr);
    }
#endif
  };

}
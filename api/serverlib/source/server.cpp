#include "pluginhost/server.hpp"
#include "pluginhost/interface.hpp"

#include <fmt/format.h>
#include <cassert>
#include <Windows.h>

namespace simp {

  PluginServer::PluginServer(
    const std::filesystem::path& _exec, 
    const std::filesystem::path& _workDir)
    : Interface(std::make_shared<PluginHostInterface>())
  {
    auto execString = _exec.generic_string();
    auto wdString = _workDir.generic_string();
    STARTUPINFO si{};
    PROCESS_INFORMATION pi{};
    CreateProcessA(
      execString.c_str(), 
      nullptr, 
      nullptr, 
      nullptr, 
      false, 
      0, 
      nullptr,
      wdString.c_str(), 
      &si, 
      &pi);

    ProcHandle = pi.hProcess;
    ThreadHandle = pi.hThread;

    Interface->Connect();
  }

  PluginServer::~PluginServer()
  {
    Interface->Shutdown();
    WaitForSingleObject(ProcHandle, INFINITE);
    CloseHandle(ProcHandle);
    CloseHandle(ThreadHandle);
  }

  uint32_t PluginServer::LoadPlugin(
    const std::filesystem::path& path,
    ErrorCallback callback)
  {
    Interface->Post(PluginHostMessage::HostLoadPlugin);
    Interface->PostString(path.generic_wstring());
    PluginHostMessage result;
    if (!Interface->Get(result)) assert(false);
    if (result.Code & PluginHostMessage::Error) {
      switch (result.Code) {
      case PluginHostMessage::ErrorLoadLibraryFailed:
        callback(fmt::format("Failed to load plugin `{}`", path.generic_string()));
        break;
      default:
        assert(false);
      }
      return -1ul;
    }
    else {
      return result.PluginIndex;
    }
  }

  void PluginServer::PluginStart(
    uint32_t index, 
    ErrorCallback callback)
  {
    Interface->Post({ PluginHostMessage::PluginStart, index });
    PluginHostMessage result;
    if (!Interface->Get(result)) assert(false);
    if (result.Code & PluginHostMessage::Error) {
      switch (result.Code) {
      case PluginHostMessage::ErrorInvalidIndex:
        callback(fmt::format("Invalid plugin index `{}`", index));
        break;
      case PluginHostMessage::ErrorMissingSymbol:
        callback(fmt::format("Plugin loaded at `{}` does not expose symbol `PluginStart`", index));
        break;
      default:
        assert(false);
      }
    }
  }

  void PluginServer::PluginFinalize(
    uint32_t index, 
    ErrorCallback callback)
  {
    Interface->Post({ PluginHostMessage::PluginFinalize, index });
    PluginHostMessage result;
    if (!Interface->Get(result)) assert(false);
    if (result.Code & PluginHostMessage::Error) {
      switch (result.Code) {
      case PluginHostMessage::ErrorInvalidIndex:
        callback(fmt::format("Invalid plugin index `{}`", index));
        break;
      case PluginHostMessage::ErrorMissingSymbol:
        callback(fmt::format("Plugin loaded at `{}` does not expose symbol `PluginFinalize`", index));
        break;
      default:
        assert(false);
      }
    }
  }

  void PluginServer::AccessVariable(
    uint32_t index, 
    uint16_t varIndex, 
    float& value,
    ErrorCallback callback)
  {
    Interface->Post({ PluginHostMessage::PluginAccessVariable, index, varIndex, value, false });
    PluginHostMessage result;
    if (!Interface->Get(result)) assert(false);
    if (result.Code & PluginHostMessage::Error) {
      switch (result.Code) {
      case PluginHostMessage::ErrorInvalidIndex:
        callback(fmt::format("Invalid plugin index `{}`", index));
        break;
      case PluginHostMessage::ErrorMissingSymbol:
        callback(fmt::format("Plugin loaded at `{}` does not expose symbol `AccessVariable`", index));
        break;
      default:
        assert(false);
      }
    }
    if (result.WriteFlag) value = result.Value;
  }

  void PluginServer::AccessSystemVariable(
    uint32_t index, 
    uint16_t varIndex, 
    const float& value,
    ErrorCallback callback)
  {
    Interface->Post({ PluginHostMessage::PluginAccessSystemVariable, index, varIndex, value, false });
    PluginHostMessage result;
    if (!Interface->Get(result)) assert(false);
    if (result.Code & PluginHostMessage::Error) {
      switch (result.Code) {
      case PluginHostMessage::ErrorInvalidIndex:
        callback(fmt::format("Invalid plugin index `{}`", index));
        break;
      case PluginHostMessage::ErrorMissingSymbol:
        callback(fmt::format("Plugin loaded at `{}` does not expose symbol `AccessSystemVariable`", index));
        break;
      default:
        assert(false);
      }
    }
  }

  void PluginServer::AccessStringVariable(
    uint32_t index, 
    uint16_t varIndex,
    const std::wstring& value,
    ErrorCallback callback)
  {
    Interface->Post({ PluginHostMessage::PluginAccessStringVariable, index, varIndex, false });
    Interface->PostString(value);
    PluginHostMessage result;
    if (!Interface->Get(result)) assert(false);
    if (result.Code & PluginHostMessage::Error) {
      switch (result.Code) {
      case PluginHostMessage::ErrorInvalidIndex:
        callback(fmt::format("Invalid plugin index `{}`", index));
        break;
      case PluginHostMessage::ErrorMissingSymbol:
        callback(fmt::format("Plugin loaded at `{}` does not expose symbol `AccessStringVariable`", index));
        break;
      default:
        assert(false);
      }
    }
  }

  void PluginServer::AccessTrigger(
    uint32_t index, 
    uint16_t varIndex, 
    bool active,
    ErrorCallback callback)
  {
    Interface->Post({ PluginHostMessage::PluginAccessTrigger, index, varIndex, active });
    PluginHostMessage result;
    if (!Interface->Get(result)) assert(false);
    if (result.Code & PluginHostMessage::Error) {
      switch (result.Code) {
      case PluginHostMessage::ErrorInvalidIndex:
        callback(fmt::format("Invalid plugin index `{}`", index));
        break;
      case PluginHostMessage::ErrorMissingSymbol:
        callback(fmt::format("Plugin loaded at `{}` does not expose symbol `AccessTrigger`", index));
        break;
      default:
        assert(false);
      }
    }
  }


}
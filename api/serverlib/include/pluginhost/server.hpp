#pragma once

#include <memory>
#include <filesystem>
#include <cstdint>

namespace simp {

  struct PluginHostInterface;

  typedef void(*ErrorCallback)(const std::string&);

  struct PluginServer {
    std::shared_ptr<PluginHostInterface> Interface;
    void* ProcHandle;
    void* ThreadHandle;
    PluginServer(
      const std::filesystem::path& _exec, 
      const std::filesystem::path& _workDir);
    ~PluginServer();
    uint32_t LoadPlugin(
      const std::filesystem::path& path, 
      ErrorCallback callback);
    void PluginStart(
      uint32_t index, 
      ErrorCallback callback);
    void PluginFinalize(
      uint32_t index, 
      ErrorCallback callback);
    void AccessVariable(
      uint32_t index, 
      uint16_t varIndex, 
      float& value,
      ErrorCallback callback);
    void AccessSystemVariable(
      uint32_t index, 
      uint16_t varIndex, 
      const float& value,
      ErrorCallback callback);
    void AccessStringVariable(
      uint32_t index, 
      uint16_t varIndex, 
      const std::wstring& value,
      ErrorCallback callback);
    void AccessTrigger(
      uint32_t index,
      uint16_t varIndex, 
      bool active,
      ErrorCallback callback);
  };

}
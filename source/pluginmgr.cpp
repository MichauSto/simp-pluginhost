#include "plugin.hpp"

namespace simp {

  PluginHostMessage simp::PluginMgr::LoadPlugin(const std::wstring& path)
  {
    HMODULE handle = LoadLibraryW(path.c_str());
    if (!handle) return PluginHostMessage::ErrorLoadLibraryFailed;

    auto index = Plugins.size();
    auto& plugin = Plugins.emplace_back();

    plugin.Handle = handle;
    plugin.PluginStart = (FnPluginStart)GetProcAddress(handle, "PluginStart");
    plugin.PluginFinalize = (FnPluginFinalize)GetProcAddress(handle, "PluginFinalize");
    plugin.AccessVariable = (FnAccessFloat)GetProcAddress(handle, "AccessVariable");
    plugin.AccessStringVariable = (FnAccessString)GetProcAddress(handle, "AccessStringVariable");
    plugin.AccessSystemVariable = (FnAccessFloat)GetProcAddress(handle, "AccessSystemVariable");
    plugin.AccessTrigger = (FnAccessTrigger)GetProcAddress(handle, "AccessTrigger");

    return { PluginHostMessage::None, index };
  }

  PluginHostMessage PluginMgr::PluginStart(uint32_t index)
  {
    if (index >= Plugins.size()) return PluginHostMessage::ErrorInvalidIndex;
    const auto& plugin = Plugins[index];
    if (!plugin.PluginStart) return PluginHostMessage::ErrorMissingSymbol;
    plugin.PluginStart(nullptr);
    return {};
  }

  PluginHostMessage PluginMgr::PluginFinalize(uint32_t index)
  {
    if (index >= Plugins.size()) return PluginHostMessage::ErrorInvalidIndex;
    const auto& plugin = Plugins[index];
    if (!plugin.PluginFinalize) return PluginHostMessage::ErrorMissingSymbol;
    plugin.PluginFinalize();
    return {};
  }

  PluginHostMessage PluginMgr::PluginAccessVariable(uint32_t index, uint16_t varIndex, float value)
  {
    bool write = false;
    if (index >= Plugins.size()) return PluginHostMessage::ErrorInvalidIndex;
    const auto& plugin = Plugins[index];
    if (!plugin.AccessVariable) return PluginHostMessage::ErrorMissingSymbol;
    plugin.AccessVariable(varIndex, &value, &write);
    return { PluginHostMessage::None, index, varIndex, value, write };
  }

  PluginHostMessage PluginMgr::PluginAccessSystemVariable(uint32_t index, uint16_t varIndex, float value)
  {
    bool write = false;
    if (index >= Plugins.size()) return PluginHostMessage::ErrorInvalidIndex;
    const auto& plugin = Plugins[index];
    if (!plugin.AccessSystemVariable) return PluginHostMessage::ErrorMissingSymbol;
    plugin.AccessSystemVariable(varIndex, &value, &write);
    return {};
  }

  PluginHostMessage PluginMgr::PluginAccessStringVariable(uint32_t index, uint16_t varIndex, const std::wstring& value)
  {
    bool write = false;
    if (index >= Plugins.size()) return PluginHostMessage::ErrorInvalidIndex;
    const auto& plugin = Plugins[index];
    if (!plugin.AccessStringVariable) return PluginHostMessage::ErrorMissingSymbol;
    plugin.AccessStringVariable(varIndex, value.c_str(), &write);
    return {};
  }

  PluginHostMessage PluginMgr::PluginAccessTrigger(uint32_t index, uint16_t varIndex, bool active)
  {
    if (index >= Plugins.size()) return PluginHostMessage::ErrorInvalidIndex;
    const auto& plugin = Plugins[index];
    if (!plugin.AccessTrigger) return PluginHostMessage::ErrorMissingSymbol;
    plugin.AccessTrigger(varIndex, &active);
    return { PluginHostMessage::None, index, varIndex, active };
  }

}
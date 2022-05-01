#pragma once

#include "pluginhost/interface.hpp"

#include <string>
#include <vector>
#include <functional>

namespace simp {

  typedef void(__stdcall *FnPluginStart)(void*);
  typedef void(__stdcall *FnPluginFinalize)();
  typedef void(__stdcall *FnAccessFloat)(uint16_t, float*, bool*);
  typedef void(__stdcall *FnAccessString)(uint16_t, const wchar_t*, bool*);
  typedef void(__stdcall *FnAccessTrigger)(uint16_t, bool*);

  struct Plugin {
    HMODULE Handle;
    FnPluginStart PluginStart;
    FnPluginFinalize PluginFinalize;
    FnAccessFloat AccessVariable;
    FnAccessString AccessStringVariable;
    FnAccessFloat AccessSystemVariable;
    FnAccessTrigger AccessTrigger;
  };

  struct PluginMgr {
    ~PluginMgr();
    std::vector<Plugin> Plugins;

    bool GetMsg(PluginHostInterface& iface);
    PluginHostMessage LoadPlugin(const std::wstring& path);

    PluginHostMessage PluginStart(uint32_t index);
    PluginHostMessage PluginFinalize(uint32_t index);
    PluginHostMessage PluginAccessVariable(uint32_t index, uint16_t varIndex, float value);
    PluginHostMessage PluginAccessSystemVariable(uint32_t index, uint16_t varIndex, float value);
    PluginHostMessage PluginAccessStringVariable(uint32_t index, uint16_t varIndex, const std::wstring& value);
    PluginHostMessage PluginAccessTrigger(uint32_t index, uint16_t varIndex, bool active);

  };

}
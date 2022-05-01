#include "plugin.hpp"

namespace simp {

  PluginMgr::~PluginMgr()
  {
    for (const auto& plugin : Plugins) {
      FreeLibrary(plugin.Handle);
    }
  }

  bool PluginMgr::GetMsg(PluginHostInterface& iface)
  {
    PluginHostMessage msg;
    if (!iface.Get(msg)) return false;
    if (msg.Code & PluginHostMessage::Host) {
      switch (msg.Code) {
      case PluginHostMessage::HostLoadPlugin:
        iface.Post(LoadPlugin(iface.GetString()));
        return true;
      case PluginHostMessage::HostShutdown:
        return false;
      }
    }
    else if (msg.Code & PluginHostMessage::Plugin) {
      switch (msg.Code) {
      case PluginHostMessage::PluginStart:
        iface.Post(PluginStart(msg.PluginIndex));
        return true;
      case PluginHostMessage::PluginFinalize:
        iface.Post(PluginFinalize(msg.PluginIndex));
        return true;
      case PluginHostMessage::PluginAccessVariable:
        iface.Post(PluginAccessVariable(msg.PluginIndex, msg.VarIndex, msg.Value));
        return true;
      case PluginHostMessage::PluginAccessSystemVariable:
        iface.Post(PluginAccessSystemVariable(msg.PluginIndex, msg.VarIndex, msg.Value));
        return true;
      case PluginHostMessage::PluginAccessStringVariable:
        iface.Post(PluginAccessStringVariable(msg.PluginIndex, msg.VarIndex, iface.GetString()));
        return true;
      case PluginHostMessage::PluginAccessTrigger:
        iface.Post(PluginAccessTrigger(msg.PluginIndex, msg.VarIndex, msg.WriteFlag));
        return true;
      }
    }
    iface.Post(PluginHostMessage::ErrorUnknownCommand);
    return true;
  }

}
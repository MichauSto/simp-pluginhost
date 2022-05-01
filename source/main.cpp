#include "plugin.hpp"
#include <pluginhost/interface.hpp>

#include <cstdio>

int main(int argc, const char** argv) {
  using namespace simp;
  PluginHostInterface iface{};
  PluginMgr mgr{};
  while (mgr.GetMsg(iface));
  return 0;

}
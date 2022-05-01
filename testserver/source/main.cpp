#include "pluginhost/server.hpp"

#include <Windows.h>
#include <cstdio>

int main(int* argc, const char** argv) {
  using namespace simp;

  auto callback = [](const std::string& str) -> void {
    printf("Error: %s\n", str.c_str());
  };

  PluginServer server{ SPH_EXECUTABLE, SPH_OMSI_DIR };
  printf("start\n");
  auto index = server.LoadPlugin("plugins/stopiontko-plugin-pro.dll", callback);
  server.PluginStart(index, callback);

  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  LARGE_INTEGER end;
  QueryPerformanceFrequency(&frequency);

  for (int i = 0; i < 60 * 10; ++i) {
    QueryPerformanceCounter(&start);
    server.AccessSystemVariable(index, 0, 1.f / 60.f, callback);
    float value = 0.f; // i < 300 ? (i > 30 ? 1.f : 0.f) : 0.f;

    for(int i = 0; i < 20; ++i)
      server.AccessVariable(index, 0, value, callback);
    QueryPerformanceCounter(&end);
    float interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("%f\n", interval); 
    Sleep(16);
  }

  server.PluginFinalize(index, callback);

  //system("pause");
  return 0;
}
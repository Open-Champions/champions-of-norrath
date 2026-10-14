// Host check only; not a build input.
#include "detach_all_resources.cpp"
#include <string>
static std::string trace;
extern "C" void DetachSurface__14VIEffectVolumei(void* s, int slot) { trace += std::to_string(reinterpret_cast<unsigned long>(s) & 0xff) + ":" + std::to_string(slot) + ";"; }
int main() {
    alignas(256) static unsigned char o[256];
    DetachAllResources__14VIEffectVolume(o);
    return trace != "0:0;0:1;";
}

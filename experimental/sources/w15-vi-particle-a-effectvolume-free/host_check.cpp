// Host check only; not a build input.
#include "free.cpp"
#include <string>
static std::string tr;
extern "C" void DetachAllResources__14VIEffectVolume(void* s) { tr += "D" + std::to_string(reinterpret_cast<long>(s) & 0xff) + ";"; }
extern "C" void FreeParticles__14VIEffectVolume(void* s) { tr += "F" + std::to_string(reinterpret_cast<long>(s) & 0xff) + ";"; }
int main() {
    alignas(256) static unsigned char o[256];
    Free__14VIEffectVolume(o);
    return tr != "D0;F0;";
}

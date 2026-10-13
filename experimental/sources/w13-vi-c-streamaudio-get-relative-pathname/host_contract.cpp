// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "streamaudio-get-relative-pathname.cpp"

int main() {
    alignas(8) unsigned char object[160] = {0};
    if (GetRelativePathname__C19VIStreamAudioSprite(object) != reinterpret_cast<const char*>(object + 72)) return 1;
    alignas(8) unsigned char other[160] = {0};
    if (GetRelativePathname__C19VIStreamAudioSprite(other) != reinterpret_cast<const char*>(other + 72)) return 2;
    return 0;
}

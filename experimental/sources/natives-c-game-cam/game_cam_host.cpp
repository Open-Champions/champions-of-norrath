#include <cstdint>
#include <cstdio>

extern "C" void* gameCam;

int main() {
    if (gameCam != nullptr) return 1;
    gameCam = reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x20));
    if (gameCam == nullptr) return 2;
    std::printf("game cam passed\n");
    return 0;
}

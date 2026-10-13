// Host contract for msg_draw__7OniBall; not an EE input. The function must not change the object, its neighbourhood or any
// global, whatever the argument holds. The authored function was also compared with the original in a private R5900 run (four
// random objects; same return register, same memory, no calls).
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char before[256], after[256];
    for (int i = 0; i < 256; ++i) before[i] = static_cast<unsigned char>(i * 37 + 11);
    std::memcpy(after, before, sizeof before);
    msg_draw__7OniBall(after + 64);
    if (std::memcmp(before, after, sizeof before) != 0) { std::puts("fail: memory changed"); return 1; }
    msg_draw__7OniBall(nullptr);
    return 0;
}

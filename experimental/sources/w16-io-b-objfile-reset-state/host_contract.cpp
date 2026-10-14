#include "reset_state.cpp"

#include <cstdio>
#include <cstring>

int main() {
    alignas(8) unsigned char f[640];
    std::memset(f, 0x5a, sizeof f);
    if (ResetState__9VIObjFile(f) != 0) return 1;
    const int zero_words[] = {560, 564, 568, 572, 576, 580, 584, 588};
    for (int off : zero_words)
        if (*reinterpret_cast<int*>(f + off) != 0) return 2;
    if (*reinterpret_cast<int*>(f + 232) != -1) return 3;
    for (unsigned i = 0; i < sizeof f; ++i) {
        bool touched = i >= 232 && i < 236;
        for (int off : zero_words) if (static_cast<int>(i) >= off && static_cast<int>(i) < off + 4) touched = true;
        if (!touched && f[i] != 0x5a) return 4;
    }
    std::puts("objfile reset-state passed");
    return 0;
}

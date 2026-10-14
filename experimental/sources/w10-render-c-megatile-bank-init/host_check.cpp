// Host contract for megatileBankInit__FP16MegatileInstBankPv. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_CONTRACT=1 host_check.cpp megatile_bank_init.cpp
#include <cstdio>
struct MegatileInstBank { unsigned first, second; int count; };
extern "C" void megatileBankInit__FP16MegatileInstBankPv(MegatileInstBank*, void*);
static unsigned arena[128];
extern "C" unsigned* norrath_host_words(unsigned address) { return arena + address / 4; }
int main() {
    // second = 40 and base = 200, so the walk starts at byte 240 (word 60)
    unsigned init[9] = {1, 2, 3, 10, 20, 30, 100, 200, 300};
    for (int i = 0; i < 9; i++) arena[60 + i] = init[i];
    MegatileInstBank b{5, 40, 3};
    megatileBankInit__FP16MegatileInstBankPv(&b, reinterpret_cast<void*>(200));
    if (b.first != 205 || b.second != 240 || b.count != 3) return 1;
    unsigned want[9] = {201, 202, 3, 210, 220, 30, 300, 400, 300};
    for (int i = 0; i < 9; i++) if (arena[60 + i] != want[i]) return 2;
    if (arena[69] != 0 || arena[59] != 0) return 3;
    MegatileInstBank z{1, 40, 0};
    megatileBankInit__FP16MegatileInstBankPv(&z, reinterpret_cast<void*>(200));
    if (arena[60] != 201 || z.first != 201) return 4;
    MegatileInstBank neg{1, 40, -2};
    megatileBankInit__FP16MegatileInstBankPv(&neg, reinterpret_cast<void*>(200));
    if (arena[60] != 201) return 5;
    std::printf("megatileBankInit contract passed\n");
    return 0;
}

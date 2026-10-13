// Host check only; not an EE build input. Build with
//   -include host_regs.hpp -DW12_GS_DMA_BASE='(reinterpret_cast<unsigned long>(w12_regs))' -DsceGsPutDrawEnv=w12_pde
#include <cstdio>
#include <cstring>
#include <cstdint>
extern "C" { alignas(64) volatile unsigned w12_regs[16] = {}; }
static int printed; static char text[128];
extern "C" int scePrintf(const char* f, ...) { ++printed; std::strncpy(text, f, 127); return 0; }
extern "C" int w12_pde(unsigned long long*);
int main() {
    alignas(16) unsigned long long packet[2] = {0xfffffffffff08005ull, 0};
    const unsigned address = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(packet));
    w12_regs[0] = 0; w12_regs[4] = 0x55; w12_regs[8] = 0x66;
    if (w12_pde(packet) != 0) return 1;
    if (w12_regs[8] != 6) return 2;                                      // (0x8005 & 0x7fff) + 1
    const unsigned expect = (address & 0x70000000u) == 0x70000000u ? ((address & 0x0fffffffu) | 0x80000000u)
                                                                    : (address & 0x0fffffffu);
    if (w12_regs[4] != expect) return 3;
    if (w12_regs[0] != 0x101 || printed) return 4;
    packet[0] = 0x7fff;
    w12_regs[0] = 0x2ff & ~0x100u;                                      // bit 8 clear: no wait
    if (w12_pde(packet) != 0 || w12_regs[8] != 0x8000 || w12_regs[0] != 0x101) return 5;
    packet[0] = 0;
    w12_regs[0] = 0x100; w12_regs[4] = 0x77; w12_regs[8] = 0x88;       // never clears: times out
    if (w12_pde(packet) != -1) return 6;
    if (printed != 1 || std::strcmp(text, "sceGsPutDrawEnv: DMA Ch.2 does not terminate\r\n")) return 7;
    if (w12_regs[0] != 0x100 || w12_regs[4] != 0x77 || w12_regs[8] != 0x88) return 8;   // untouched
    std::puts("sceGsPutDrawEnv ok");
    return 0;
}

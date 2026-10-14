// Observed _engineCheckStack__FPCci at 0x00191310, 144 bytes.
// It reads 5120 bytes starting at 0x01fc0000, the address of _stack.
// Each byte is compared with 0xA5. A mismatch prints the text at
// 0x00464328, which has no ELF symbol, then stores zero at address 0.
// The store is not followed by a branch; the scan continues if the store
// returns. On the EE that store faults. The site and line are the two
// arguments and are not derived from the failing offset.
#include "ee_mmio.hpp"

extern "C" void scePrintf(const char* text, const char* site, int line);

extern "C" void _engineCheckStack__FPCci(const char* site, int line) {
    unsigned index = 0;
    while (index < 5120u) {
        const unsigned char byte = NORRATH_LOAD8(0x01fc0000u + index);
        index += 1u;
        if (byte != 0xa5u) {
            scePrintf("stack error!  %s:%d\n", site, line);
#if defined(NORRATH_HOST_CONTRACT)
            NORRATH_STORE32(0u, 0u);
#else
            // The observed store is sw zero, 0(zero). A C store of that
            // form is deleted as a null write under the reviewed flags and
            // does not fall through. The instruction itself falls through.
            asm volatile("sw $0, 0($0)" ::: "memory");
#endif
        }
    }
}

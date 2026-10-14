// Host check only; not an EE build input. Build the source with -Dold_find_exception_handler=w15_old_find;
// this file is compiled without it. The expected answers are worked out by hand from the stated rule.
#include <cstdio>
#include "../w15-libs-eh-a-common/eh.hpp"
using w15eh::OldHandlerEntry;
extern "C" unsigned w15_old_find(unsigned pc, OldHandlerEntry* table);
int main() {
    const unsigned end = 0xffffffffu;
    OldHandlerEntry t[] = {{0x100, 0x200, 0xa1}, {0x120, 0x180, 0xa2}, {0x130, 0x150, 0xa3}, {0x400, 0x500, 0xa4}, {end, 0, 0}, {0, 0x10000, 0xbad}};
    if (w15_old_find(0x140, t) != 0xa3) return 1;     // innermost of three nested regions
    if (w15_old_find(0x1a0, t) != 0xa1) return 2;     // inside the outer one only
    if (w15_old_find(0x125, t) != 0xa2) return 3;
    if (w15_old_find(0x200, t) != 0) return 4;        // end is exclusive
    if (w15_old_find(0x100, t) != 0xa1) return 5;     // start is inclusive
    if (w15_old_find(0x450, t) != 0xa4) return 6;
    if (w15_old_find(0x50, t) != 0) return 7;
    if (w15_old_find(0x140, nullptr) != 0) return 8;
    if (w15_old_find(0x600, t) != 0) return 9;        // nothing at or after the terminator is read
    OldHandlerEntry order[] = {{0x120, 0x180, 0xb1}, {0x100, 0x200, 0xb2}, {end, 0, 0}};
    if (w15_old_find(0x130, order) != 0xb1) return 10;   // a wider later entry does not replace the best
    OldHandlerEntry tie[] = {{0x100, 0x200, 0xc1}, {0x100, 0x200, 0xc2}, {end, 0, 0}};
    if (w15_old_find(0x130, tie) != 0xc2) return 11;     // an equal range replaces it
    OldHandlerEntry stop[] = {{0x100, 0x200, 0xd1}, {0x300, 0x310, 0xd2}, {0x110, 0x210, 0xd3}, {end, 0, 0}};
    if (w15_old_find(0x150, stop) != 0xd1) return 12;    // the scan stops at the later start, so the entry after it is not seen
    std::puts("old_find_exception_handler ok");
    return 0;
}

// Observed addpmlistptrscap__Fv at 0x00268798, 396 bytes in SLUS_205.65. pmlistptrs (0x00435b68,
// 0x400 bytes) is an array of pointer cells and the signed word pmlistptrsused (0x00485e1c) is
// how many cells are in use. The cells hold zero-terminated runs of pointers (a run is one
// piece list, ended by a zero cell).
//  1. A zero cell is stored at index `used` and `used` is incremented (it is read again
//     afterwards, in case the store aliased it).
//  2. The last run is found by scanning back from cell used-2 to the previous zero cell (or the
//     array start): `run` is the cell after that zero.
//  3. Every cell index i from 0 to used-1 is tried as the start of a match of that last run
//     against earlier cells: a running match length m compares run[m] with cell i. When run[m]
//     is zero and cell i is also zero and they are different cells, an earlier identical run
//     ends at i, and the result becomes the start of the run that ends at cell i. A mismatch
//     restarts m at 0.
//  4. When an earlier run was found, the pointer count is lowered by the length of that run plus
//     its zero cell (one decrement per cell, publishing the count after each), which drops the
//     freshly added copy. The result is the start of the run that stays in use.
// Returns that run start. Inferred: the routine de-duplicates the run just added to the
// piece lists. Unknown: who guarantees the array capacity (there is no bound check) and
// whether partial matches that straddle a zero cell are intended.
extern "C" {
typedef __UINTPTR_TYPE__ Address;
extern Address pmlistptrs[];  // pointer-sized cells; only compared with zero and with each other
extern int pmlistptrsused;

char** addpmlistptrscap__Fv() {
    const Address base = reinterpret_cast<Address>(pmlistptrs);
#define CELL(a) (*reinterpret_cast<volatile Address*>(a))

    int old_used = pmlistptrsused;
    pmlistptrsused = old_used + 1;
    CELL(base + sizeof(Address) * old_used) = 0;
    int count = pmlistptrsused;

    Address back = base + sizeof(Address) * count - 2 * sizeof(Address);
    while (back >= base) {
        if (CELL(back) == 0) break;
        back -= sizeof(Address);
    }
    const Address run = back + sizeof(Address);
    Address result = run;
    bool found_earlier = false;
    int matched = 0;
    Address earlier = base;

    for (int scanned = 0; scanned < count; scanned++, earlier += sizeof(Address)) {
        const Address probe = run + sizeof(Address) * matched;
        Address probe_value = CELL(probe);
        matched++;
        if (probe_value == 0 && CELL(earlier) == 0 && probe != earlier) {
            back = earlier - sizeof(Address);
            found_earlier = true;
            while (back >= base) {
                if (CELL(back) == 0) break;
                back -= sizeof(Address);
            }
            result = back + sizeof(Address);
            break;
        }
        if (CELL(probe) != CELL(earlier)) matched = 0;
    }

    if (found_earlier) {
        Address cursor = result;
        while (CELL(cursor) != 0) {
            count--;
            cursor += sizeof(Address);
            pmlistptrsused = count;
        }
        count--;
        pmlistptrsused = count;
    }
    return reinterpret_cast<char**>(result);
#undef CELL
}
}

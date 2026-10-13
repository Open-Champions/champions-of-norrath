// Observed P_LightningIsLive__Fi at 0x00184b78, extent 100. Argument: a bolt handle. bolt is
// the 20-record table (1844 bytes each) at 0x006f6e50; the handle's low byte selects a record
// (no range check) and the handle must equal the word at record+1840. The result is 1 only when
// the handle is nonzero, that word equals the whole handle, the signed byte at +1836 is nonzero,
// and both the words at +1824 and +1828 are nonzero; otherwise 0. (The last branch's delay slot loads
// 1, so a nonzero +1828 yields 1 and a zero one falls through to a return of 0.)
// Inferred: +1840 holds the handle given out when the bolt started, +1836 an in-use flag, and
// +1824 / +1828 the addresses of the two trail records P_StartLightning stores there. Unknown:
// whether the bolt is cleared by zeroing those words or the flag.
typedef int __attribute__((may_alias)) word;
extern "C" {
extern unsigned char bolt[20 * 1844];
int P_LightningIsLive__Fi(int handle) {
    if (handle == 0) return 0;
    const unsigned char* record = bolt + (handle & 0xff) * 1844;
    if (*reinterpret_cast<const word*>(record + 1840) != handle) return 0;
    if (static_cast<signed char>(record[1836]) == 0) return 0;
    if (*reinterpret_cast<const word*>(record + 1824) == 0) return 0;
    if (*reinterpret_cast<const word*>(record + 1828) == 0) return 0;
    return 1;
}
}

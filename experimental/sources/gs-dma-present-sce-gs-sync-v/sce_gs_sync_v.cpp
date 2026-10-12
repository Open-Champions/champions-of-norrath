/* Observed sceGsSyncV at 0x002d1ea0, 148 bytes. sceGsGetGParam's word at
 * offset 8 selects the wait. Zero calls VSync and, when the halfword at
 * offset 0 is 1, returns bit 13 of the 64-bit word at 0x12001000. Nonzero
 * calls VSync2 and, when that halfword is 1, returns bit 13 of VSync2's
 * result. Any other halfword returns 1. The incoming mode is still in a0
 * after sceGsGetGParam because that 12-byte body does not write a0, so both
 * waits receive it. Fresh compilation is not an original-byte match.
 */
extern "C" unsigned char* sceGsGetGParam();
extern "C" int VSync(int mode);
extern "C" int VSync2(int mode);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned long long norrath_host_gs_load64(unsigned address);
#define NORRATH_LOAD64(address) norrath_host_gs_load64(address)
#else
static inline unsigned long long norrath_gs_load64(unsigned address) {
    return *reinterpret_cast<volatile unsigned long long*>(static_cast<unsigned long>(address));
}
#define NORRATH_LOAD64(address) norrath_gs_load64(address)
#endif

extern "C" int sceGsSyncV(int mode) {
    unsigned char* param = sceGsGetGParam();
    const unsigned handler = *reinterpret_cast<unsigned*>(param + 8);
    if (handler == 0u) {
        (void)VSync(mode);
        const short interlace = *reinterpret_cast<short*>(param);
        if (interlace != 1) return 1;
        const unsigned long long csr = NORRATH_LOAD64(0x12001000u);
        return static_cast<int>((csr >> 13) & 1ull);
    }
    const int field = VSync2(mode);
    const short interlace = *reinterpret_cast<short*>(param);
    if (interlace != 1) return 1;
    return static_cast<int>((static_cast<unsigned>(field) >> 13) & 1u);
}

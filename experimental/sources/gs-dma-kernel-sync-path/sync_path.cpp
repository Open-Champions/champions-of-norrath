/* Observed sceGsSyncPath at 0x002d1f38, 788 bytes. A nonzero mode returns a
 * bit mask and does not wait: bit 0 from bit 8 of 0x10009000, bit 1 from bit
 * 8 of 0x1000a000, bit 2 from 0x10003c00 masked with 0x1f000003, bit 3 from
 * bit 8 of VU0 vi29, and bit 4 from 0x10003020 masked with 0xc00. Mode 0
 * waits for those same conditions in that order. The spin count starts at 0
 * and is shared. It gives up when the count entering a step is greater than
 * 0x01000000, after 0x01000001 reads inside that step plus the read that
 * found the condition busy. The second argument is cleared before it is
 * read. A timeout prints one of the five termination lines, then the ten
 * register lines, and returns -1. A finished wait returns 0. This source is
 * not an original-byte match.
 */
extern "C" int scePrintf(const char* text, ...);

static constexpr unsigned norrath_gs_sync_path_limit = 0x01000000u;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_sync_limit_valid;
extern "C" unsigned norrath_host_sync_limit;
extern "C" unsigned norrath_host_path_word(unsigned address);
extern "C" unsigned norrath_host_path_vi29();
static unsigned norrath_limit() {
    return norrath_host_sync_limit_valid != 0u ? norrath_host_sync_limit : norrath_gs_sync_path_limit;
}
static unsigned norrath_word(unsigned address) { return norrath_host_path_word(address); }
static unsigned norrath_vi29() { return norrath_host_path_vi29(); }
#else
static unsigned norrath_limit() { return norrath_gs_sync_path_limit; }
static unsigned norrath_word(unsigned address) {
    return *reinterpret_cast<volatile unsigned*>(address);
}
static unsigned norrath_vi29() {
    unsigned value;
    asm volatile("cfc2 %0, $29" : "=r"(value));
    return value;
}
#endif

static int norrath_wait(int spins, unsigned (*read)(), unsigned mask) {
    if ((read() & mask) == 0u)
        return spins;
    unsigned snapshot = static_cast<unsigned>(spins);
    for (;;) {
        if (norrath_limit() < snapshot)
            return -1;
        spins += 1;
        if ((read() & mask) == 0u)
            return spins;
        snapshot = static_cast<unsigned>(spins);
    }
}

static unsigned norrath_read_d1() { return norrath_word(0x10009000u); }
static unsigned norrath_read_d2() { return norrath_word(0x1000a000u); }
static unsigned norrath_read_vif() { return norrath_word(0x10003c00u); }
static unsigned norrath_read_gif() { return norrath_word(0x10003020u); }

static void norrath_dump() {
    scePrintf("\t<D1_CHCR=%08x:", norrath_word(0x10009000u));
    scePrintf("D1_TADR=%08x:", norrath_word(0x10009030u));
    scePrintf("D1_MADR=%08x:", norrath_word(0x10009010u));
    scePrintf("D1_QWC=%08x>\r\n", norrath_word(0x10009020u));
    scePrintf("\t<D2_CHCR=%08x:", norrath_word(0x1000a000u));
    scePrintf("D2_TADR=%08x:", norrath_word(0x1000a030u));
    scePrintf("D2_MADR=%08x:", norrath_word(0x1000a010u));
    scePrintf("D2_QWC=%08x>\r\n", norrath_word(0x1000a020u));
    scePrintf("\t<VIF1_STAT=%08x:", norrath_word(0x10003c00u));
    scePrintf("GIF_STAT=%08x>\r\n", norrath_word(0x10003020u));
}

extern "C" int sceGsSyncPath(int mode, int timeout) {
    (void)timeout;
    if (mode != 0) {
        unsigned bits = (norrath_word(0x10009000u) & 0x100u) != 0u ? 1u : 0u;
        if ((norrath_word(0x1000a000u) & 0x100u) != 0u) bits |= 2u;
        if ((norrath_word(0x10003c00u) & 0x1f000003u) != 0u) bits |= 4u;
        if ((norrath_vi29() & 0x100u) != 0u) bits |= 8u;
        if ((norrath_word(0x10003020u) & 0xc00u) != 0u) bits |= 0x10u;
        return static_cast<int>(bits);
    }
    int spins = norrath_wait(0, norrath_read_d1, 0x100u);
    if (spins < 0) {
        scePrintf("sceGsSyncPath: DMA Ch.1 does not terminate\r\n");
        norrath_dump();
        return -1;
    }
    spins = norrath_wait(spins, norrath_read_d2, 0x100u);
    if (spins < 0) {
        scePrintf("sceGsSyncPath: DMA Ch.2 does not terminate\r\n");
        norrath_dump();
        return -1;
    }
    spins = norrath_wait(spins, norrath_read_vif, 0x1f000003u);
    if (spins < 0) {
        scePrintf("sceGsSyncPath: VIF1 does not terminate\r\n");
        norrath_dump();
        return -1;
    }
    spins = norrath_wait(spins, norrath_vi29, 0x100u);
    if (spins < 0) {
        scePrintf("sceGsSyncPath: VU1 does not terminate\r\n");
        norrath_dump();
        return -1;
    }
    spins = norrath_wait(spins, norrath_read_gif, 0xc00u);
    if (spins < 0) {
        scePrintf("sceGsSyncPath: GIF does not terminate\r\n");
        norrath_dump();
        return -1;
    }
    return 0;
}

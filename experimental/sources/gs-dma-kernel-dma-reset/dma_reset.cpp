/* Observed sceDmaReset at 0x002d2cd0, 220 bytes. It keeps bit 0 of the word
 * at 0x1000e000. For each of the ten dch entries, a nonzero isclr word clears
 * channel offsets 128, 0, 48, 16, 80, and 64. isclr is the ten words at
 * 0x0047b388: 1, 1, 1, 1, 1, 0, 0, 0, 1, 1. It then writes 0xff1f to
 * 0x1000e010, reads that word back, keeps only bits in 0xff1f0000, and
 * stores the result. Twenty stack bytes are cleared and passed to
 * sceDmaPutEnv. Mode 1 sets bit 0 of 0x1000e000. The saved bit is returned.
 */
extern "C" unsigned dch[10];
extern "C" void memclr(unsigned char* dest, int count);
extern "C" void sceDmaPutEnv(void* env);

extern "C" unsigned isclr[10] = {1u, 1u, 1u, 1u, 1u, 0u, 0u, 0u, 1u, 1u};

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned* norrath_host_dma_channel(int index);
extern "C" unsigned norrath_host_d_ctrl;
extern "C" unsigned norrath_host_d_stat;
extern "C" int norrath_host_d_stat_read_valid;
extern "C" unsigned norrath_host_d_stat_read;
static unsigned* norrath_channel(int index) { return norrath_host_dma_channel(index); }
static unsigned norrath_read_ctrl() { return norrath_host_d_ctrl; }
static void norrath_write_ctrl(unsigned value) { norrath_host_d_ctrl = value; }
static unsigned norrath_read_stat() {
    if (norrath_host_d_stat_read_valid != 0) {
        norrath_host_d_stat_read_valid = 0;
        return norrath_host_d_stat_read;
    }
    return norrath_host_d_stat;
}
static void norrath_write_stat(unsigned value) { norrath_host_d_stat = value; }
#else
static unsigned* norrath_channel(int index) {
    return reinterpret_cast<unsigned*>(static_cast<unsigned long>(dch[index]));
}
static unsigned norrath_read_ctrl() {
    return *reinterpret_cast<volatile unsigned*>(0x1000e000u);
}
static void norrath_write_ctrl(unsigned value) {
    *reinterpret_cast<volatile unsigned*>(0x1000e000u) = value;
}
static unsigned norrath_read_stat() {
    return *reinterpret_cast<volatile unsigned*>(0x1000e010u);
}
static void norrath_write_stat(unsigned value) {
    *reinterpret_cast<volatile unsigned*>(0x1000e010u) = value;
}
#endif

extern "C" int sceDmaReset(int mode) {
    const unsigned saved = norrath_read_ctrl() & 1u;
    for (int index = 0; index < 10; ++index) {
        if (isclr[index] == 0u)
            continue;
        unsigned* channel = norrath_channel(index);
        channel[32] = 0;
        channel[0] = 0;
        channel[12] = 0;
        channel[4] = 0;
        channel[20] = 0;
        channel[16] = 0;
    }
    norrath_write_stat(0xff1fu);
    norrath_write_stat(norrath_read_stat() & 0xff1f0000u);
    unsigned char env[20];
    memclr(env, 20);
    sceDmaPutEnv(env);
    if (mode == 1)
        norrath_write_ctrl(norrath_read_ctrl() | 1u);
    return static_cast<int>(saved);
}

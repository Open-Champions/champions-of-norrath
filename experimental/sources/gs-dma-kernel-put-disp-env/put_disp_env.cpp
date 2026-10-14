/* Observed sceGsPutDispEnv at 0x002d1d18, 188 bytes. It reads gp.15 offset 6.
 * When that halfword is 1, the environment qwords at offsets 0, 16, 24, and
 * 32 are stored to 0x12000000, 0x12000070, 0x12000080, and 0x120000c0.
 * Otherwise offsets 0, 8, 16, 24, and 32 go to 0x12000000, 0x12000020,
 * 0x12000090, 0x120000a0, and 0x120000e0. This source is not an original-byte
 * match. Which GS registers those addresses are is inferred from the public
 * GS map and is not established by the instruction bytes alone.
 */
extern "C" unsigned char* sceGsGetGParam();

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_gs_store(unsigned address, unsigned long long value);
static void norrath_gs_store(unsigned address, unsigned long long value) {
    norrath_host_gs_store(address, value);
}
#else
static void norrath_gs_store(unsigned address, unsigned long long value) {
    *reinterpret_cast<volatile unsigned long long*>(address) = value;
}
#endif

static short norrath_half_at(const unsigned char* param, int offset) {
    return static_cast<short>(param[offset] | (param[offset + 1] << 8));
}

static unsigned long long norrath_load64(const unsigned char* env, int offset) {
    unsigned long long value = 0;
    for (int byte = 0; byte < 8; ++byte)
        value |= static_cast<unsigned long long>(env[offset + byte]) << (8 * byte);
    return value;
}

extern "C" void sceGsPutDispEnv(unsigned char* env) {
    const short kind = norrath_half_at(sceGsGetGParam(), 6);
    if (kind == 1) {
        norrath_gs_store(0x12000000u, norrath_load64(env, 0));
        norrath_gs_store(0x12000070u, norrath_load64(env, 16));
        norrath_gs_store(0x12000080u, norrath_load64(env, 24));
        norrath_gs_store(0x120000c0u, norrath_load64(env, 32));
        return;
    }
    norrath_gs_store(0x12000000u, norrath_load64(env, 0));
    norrath_gs_store(0x12000020u, norrath_load64(env, 8));
    norrath_gs_store(0x12000090u, norrath_load64(env, 16));
    norrath_gs_store(0x120000a0u, norrath_load64(env, 24));
    norrath_gs_store(0x120000e0u, norrath_load64(env, 32));
}

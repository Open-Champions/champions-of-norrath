/* soundSetAudioEffect__Fiiiii at 0x0024e680 is 1060 bytes. Observed, in
 * order (sceSdRemote is called with first argument 1 every time):
 *
 *   1. sceSifCallRpc command 176 (0xb0), mode 0, the 16-byte send buffer and
 *      64-byte receive buffer, null end function and parameter.
 *   2. effectsBufferSize (0x0048599c) = effectModeSize[type] with the
 *      argument as given, before any limiting (the table, 10 words at
 *      0x0042de10, is authored in w9-audio-effect-mode-size).
 *   3. limit = 0x1fffff - effectsBufferSize. For each of the nmSounds
 *      records (16 bytes, soundList): with header = the word at +4 taken as
 *      an address, the word at +36 of the header becomes 0x5000 when it is
 *      not above 0x1fffff and the word at +8 of the record is not below
 *      limit (signed comparisons).
 *   4. A 20-byte block from memAlloc__FiPc (name ""), filled with: the type
 *      limited to 0..9 at +4, the second argument limited to -32768..32767
 *      as a halfword at +8, the third likewise at +10, the fourth limited to
 *      0..127 at +12, the fifth likewise at +16.
 *   5. sceSdRemote(1, 0x8050, 0x1d01, 0x1fffff).
 *   6. Up to two passes (pass = 0, 1) of: sceSdRemote(1, 0x8190, 1, pass,
 *      limited type), then sceSdRemote(1, 0x80f0, pass, 1). A pass fails when
 *      the first result is nonzero or the second result is not 1. The loop
 *      stops after the first pass that does not fail, or after pass 1.
 *   7. sceSdRemote(1, 0x81a0, 1, block), (1, 0x81b0, 1, block), then
 *      (1, 0x8030, c, v) for (c, v) = (0x1800, 0xffffff), (0x1a00, 0xffffff),
 *      (0x1900, 0), (0x1b00, 0), (0x1801, 0xffffff), (0x1a01, 0xffffff),
 *      (0x1901, 0x7ffff), (0x1b01, 0x7ffff); (1, 0x8010, 0x800, 0xc00),
 *      (1, 0x8010, 0x801, 0xf0f), (1, 0x8070, 3, 1).
 *   8. memFree__FPv(block), then sceSifCallRpc command 192 (0xc0), otherwise
 *      as step 1.
 *
 * Not reproduced: both RPCs copy 16 bytes of an uninitialised stack local
 * into the send buffer; this source leaves the send buffer as it is. The
 * fifth argument of the 0x8190 call is the limited type; the other calls
 * leave the fifth argument register alone. Nothing is returned. Inferred
 * only: the argument roles (effect mode, two signed depths, delay and
 * feedback) from the limits and the table of buffer sizes; the SPU2 meaning
 * of the command words is not established here.
 */
extern "C" {
extern int effectsBufferSize;
extern int nmSounds;
extern int effectModeSize[];
extern unsigned char soundList[];
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_rpc_send_view[];
extern unsigned char norrath_sound_rpc_receive_view[];
int sceSdRemote(int one, ...);
void* memAlloc__FiPc(int size, char* name);
void memFree__FPv(void* block);
int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int send_bytes,
                           void* receive, int receive_bytes, void* end_function,
                           void* end_parameter);

#if defined(NORRATH_HOST_CONTRACT)
void* norrath_host_pointer(unsigned address);
#endif
static unsigned char* pointer_at(unsigned address) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(address));
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}

static void call_command(int command) {
    norrath_sound_call_rpc(norrath_sound_rpc_client_view, command, 0, norrath_sound_rpc_send_view, 16,
                           norrath_sound_rpc_receive_view, 64, nullptr, nullptr);
}

static int limit(int value, int low, int high) {
    if (value < low) value = low;
    if (value > high) value = high;
    return value;
}

void soundSetAudioEffect__Fiiiii(int type, int a1, int a2, int a3, int a4) {
    call_command(176);
    effectsBufferSize = effectModeSize[type];
    const int low_water = 0x1fffff - effectsBufferSize;
    const int count = nmSounds;
    for (int index = 0; index < count; ++index) {
        const unsigned char* record = soundList + index * 16;
        unsigned address;
        int header_word, size;
        __builtin_memcpy(&address, record + 4, sizeof address);
        unsigned char* header = pointer_at(address);
        __builtin_memcpy(&header_word, header + 36, sizeof header_word);
        if (0x1fffff < header_word) continue;
        __builtin_memcpy(&size, record + 8, sizeof size);
        if (size < low_water) continue;
        const int fill = 0x5000;
        __builtin_memcpy(header + 36, &fill, sizeof fill);
    }

    unsigned char* block = static_cast<unsigned char*>(memAlloc__FiPc(20, const_cast<char*>("")));
    const int mode = limit(type, 0, 9);
    const short depth_a = static_cast<short>(limit(a1, -32768, 32767));
    const short depth_b = static_cast<short>(limit(a2, -32768, 32767));
    const int third = limit(a3, 0, 127);
    const int fourth = limit(a4, 0, 127);
    __builtin_memcpy(block + 10, &depth_b, sizeof depth_b);
    __builtin_memcpy(block + 8, &depth_a, sizeof depth_a);
    __builtin_memcpy(block + 12, &third, sizeof third);
    __builtin_memcpy(block + 16, &fourth, sizeof fourth);
    __builtin_memcpy(block + 4, &mode, sizeof mode);
    sceSdRemote(1, 0x8050, 0x1d01, 0x1fffff);

    for (int pass = 0; pass < 2; ++pass) {
        bool failed = false;
        int stored_mode;
        __builtin_memcpy(&stored_mode, block + 4, sizeof stored_mode);
        if (sceSdRemote(1, 0x8190, 1, pass, stored_mode) != 0) failed = true;
        if (sceSdRemote(1, 0x80f0, pass, 1) != 1) failed = true;
        if (!failed) break;
    }

    const int block_address = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(block));
    sceSdRemote(1, 0x81a0, 1, block_address);
    sceSdRemote(1, 0x81b0, 1, block_address);
    sceSdRemote(1, 0x8030, 0x1800, 0xffffff);
    sceSdRemote(1, 0x8030, 0x1a00, 0xffffff);
    sceSdRemote(1, 0x8030, 0x1900, 0);
    sceSdRemote(1, 0x8030, 0x1b00, 0);
    sceSdRemote(1, 0x8030, 0x1801, 0xffffff);
    sceSdRemote(1, 0x8030, 0x1a01, 0xffffff);
    sceSdRemote(1, 0x8030, 0x1901, 0x7ffff);
    sceSdRemote(1, 0x8030, 0x1b01, 0x7ffff);
    sceSdRemote(1, 0x8010, 0x800, 0xc00);
    sceSdRemote(1, 0x8010, 0x801, 0xf0f);
    sceSdRemote(1, 0x8070, 3, 1);
    memFree__FPv(block);
    call_command(192);
}
}

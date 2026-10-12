/* transferVAG__FP10_vagHeaderi at 0x0024d0c8 is 568 bytes. Arguments: the
 * header and a destination address in sound memory. The body size is the
 * word at offset 44 of the header and the body bytes start at offset 48.
 *
 *   1. sceSifCallRpc, command 176 (0xb0), mode 0, send sbuffin 16 bytes,
 *      receive sbuff 64 bytes, null end function and parameter.
 *   2. memAlloc__FiPc(size, "") for a staging copy. On null: soundError with
 *      the allocation-failure text, then the assert with file "sound.cc",
 *      line 1807, condition "0", and the live return register. If the
 *      assert returns, the copy proceeds from a null pointer, as the
 *      original would.
 *   3. memcpy of the body into the staging copy, then FlushCache(0).
 *   4. The size rounded up to a multiple of 64 is moved in pieces of at most
 *      4096 bytes. For each piece: sceSdTransToIOP(piece, IOPbuffer, length,
 *      1), soundError on a result of -1; sceSdRemote(1, 0x80d0, 0, 0,
 *      IOPbuffer, destination, length), soundError on -1; the destination
 *      advances by the length; sceSdRemote(1, 0x80f0, 0, 1), soundError on
 *      -1. A rounded size of zero or less moves nothing.
 *   5. memFree of the staging copy, then sceSifCallRpc, command 192 (0xc0),
 *      with the same buffers and lengths as step 1.
 *
 * Not reproduced: before each of the two RPC calls the original copies 16
 * bytes from its own stack frame into sbuffin. Nothing in this function
 * writes that frame area, so the bytes sent are whatever the stack held.
 * This source leaves sbuffin as the caller left it.
 *
 * The soundError texts are the original diagnostic strings.
 */
extern "C" {
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_rpc_send_view[];
extern unsigned char norrath_sound_rpc_receive_view[];
extern unsigned char norrath_sound_iop_buffer_word_view[];
int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int send_bytes,
                           void* receive, int receive_bytes, void* end_function,
                           void* end_parameter);
void norrath_sound_assert(const char* file, unsigned line, const char* condition, int captured_ra);
void* memAlloc__FiPc(int bytes, char* label);
void memFree__FPv(void* block);
void* memcpy(void* destination, const void* source, unsigned long count);
void FlushCache(int mode);
int sceSdTransToIOP(void* source, int destination, int size, int mode);
int sceSdRemote(int one, int command, ...);
void soundError__FPc(char* text);

static inline void rpc(int command) {
    norrath_sound_call_rpc(norrath_sound_rpc_client_view, command, 0, norrath_sound_rpc_send_view, 16,
                           norrath_sound_rpc_receive_view, 64, nullptr, nullptr);
}

void transferVAG__FP10_vagHeaderi(void* header, int destination) {
    unsigned char* vag = static_cast<unsigned char*>(header);
    rpc(176);
    const int size = *reinterpret_cast<int*>(vag + 44);
    unsigned char* staging = static_cast<unsigned char*>(memAlloc__FiPc(size, const_cast<char*>("")));
    if (staging == nullptr) {
        soundError__FPc(const_cast<char*>("Couldn't allocate aligned VAG body copy."));
        int captured;
#if defined(NORRATH_HOST_CONTRACT)
        captured = 0x24d17c;
#else
        asm volatile("move %0,$31" : "=r"(captured)::"memory");
#endif
        norrath_sound_assert("sound.cc", 1807, "0", captured);
    }
    memcpy(staging, vag + 48, size);
    FlushCache(0);
    unsigned char* piece = staging;
    int remaining = (size + 63) & -64;
    while (remaining > 0) {
        const int length = remaining < 4097 ? remaining : 4096;
        const int result = sceSdTransToIOP(piece, *reinterpret_cast<int*>(norrath_sound_iop_buffer_word_view),
                                           length, 1);
        piece += length;
        remaining -= length;
        if (result == -1) soundError__FPc(const_cast<char*>("Failed to transfer to IOP."));
        const int to_spu = sceSdRemote(1, 0x80d0, 0, 0, *reinterpret_cast<int*>(norrath_sound_iop_buffer_word_view),
                                       destination, length);
        destination += length;
        if (to_spu == -1) soundError__FPc(const_cast<char*>("Failed to transfer to SPU2."));
        const int complete = sceSdRemote(1, 0x80f0, 0, 1);
        if (complete == -1) soundError__FPc(const_cast<char*>("Complete transfer to SPU2 failed"));
    }
    memFree__FPv(staging);
    rpc(192);
}
}

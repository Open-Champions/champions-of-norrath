// Observed: sceInetAddress2Name at 0x002e5278, 204 bytes, pinned SLUS_205.65. Nine arguments (a0-a3, t0-t3, one stack word):
// (object, buffer, flags, name, capacity, address, fifth_word, sixth_word, stack_word).
//  1. buffer[0] = flags, buffer[1] = capacity, buffer[2] = fifth_word, buffer[3] = sixth_word.
//  2. MemcpyD(object, buffer, 4, address, 16). Negative result: returned.
//  3. buffer[8] = 0, then = stack_word when bit 7 (0x80) of flags is set.
//  4. CallRpc(object, 18, buffer, 36, capacity + 32). Negative result: -542.
//  5. StrcpyS(object, name, buffer, 8) (name copied from buffer word 8 on). Negative result: returned.
//  6. Otherwise buffer[0] is returned.
// Inferred: a request block in words 0-8 and the reply text from word 8; capacity bounds the reply text. The startup thread
// calls it with flags 0, capacity 128 and the last three arguments zero. Parameter names are roles from that use, not SDK names.
// Unknown: the meaning of words 2, 3 and 8 and of flag bit 0x80.
extern "C" {
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size);
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int StrcpyS(void* object, char* destination, void* buffer, int word_offset);

int sceInetAddress2Name(void* object, int* buffer, int flags, char* name, int capacity, const void* address,
                        int fifth_word, int sixth_word, int stack_word) {
    buffer[0] = flags;
    buffer[1] = capacity;
    buffer[2] = fifth_word;
    buffer[3] = sixth_word;
    const int copied = MemcpyD(object, buffer, 4, address, 16);
    if (copied < 0) return copied;
    buffer[8] = 0;
    if ((flags & 0x80) != 0) buffer[8] = stack_word;
    if (CallRpc(object, 18, buffer, 36, capacity + 32) < 0) return -542;
    const int text_result = StrcpyS(object, name, buffer, 8);
    if (text_result < 0) return text_result;
    return buffer[0];
}
}

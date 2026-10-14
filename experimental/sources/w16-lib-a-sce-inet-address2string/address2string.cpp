// Observed: sceInetAddress2String at 0x002e51d8, 156 bytes, pinned SLUS_205.65. Five arguments
// (object, buffer, text, capacity, address):
//  1. buffer[0] = capacity.
//  2. MemcpyD(object, buffer, 1, address, 16) (16 bytes of address into buffer word 1 onward). Negative result: returned.
//  3. CallRpc(object, 7, buffer, 20, capacity + 20). Negative result: -542.
//  4. StrcpyS(object, text, buffer, 5) (text copied from buffer word 5 on). Negative result: returned.
//  5. Otherwise buffer[0] is returned.
// Inferred: a request/reply block with the request in words 0-4 and the reply text in words 5 onward, so capacity is the
// text capacity the server may write. Names are role guesses from this use, not SDK names for the parameters.
// Unknown: the 16-byte address layout and the reply code in word 0.
extern "C" {
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size);
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int StrcpyS(void* object, char* destination, void* buffer, int word_offset);

int sceInetAddress2String(void* object, int* buffer, char* text, int capacity, const void* address) {
    buffer[0] = capacity;
    const int copied = MemcpyD(object, buffer, 1, address, 16);
    if (copied < 0) return copied;
    if (CallRpc(object, 7, buffer, 20, capacity + 20) < 0) return -542;
    const int text_result = StrcpyS(object, text, buffer, 5);
    if (text_result < 0) return text_result;
    return buffer[0];
}
}

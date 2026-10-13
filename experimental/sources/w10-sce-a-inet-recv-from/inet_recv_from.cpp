/* Observed sceInetRecvFrom at 0x002e5510, 268 bytes. Arguments, in order: the
 * library object, a request buffer, a handle word, a data destination, its
 * size, an optional in/out word (null allowed), a 16-byte address destination,
 * an optional output word (null allowed) and, on the stack, one more word.
 * Buffer word 0 gets the handle; word 1 the in/out word's current value, or
 * zero when it is null; word 2 the size; word 3 the stack word. CallRpc(object,
 * 13, buffer, 16, size + 28) runs; a negative result returns -542, and a
 * negative reply word 0 is returned as it is. Then a non-null in/out word
 * receives buffer word 1, MemcpyS copies 16 bytes from buffer word 2 to the
 * address destination (negative: returned as it is), a non-null output word
 * receives buffer word 6, and MemcpyS copies buffer word 0 bytes from buffer
 * word 7 to the data destination (negative: returned as it is). The result is
 * buffer word 0 read after those copies. What the words mean is not
 * established here.
 */
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int MemcpyS(void* object, void* destination, void* source, int word_offset, int size);

int sceInetRecvFrom(void* object, int* buffer, int handle, void* data, int size, int* io_word, void* address,
                    int* output_word, int ninth) {
    buffer[0] = handle;
    buffer[1] = io_word != nullptr ? *io_word : 0;
    buffer[2] = size;
    buffer[3] = ninth;
    if (CallRpc(object, 13, buffer, 16, size + 28) < 0) return -542;
    if (buffer[0] < 0) return buffer[0];
    if (io_word != nullptr) *io_word = buffer[1];
    const int address_copied = MemcpyS(object, address, buffer, 2, 16);
    if (address_copied < 0) return address_copied;
    if (output_word != nullptr) *output_word = buffer[6];
    const int data_copied = MemcpyS(object, data, buffer, 7, buffer[0]);
    if (data_copied < 0) return data_copied;
    return buffer[0];
}
}

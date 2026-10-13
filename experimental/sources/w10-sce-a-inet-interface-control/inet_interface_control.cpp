/* Observed sceInetInterfaceControl at 0x002e5930, 168 bytes. Arguments: the
 * library object, a request buffer, two words, a data block and its size.
 * Buffer words 0, 1 and 2 get the two words and the size. MemcpyD(object,
 * buffer, 3, data, size) copies the data into the buffer at word 3 (a
 * negative result is returned as it is), CallRpc(object, 9, buffer, size + 12,
 * size + 12) runs (negative: -542), and MemcpyS(object, data, buffer, 3, size)
 * copies the reply data back over the block (negative: returned as it is).
 * The result is buffer word 0. What the two words and the block mean is not
 * established here.
 */
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size);
int MemcpyS(void* object, void* destination, void* source, int word_offset, int size);

int sceInetInterfaceControl(void* object, int* buffer, int first, int second, void* data, int size) {
    buffer[0] = first;
    buffer[1] = second;
    buffer[2] = size;
    const int copied = MemcpyD(object, buffer, 3, data, size);
    if (copied < 0) return copied;
    if (CallRpc(object, 9, buffer, size + 12, size + 12) < 0) return -542;
    const int returned = MemcpyS(object, data, buffer, 3, size);
    if (returned < 0) return returned;
    return buffer[0];
}
}

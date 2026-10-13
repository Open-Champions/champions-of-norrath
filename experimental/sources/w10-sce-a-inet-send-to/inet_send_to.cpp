/* Observed sceInetSendTo at 0x002e56d0, 228 bytes. Arguments, in order: the
 * library object, a request buffer, a handle word, a data block, its size, an
 * optional in/out word (null allowed), a 16-byte address block, one more word
 * and, on the stack, one more word. Buffer word 0 gets the handle; word 1 gets
 * the in/out word's current value, or zero when it is null; word 2 gets the
 * size; word 3 gets the stack word; word 4 gets the eighth argument. MemcpyD
 * copies the 16-byte address block into the buffer at word 5 and then the data
 * block at word 9 (negative results are returned as they are), CallRpc(object,
 * 14, buffer, size + 36, 8) runs (negative: -542), then a non-null in/out
 * word receives buffer word 1. The result is buffer word 0. What the words
 * mean is not established here.
 */
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size);

int sceInetSendTo(void* object, int* buffer, int handle, const void* data, int size, int* io_word,
                  const void* address, int eighth, int ninth) {
    buffer[0] = handle;
    buffer[1] = io_word != nullptr ? *io_word : 0;
    buffer[2] = size;
    buffer[4] = eighth;
    buffer[3] = ninth;
    const int address_copied = MemcpyD(object, buffer, 5, address, 16);
    if (address_copied < 0) return address_copied;
    const int data_copied = MemcpyD(object, buffer, 9, data, size);
    if (data_copied < 0) return data_copied;
    if (CallRpc(object, 14, buffer, size + 36, 8) < 0) return -542;
    if (io_word != nullptr) *io_word = buffer[1];
    return buffer[0];
}
}

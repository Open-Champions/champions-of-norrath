/* Observed sceInetCreate at 0x002e5348, 108 bytes. Arguments: the library
 * object, a request buffer and a source block. MemcpyD(object, buffer, 1,
 * source, 64) copies 64 bytes of the source into the buffer at word 1; a
 * negative result is returned as it is. CallRpc(object, 1, buffer, 68, 4)
 * follows; a negative result returns -542. Otherwise buffer word 0 is the
 * reply: a nonzero word is returned, a zero word is replaced by -540.
 * The block's meaning is not established here (the callers pass a 64-byte
 * block). MemcpyD's parameter meanings beyond this call are not established.
 */
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size);

int sceInetCreate(void* object, int* buffer, const void* source) {
    const int copied = MemcpyD(object, buffer, 1, source, 64);
    if (copied < 0) return copied;
    if (CallRpc(object, 1, buffer, 68, 4) < 0) return -542;
    const int reply = buffer[0];
    return reply != 0 ? reply : -540;
}
}

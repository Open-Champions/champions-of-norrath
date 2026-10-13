/* Observed sceInetOpen at 0x002e53b8, 72 bytes. Arguments: the library
 * object, a request buffer and two words. The words are stored in buffer
 * words 0 and 1 and CallRpc(object, 2, buffer, 8, 4) runs. A negative result
 * returns -542; otherwise buffer word 0 is returned. What the two words
 * select is not established here.
 */
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);

int sceInetOpen(void* object, int* buffer, int first, int second) {
    buffer[0] = first;
    buffer[1] = second;
    if (CallRpc(object, 2, buffer, 8, 4) < 0) return -542;
    return buffer[0];
}
}

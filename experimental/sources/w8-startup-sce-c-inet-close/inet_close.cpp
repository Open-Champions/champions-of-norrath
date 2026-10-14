// Observed sceInetClose at 0x002e5400, 72 bytes. The second argument is a
// request buffer. The third and fourth arguments are stored at its words 0 and
// 1, then CallRpc runs on the first argument with function 3, the buffer, send
// size 8 and receive size 4. A negative CallRpc result returns -542; otherwise
// the buffer's word 0 is returned.
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int sceInetClose(void* object, int* buffer, int first, int second) {
    buffer[0] = first;
    buffer[1] = second;
    if (CallRpc(object, 3, buffer, 8, 4) < 0) return -542;
    return buffer[0];
}
}

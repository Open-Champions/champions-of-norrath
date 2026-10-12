// Observed sceInetGetInterfaceList at 0x002e58a8, 136 bytes. The fourth
// argument (count) is stored at buffer word 0 and CallRpc runs on the first
// argument with function 8, send size 4 and receive size count * 4 + 4. A
// negative CallRpc result returns -542. Otherwise the buffer's word 0 is the
// result; if it is negative it is returned. If not, MemcpyS runs with the first
// argument, the third argument as destination, the buffer as source, a flag of
// 1 and a size of result * 4. A negative MemcpyS result is returned. Otherwise
// buffer word 0 is returned. MemcpyS's parameter meanings beyond this call
// are not established.
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int MemcpyS(void* object, void* destination, void* source, int flag, int size);
int sceInetGetInterfaceList(void* object, int* buffer, void* destination, int count) {
    buffer[0] = count;
    if (CallRpc(object, 8, buffer, 4, count * 4 + 4) < 0) return -542;
    const int result = buffer[0];
    if (result < 0) return result;
    const int copied = MemcpyS(object, destination, buffer, 1, result * 4);
    if (copied < 0) return copied;
    return buffer[0];
}
}

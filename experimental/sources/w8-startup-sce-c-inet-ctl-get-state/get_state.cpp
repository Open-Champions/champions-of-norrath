// Observed sceInetCtlGetState at 0x002e5ce8, 140 bytes. The third argument
// is stored at buffer word 0. Word 1 is set to the integer the fourth argument
// points at, or to zero for a null pointer. CallRpc then runs on the first
// argument with function 53, send size 8 and receive size 8. A negative result
// returns -542. Otherwise a non-null fourth argument receives buffer word 1,
// and buffer word 0 is returned.
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int sceInetCtlGetState(void* object, int* buffer, int interface_id, int* state) {
    buffer[0] = interface_id;
    buffer[1] = state != nullptr ? *state : 0;
    if (CallRpc(object, 53, buffer, 8, 8) < 0) return -542;
    if (state != nullptr) *state = buffer[1];
    return buffer[0];
}
}

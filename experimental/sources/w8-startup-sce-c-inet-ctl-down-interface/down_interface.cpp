// Observed sceInetCtlDownInterface at 0x002e5c58, 72 bytes. The third argument
// is stored at word 0 of the request buffer (second argument), then CallRpc
// runs on the first argument with function 51, send size 4 and receive size 4.
// A negative CallRpc result returns -542; otherwise buffer word 0 is returned.
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);
int sceInetCtlDownInterface(void* object, int* buffer, int interface_id) {
    buffer[0] = interface_id;
    if (CallRpc(object, 51, buffer, 4, 4) < 0) return -542;
    return buffer[0];
}
}

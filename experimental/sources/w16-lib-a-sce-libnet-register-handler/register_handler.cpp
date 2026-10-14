// Observed: sceLibnetRegisterHandler at 0x002e5ea8, 64 bytes, pinned SLUS_205.65. With (object, buffer):
//  CallRpc(object, 30, buffer, 0, 4). A negative result returns -542. Otherwise the first word of the buffer is returned.
// Inferred: the buffer is both the reply area and the value source; callers pass a heap block (the startup thread passes
// the block it just allocated).
// Unknown: what the reply word means (CallRpc's own contract decides) and why the send size is zero.
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size);

int sceLibnetRegisterHandler(void* object, int* buffer) {
    if (CallRpc(object, 30, buffer, 0, 4) < 0) return -542;
    return buffer[0];
}
}

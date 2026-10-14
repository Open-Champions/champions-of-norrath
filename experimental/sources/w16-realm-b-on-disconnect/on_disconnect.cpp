// Observed onDisconnect__16VIRealmInterface at 0x01109258, 40 bytes. With (this): printf("Disconnected from realm interface\n") is
// called, and the word `connected` (0x011b017c) is set to 0. The store sits in the call's delay slot, so it happens before the call
// body runs; the flag is therefore already clear when printf executes. `this` is not read.
// Inferred: `connected` is the realm-session-up flag that the interface sets and clears around the login handshake.
// Unknown: who owns `connected` and what else reads it; the return type (nothing is returned).
extern "C" {
extern int connected;
int printf(const char* format, ...);

void onDisconnect__16VIRealmInterface(void* self) {
    (void)self;
    connected = 0;
    printf("Disconnected from realm interface\n");
}
}

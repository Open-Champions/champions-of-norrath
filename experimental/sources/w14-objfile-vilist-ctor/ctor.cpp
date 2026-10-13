// Observed: __7VILList at 0x01072788, 20 bytes. Saves and restores the return address and ends in a jump to Init__7VILList
// with the same argument; it does nothing else.
// Inferred: the constructor of the list class is just its Init.
// Unknown: whether the frame in the image comes from a different code shape; this source compiles to a bare tail jump.
extern "C" {
void Init__7VILList(void* self);

void __7VILList(void* self) {
    Init__7VILList(self);
}
}

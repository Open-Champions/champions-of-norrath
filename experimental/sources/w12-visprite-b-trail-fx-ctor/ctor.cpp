// Observed: __9VITrailFx at 0x01027240 is 56 bytes in the pinned SLUS-20565 image. With the
// object as this it calls __7VILNode(this), then calls __7VILList(this+28), then calls
// __7VILList(this+40), then stores 0 at this+52. Nothing is returned.
// Unknown: the layout of the 56-byte element beyond the node at +0 and the two lists at +28 and
// +40 (12-byte list objects, inferred from the destructor offsets). Nothing here ran on the EE.
extern "C" {
void __7VILNode(void* self);
void __7VILList(void* self);

void fn_9VITrailFx(void* self) __asm__("__9VITrailFx");
void fn_9VITrailFx(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __7VILNode(self);
    __7VILList(bytes + 28);
    __7VILList(bytes + 40);
    *reinterpret_cast<unsigned*>(bytes + 52) = 0;
}
}

// Observed msg_collisionWorld__10GameObjectG6Point3 at 0x312e88, 36 bytes, weak
// binding. The image copies the 12-byte Point3 argument, passed by address in
// the second integer register, into its own stack frame and returns. Nothing
// reads that copy and the object is not touched. This definition is a normal
// global; the original symbol binding was weak.
extern "C" void msg_collisionWorld__10GameObjectG6Point3(void* self, const void* point) {
    (void)self;
    (void)point;
}

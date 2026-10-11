// Observed msg_collisionWorld__10GameObjectG6Point3 at 0x312e88, 36 bytes.
// The 12-byte point arrives by pointer and is copied onto the stack, then discarded.
// The object is not read or written.
extern "C" void norrath_vt_msg_collision_world_10GameObject(void* self, const void* point) {
    (void)self;
    volatile unsigned char sink[12];
    const auto* incoming = static_cast<const unsigned char*>(point);
    for (unsigned i = 0; i < 12u; ++i) sink[i] = incoming[i];
}

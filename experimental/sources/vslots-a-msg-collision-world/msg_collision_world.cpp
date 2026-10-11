// Observed msg_collisionWorld__8CreatureG6Point3 at 0x00113708, 36 bytes.
// The 12-byte point arrives in the second argument and is copied onto the stack.
// The creature is not read or written. The copy is discarded.
extern "C" void msg_collisionWorld__8CreatureG6Point3(void* self, const void* point) {
    (void)self;
    volatile unsigned char sink[12];
    const auto* incoming = static_cast<const unsigned char*>(point);
    for (unsigned i = 0; i < 12u; ++i) sink[i] = incoming[i];
}

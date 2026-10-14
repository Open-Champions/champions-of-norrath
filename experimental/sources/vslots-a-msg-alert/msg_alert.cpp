// Observed msg_alert__8CreatureifG6Point3P10GameObject at 0x001134b8, 36 bytes.
// The point is the third explicit argument. Its 12 bytes are copied onto the
// stack and discarded. The creature, code, amount, and other object are not read.
extern "C" void msg_alert__8CreatureifG6Point3P10GameObject(
    void* self, int code, float amount, const void* point, void* other) {
    (void)self;
    (void)code;
    (void)amount;
    (void)other;
    volatile unsigned char sink[12];
    const auto* incoming = static_cast<const unsigned char*>(point);
    for (unsigned i = 0; i < 12u; ++i) sink[i] = incoming[i];
}

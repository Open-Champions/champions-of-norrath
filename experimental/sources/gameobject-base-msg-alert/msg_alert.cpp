// Observed msg_alert__10GameObjectifG6Point3P10GameObject at 0x312eb8, 36 bytes. The point is the third explicit argument.
// The 12-byte point arrives by pointer and is copied onto the stack, then discarded.
// The object is not read or written.
extern "C" void norrath_vt_msg_alert_10GameObject(void* self, int code, float amount, const void* point, void* other) {
    (void)self; (void)code; (void)amount; (void)other;
    volatile unsigned char sink[12];
    const auto* incoming = static_cast<const unsigned char*>(point);
    for (unsigned i = 0; i < 12u; ++i) sink[i] = incoming[i];
}

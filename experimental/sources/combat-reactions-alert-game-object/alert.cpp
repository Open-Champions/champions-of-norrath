// Observed msg_alert__10GameObjectifG6Point3P10GameObject at 0x312eb8, 36 bytes, weak binding. This definition is a normal global.
// The image copies the 12-byte Point3 argument, passed by address in the third
// integer register, into its own stack frame and returns. Nothing reads that
// copy, and the object and the other object are not touched. The float is in a
// floating-point register and the other object is in the fourth integer
// register.
// Unknown: the meaning of the integer and float arguments.
extern "C" void msg_alert__10GameObjectifG6Point3P10GameObject(void* self, int kind, float amount, const void* point, void* other) {
    (void)self;
    (void)kind;
    (void)amount;
    (void)point;
    (void)other;
}

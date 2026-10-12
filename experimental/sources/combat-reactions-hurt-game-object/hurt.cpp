// Observed msg_hurt__10GameObjectfP10GameObjecti at 0x312e48, 8 bytes, weak
// binding: the body returns at once. It reads no argument and writes nothing.
// This definition is a normal global; the original symbol binding was weak.
// Inferred: the float is passed in a floating-point register and the other
// two arguments are in the second and third integer registers, as in the
// Creature override of the same name.
extern "C" void msg_hurt__10GameObjectfP10GameObjecti(void* self, float amount, void* other, int kind) {
    (void)self;
    (void)amount;
    (void)other;
    (void)kind;
}

// Observed msg_hurt__14WoodElfSoldierfP10GameObjecti at 0x00249cc8, 32 bytes.
// Calls the Creature hurt handler (msg_hurt__8CreaturefP10GameObjecti) with the same object, float
// amount, source object and integer, and returns. Nothing else is read or written.
// Inferred: the symbol name says this is the WoodElfSoldier override and that the base handler does
// the work. Unknown: the meaning of the integer. Nothing here ran on the EE.
class GameObject;
extern "C" {
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, GameObject* source, int kind);
void msg_hurt__14WoodElfSoldierfP10GameObjecti(void* self, float amount, GameObject* source, int kind);
}

extern "C" void msg_hurt__14WoodElfSoldierfP10GameObjecti(void* self, float amount, GameObject* source, int kind) {
    msg_hurt__8CreaturefP10GameObjecti(self, amount, source, kind);
}

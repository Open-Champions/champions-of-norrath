// Observed initMiniboss__11MaleDarkElf at 0x00289840, 48 bytes.
// Calls eRand__10GameObject on the object and stores (result & 15) + 1 as the byte at object+76.
// Inferred: the byte is a variant number from 1 to 16, read back by getMinibossName__11MaleDarkElf.
// The call's return value is not used. Nothing here ran on the EE.
extern "C" {
unsigned eRand__10GameObject(void* self);
}

extern "C" void initMiniboss__11MaleDarkElf(void* self) {
    static_cast<unsigned char*>(self)[76] = static_cast<unsigned char>((eRand__10GameObject(self) & 15u) + 1u);
}

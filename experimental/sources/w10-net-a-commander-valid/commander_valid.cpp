// Observed commanderValid__14WoodElfSoldier at 0x00249e60, 44 bytes. The word at
// offset 960 of the object is a pointer; a null pointer returns 0. Otherwise it
// follows that object's word at +12 to a second object and returns 1 when bit 8
// of the word at +12 of the second object is clear, else 0.
// Inferred: offset 960 holds the soldier's commander; the pointer at +12 of the
// commander is a type/descriptor record whose flag bit 8 marks a commander as
// unusable. Callers (the WoodElfSoldier set-state routine) leave the commander
// role when the result is nonzero.
// Unknown: the name of the +12 record and of flag bit 8.
extern "C" int commanderValid__14WoodElfSoldier(void* self) {
    unsigned char* commander = *reinterpret_cast<unsigned char**>(static_cast<unsigned char*>(self) + 960);
    if (commander == 0) return 0;
    unsigned char* record = *reinterpret_cast<unsigned char**>(commander + 12);
    return (*reinterpret_cast<unsigned*>(record + 12) & 8) == 0;
}

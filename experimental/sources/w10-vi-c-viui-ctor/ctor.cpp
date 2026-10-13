// Observed: __4VIUI at 0x0102f1c8 is 72 bytes. With the object as this it runs
// __13VIStringTable on this, this+16 and this+32, then __6VIFont on this+56, then stores 0 to
// the words at this+72, this+80 and this+76 (in that order). Nothing is returned.
// Inferred: three string tables and one font are embedded members. Unknown: the meaning of
// the three cleared words and of the bytes between the members.
extern "C" {
void __13VIStringTable(void* self);
void __6VIFont(void* self);

void __4VIUI(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __13VIStringTable(self);
    __13VIStringTable(bytes + 16);
    __13VIStringTable(bytes + 32);
    __6VIFont(bytes + 56);
    *reinterpret_cast<unsigned*>(bytes + 72) = 0;
    *reinterpret_cast<unsigned*>(bytes + 80) = 0;
    *reinterpret_cast<unsigned*>(bytes + 76) = 0;
}
}

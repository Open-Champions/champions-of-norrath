// Observed: CalcDataSize__10VIZoneRoom at 0x0111f578 is 48 bytes in the pinned SLUS-20565 image. With A = word at +80 and B = word
// at +88, it stores 0 at +92, A*32 at +96 and A*32 + B*12 at +112, and returns zero. Arithmetic is 32-bit and wraps.
// Inferred: A counts 32-byte records and B counts 12-byte records; +112 is the total byte size that Allocate__10VIZoneRoom
// reserves, +92 is the offset of the first record array (zero) and +96 the offset of the 12-byte records after the 32-byte ones.
// Unknown: the names and layout of the room object, and what the word at +84 holds. Nothing here ran on
// the EE.
extern "C" {
int CalcDataSize__10VIZoneRoom(unsigned char* self) {
    int count12 = *reinterpret_cast<int*>(self + 88);
    int count32 = *reinterpret_cast<int*>(self + 80);
    *reinterpret_cast<int*>(self + 92) = 0;
    int size32 = count32 << 5;
    *reinterpret_cast<int*>(self + 96) = size32;
    *reinterpret_cast<int*>(self + 112) = size32 + count12 * 12;
    return 0;
}
}

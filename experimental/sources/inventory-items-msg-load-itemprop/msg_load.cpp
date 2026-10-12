// Observed msg_load__8ItemPropPc at 0x155808, 280 bytes.
// Steps, in order: the word at object+12 gets bit 20 (0x100000) set and bit 11
// (0x800) cleared; 72 bytes from the buffer are copied to object+328 and
// item::unpack runs on them; Prop::msg_load runs with the object and the
// buffer plus 72. When bit 39 of the 64-bit value at object+336 (bit 7 of the
// byte at object+340) is set, bits 12 and 13 (0x3000) of the word at object+12
// are cleared. matMakeTransYaw then receives the matrix at object+272, a stack
// copy of the 12 bytes at object+16, and the signed halfword at object+56 plus
// 16384 truncated to 16 bits. The return is 72 plus Prop::msg_load's result.
// Unknown: what the flag bits and the matrix mean.

#include "../inventory-items-object/object.hpp"

extern "C" {
void unpack__4itemPQ24item11description(void* description);
int msg_load__4PropPc(void* object, const unsigned char* buffer);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
}

extern "C" int msg_load__8ItemPropPc(void* object_storage, const unsigned char* buffer) {
    using namespace inventory_object;
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    write_word(object + 12, (read_word(object + 12) | 0x100000u) & ~0x800u);
    copy_bytes(object + 328, buffer, 72);
    unpack__4itemPQ24item11description(object + 328);
    const int used = msg_load__4PropPc(object, buffer + 72);
    if ((object[340] & 0x80u) != 0u) write_word(object + 12, read_word(object + 12) & ~0x3000u);
    unsigned char point[12];
    copy_bytes(point, object + 16, 12);
    const unsigned half = unsigned(object[56]) | (unsigned(object[57]) << 8);
    matMakeTransYaw__FP8Matrix34G6Point3s(object + 272, point, static_cast<short>(half + 16384u));
    return 72 + used;
}

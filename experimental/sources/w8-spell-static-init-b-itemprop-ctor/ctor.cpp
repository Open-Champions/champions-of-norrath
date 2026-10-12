// __8ItemPropG6Point3PQ24item11descriptioni at 0x14b9e8, 480 bytes, read from the
// pinned SLUS-20565 image. Nothing here has run on the EE.
//
// Observed, in the image's order. The Point3 argument is copied to the stack and
// its address is passed on. getProxyMeshId__4itemPQ24item11description is called
// twice on the description argument; the first result picks the 16-byte record
// of `_4item$globalItem` whose first word is passed on and the second result
// picks the record whose second word is passed on. Then the base constructor
// __4PropG6Point3PcN22li runs with (this, the point copy, the string "equipmnt.lmp",
// those two table words, -1, the integer argument). Then the vtable word at 72
// is set to the address of `_vt$8ItemProp`, and the 72 bytes of the description
// are copied to offset 328 (the image first stores -1 halfwords and bytes and
// zero halfwords into the same range, which the copy overwrites; they have no
// effect and are not written here). The word at 12 is or-ed with 0x102000 and has
// 0x1800 cleared; when bit 39 of the 64-bit value at 336 (that is, bit 7 of the
// byte at 340, inside the copy) is set, 0x3000 is cleared too. Then
// objectUpdateInGrid__FP10GameObject runs on the object, and
// matMakeTransYaw__FP8Matrix34G6Point3s builds the matrix at 272 from a copy of
// the point at 16 and the unsigned halfword at 56 plus 16384, taken as a signed
// halfword.
// Inferred: the object is an item prop because of the symbol; the description
// type name comes from the mangled symbol.
// Unknown: the meaning of the strings in the table, of the halfword at 56 and of
// the bits tested.
extern "C" {
extern unsigned char _4item$globalItem[];
extern unsigned char _vt$8ItemProp[];
int getProxyMeshId__4itemPQ24item11description(const unsigned char* description);
void __4PropG6Point3PcN22li(void* self, const float* point, const char* name, const char* first,
                            const char* second, long minus_one, int argument);
void objectUpdateInGrid__FP10GameObject(void* object);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const float* point, short yaw);

struct norrath_item_prop_record {
    const char* first;
    const char* second;
    unsigned char rest[8];
};

static unsigned char* norrath_bytes(void* base, unsigned offset) {
    return static_cast<unsigned char*>(base) + offset;
}

void __8ItemPropG6Point3PQ24item11descriptioni(void* self, const float* point, const unsigned char* description,
                                                int argument) {
    const float placed[3] = {point[0], point[1], point[2]};
    const norrath_item_prop_record* table = reinterpret_cast<const norrath_item_prop_record*>(_4item$globalItem);
    const int first_id = getProxyMeshId__4itemPQ24item11description(description);
    const int second_id = getProxyMeshId__4itemPQ24item11description(description);
    __4PropG6Point3PcN22li(self, placed, "equipmnt.lmp", table[first_id].first, table[second_id].second, -1, argument);

    *reinterpret_cast<const void**>(norrath_bytes(self, 72)) = _vt$8ItemProp;
    unsigned char* copy = norrath_bytes(self, 328);
    for (int i = 0; i < 72; ++i) copy[i] = description[i];

    unsigned flags = *reinterpret_cast<unsigned*>(norrath_bytes(self, 12));
    flags = (flags | 0x102000u) & ~0x1800u;
    *reinterpret_cast<unsigned*>(norrath_bytes(self, 12)) = flags;
    if (norrath_bytes(self, 340)[0] & 0x80u) *reinterpret_cast<unsigned*>(norrath_bytes(self, 12)) = flags & ~0x3000u;

    objectUpdateInGrid__FP10GameObject(self);
    const float* position = reinterpret_cast<const float*>(norrath_bytes(self, 16));
    const float moved[3] = {position[0], position[1], position[2]};
    const unsigned short turn = *reinterpret_cast<const unsigned short*>(norrath_bytes(self, 56));
    matMakeTransYaw__FP8Matrix34G6Point3s(norrath_bytes(self, 272), moved, static_cast<short>(turn + 16384));
}
}

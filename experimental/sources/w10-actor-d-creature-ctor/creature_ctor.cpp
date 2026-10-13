// Observed __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc at 0x00108e58, 852 bytes.
// Arguments: the object, a Point3 passed by address, a short, a resource map pointer, a float
// (scale) and the tag argument list. It returns nothing the callers use.
// The Point3 is copied to a local, and the GameObject constructor runs first with
// (object, local copy, map, argument list). Then, with no call in between:
//  - byte +293 = 8; floats +760, +764, +768 = 0.0; table pointer +72 = _vt$8Creature; words +576
//    and +752 = 0; the local Point3 copied to +776..787; float +224 = 1.0; floats +228 and +192 =
//    0.0; bit 0x20 of the word +12 set; halfword +56 = the short; floats +564 and +560 = scale;
//    words +552 and +556 = -1; word +888 = 0; words +580 and +584 = -1; words +708, +712, +744,
//    +256, +716 = 0; halfwords +284, +792, +796 = 0; byte +570 = 0; words +808, +812 = 0; bytes +800,
//    +76, +156 = 0; byte +85 = 0xff; float +60 = float +24; float +788 = 1000.0; words +208,
//    +216, +212 = 0; byte +569 = the old gameCreatureThinkSpinner, which is then incremented
//    (it is a byte global).
// Then tag lookups (each names a tag in the object's argument list; the second argument of every
// call below is the tag name and the third is 0):
//  - only when the signed byte cutMode is zero: findTagString "initScript"; a non-null result goes
//    to scriptStart__FPc;
//  - findTagInt "wait": if nonzero, byte +570 = findTagInt "wait" (looked up a second time);
//  - findTagInt "hide": if nonzero, byte +570 = 3 and then objectRemoveFromGrid is called;
//  - findTagString "ShowName": if non-null, word +892 = a second lookup of it and bit 0x100000 of
//    the word +12 is set;
//  - findTagInt "Dead": if nonzero, float +192 = 1000.0;
//  - findTagInt "HomeRange": if nonzero, float +788 = (float) a second lookup;
//  - findTagInt "Scale": if nonzero, float +224 = (float) a second lookup * 0.01;
//  - findTagInt "MiniBoss": if nonzero, bit 0 of the 64-bit word +160 is set;
//  - findTagString "light1", then "light2", then "ambient", stopping at the first non-null: if any
//    is non-null, bit 0x40000000 of the 64-bit word +160 is set;
//  - finally halfword +798 = the unsigned halfword at +56.
// Inferred: the tag names are keys read from the placed object's argument list; the field names
// and the meaning of the scale (+560, +564) are not established. Unknown: the argument types beyond
// their register use, the meaning of 293, 569/570 and the other zeroed fields. The 0.01 factor is
// the single 0x3c23d70a at gp-32412. Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};

extern "C" {
void gameObject_construct(void* self, const Point3* point, void* map, char** arguments) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
char* findTagString__10GameObjectPci(void* self, const char* name, int index);
int findTagInt__10GameObjectPci(void* self, const char* name, int index);
void scriptStart__FPc(char* name);
void objectRemoveFromGrid__FP10GameObject(void* self);
extern unsigned char vt_Creature[] __asm__("_vt$8Creature");
extern unsigned char gameCreatureThinkSpinner;
extern signed char cutMode;
}

namespace {
template <typename T>
inline T& field(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
}  // namespace

extern "C" void creature_construct(void* self, const Point3* point, short type, void* map, float scale, char** arguments)
    __asm__("__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc");
extern "C" void creature_construct(void* self, const Point3* point, short type, void* map, float scale, char** arguments) {
    unsigned char* o = static_cast<unsigned char*>(self);
    Point3 local = *point;
    gameObject_construct(self, &local, map, arguments);
    o[293] = 8;
    field<float>(o, 760) = 0.0f;
    field<float>(o, 764) = 0.0f;
    field<float>(o, 768) = 0.0f;
    field<unsigned>(o, 72) = low32(vt_Creature);
    field<unsigned>(o, 576) = 0;
    field<unsigned>(o, 752) = 0;
    field<Point3>(o, 776) = local;
    field<float>(o, 224) = 1.0f;
    field<float>(o, 228) = 0.0f;
    field<float>(o, 192) = 0.0f;
    field<unsigned>(o, 12) |= 0x20u;
    field<short>(o, 56) = type;
    field<float>(o, 564) = scale;
    field<unsigned long long>(o, 552) = 0xffffffffffffffffull;
    field<unsigned>(o, 888) = 0;
    field<float>(o, 560) = scale;
    field<int>(o, 580) = -1;
    field<int>(o, 584) = -1;
    field<unsigned>(o, 708) = 0;
    field<unsigned>(o, 712) = 0;
    field<short>(o, 284) = 0;
    field<unsigned>(o, 744) = 0;
    o[570] = 0;
    field<unsigned>(o, 256) = 0;
    field<unsigned>(o, 716) = 0;
    field<short>(o, 792) = 0;
    field<short>(o, 796) = 0;
    field<unsigned>(o, 808) = 0;
    field<unsigned>(o, 812) = 0;
    o[800] = 0;
    o[76] = 0;
    o[85] = 0xff;
    o[156] = 0;
    field<float>(o, 60) = field<float>(o, 24);
    const unsigned char spinner = gameCreatureThinkSpinner;
    field<float>(o, 788) = 1000.0f;
    field<unsigned>(o, 208) = 0;
    field<unsigned>(o, 216) = 0;
    field<unsigned>(o, 212) = 0;
    gameCreatureThinkSpinner = static_cast<unsigned char>(spinner + 1);
    o[569] = spinner;
    if (cutMode == 0) {
        char* script = findTagString__10GameObjectPci(self, "initScript", 0);
        if (script != nullptr) scriptStart__FPc(script);
    }
    if (findTagInt__10GameObjectPci(self, "wait", 0) != 0) o[570] = static_cast<unsigned char>(findTagInt__10GameObjectPci(self, "wait", 0));
    if (findTagInt__10GameObjectPci(self, "hide", 0) != 0) {
        o[570] = 3;
        objectRemoveFromGrid__FP10GameObject(self);
    }
    if (findTagString__10GameObjectPci(self, "ShowName", 0) != nullptr) {
        field<unsigned>(o, 892) = low32(findTagString__10GameObjectPci(self, "ShowName", 0));
        field<unsigned>(o, 12) |= 0x100000u;
    }
    if (findTagInt__10GameObjectPci(self, "Dead", 0) != 0) field<float>(o, 192) = 1000.0f;
    if (findTagInt__10GameObjectPci(self, "HomeRange", 0) != 0) field<float>(o, 788) = static_cast<float>(findTagInt__10GameObjectPci(self, "HomeRange", 0));
    if (findTagInt__10GameObjectPci(self, "Scale", 0) != 0) field<float>(o, 224) = static_cast<float>(findTagInt__10GameObjectPci(self, "Scale", 0)) * 0.01f;
    if (findTagInt__10GameObjectPci(self, "MiniBoss", 0) != 0) field<unsigned long long>(o, 160) |= 1ull;
    if (findTagString__10GameObjectPci(self, "light1", 0) != nullptr || findTagString__10GameObjectPci(self, "light2", 0) != nullptr ||
        findTagString__10GameObjectPci(self, "ambient", 0) != nullptr)
        field<unsigned long long>(o, 160) |= 0x40000000ull;
    field<unsigned short>(o, 798) = field<unsigned short>(o, 56);
}

// Observed __15GlowingCylinderG6Point3iiii at 0x001e5828, 352 bytes. Arguments: object, a position
// and four ints (called A, B, C, D here). In the order of the image:
//  - the GameObject constructor runs with a copy of the position, a null resource map and a null
//    argument list;
//  - word +12 gets 0x4009 ORed in, A is stored as a word at +76, the table pointer
//    (_vt$15GlowingCylinder) goes to +72, B, C and D are stored as halfwords at +80, +82 and +84;
//  - objectAddToRunList__FP10GameObject runs, then byte +40 is set to 48;
//  - P_StartTrail__Fiifiiiii(1, 244, 0.4 (bits 0x3ecccccc), 4, 86, 126, 250, 200) runs and its result
//    is stored at +88; P_ExtendTrail__FiG6Point3 runs with that result and a copy of the 12 bytes at
//    +16;
//  - the same two calls again, with the second result stored at +92.
// Nothing is returned. Inferred: a trail is an effect handle; the object keeps two of them.
// Unknown: the meaning of A to D and of the trail arguments. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 position, void* resourceMap, char** arguments);
void objectAddToRunList__FP10GameObject(void* object);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
extern unsigned char vt_GlowingCylinder[] __asm__("_vt$15GlowingCylinder");
}

extern "C" void __15GlowingCylinderG6Point3iiii(void* self, Point3 position, int a, int b, int c, int d) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const float trailParameter = 0x1.999998p-2f;
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(self, position, nullptr, nullptr);
    *reinterpret_cast<unsigned*>(object + 12) |= 0x4009u;
    *reinterpret_cast<int*>(object + 76) = a;
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_GlowingCylinder));
    *reinterpret_cast<short*>(object + 80) = static_cast<short>(b);
    *reinterpret_cast<short*>(object + 82) = static_cast<short>(c);
    *reinterpret_cast<short*>(object + 84) = static_cast<short>(d);
    objectAddToRunList__FP10GameObject(self);
    object[40] = 48;
    const int first = P_StartTrail__Fiifiiiii(1, 244, trailParameter, 4, 86, 126, 250, 200);
    *reinterpret_cast<int*>(object + 88) = first;
    P_ExtendTrail__FiG6Point3(first, *reinterpret_cast<Point3*>(object + 16));
    const int second = P_StartTrail__Fiifiiiii(1, 244, trailParameter, 4, 86, 126, 250, 200);
    *reinterpret_cast<int*>(object + 92) = second;
    P_ExtendTrail__FiG6Point3(second, *reinterpret_cast<Point3*>(object + 16));
}

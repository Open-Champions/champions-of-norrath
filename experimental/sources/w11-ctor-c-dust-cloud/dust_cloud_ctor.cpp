// Observed __9dustCloudG6Point3iiii at 0x001e6828, 228 bytes. Arguments: object, a position and
// four ints (called A, B, C, D here). In the order of the image:
//  - the GameObject constructor runs with a copy of the position, a null resource map and a null
//    argument list;
//  - the table pointer (_vt$9dustCloud) goes to +72 and the position is copied again to +76
//    (12 bytes);
//  - C goes to +100, D to +104, frameNm + A to +88, and B to +92, raised to 10 when it is below 10;
//  - word +12 gets 3 ORed in, 10.0f is stored at +96, and objectAddToRunList__FP10GameObject runs.
// Nothing is returned. Inferred: +88 is a frame number at which the cloud ends (the sum with the
// live frame counter); the rest of the fields are only known by position.
// Unknown: the meaning of B, C, D and of the float at +96. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 position, void* resourceMap, char** arguments);
void objectAddToRunList__FP10GameObject(void* object);
extern unsigned char vt_dustCloud[] __asm__("_vt$9dustCloud");
extern int frameNm;
}

extern "C" void __9dustCloudG6Point3iiii(void* self, Point3 position, int a, int b, int c, int d) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(self, position, nullptr, nullptr);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_dustCloud));
    *reinterpret_cast<Point3*>(object + 76) = position;
    *reinterpret_cast<int*>(object + 100) = c;
    *reinterpret_cast<int*>(object + 104) = d;
    *reinterpret_cast<int*>(object + 88) = frameNm + a;
    *reinterpret_cast<int*>(object + 92) = b;
    if (b < 10) *reinterpret_cast<int*>(object + 92) = 10;
    *reinterpret_cast<unsigned*>(object + 12) |= 3u;
    *reinterpret_cast<float*>(object + 96) = 10.0f;
    objectAddToRunList__FP10GameObject(self);
}

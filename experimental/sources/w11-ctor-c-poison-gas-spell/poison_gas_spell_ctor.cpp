// Observed __14poisonGasSpellG6Point3iiiP10GameObjecti at 0x001e6708, 276 bytes. Arguments: object, a
// position, three ints (A, B, C), a GameObject pointer (L) and a last int (D). In the order of the image:
//  - the GameObject constructor runs with a copy of the position, a null resource map and a null
//    argument list;
//  - the word at +76 is cleared, the table pointer (_vt$14poisonGasSpell) goes to +72, the position is
//    copied to +80 (12 bytes), B goes to +96, frameNm + A to +92, C to +104 and D to +108;
//  - when L is not null, listOnewayLink__FP8ListHeadT0Pv(object + 76, L, L) runs;
//  - the word at +96 is raised to 10 when it is below 10 (signed), word +12 gets 3 ORed in, 10.0f is
//    stored at +100 and objectAddToRunList__FP10GameObject runs.
// Nothing is returned. Inferred: the cleared word at +76 is the head of a one-way list that L joins
// (it is the head argument of the link call); +92 is an end frame.
// Unknown: the meaning of B, C, D and of the float at +100. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 position, void* resourceMap, char** arguments);
void objectAddToRunList__FP10GameObject(void* object);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
extern unsigned char vt_poisonGasSpell[] __asm__("_vt$14poisonGasSpell");
extern int frameNm;
}

extern "C" void __14poisonGasSpellG6Point3iiiP10GameObjecti(void* self, Point3 position, int a, int b, int c, void* list, int d) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(self, position, nullptr, nullptr);
    *reinterpret_cast<unsigned*>(object + 76) = 0u;
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_poisonGasSpell));
    *reinterpret_cast<Point3*>(object + 80) = position;
    *reinterpret_cast<int*>(object + 96) = b;
    *reinterpret_cast<int*>(object + 92) = frameNm + a;
    *reinterpret_cast<int*>(object + 104) = c;
    *reinterpret_cast<int*>(object + 108) = d;
    if (list) listOnewayLink__FP8ListHeadT0Pv(object + 76, list, list);
    if (*reinterpret_cast<int*>(object + 96) < 10) *reinterpret_cast<int*>(object + 96) = 10;
    *reinterpret_cast<unsigned*>(object + 12) |= 3u;
    *reinterpret_cast<float*>(object + 100) = 10.0f;
    objectAddToRunList__FP10GameObject(self);
}

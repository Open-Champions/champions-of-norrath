// Observed __10SpawnMummyG6Point3PcP10GameObject at 0x0023c150, 228 bytes. Arguments: object, a
// position, a text pointer T and a GameObject pointer L. In the order of the image:
//  - the GameObject constructor runs with a copy of the position, a null resource map and a null
//    argument list;
//  - word +12 gets 9 ORed in and the table pointer of _vt$9DustDevil goes to +72 (the constructor of
//    that parent class is inlined here: the same table, then the steps that follow);
//  - P_CreateParticleSystem__Fv() runs and its result goes to +84; the word at +76 is cleared and
//    frameNm + 60 goes to +80;
//  - objectAddToRunList__FP10GameObject runs;
//  - T goes to +88, +96 is cleared, the table pointer is replaced by _vt$10SpawnMummy, the word at
//    +100 is cleared, listOnewayLink__FP8ListHeadT0Pv(object + 100, L, L) runs (L is not tested for
//    null) and the word at +92 is cleared.
// Nothing is returned. Inferred: the parent is DustDevil (its table and the inlined steps),
// +100 is the head of a one-way list that L joins, +80 is an end frame (60 frames on).
// Unknown: what T and the cleared words are used for. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 position, void* resourceMap, char** arguments);
void* P_CreateParticleSystem__Fv();
void objectAddToRunList__FP10GameObject(void* object);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
extern unsigned char vt_DustDevil[] __asm__("_vt$9DustDevil");
extern unsigned char vt_SpawnMummy[] __asm__("_vt$10SpawnMummy");
extern int frameNm;
}

extern "C" void __10SpawnMummyG6Point3PcP10GameObject(void* self, Point3 position, char* text, void* list) {
    unsigned char* object = static_cast<unsigned char*>(self);
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(self, position, nullptr, nullptr);
    *reinterpret_cast<unsigned*>(object + 12) |= 9u;
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_DustDevil));
    *reinterpret_cast<unsigned*>(object + 84) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(P_CreateParticleSystem__Fv()));
    *reinterpret_cast<unsigned*>(object + 76) = 0u;
    *reinterpret_cast<int*>(object + 80) = frameNm + 60;
    objectAddToRunList__FP10GameObject(self);
    *reinterpret_cast<unsigned*>(object + 88) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(text));
    *reinterpret_cast<unsigned*>(object + 96) = 0u;
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_SpawnMummy));
    *reinterpret_cast<unsigned*>(object + 100) = 0u;
    listOnewayLink__FP8ListHeadT0Pv(object + 100, list, list);
    *reinterpret_cast<unsigned*>(object + 92) = 0u;
}

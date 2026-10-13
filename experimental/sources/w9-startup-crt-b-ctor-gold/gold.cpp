// Observed __4GoldG6Point3i at 0x00154cc0, 188 bytes: the Gold constructor, with
// a copy of the position and one integer. In the order of the image:
//  - the position is copied to a local and GameObject's constructor
//    (0x001411f0) runs on the object with it, a null resource map and null
//    arguments;
//  - the vtable _vt$4Gold (0x0044cd48) goes to object+72; byte object+93 (the
//    animation state at +88, byte 5) becomes 1; the integer goes to object+128;
//    object+12 gets bits 0x220c set; object+76 becomes 0;
//  - animInitState (0x001637a8) runs on object+88; the word frameNm
//    (0x004844a4) is copied to object+80; the animation header is looked up
//    from the resource map goldAnm (0x00600d20) and added to the state at +88
//    with flags 516;
//  - objectAddToRunList runs on the object and its result is returned.
// Inferred: object+88 is an AnimationState (from the callee names); the names
// of the offsets are not known. Unknown: what the integer at +128 means.
struct Point3 { float x, y, z; };

extern "C" {
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 position, void* resource_map, char** arguments);
void animInitState__FP14AnimationState(void* state);
void* get__11ResourceMap(void* map);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
unsigned objectAddToRunList__FP10GameObject(void* object);
extern unsigned char _vt$4Gold[];
extern unsigned char goldAnm[];
extern unsigned frameNm;

unsigned __4GoldG6Point3i(void* self, Point3 position, int value) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* state = object + 88;
    __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(self, position, nullptr, nullptr);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$4Gold));
    state[5] = 1;
    *reinterpret_cast<int*>(object + 128) = value;
    *reinterpret_cast<unsigned*>(object + 12) |= 0x220cu;
    object[76] = 0;
    animInitState__FP14AnimationState(state);
    *reinterpret_cast<unsigned*>(object + 80) = frameNm;
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, get__11ResourceMap(goldAnm), 516);
    return objectAddToRunList__FP10GameObject(self);
}
}

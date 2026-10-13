#include "../w11-actor-c-common/c.hpp"
// Observed __5WheelPcifP7Vehiclei at 0x00276fc8, 436 bytes. Arguments: the wheel w, a resource name, an int
// a, a float f, the owning vehicle v and an int b. In order:
//   1. GameObject constructor (position = the three floats at v+16, null resource map, null argument list).
//   2. Word at w+12 gets 0x4001 or-ed in, the byte at w+89 is 8, the word at w+396 is v and the word at w+72
//      is the table _vt$5Wheel.
//   3. The name is copied with strcpy, no length check, into a local buffer on the stack (at most 128 bytes fit before the saved registers); strstr finds ".vif" in
//      the copy and the five bytes there (".vif" and its next byte) are overwritten with ".tex" and a
//      terminator. The result of strstr is not tested, so a name without ".vif" writes through a null pointer.
//   4. Word at w+76 is lumpFindResource(v+76, name) and the word at w+80 is lumpFindResource(v+76, the changed
//      copy): the model and its texture, by file extension.
//   5. The 64-bit value at w+352 is modelGetActiveChunks(w[+76], 0); the object is added to the run list.
//   6. Byte w+40 = 24, float w+28 = 24.0. modelGetBoundingBox(w[+76], chunks, ...) fills six ints in a local
//      array; with d = (sixth - third) the float at w+376 is (d/2 as int, rounded toward zero) * 0.025641024
//      (0x3cd20d20, about 1/39), the float at w+380 is twice that times 3.1415925 (0x40490fda), the float at w+372
//      is f, the word at w+384 is b and the word at w+388 is a.
// Inferred: a wheel's model is found from its name, with the texture beside it; +376/+380 look like a
// radius and a circumference (2*pi*r) of the model scaled to world units. Unknown: the ordering of the six
// bounding-box ints and thus which axis is measured, and the meaning of a, b and f. The function returns
// whatever is left in the result register after the float arithmetic (an int), which callers should not
// use; it is declared void here. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
void* gameobject_construct(void* self, Point3 position, void* resource_map, char** arguments)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
extern unsigned char vt_Wheel[] __asm__("_vt$5Wheel");
char* strcpy(char* destination, const char* source);
char* strstr(const char* haystack, const char* needle);
void* lumpFindResource__FPcT0(void* lump, const char* name);
long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int arg);
void objectAddToRunList__FP10GameObject(void* object);
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* model, long long chunks, int*, int*, int*, int*, int*, int*);

void wheel_construct(void* self, const char* name, int a, float f, void* vehicle, int b) __asm__("__5WheelPcifP7Vehiclei");
void wheel_construct(void* self, const char* name, int a, float f, void* vehicle, int b) {
    using namespace w11c;
    const Point3 position = *reinterpret_cast<const Point3*>(bytes(vehicle) + 16);
    gameobject_construct(self, position, nullptr, nullptr);
    word(self, 12) |= 0x4001u;
    ubyte(self, 89) = 8;
    word(self, 396) = to_word(vehicle);
    word(self, 72) = to_word(vt_Wheel);
    char copy[80];
    strcpy(copy, name);
    char* extension = strstr(copy, ".vif");
    extension[0] = '.'; extension[1] = 't'; extension[2] = 'e'; extension[3] = 'x'; extension[4] = '\0';
    word(self, 76) = to_word(lumpFindResource__FPcT0(bytes(vehicle) + 76, name));
    word(self, 80) = to_word(lumpFindResource__FPcT0(bytes(vehicle) + 76, copy));
    void* const model = link(self, 76);
    *reinterpret_cast<long long*>(bytes(self) + 352) = modelGetActiveChunks__FP12_modelHeaderi(model, 0);
    objectAddToRunList__FP10GameObject(self);
    ubyte(self, 40) = 24;
    real(self, 28) = 24.0f;
    int box[6];
    modelGetBoundingBox__FP12_modelHeaderlPiN52(link(self, 76), *reinterpret_cast<long long*>(bytes(self) + 352),
                                                &box[0], &box[1], &box[2], &box[3], &box[4], &box[5]);
    word(self, 388) = static_cast<unsigned>(a);
    word(self, 384) = static_cast<unsigned>(b);
    real(self, 372) = f;
    const float radius = static_cast<float>((box[5] - box[2]) / 2) * __builtin_bit_cast(float, 0x3cd20d20u);
    real(self, 376) = radius;
    real(self, 380) = (radius * __builtin_bit_cast(float, 0x40490fdau)) * 2.0f;
}
}

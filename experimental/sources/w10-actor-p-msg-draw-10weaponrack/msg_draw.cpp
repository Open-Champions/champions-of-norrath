// Observed msg_draw__10WeaponRack at 0x00216118, 392 bytes. D is the item description embedded at
// object+80, P the point at +16, G the global item table (_4item$globalItem, 16-byte entries).
//  1. The mesh index from getProxyMeshId__4itemPQ24item11description(D) selects an entry of G;
//     its word at +8 is kept as the model. The call is made a second time and the word at +12 of
//     that entry is kept as the texture. Both words are read before anything else is called.
//  2. matMakeTransYaw__FP8Matrix34G6Point3s builds the local matrix from the point
//     (P.x - 0, P.y, P.z - 14.0) and the yaw (unsigned halfword at +56, plus 3000, as a short).
//  3. setupHalo__4itemPQ24item11descriptionf(D, 5.0).
//  4. When the byte at offset 30 of the block that the first word of D points to is below 2 (unsigned),
//     matTrans__FP8Matrix34fff(matrix, 0, 0, 28.0) and matRot__FP8Matrix34is(matrix, 0, -16384) run;
//     otherwise only matRot(matrix, 0, 16384) runs.
//  5. modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi
//     is called with the model, the texture, 3, a copy of P, the matrix, no animation state, -1,
//     no draw record and 0. Its result is not used.
// Unknown: what the three numbers 14, 28 and 3000 are for, and what offset 30 of the description
// block selects. Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
extern unsigned char globalItem[] __asm__("_4item$globalItem");
int getProxyMeshId__4itemPQ24item11description(void* description);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, Point3 position, short yaw);
void setupHalo__4itemPQ24item11descriptionf(void* description, float radius);
void matTrans__FP8Matrix34fff(void* matrix, float x, float y, float z);
void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int a, Point3 position, void* matrix, void* animation, unsigned long mask, void* record, int b);
}

extern "C" void msg_draw__10WeaponRack(void* self) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const description = o + 80;
    const Point3& position = *reinterpret_cast<const Point3*>(o + 16);
    void* const model = from_word(word(globalItem + (getProxyMeshId__4itemPQ24item11description(description) << 4), 8));
    void* const texture = from_word(word(globalItem + (getProxyMeshId__4itemPQ24item11description(description) << 4), 12));
    float matrix[12];
    const Point3 offset = {0.0f, 0.0f, 14.0f};
    const Point3 local = {position.x - offset.x, position.y, position.z - offset.z};
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, local, static_cast<short>(static_cast<unsigned short>(half(o, 56)) + 3000));
    setupHalo__4itemPQ24item11descriptionf(description, 5.0f);
    if (from_word(word(description, 0))[30] < 2) {
        matTrans__FP8Matrix34fff(matrix, 0.0f, 0.0f, 28.0f);
        matRot__FP8Matrix34is(matrix, 0, -16384);
    } else {
        matRot__FP8Matrix34is(matrix, 0, 16384);
    }
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(model, texture, 3, position, matrix, nullptr,
                                                                                               static_cast<unsigned long>(-1), nullptr, 0);
}

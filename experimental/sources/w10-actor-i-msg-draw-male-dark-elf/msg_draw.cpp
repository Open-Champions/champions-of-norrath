// Observed msg_draw__11MaleDarkElf at 0x00287610, 380 bytes.
// Let b be the resource block whose pointer is the word at object+36, and i the word at object+964.
// When i is nonzero, fxRunWeapon is called first with: the description at object+1040; the word at
// b+8*i+4; the weapon state at object+972; a stack copy of the 12 bytes at object+16; the halfword
// at object+56; b's word +12; the animation state at object+288; a flag that is 1 when the halfword
// at object+572 is 6, 13 or 14 and otherwise 0; then 0.5f, 1.0f, 0 and a null matrix pointer.
// Then draw__8Creature(object, b[12], b[20], 0) gives a draw record r. For each of the words at
// object+964, +968 and +1112 in that order, when nonzero (k), modelDrawAttachment is called with the
// block's words 8*k+4 and 8*k+12 (b re-read from the object), 0, b[12], r, -1, -1 and 0.
// Inferred: the three words index attachments in the resource block (a weapon, and two more
// pieces); the flag marks states in which the weapon effect is active. Unknown: what states 6, 13
// and 14 are. Nothing here ran on the EE.
extern "C" {
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* header, void* weapon_state, const void* position, short heading, void* header_again,
    void* animation, float first, int flag, int zero, float second, void* matrix);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* header, void* texture, void* other);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* header, void* texture, int index, void* header_again, void* record, unsigned long mask, int a, int b);
}

namespace {
struct Point3 { float x, y, z; };
}

extern "C" void msg_draw__11MaleDarkElf(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<unsigned*>(object + 964) != 0u) {
        unsigned char* block = *reinterpret_cast<unsigned char**>(object + 36);
        const unsigned index = *reinterpret_cast<unsigned*>(object + 964);
        const int state = *reinterpret_cast<short*>(object + 572);
        const Point3 position = *reinterpret_cast<const Point3*>(object + 16);
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            object + 1040, *reinterpret_cast<void**>(block + index * 8 + 4), object + 972, &position,
            *reinterpret_cast<short*>(object + 56), *reinterpret_cast<void**>(block + 12), object + 288, 0.5f,
            state == 6 || state == 13 || state == 14 ? 1 : 0, 0, 1.0f, nullptr);
    }
    unsigned char* block = *reinterpret_cast<unsigned char**>(object + 36);
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(
        object, *reinterpret_cast<void**>(block + 12), *reinterpret_cast<void**>(block + 20), nullptr);
    static const unsigned offsets[3] = {964u, 968u, 1112u};
    for (int n = 0; n < 3; ++n) {
        const unsigned index = *reinterpret_cast<unsigned*>(object + offsets[n]);
        if (index == 0u) continue;
        block = *reinterpret_cast<unsigned char**>(object + 36);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            *reinterpret_cast<void**>(block + index * 8 + 4), *reinterpret_cast<void**>(block + index * 8 + 12), 0,
            *reinterpret_cast<void**>(block + 12), record, 0xffffffffu, -1, 0);
    }
}

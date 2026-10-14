// Observed msg_draw__7Critter at 0x001f1810, 228 bytes.
// Saves the 12 bytes at object+16 (a Point3) in a local, adds the float at object+1080 to the float
// at object+24 and stores the sum back, then draws and finally restores all 12 bytes from the
// local. Let s be the signed halfword at +572, f the float at +1080, model the word at +1036 and
// texture the word at +1040:
//  - s is 14 or 16: fade = (int)(f * 0.5); modelSetFade__Fc is called with fade as a signed char;
//    the Creature draw routine runs (object, model, texture, 0) only when fade < 98 (signed); then
//    modelResetOptionalParams__Fv is called;
//  - any other s: the Creature draw routine runs (object, model, texture, 0).
// The draw result is not used. Nothing is returned.
// Inferred: the +24 shift lifts the drawn model by f and the fade applies while in states 14 and 16.
// Unknown: what those states are, and the type of the words at +1036 and +1040 beyond the draw
// routine's parameter types (model header and texture). Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
void modelSetFade__Fc(char value);
void modelResetOptionalParams__Fv();
}

namespace {
struct Point3 {
    float x, y, z;
};
}  // namespace

extern "C" void msg_draw__7Critter(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    Point3* position = reinterpret_cast<Point3*>(object + 16);
    const float lift = *reinterpret_cast<const float*>(object + 1080);
    const Point3 saved = *position;
    const short state = *reinterpret_cast<short*>(object + 572);
    *reinterpret_cast<float*>(object + 24) = saved.z + lift;
    if (state == 14 || state == 16) {
        const int fade = static_cast<int>(lift * 0.5f);
        modelSetFade__Fc(static_cast<char>(fade));
        if (fade < 98)
            draw__8CreatureP12_modelHeaderP8_textureT1(self, w10_actor_d::word_at(object, 1036), w10_actor_d::word_at(object, 1040), nullptr);
        modelResetOptionalParams__Fv();
    } else {
        draw__8CreatureP12_modelHeaderP8_textureT1(self, w10_actor_d::word_at(object, 1036), w10_actor_d::word_at(object, 1040), nullptr);
    }
    *position = saved;
}

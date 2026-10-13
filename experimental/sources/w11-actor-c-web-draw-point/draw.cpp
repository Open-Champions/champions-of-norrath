#include "../w11-actor-c-common/c.hpp"
// Observed draw__3WebG6Point3 at 0x0020dde8, 588 bytes. Arguments: the web w and a Point3 P (passed by
// address; copied to a local first). Fields used: int at w+0, float at w+4, int at w+8, word at w+12,
// the Point3 at w+16 (V), and the pointer word at w+28 (the list node that start__3Web... linked; its word
// at +12 is a creature).
//   Nothing is drawn when the int at w+8 is not positive. Otherwise M = P + V and Q = M * 0.5 are formed
//   componentwise, and P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(P, Q, 255, 255, 255, low byte of w[8], 255, 255,
//   255, 0, 0.0) draws a line. Then DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState
//   is called once with (Q, 0, V, w[8], w[0], w[4], 5, creature, model, animation) where, with c the
//   creature of the node at w+28 (when the pointer is nonzero):
//   - w[12] nonzero and the node nonzero and bit 5 (0x20) of c's word at +12 set: model is the result of the
//     table slot at byte 128 (table at c+72, called on c) and animation is c+288;
//   - w[12] nonzero and the node nonzero but that bit clear: the same model, creature c, animation null;
//   - otherwise (w[12] zero, or no node): creature, model and animation are all zero.
// Inferred: the draw routine for the spider web's strand, with the creature-dependent splay drawn when a
// creature is held; the slot at byte 128 is the GameObject getModel slot in the published Sparks table.
// The meaning of the numeric arguments is not established. Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
void P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(Point3 from, Point3 to, unsigned char a, unsigned char b, unsigned char c, unsigned char d,
                                          unsigned char e, unsigned char f, unsigned char g, unsigned char h, float width);
void DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(Point3 a, int b, Point3 c, int d, int e, int f, float g,
                                                                                   void* creature, void* model, void* animation);
}

extern "C" void draw__3WebG6Point3(void* self, Point3 point) {
    using namespace w11c;
    Point3 p = point;
    if (static_cast<int>(word(self, 8)) <= 0) return;
    const float mx = p.x + real(self, 16);
    const float my = p.y + real(self, 20);
    const float mz = p.z + real(self, 24);
    Point3 q = {mx * 0.5f, my * 0.5f, mz * 0.5f};
    P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(p, q, 255, 255, 255, static_cast<unsigned char>(word(self, 8)), 255, 255, 255, 0, 0.0f);
    Point3 v = {real(self, 16), real(self, 20), real(self, 24)};
    const int count = static_cast<int>(word(self, 8));
    if (word(self, 12) != 0u && word(self, 28) != 0u) {
        unsigned char* creature = link(link(self, 28), 12);
        if ((word(creature, 12) & 0x20u) != 0u) {
            void* model = call_slot(creature, 128);
            DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
                q, 0, v, count, static_cast<int>(word(self, 0)), 5, real(self, 4), creature, model, creature + 288);
        } else {
            void* model = call_slot(creature, 128);
            DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
                q, 0, v, count, static_cast<int>(word(self, 0)), 5, real(self, 4), creature, model, nullptr);
        }
        return;
    }
    DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(
        q, 0, v, count, static_cast<int>(word(self, 0)), 5, real(self, 4), nullptr, nullptr, nullptr);
}

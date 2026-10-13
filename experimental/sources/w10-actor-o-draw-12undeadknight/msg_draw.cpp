// Observed msg_draw__12UndeadKnight at 0x0023c638, 352 bytes. Let R be the block at the word
// object+36 and w the word at object+960.
//  1. When w is nonzero: the 12 bytes at object+16 are overwritten with 12 bytes copied from the
//     function's own outgoing-argument slot at its stack pointer, which the image never writes
//     before this point (a stack residue; the value is whatever the caller's frame left). The
//     halfword at +56 becomes 0. modelGetCharAttachmentPos(R[12], the point {0,0,0}, 0, the
//     animation state at +288, 9, &A, &B, the float at +224) fills the two stack points A and B.
//     The position at +16 then becomes {x - A.x, y - A.y, z - A.z}. B is not read.
//  2. draw(this, R[12], R[20], 0) draws the creature and returns a record (R is read again).
//  3. When the word at +968 is nonzero: modelDrawAttachment(R[180], R[188], 0, R[12], the record, -1,
//     -1, 0). When the word at +1112 is nonzero: the same with R[196] and R[204].
// Unknown: whether the stack-residue copy is a defect of the original (it looks like a saved
// position that was never saved); the authored code reads an uninitialised local through an empty
// asm statement, so its value is as indeterminate as the image's. What the attachments are is
// unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 point, short yaw, void* state, int index, Point3* first, Point3* second, float scale);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model2, void* record,
                                                                         unsigned long b, int c, int d);

void msg_draw__12UndeadKnight(void* self) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3* const position = reinterpret_cast<Point3*>(o + 16);
    void* record;
    if (word(o, 960) != 0) {
        Point3 residue;
        __asm__("" : "=m"(residue));
        *position = residue;
        Point3 first;
        Point3 second;
        const Point3 zero = {0.0f, 0.0f, 0.0f};
        half(o, 56) = 0;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            from_word(word(from_word(word(o, 36)), 12)), zero, 0, o + 288, 9, &first, &second, real(o, 224));
        position->x = position->x - first.x;
        position->y = position->y - first.y;
        position->z = position->z - first.z;
        unsigned char* const group = from_word(word(o, 36));
        record = draw__8CreatureP12_modelHeaderP8_textureT1(self, from_word(word(group, 12)), from_word(word(group, 20)), nullptr);
    } else {
        unsigned char* const group = from_word(word(o, 36));
        record = draw__8CreatureP12_modelHeaderP8_textureT1(self, from_word(word(group, 12)), from_word(word(group, 20)), nullptr);
    }
    if (word(o, 968) != 0) {
        unsigned char* const group = from_word(word(o, 36));
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            from_word(word(group, 180)), from_word(word(group, 188)), 0, from_word(word(group, 12)), record, static_cast<unsigned long>(-1), -1, 0);
    }
    if (word(o, 1112) != 0) {
        unsigned char* const group = from_word(word(o, 36));
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            from_word(word(group, 196)), from_word(word(group, 204)), 0, from_word(word(group, 12)), record, static_cast<unsigned long>(-1), -1, 0);
    }
}
}

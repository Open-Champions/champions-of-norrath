// Observed msg_draw__6PhProp at 0x0014a468, 420 bytes. In order:
//   1. the twelve floats at +272..+316 (a Matrix34, address object+272) are loaded from +468, +480, +492,
//      +472, +484, +496, +476, +488, +500, +416, +420, +424 respectively;
//   2. matScale__FP8Matrix34f(matrix, float +576); matTrans__FP8Matrix34fff(matrix, -float +240,
//      -float +244, -float +248);
//   3. when the byte at +568 is non-zero: with frameNm the global frame counter, if (frameNm - 3) is less
//      than the int at +260, or the doubleword at (word +328) + 40 has bit 39 set, then byte 3 of the
//      function-local 8-byte table `halos` is set to (int)(|isin(frameNm << 9)| * 12.0 + 12.0) and
//      modelSetHalo__FP7HaloDefi(halos, 1) is called; otherwise setupHalo__4itemPQ24item11descriptionf
//      is called with the address object+328 and 5.0;
//   4. modelDraw(word +76, word +80, 1, point (floats +416..+424 passed by value), matrix, null, the
//      doubleword at +232, null, 0);
//   5. when the byte at +568 is non-zero, modelResetOptionalParams__Fv().
// The static table initially holds 0xff,0xff,0xff,0xff then the single 409.0 (bytes 00 80 cc 43). Inferred:
// the table is one halo definition (colour bytes and a size) from the table's use by modelSetHalo; the
// pulse varies the fourth colour byte. Unknown: the roles of +568 and +576 beyond the names of the calls.
// Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern int frameNm;
float isin__Fi(int angle);
void matScale__FP8Matrix34f(void* matrix, float scale);
void matTrans__FP8Matrix34fff(void* matrix, float x, float y, float z);
void modelSetHalo__FP7HaloDefi(void* table, int count);
void setupHalo__4itemPQ24item11descriptionf(void* description, float size) __asm__("setupHalo__4itemPQ24item11descriptionf");
void modelDraw(void* model, void* texture, int mode, Point3 point, void* matrix, void* animation, unsigned long long flags,
               void* record, int last) __asm__("modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi");
void modelResetOptionalParams__Fv();

void msg_draw__6PhProp(void* self) {
    using namespace w11b;
    static unsigned char halos[8] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x80, 0xcc, 0x43};
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* matrix = o + 272;
    real(o, 272) = real(o, 468);
    real(o, 276) = real(o, 480);
    real(o, 280) = real(o, 492);
    real(o, 284) = real(o, 472);
    real(o, 288) = real(o, 484);
    real(o, 292) = real(o, 496);
    real(o, 296) = real(o, 476);
    real(o, 300) = real(o, 488);
    real(o, 304) = real(o, 500);
    real(o, 308) = real(o, 416);
    real(o, 312) = real(o, 420);
    real(o, 316) = real(o, 424);
    matScale__FP8Matrix34f(matrix, real(o, 576));
    matTrans__FP8Matrix34fff(matrix, -real(o, 240), -real(o, 244), -real(o, 248));
    if (*reinterpret_cast<signed char*>(o + 568) != 0) {
        const int frame = frameNm;
        if (frame - 3 < sword(o, 260) || (dword(from_word(word(o, 328)), 40) & 0x8000000000ull) != 0) {
            float s = isin__Fi(frame << 9);
            if (s < 0.0f) s = -s;
            halos[3] = static_cast<unsigned char>(static_cast<int>(s * 12.0f + 12.0f));
            modelSetHalo__FP7HaloDefi(halos, 1);
        } else {
            setupHalo__4itemPQ24item11descriptionf(o + 328, 5.0f);
        }
    }
    Point3 point = *reinterpret_cast<Point3*>(o + 416);
    modelDraw(from_word(word(o, 76)), from_word(word(o, 80)), 1, point, matrix, nullptr, dword(o, 232), nullptr, 0);
    if (*reinterpret_cast<signed char*>(o + 568) != 0) modelResetOptionalParams__Fv();
}
}

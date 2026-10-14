#include "../w11-actor-c-common/c.hpp"
// Observed addSmokelet__10StaticFire at 0x0028f118, 300 bytes. The object s keeps the same ring buffer of
// 10-byte records of five halfwords as updateSmokelets (base: the word at s+1188; capacity, head and tail
// halfwords at s+118, s+120 and s+122). Steps:
//   1. Draw a vector by rejection: each component is (int)eRand(s) * 2^-16 doubled and lowered by one, drawn x, y,
//      z in turn; the three draws repeat while 1.0 < x*x + y*y + z*z.
//   2. Scale it by (float at s+28) * (float at s+100) and replace the z component by its absolute value
//      (compare against zero, so NaN keeps its sign).
//   3. Let next = head + 1, set to 0 when it equals the capacity. When next equals the tail the ring is full and
//      nothing else happens.
//   4. Otherwise, in the record at the old head: halfwords 0, 2 and 4 get the scaled vector times 16.0 truncated to
//      int (low 16 bits stored); halfword 6 gets (int)(((int)eRand(s) * 2^-16 - 0.5) * k * 224.0) and halfword 8 the
//      same with a second draw, where k is the single-precision constant 1/60 that the original reads from a
//      global-pointer-relative word (value 0x3c888889). The head halfword is then set to next.
// Inferred: the records are particles with a position (fields 0..4) and a velocity (fields 6, 8), as updateSmokelets
// moves them; the 1/60 factor suggests a per-frame time step. Names unestablished. Nothing here ran on the EE.
extern "C" unsigned eRand__10GameObject(void* self);

extern "C" void addSmokelet__10StaticFire(void* self) {
    using namespace w11c;
    const float unit = __builtin_bit_cast(float, 0x37800000u);
    float x, y, z;
    do {
        x = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * unit;
        x = x + x - 1.0f;
        y = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * unit;
        y = y + y - 1.0f;
        z = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * unit;
        z = z + z - 1.0f;
    } while (1.0f < x * x + y * y + z * z);
    const float scale = real(self, 28) * real(self, 100);
    x = x * scale;
    y = y * scale;
    z = z * scale;
    if (z < 0.0f) z = -z;
    const int head = shalf(self, 120);
    int next = head + 1;
    if (next == shalf(self, 118)) next = 0;
    if (next == shalf(self, 122)) return;
    unsigned char* record = from_word(word(self, 1188)) + head * 10;
    const float sixteen = 16.0f;
    half(record, 0) = static_cast<unsigned short>(static_cast<int>(x * sixteen));
    half(record, 2) = static_cast<unsigned short>(static_cast<int>(y * sixteen));
    half(record, 4) = static_cast<unsigned short>(static_cast<int>(z * sixteen));
    const float step = __builtin_bit_cast(float, 0x3c888889u);
    const float half_one = 0.5f;
    float v = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * unit;
    half(record, 6) = static_cast<unsigned short>(static_cast<int>((v - half_one) * step * 224.0f));
    v = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * unit;
    half(record, 8) = static_cast<unsigned short>(static_cast<int>((v - half_one) * step * 224.0f));
    half(self, 120) = static_cast<unsigned short>(next);
}

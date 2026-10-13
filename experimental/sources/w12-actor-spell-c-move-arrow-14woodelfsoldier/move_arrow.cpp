// Observed moveArrow__14WoodElfSoldier at 0x00249a98, 220 bytes. The argument is the soldier S. Only acts when
// the word at +988 is non-zero. Then, with A the signed halfword at +984:
//  - V = (icos(A) * 400, isin(A) * 400, isin(-8192) * 400), computed in that order (icos__Fi / isin__Fi
//    return floats and take the integer angle; A is read again for each call);
//  - D = V * k componentwise, where k is the float the image reads from its global pool at 0x00482008
//    (stored bits 0x3c888888, about 0.0166666657; no symbol names it, so the bits are written here);
//  - the three floats at +972 (x, y, z) are increased by D;
//  - when the updated float at +980 (the third component) is below 0.0 the word at +988 is cleared.
// Inferred: +972 is a position and +980 its third component (it is the one that receives the third
// term). Unknown: what +984 and +988 mean (an angle and an active flag are guesses from use) and whether k is a
// time step. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
float icos__Fi(int angle);
float isin__Fi(int angle);

void moveArrow__14WoodElfSoldier(void* self) {
    using namespace w12c;
    unsigned char* const s = static_cast<unsigned char*>(self);
    if (word(s, 988) == 0u) return;
    const float speed = 400.0f;
    const float x = icos__Fi(half(s, 984)) * speed;
    const float y = isin__Fi(half(s, 984)) * speed;
    const float z = isin__Fi(-8192) * speed;
    const float k = bits(0x3c888888u);
    real(s, 972) = real(s, 972) + x * k;
    real(s, 976) = real(s, 976) + y * k;
    real(s, 980) = real(s, 980) + z * k;
    if (real(s, 980) < 0.0f) word(s, 988) = 0u;
}
}

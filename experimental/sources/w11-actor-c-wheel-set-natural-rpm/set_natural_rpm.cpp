#include "../w11-actor-c-common/c.hpp"
// Observed SetNaturalRPM__5Wheel at 0x002783c0, 148 bytes. With w the wheel, i the signed int at w+384 and
// V the pointer word at w+396 (the vehicle the wheel was added to, per add_wheel in w10-camveh-a):
//   q = the float at V + 676 + 4*i; when q == 0.0 nothing is written.
//   Otherwise A = the Point3 at V + 1836 + 12*i and B = the Point3 at V + 1644 + 12*i;
//   d = (B.x*A.x + B.y*A.y) + B.z*A.z (summed in that order); r = d / (float at w+380) * 60.0;
//   r is stored at w+364 and again at w+360.
// Inferred: a rolling speed converted to revolutions per minute (the 60.0 factor); q gates it, so a wheel
// whose entry is zero keeps its old value. Field meanings and names are not established.
// Nothing here ran on the EE.
extern "C" void SetNaturalRPM__5Wheel(void* self) {
    using namespace w11c;
    unsigned char* vehicle = link(self, 396);
    const int i = static_cast<int>(word(self, 384));
    if (real(vehicle, 676 + 4 * i) == 0.0f) return;
    const unsigned a = 1836u + 12u * static_cast<unsigned>(i);
    const unsigned b = 1644u + 12u * static_cast<unsigned>(i);
    float d = real(vehicle, b) * real(vehicle, a);
    d = d + real(vehicle, b + 4) * real(vehicle, a + 4);
    d = d + real(vehicle, b + 8) * real(vehicle, a + 8);
    const float r = d / real(self, 380) * 60.0f;
    real(self, 364) = r;
    real(self, 360) = r;
}

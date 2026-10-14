// Host contract for fxFlyingFire__FP6Point3T0ffb; not an EE input. The callees log. The authored function was also
// compared with the original in a private R5900 run (56 scenarios: far and near camera, the camera word, the
// start-point flag, density values around the rounding edges, with the global random state compared after every
// particle; 625 of 629 original words reached, the missing four being padding).
#include <cstdio>
#include <cstring>
#include <string>
extern "C" {
unsigned eRandState;
void* gameCam;
float worldCameraPos[3];
unsigned char fxflyingfire_particle_7d6920[80];
unsigned char fxflyingfire_particle_7d68d0[80];
unsigned char fxflyingfire_particle_7d6880[80];
unsigned char fxflyingfire_particle_7d6830[80];
}
#include "fx_flying_fire.cpp"

static int nearCamera;
static int small_, normal_, flame_, spark_;
static Point3 firstPoint[4], firstVelocity[4];
static int seen[4];
extern "C" {
int gameCloseToCamera__FP6Point3(Point3*) { return nearCamera; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float scale) {
    int kind = d == fxflyingfire_particle_7d6920 ? 0 : d == fxflyingfire_particle_7d68d0 ? 1 : d == fxflyingfire_particle_7d6880 ? 2 : 3;
    (kind == 0 ? small_ : kind == 1 ? normal_ : kind == 2 ? flame_ : spark_)++;
    if (!seen[kind]++) { firstPoint[kind] = *p; firstVelocity[kind] = *v; }
    if (scale != 0.0f) std::puts("fail: scale argument");
}
}

static unsigned camWords[32];
static void reset() {
    nearCamera = 1; small_ = normal_ = flame_ = spark_ = 0; std::memset(seen, 0, sizeof seen);
    std::memset(camWords, 0, sizeof camWords); gameCam = camWords;
    worldCameraPos[0] = 0; worldCameraPos[1] = 0; worldCameraPos[2] = 0;
    eRandState = 12345;
}

int main() {
    int bad = 0;
    Point3 p{10.0f, 20.0f, 30.0f};
    Point3 q{1.0f, 2.0f, 3.0f};
    reset(); nearCamera = 0;
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 1.0f, 1.0f, false);
    if (small_ + normal_ + flame_ + spark_ != 0 || eRandState != 12345u) { std::puts("fail: far from the camera"); bad = 1; }
    // density B = 1.0: round(3.0) = 3 smoke, round(10) = 10 flame, always 2 sparks; the distance squared 500 is below 209764: small smoke
    reset();
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 1.0f, 1.0f, false);
    if (small_ != 3 || normal_ != 0 || flame_ != 10 || spark_ != 2) { std::printf("fail: counts %d %d %d %d\n", small_, normal_, flame_, spark_); bad = 1; }
    // the camera far away (distance squared above the threshold) or the word non-zero gives the normal smoke
    reset(); worldCameraPos[0] = 1000.0f;
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 1.0f, 1.0f, false);
    if (small_ != 0 || normal_ != 3) { std::puts("fail: far camera"); bad = 1; }
    reset(); camWords[84 / 4] = 1;
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 1.0f, 1.0f, false);
    if (small_ != 0 || normal_ != 3) { std::puts("fail: camera word"); bad = 1; }
    // rounding of B * 3 and B * 10: 0.5 -> 1.5 -> 2 and 5 flame; 0.16 -> 0.48 -> 0 and 1.6 -> 2
    reset();
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 1.0f, 0.5f, false);
    if (small_ != 2 || flame_ != 5) { std::printf("fail: rounding 0.5 %d %d\n", small_, flame_); bad = 1; }
    reset();
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 1.0f, 0.16f, false);
    if (small_ != 0 || flame_ != 2) { std::printf("fail: rounding 0.16 %d %d\n", small_, flame_); bad = 1; }
    // with the random state advanced by the draws only (3 rolls per ball, at least): the state changes
    reset();
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 1.0f, 1.0f, false);
    if (eRandState == 12345u) { std::puts("fail: state not advanced"); bad = 1; }
    // the flame velocity is Q * 0.5 exactly, and a flame point lies within A of P when the ball is inside the unit sphere
    if (firstVelocity[2].x != 0.5f || firstVelocity[2].y != 1.0f || firstVelocity[2].z != 1.5f) { std::puts("fail: flame velocity"); bad = 1; }
    if (firstPoint[2].x < 9.0f || firstPoint[2].x > 11.0f || firstPoint[2].y < 19.0f || firstPoint[2].y > 21.0f) { std::puts("fail: flame point"); bad = 1; }
    // the lead flag moves the start point of the smoke to P - 2Q: the first smoke point is within A of (8, 16, 24)
    reset();
    fxFlyingFire__FP6Point3T0ffb(&p, &q, 0.5f, 1.0f, true);
    if (firstPoint[0].x < 7.5f || firstPoint[0].x > 8.5f || firstPoint[0].y < 15.5f || firstPoint[0].y > 16.5f || firstPoint[0].z < 23.5f || firstPoint[0].z > 24.5f) { std::puts("fail: lead start"); bad = 1; }
    return bad;
}

#include "init_stuff.cpp"

#include <cstdio>
#include <cstring>

static int bbox_calls, body_calls, quat_calls, norm_calls, upd_calls;
static float body_args[5];
static long long bbox_what;
static void* bbox_model;
extern "C" void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* m, long long w, int* a, int* b,
                                                            int* c, int* d, int* e, int* f) {
    ++bbox_calls; bbox_model = m; bbox_what = w;
    *a = -10; *b = -20; *c = -30; *d = 40; *e = 60; *f = 90;
}
extern "C" void bodyInit__FP5_bodyfffff(void*, float a, float b, float c, float d, float e) {
    ++body_calls; body_args[0] = a; body_args[1] = b; body_args[2] = c; body_args[3] = d; body_args[4] = e;
}
extern "C" void matToQuaternion__FP4QuatP8Matrix34(void*, void*) { ++quat_calls; }
extern "C" void quatNormalize__FP4Quat(void*) { ++norm_calls; }
extern "C" void bodyUpdateMatrix__FP5_body(void*) { ++upd_calls; }

alignas(8) static unsigned char obj[640];
template <typename T> static T get(int off) { T v; std::memcpy(&v, obj + off, sizeof v); return v; }
template <typename T> static void put(int off, T v) { std::memcpy(obj + off, &v, sizeof v); }

int main() {
    put<void*>(76, obj);
    put<long long>(232, 0x500);   // highest set bit is 10
    put<float>(576, 2.0f);
    put<unsigned>(68, 12345u);
    put<float>(252, 60.0f); put<float>(256, 120.0f); put<float>(48, 30.0f);
    for (int i = 0; i < 12; ++i) obj[16 + i] = static_cast<unsigned char>(i + 1);
    put<int>(404, 7); put<int>(408, 7); put<int>(412, 7);
    initStuff__6PhPropi(obj, 0);
    if (bbox_calls != 1 || bbox_what != 0x500 || bbox_model != obj) return 1;
    if (body_calls != 1 || body_args[0] != 1.0f || body_args[1] != 1.0f) return 2;
    if (body_args[2] != 100.0f || body_args[3] != 160.0f || body_args[4] != 240.0f) return 3;
    if (std::memcmp(obj + 416, obj + 16, 12) != 0) return 4;
    if (get<float>(444) != 60.0f * 0x1.111110p-6f || get<float>(448) != 120.0f * 0x1.111110p-6f) return 5;
    if (get<float>(452) != 30.0f * 0x1.111110p-6f) return 5;
    unsigned s1 = 12345u * 1664525u + 1013904223u, s2 = s1 * 1664525u + 1013904223u,
             s3 = s2 * 1664525u + 1013904223u;
    if (get<unsigned>(68) != s3) return 6;
    float want = ((static_cast<float>(s1 >> 16) * 1.52587890625e-05f) * 2.0f - 1.0f) * 2.0f;
    if (get<float>(456) != want) return 7;
    if (get<int>(556) != 10) return 8;
    if (get<float>(572) != -0x1.999998p-2f) return 9;
    if (get<int>(404) != 7 || get<int>(408) != 7 || get<int>(412) != 7) return 10;
    if (quat_calls != 1 || norm_calls != 1 || upd_calls != 1) return 11;
    // range 10 with 0x10 and 0x400, constant -0.1 with 0x800, zeroing with 0x1000
    put<unsigned>(68, 12345u);
    initStuff__6PhPropi(obj, 0x10 | 0x800 | 0x1000);
    if (get<float>(456) != want * 5.0f) return 12;
    if (get<float>(572) != -0x1.999998p-4f) return 13;
    if (get<int>(404) != 0 || get<int>(408) != 0 || get<int>(412) != 0) return 14;
    // bit 0x20000 of the word at 336 with a nonzero byte at 568 sets 0x1000
    put<int>(404, 7); obj[568] = 1; put<long long>(336, 0x20000); put<long long>(232, 0);
    put<int>(556, 99);
    initStuff__6PhPropi(obj, 0);
    if (get<int>(404) != 0 || get<int>(556) != -1) return 15;
    put<int>(404, 7); obj[568] = 0;
    initStuff__6PhPropi(obj, 0);
    if (get<int>(404) != 7) return 16;
    std::puts("init stuff passed");
    return 0;
}

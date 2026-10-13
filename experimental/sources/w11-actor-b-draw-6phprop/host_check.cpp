// Host contract for msg_draw__6PhProp; not an EE input. Includes the source in host mode (32-bit words are
// offsets from w11_actor_b_host_base) and logs the callees' arguments.
#define W11_ACTOR_B_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
int frameNm;
static unsigned char arena[2048] __attribute__((aligned(16)));
static std::string calls;
static unsigned char halo_at_call[8];
static float sin_value;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
float isin__Fi(int a) { calls += "isin" + std::to_string(a) + ";"; return sin_value; }
void matScale__FP8Matrix34f(void* m, float s) { calls += "scale" + f(s) + ";"; (void)m; }
void matTrans__FP8Matrix34fff(void*, float x, float y, float z) { calls += "trans" + f(x) + "," + f(y) + "," + f(z) + ";"; }
void modelSetHalo__FP7HaloDefi(void* t, int n) { calls += "halo" + std::to_string(n) + ";"; std::memcpy(halo_at_call, t, 8); }
void setupHalo__4itemPQ24item11descriptionf(void* d, float s) { calls += "setup" + std::to_string(static_cast<unsigned char*>(d) - arena) + "," + f(s) + ";"; }
void modelDraw(void* model, void* tex, int mode, Point3 p, void* m, void* anim, unsigned long long flags, void* rec, int last) {
    calls += "draw" + std::to_string(static_cast<unsigned char*>(model) - arena) + "," + std::to_string(static_cast<unsigned char*>(tex) - arena) + "," + std::to_string(mode) + "," +
             f(p.x) + "," + f(p.y) + "," + f(p.z) + "," + (m == arena + 272 ? "M" : "bad") + (anim ? "A" : "") + "," + std::to_string(flags) + (rec ? "R" : "") + "," + std::to_string(last) + ";";
}
void modelResetOptionalParams__Fv() { calls += "reset;"; }
}
static void put(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
static void putw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static float get(unsigned off) { float v; std::memcpy(&v, arena + off, 4); return v; }

static int setup(int flag, int frame, int limit, unsigned long long bits, float s) {
    std::memset(arena, 0, sizeof arena);
    for (int i = 0; i < 40; ++i) put(240 + i * 4, 0.0f);
    put(240, 1.0f); put(244, 2.0f); put(248, 3.0f);
    put(416, 10.0f); put(420, 11.0f); put(424, 12.0f);
    float src[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const unsigned idx[9] = {468, 472, 476, 480, 484, 488, 492, 496, 500};
    for (int i = 0; i < 9; ++i) put(idx[i], src[i]);
    put(576, 0.5f);
    arena[568] = static_cast<unsigned char>(flag);
    putw(76, 1111); putw(80, 2222);
    putw(260, static_cast<unsigned>(limit));
    putw(328, 1024);   // the description pointer is a host offset
    std::memcpy(arena + 1024 + 40, &bits, 8);
    std::memcpy(arena + 232, &bits, 8);
    frameNm = frame;
    w11_actor_b_host_base = arena;
    sin_value = s;
    calls.clear();
    return 0;
}

int main() {
    // flag clear: no halo work
    setup(0, 10, 100, 0x1234567812345678ull, 0.5f);
    msg_draw__6PhProp(arena);
    // matrix: eight of nine rotation floats map as listed, point floats fill +308..+316
    const float want[12] = {1, 4, 7, 2, 5, 8, 3, 6, 9, 10, 11, 12};
    for (int i = 0; i < 12; ++i)
        if (get(272 + i * 4) != want[i]) { std::printf("fail: matrix[%d]\n", i); return 1; }
    if (calls != "scale0.5;trans-1,-2,-3;draw1111,2222,1,10,11,12,M,1311768465173141112,0;") { std::puts(calls.c_str()); std::puts("fail: calls (flag clear)"); return 2; }
    // flag set, frame - 3 < limit: pulse
    setup(1, 10, 100, 0, -0.25f);
    msg_draw__6PhProp(arena);
    std::string pulse = "scale0.5;trans-1,-2,-3;isin5120;halo1;draw1111,2222,1,10,11,12,M,0,0;reset;";
    if (calls != pulse) { std::puts(calls.c_str()); std::puts("fail: pulse calls"); return 3; }
    if (halo_at_call[3] != 15 || halo_at_call[0] != 0xff || halo_at_call[2] != 0xff || halo_at_call[4] != 0 || halo_at_call[5] != 0x80 || halo_at_call[6] != 0xcc || halo_at_call[7] != 0x43) { std::puts("fail: halo table"); return 4; }
    // large sine value, bit 39 selects the pulse even when frame - 3 >= limit
    setup(1, 10, 2, 1ull << 39, 1.0f);
    msg_draw__6PhProp(arena);
    if (calls.find("isin5120;halo1;") == std::string::npos || halo_at_call[3] != 24) { std::puts(calls.c_str()); std::puts("fail: bit 39 pulse"); return 5; }
    // frame - 3 == limit and bit clear: item halo setup instead
    setup(1, 10, 7, 0, 1.0f);
    msg_draw__6PhProp(arena);
    if (calls != "scale0.5;trans-1,-2,-3;setup328,5;draw1111,2222,1,10,11,12,M,0,0;reset;") { std::puts(calls.c_str()); std::puts("fail: setup halo"); return 6; }
    // frame - 3 == limit - 1: pulse
    setup(1, 10, 8, 0, 0.0f);
    msg_draw__6PhProp(arena);
    if (calls.find("isin5120;halo1;") == std::string::npos || halo_at_call[3] != 12) { std::puts(calls.c_str()); std::puts("fail: boundary pulse"); return 7; }
    return 0;
}

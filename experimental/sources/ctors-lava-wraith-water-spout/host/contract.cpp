#define NORRATH_CTORS_LAVA_WRAITH_HOST 1
#include "../water_spout.cpp"
#include "../../ctors-lava-wraith-detail/host_support.hpp"
using namespace hs;
unsigned char norrath_ctors_lava_wraith_view_44a390[4];
static unsigned char* obj;
extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* map, char** arguments) {
    event("GameObject map=%d args=%d", map != nullptr, arguments != nullptr);
    obj = (unsigned char*)object; std::memcpy(obj + 16, point, 12); put(obj + 12, 0x77, 4); return 1;
}
extern "C" float norrath_integer_cosine(int a) { event("cos %d", a); return 0.75f; }
extern "C" float norrath_integer_sine(int a) { event("sin %d", a); return -0.125f; }
static unsigned long long dp_of(float x) { return ((unsigned long long)fbits(x) << 8) | 1ull; }
extern "C" unsigned long long norrath_entity_fptodp(float x) { event("fptodp %08x", fbits(x)); return dp_of(x); }
extern "C" unsigned long long norrath_softdouble_multiply_2fb680(unsigned long long l, unsigned long long r) { event("dpmul %llx %llx", l, r); return l ^ (r >> 5); }
extern "C" float norrath_softdouble_to_float_2fbc20(unsigned long long v) { event("dptofp %llx", v); return (float)(v & 0xffff) * 0.25f; }
extern "C" unsigned norrath_actor_add_run_list(void*) { event("run flags=%08x f76=%08x", word_at(obj + 12), word_at(obj + 76)); return 0x66; }
static void run_case(int type) {
    calls.clear();
    Image<128> o, want;
    unsigned char pt[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    const unsigned r = norrath_construct_WaterSpout(o.b, pt, type);
    const short half = (short)type;
    const unsigned long long k = 0x4032666666666666ull;
    char b[200];
    std::vector<std::string> w{"GameObject map=0 args=0"};
    std::snprintf(b, sizeof b, "cos %d", (int)half); w.push_back(b);
    std::snprintf(b, sizeof b, "fptodp %08x", fbits(0.75f)); w.push_back(b);
    const unsigned long long c1 = dp_of(0.75f) ^ (k >> 5);
    std::snprintf(b, sizeof b, "dpmul %llx %llx", dp_of(0.75f), k); w.push_back(b);
    std::snprintf(b, sizeof b, "dptofp %llx", c1); w.push_back(b);
    std::snprintf(b, sizeof b, "sin %d", (int)half); w.push_back(b);
    std::snprintf(b, sizeof b, "fptodp %08x", fbits(-0.125f)); w.push_back(b);
    const unsigned long long c2 = dp_of(-0.125f) ^ (k >> 5);
    std::snprintf(b, sizeof b, "dpmul %llx %llx", dp_of(-0.125f), k); w.push_back(b);
    std::snprintf(b, sizeof b, "dptofp %llx", c2); w.push_back(b);
    std::snprintf(b, sizeof b, "run flags=00000009 f76=%08x", fbits((float)(c1 & 0xffff) * 0.25f)); w.push_back(b);
    expect_log(w, "water spout");
    for (int i = 0; i < 12; ++i) want.b[16 + i] = pt[i];
    want.w(72, low(norrath_ctors_lava_wraith_view_44a390)); want.w(40, 0xf0, 1); want.w(56, (unsigned)type, 2);
    want.w(76, fbits((float)(c1 & 0xffff) * 0.25f)); want.w(80, fbits((float)(c2 & 0xffff) * 0.25f));
    want.w(12, 9); want.w(88, 0, 2);
    expect_image(o, want, "water spout image");
    expect(r == 0x66u, "water spout return");
}
int main() { run_case(0x1234); run_case(-1); run_case(0x18001); return finish("water spout constructor"); }

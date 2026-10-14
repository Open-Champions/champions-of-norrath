// Host contract for refreshSettings__7Vehicle. Not an EE plan input.
// Built with -DNORRATH_W8_HOST on the source file as well.
#include <cmath>
#include <cstdio>
#include <cstring>
extern "C" void refreshSettings__7Vehicle(void* object);

static unsigned char object[2400];
static unsigned char vtable[256];
static unsigned char* table[4];
static int order = 0, set_at = 0, build_at = 0;
static void* set_arg = nullptr;
static short adjustment = 0;

extern "C" const unsigned char* norrath_w8_host_resolve(unsigned bits) {
    return table[bits & 3u] ? table[bits & 3u] : nullptr;
}
static float planned[4];
static void put_raw(int off, float v) { std::memcpy(object + off, &v, 4); }
static void set_slot(void* self) {
    set_at = ++order; set_arg = self;
    // the slot writes the mass and sizes that refresh then reads
    put_raw(884, planned[0]); put_raw(888, planned[1]); put_raw(892, planned[2]); put_raw(896, planned[3]);
}
static unsigned last_1224 = 0;
extern "C" void buildCollisionInfo__7Vehicle(void* self) {
    build_at = ++order;
    if (self != object) std::fprintf(stderr, "bad build arg\n");
    std::memcpy(&last_1224, object + 1224, 4);  // must already be stored when the call happens
}
static float getf(int off) { float f; std::memcpy(&f, object + off, 4); return f; }
static void putf(int off, float v) { std::memcpy(object + off, &v, 4); }
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static float from_bits(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }
#define EXPECT(c, m) do { if (!(c)) { std::fprintf(stderr, "fail: %s\n", m); return 1; } } while (0)

int main() {
    table[1] = vtable;
    table[2] = reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned long>(&set_slot));
    adjustment = 24;
    std::memcpy(vtable + 224, &adjustment, 2);
    const unsigned fn_word = 2;
    std::memcpy(vtable + 228, &fn_word, 4);
    const unsigned vt_word = 1;
    std::memcpy(object + 72, &vt_word, 4);
    // the slot's effect: it sets the mass and three sizes, which refresh then reads
    planned[0] = 3.0f; planned[1] = 39.0f; planned[2] = 78.0f; planned[3] = 117.0f;
    putf(884, 99.0f);
    refreshSettings__7Vehicle(object);
    EXPECT(set_at == 1 && build_at == 2, "call order");
    EXPECT(set_arg == object + 24, "slot called with adjusted this");
    const float k = from_bits(0x3cd20d20u), third = from_bits(0x3daaaaaau);
    const float a = 39.0f * k, b = 78.0f * k, c = 117.0f * k;
    const float aa = a * a, bb = b * b, cc = c * c;
    const float ixy = (3.0f * (aa + bb)) * third, ixz = (3.0f * (aa + cc)) * third, izy = (3.0f * (cc + bb)) * third;
    EXPECT(bits(getf(668)) == bits(3.0f), "+668 mass");
    EXPECT(bits(getf(672)) == bits(1.0f / 3.0f), "+672 inverse mass");
    EXPECT(bits(getf(1216)) == bits(ixy), "+1216");
    EXPECT(bits(getf(1220)) == bits(ixz), "+1220");
    EXPECT(bits(getf(1212)) == bits(izy), "+1212");
    EXPECT(bits(getf(1228)) == bits(1.0f / ixy), "+1228");
    EXPECT(bits(getf(1232)) == bits(1.0f / ixz), "+1232");
    EXPECT(bits(getf(1224)) == bits(1.0f / izy), "+1224");
    EXPECT(last_1224 == bits(1.0f / izy), "+1224 stored before buildCollisionInfo");
    // all three inertia terms differ for these sizes, so swapped slots would be caught
    EXPECT(ixy != ixz && ixz != izy && ixy != izy, "distinct terms");
    // a different, non-integral set of inputs
    planned[0] = 7.5f; planned[1] = 10.25f; planned[2] = 3.5f; planned[3] = 21.0f;
    refreshSettings__7Vehicle(object);
    {
        const float a2 = 10.25f * k, b2 = 3.5f * k, c2 = 21.0f * k;
        const float xx = a2 * a2, yy = b2 * b2, zz = c2 * c2;
        EXPECT(bits(getf(1216)) == bits((7.5f * (xx + yy)) * third), "second +1216");
        EXPECT(bits(getf(1220)) == bits((7.5f * (xx + zz)) * third), "second +1220");
        EXPECT(bits(getf(1212)) == bits((7.5f * (zz + yy)) * third), "second +1212");
        EXPECT(bits(getf(672)) == bits(1.0f / 7.5f), "second inverse mass");
    }
    return 0;
}

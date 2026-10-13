// Host check: powf and strstr come from the host libc; the two item callees are stubs.
#include <cmath>
#include <cstring>

struct GvEffect { short tag; short v[3]; };
struct GvDescription {
    const char* name;
    unsigned char gap0[20 - sizeof(void*)];
    int fixed;
    unsigned char gap1[29 - 24];
    signed char kind, sub;
};
struct GvItem {
    const GvDescription* description;
    short count;
    unsigned char gap[36 - sizeof(void*) - sizeof(short)];
    GvEffect effects[4];
};
extern "C" {
int getValue__4itemPQ24item11description(const GvItem*);
}
static int lowOut, highOut, armor, rangeCalls, armorCalls;
extern "C" int getDamageRangeAll__4itemPQ24item11descriptionPiT2i(const void*, int* l, int* h, int flag) { ++rangeCalls; *l = lowOut; *h = highOut; return flag; }
extern "C" int getArmorBonus__4itemPQ24item11description(const void*) { ++armorCalls; return armor; }

static GvDescription d;
static GvItem item;
static void setup(int fixed, int kind, int sub, const char* name, int count) {
    d = GvDescription{}; d.name = name; d.fixed = fixed; d.kind = kind; d.sub = sub;
    item = GvItem{}; item.description = &d; item.count = count;
    for (auto& e : item.effects) e.tag = -1;
    rangeCalls = armorCalls = 0;
}
static int expectFrom(float s, int count, bool spear = false) {
    s = s * 40.0f; if (spear) s = s * 0.005f;
    return (int)(s + 0.5f) * count;
}
#define CHECK(c, n) do { if (!(c)) return n; } while (0)

int main() {
    // positive fixed word: value = fixed * count; nothing else is called
    setup(7, 0, 0, "x", 3); CHECK(getValue__4itemPQ24item11description(&item) == 21 && !rangeCalls && !armorCalls, 1);
    setup(0, 0, 0, "x", 3); item.effects[0].tag = 37; 
    // kind 2: base 5, then sum
    setup(0, 2, 0, "x", 2);
    CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(powf(5.0f, 1.95f), 2) && !rangeCalls && !armorCalls, 2);
    setup(0, 2, 0, "a spear", 2);
    CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(powf(5.0f, 1.95f), 2, true), 3);
    // kind 1: armor bonus * 0.6
    armor = 10; setup(0, 1, 0, "x", 1);
    CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(powf(10.0f * 0.6f, 1.95f), 1) && armorCalls == 1, 4);
    // other kinds with no weapon block: base 0, powf(0, 1.95) = 0
    setup(0, 3, 0, "x", 4); CHECK(getValue__4itemPQ24item11description(&item) == 0 && !rangeCalls, 5);
    // weapon block: kind 0, sub 1 (scale 0.85)
    lowOut = 10; highOut = 30; setup(0, 0, 1, "x", 1);
    {
        float lo = 10 * 0.46599397f, hi = 30 * 0.46599397f;
        float scale = 0.85f; hi = hi * scale; lo = lo * scale;
        float b = (lo + hi) * 0.5f - (hi - lo) * 0.1f;
        CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(powf(b, 1.95f), 1) && rangeCalls == 1, 6);
    }
    // sub 5 skips the weapon block
    setup(0, 0, 5, "x", 1); CHECK(getValue__4itemPQ24item11description(&item) == 0 && !rangeCalls, 7);
    // effect tag 37: s += (v1 * 0.01) * s ; tag 5 adds powf(v1, 1.392) and 66.0 for non-zero v1
    setup(0, 2, 0, "x", 1); item.effects[0].tag = 37; item.effects[0].v[0] = 50;
    {
        float s = powf(5.0f, 1.95f); s = 50.0f * 0.01f * s + s;
        CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(s, 1), 8);
    }
    setup(0, 2, 0, "x", 1); item.effects[1].tag = 5; item.effects[1].v[0] = 4;
    {
        float s = powf(5.0f, 1.95f); float p = powf(4.0f, 1.39199293f); s = (s + p) + 66.0f;
        CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(s, 1), 9);
    }
    setup(0, 2, 0, "x", 1); item.effects[1].tag = 5; item.effects[1].v[0] = 0;   // v1 zero: no 66.0
    CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(powf(5.0f, 1.95f) + powf(0.0f, 1.39199293f), 1), 10);
    // tag 13: powf only, unless kind == 1 (then it acts as tag 26 with 48.0)
    setup(0, 2, 0, "x", 1); item.effects[2].tag = 13; item.effects[2].v[0] = 9;
    CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(powf(5.0f, 1.95f) + powf(9.0f, 0.753996968f), 1), 11);
    // with kind == 1 tag 13 is handled as tag 26 and reads the values of the first tag-26 entry
    armor = 0; setup(0, 1, 0, "x", 1); item.effects[2].tag = 13; item.effects[2].v[0] = 5; item.effects[3].tag = 26; item.effects[3].v[0] = 9;
    {
        float s = powf(0.0f, 1.95f); float p = powf(9.0f, 1.99698794f); s = (s + p) + 48.0f; s = (s + p) + 48.0f;
        CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(s, 1), 12);
    }
    // an unknown tag and a negative tag add nothing; the first entry with a tag supplies the values
    setup(0, 2, 0, "x", 1); item.effects[0].tag = 99; item.effects[1].tag = -3;
    CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(powf(5.0f, 1.95f), 1), 13);
    setup(0, 2, 0, "x", 1); item.effects[0].tag = 0; item.effects[0].v[0] = 3; item.effects[1].tag = 0; item.effects[1].v[0] = 8;
    {
        float s = powf(5.0f, 1.95f); float p = powf(3.0f, 1.85397291f); s = s + p; s = s + p;   // both entries read the first entry's value
        CHECK(getValue__4itemPQ24item11description(&item) == expectFrom(s, 1), 14);
    }
    return 0;
}

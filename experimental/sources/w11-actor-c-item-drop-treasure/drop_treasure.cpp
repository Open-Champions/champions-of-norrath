// Observed dropTreasure__4itemG6Point3iffffffPci at 0x00100e20, 2484 bytes (static member function of item).
// Arguments, in register order: a point P (passed by reference to a copy), an int h, six floats a..f, a text pointer
// name and an int named. Every random draw is a step of the global generator syncRandState (state * 1664525 +
// 1013904223, draw = upper 16 bits, written back after each step); "u" below is a draw taken as a float
// without scaling, so a test u < 65536 * p has probability p.
//  1. With nmPlayers >= 2: c = c + c, f = f + f and d = (float)((double)d * 1.5) through the image's soft-float
//     helpers fptodp, dpmul and dptofp. e is clamped to [0, 9]; with i = (int)e and t = e - i, the table row
//     tab.1068 + 12*i and the next one give a = r0*(1-t) + r3*t and b = r1*(1-t) + r4*t (12 rows of three floats,
//     a data table of the image). One draw u1/65536 sets the factor m: 0.5 when below a, 3.0 when above a + b,
//     otherwise 1.0 (checked in that order).
//  2. A second draw gives a count k: 1, or 2 when at least 61000, or 3 when at least 64000. c = c * gameTreasureRamp().
//     k times: when a draw is below d * 65536 another draw gives dropGold(P, (int)((x % 40 + 40) * c * 0.035)).
//  3. Item 1: when a draw u is below ((nmPlayers - 1) * 0.05 + 0.125) * 65536, an item description (below) is
//     filled: a draw below 8000 calls item::create("arrows", desc, -1, 0), otherwise makeRandomPotion(desc);
//     a PhProp is then built (blockAlloc(584), constructor at (P.x, P.y, P.z + a)).
//  4. Item 2: when a draw u is below 655.36 (about 1%), a description is filled by
//     createRandomCombineableItem(desc, (short)(int)worldDifficulty, name) and, when that succeeds, a PhProp is built as
//     above; when it fails the function returns.
//  5. Item 3: when a draw is below f * 65536: description filled by createNamedRandom(desc, 7*level, kind, (int)worldDifficulty) when
//     a draw is below 1966.08 or named is non-zero, else createRandom(desc, level, kind, -1, (int)worldDifficulty); the function
//     returns when that fails. level = (int)q where q = (m * (draw*2^-16 * 0.4 + 0.8)) * (gameItemTreasureRamp() * 200),
//     multiplied by 3 when a further draw is below 655.35 and doubled when below 3276.75; kind is 0 when a draw is
//     below 30000, otherwise 2 when a second draw is at least 60000 and 1 otherwise. A PhProp is built, then its
//     floats at +252 and +256 are set to cos and sin of (h + (draw & 0x3fff) - 8192) times 24.0 times b, and
//     initStuff(prop, 1089) runs.
//  The description fill is: four 8-byte entries at +36 each with -1 in the first halfword, bytes +32 and +33 set to -1
//  and the eight halfwords at +16..+31 set to 0. Each PhProp is constructed with the lump "equipmnt.lmp", the two
//  words at +0 and +4 of the globalItem entry (16 bytes each) for getProxyMeshId(desc) (called twice, as in
//  the image), the long 1, the int 1089, the description and 1.0.
// Inferred: this drops gold, a potion or arrows, a random combineable item and a random equipment item
// around a point; names of the six floats are not established. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern int nmPlayers;
extern unsigned syncRandState;
extern float worldDifficulty;
extern unsigned char globalItem[] __asm__("_4item$globalItem");
unsigned long long fptodp(float value);
unsigned long long dpmul(unsigned long long a, unsigned long long b);
float dptofp(unsigned long long value);
float gameTreasureRamp__Fv(void);
float gameItemTreasureRamp__Fv(void);
void dropGold__4itemG6Point3i(Point3 position, int amount);
int create__4itemPcPQ24item11descriptionii(const char* name, void* description, int a, int b);
void makeRandomPotion__4itemPQ24item11description(void* description);
int createRandomCombineableItem__4itemPQ24item11descriptionsPc(void* description, short level, const char* name);
int createNamedRandom__4itemPQ24item11descriptioniii(void* description, int a, int b, int c);
int createRandom__4itemPQ24item11descriptioniiii(void* description, int a, int b, int c, int d);
int getProxyMeshId__4itemPQ24item11description(void* description);
void* blockAlloc__Fi(int size);
void phprop_construct(void* self, Point3 position, const char* lump, const char* a, const char* b, long long one, int mode, void* description, float scale)
    __asm__("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
void initStuff__6PhPropi(void* prop, int mode);
float icos__Fi(int angle);
float isin__Fi(int angle);

namespace {
const unsigned tab_bits[36] = {
    0x3f7d70a3, 0x3c1374bc, 0x3a83126e, 0x3f666666, 0x3db851eb, 0x3c23d70a, 0x3f333333, 0x3e851eb8, 0x3d23d70a,
    0x3f000000, 0x3edc28f5, 0x3d8f5c28, 0x3e999999, 0x3f199999, 0x3dcccccc, 0x3e4ccccc, 0x3f199999, 0x3e4ccccc,
    0x3dcccccc, 0x3f000000, 0x3ecccccc, 0x00000000, 0x3e999999, 0x3f333333, 0x00000000, 0x3dcccccc, 0x3f666666,
    0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 0x3f800000};

inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }

inline unsigned draw() {
    syncRandState = syncRandState * 1664525u + 1013904223u;
    return syncRandState >> 16;
}

inline void fill_description(unsigned char* d) {
    for (int entry = 3; entry >= 0; --entry) *reinterpret_cast<short*>(d + 36 + 8 * entry) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int i = 0; i < 8; ++i) *reinterpret_cast<short*>(d + 16 + 2 * i) = 0;
}

void build_prop(const Point3& p, float lift, unsigned char* description, void** out) {
    void* block = blockAlloc__Fi(584);
    if (block == nullptr) {
        *out = nullptr;
        return;
    }
    Point3 position = {p.x + 0.0f, p.y, p.z + lift};
    const int first = getProxyMeshId__4itemPQ24item11description(description);
    const int second = getProxyMeshId__4itemPQ24item11description(description);
    phprop_construct(block, position, "equipmnt.lmp", *reinterpret_cast<const char* const*>(globalItem + first * 16),
                     *reinterpret_cast<const char* const*>(globalItem + second * 16 + 4), 1, 1089, description, 1.0f);
    *out = block;
}
}  // namespace

void dropTreasure__4itemG6Point3iffffffPci(Point3 point, int heading, float lift, float scale, float c, float d, float e, float f,
                                          const char* name, int named) {
    const Point3 origin = point;
    if (nmPlayers >= 2) {
        c = c + c;
        f = f + f;
        d = dptofp(dpmul(fptodp(d), 0x3ff8000000000000ull));
    }
    if (e < 0.0f) e = 0.0f;
    if (9.0f < e) e = 9.0f;
    const int row_index = static_cast<int>(e);
    const float t = e - static_cast<float>(row_index);
    const float rest = 1.0f - t;
    const unsigned* row = tab_bits + row_index * 3;
    const float unit = bits(0x37800000u);
    const float u1 = static_cast<float>(static_cast<int>(draw())) * unit;
    const float a = bits(row[0]) * rest + bits(row[3]) * t;
    const float b = bits(row[1]) * rest + bits(row[4]) * t;
    float m = 1.0f;
    if (u1 < a) m = 0.5f;
    if (a + b < u1) m = 3.0f;
    const unsigned pick = draw();
    int count = 1;
    if (60999u < pick) count = 63999u < pick ? 3 : 2;
    c = c * gameTreasureRamp__Fv();
    if (count != 0) {
        d = d * 65536.0f;
        const float gold_unit = bits(0x3d0f5c28u);
        do {
            if (static_cast<float>(static_cast<int>(draw())) < d) {
                const unsigned x = draw();
                dropGold__4itemG6Point3i(origin, static_cast<int>(static_cast<float>(x % 40u + 40u) * c * gold_unit));
            }
        } while (--count != 0);
    }
    unsigned char description[144];
    void* prop;
    // Item 1
    const float chance1 = (static_cast<float>(nmPlayers - 1) * bits(0x3d4ccccc) + 0.125f) * 65536.0f;
    if (static_cast<float>(static_cast<int>(draw())) < chance1) {
        const unsigned kind = draw();
        fill_description(description);
        if (kind < 8000u) {
            create__4itemPcPQ24item11descriptionii("arrows", description, -1, 0);
        } else {
            makeRandomPotion__4itemPQ24item11description(description);
        }
        build_prop(origin, lift, description, &prop);
    }
    // Item 2
    if (static_cast<float>(static_cast<int>(draw())) < bits(0x4423d70au)) {
        fill_description(description);
        if (createRandomCombineableItem__4itemPQ24item11descriptionsPc(description, static_cast<short>(static_cast<int>(worldDifficulty)), name) == 0) return;
        build_prop(origin, lift, description, &prop);
    }
    // Item 3
    if (!(static_cast<float>(static_cast<int>(draw())) < f * 65536.0f)) return;
    fill_description(description);
    const float ramp = gameItemTreasureRamp__Fv();
    const float v = static_cast<float>(static_cast<int>(draw())) * unit;
    float q = (m * (v * bits(0x3ecccccc) + bits(0x3f4ccccc))) * (ramp * 200.0f);
    const float w = static_cast<float>(static_cast<int>(draw()));
    if (w < bits(0x4423d666u)) {
        q = q * 3.0f;
    } else if (w < bits(0x454ccbffu)) {
        q = q + q;
    }
    const int level = static_cast<int>(q);
    int kind = 0;
    if (!(draw() < 30000u)) kind = 59999u < draw() ? 2 : 1;
    int ok;
    const float roll = static_cast<float>(static_cast<int>(draw()));
    if (roll < bits(0x44f5c28fu) || named != 0) {
        ok = createNamedRandom__4itemPQ24item11descriptioniii(description, level * 7, kind, static_cast<int>(worldDifficulty));
    } else {
        ok = createRandom__4itemPQ24item11descriptioniiii(description, level, kind, -1, static_cast<int>(worldDifficulty));
    }
    if (ok == 0) return;
    build_prop(origin, lift, description, &prop);
    if (prop == nullptr) return;
    const int angle = heading + static_cast<int>(draw() & 0x3fffu) - 8192;
    const float cosine = icos__Fi(angle);
    const float sine = isin__Fi(angle);
    *reinterpret_cast<float*>(static_cast<unsigned char*>(prop) + 252) = cosine * 24.0f * scale;
    *reinterpret_cast<float*>(static_cast<unsigned char*>(prop) + 256) = sine * 24.0f * scale;
    initStuff__6PhPropi(prop, 1089);
}
}

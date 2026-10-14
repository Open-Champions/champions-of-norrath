// Host contract for clone__6Player; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_10GameObject[8], vt_8Creature[8], vt_6Player[8];
static unsigned char arena[1472] __attribute__((aligned(16)));
static unsigned char src[1472] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned wordAt72AtCall[32];
static int callCount;
static void note() { std::memcpy(&wordAt72AtCall[callCount++], arena + 72, 4); }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    note(); std::memset(d, 0, 4);
}
void AnimationState8_copy(void* d, const void* s) {
    calls += "an" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    note(); std::memcpy(d, s, 264);
}
void AnimationState_copy(void* d, const void* s) {
    calls += "ab" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    note(); std::memcpy(d, s, 40);
}
static unsigned char light_seen[2][60];
static int light_index;
void lightAdd__FP5Light(void* l) {
    int off = static_cast<unsigned char*>(l) - arena;
    calls += "light" + std::to_string(off) + ";";
    std::memcpy(light_seen[light_index++], l, 60);
}
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
#define LIGHT(off) (lightExpected(off))
static bool lightFirst, lightSecond;
static std::string lightExpected(int off) {
    bool on = off == 1208 ? lightFirst : lightSecond;
    return on ? "light" + std::to_string(off) + ";" : "";
}

int main() {
    int bad = 0;
    for (int i = 0; i < 1472; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    std::memset(arena, 0xa5, sizeof arena);
    failNext = true;
    if (clone__6Player(src) != nullptr || calls != "alloc1408;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false;
    for (int mask = 0; mask < 16; ++mask) {
        // bits: first light word 0 zero, word 1 zero, second light word 0 zero, word 1 zero
        for (int i = 0; i < 1472; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
        if (mask & 1) std::memset(src + 1208, 0, 4);
        if (mask & 2) std::memset(src + 1212, 0, 4);
        if (mask & 4) std::memset(src + 1268, 0, 4);
        if (mask & 8) std::memset(src + 1272, 0, 4);
        lightFirst = !((mask & 1) && (mask & 2));
        lightSecond = !((mask & 4) && (mask & 8));
        std::memset(arena, 0xa5, sizeof arena); calls.clear(); callCount = 0; light_index = 0;
        void* r = clone__6Player(src);
        if (r != arena) { std::puts("fail: returns the new object"); bad = 1; }
        std::string want = "alloc1408;";
        want += std::string() + "lh0<0;" + "lh4<4;" + "lh8<8;" + "an288<288;" + "lh576<576;" + "lh752<752;" + "lh888<888;" + "lh976<976;" + "lh1000<1000;" + "lh1024<1024;" + "lh1048<1048;" + "lh1072<1072;" + "lh1192<1192;" + LIGHT(1208) + LIGHT(1268) + "ab1356<1356;" + "lh1396<1396;";
        if (calls != want) { std::puts(calls.c_str()); std::puts(want.c_str()); std::puts("fail: call order"); bad = 1; }
        int li = 0;
        if (lightFirst) { for (int k = 0; k < 8; ++k) if (light_seen[li][k] != 0) { std::puts("fail: first light words not cleared at the call"); bad = 1; }
            if (std::memcmp(light_seen[li] + 8, src + 1216, 52) != 0) { std::puts("fail: first light body"); bad = 1; } ++li; }
        if (lightSecond) { for (int k = 0; k < 8; ++k) if (light_seen[li][k] != 0) { std::puts("fail: second light words not cleared at the call"); bad = 1; }
            if (std::memcmp(light_seen[li] + 8, src + 1276, 52) != 0) { std::puts("fail: second light body"); bad = 1; } }
        // a light with both words zero stays zero and is not registered; otherwise the words are cleared
        for (int k = 0; k < 8; ++k) { if (arena[1208 + k] != 0 || arena[1268 + k] != 0) { std::puts("fail: light words not zero afterwards"); bad = 1; break; } }
    if (wordAt72AtCall[0] != lo(vt_10GameObject)) { std::puts("fail: table word at call 0"); bad = 1; }
    if (wordAt72AtCall[1] != lo(vt_10GameObject)) { std::puts("fail: table word at call 1"); bad = 1; }
    if (wordAt72AtCall[2] != lo(vt_10GameObject)) { std::puts("fail: table word at call 2"); bad = 1; }
    if (wordAt72AtCall[3] != lo(vt_8Creature)) { std::puts("fail: table word at call 3"); bad = 1; }
    if (wordAt72AtCall[4] != lo(vt_8Creature)) { std::puts("fail: table word at call 4"); bad = 1; }
    if (wordAt72AtCall[5] != lo(vt_8Creature)) { std::puts("fail: table word at call 5"); bad = 1; }
    if (wordAt72AtCall[6] != lo(vt_8Creature)) { std::puts("fail: table word at call 6"); bad = 1; }
    if (wordAt72AtCall[7] != lo(vt_6Player)) { std::puts("fail: table word at call 7"); bad = 1; }
    if (wordAt72AtCall[8] != lo(vt_6Player)) { std::puts("fail: table word at call 8"); bad = 1; }
    if (wordAt72AtCall[9] != lo(vt_6Player)) { std::puts("fail: table word at call 9"); bad = 1; }
    if (wordAt72AtCall[10] != lo(vt_6Player)) { std::puts("fail: table word at call 10"); bad = 1; }
    if (wordAt72AtCall[11] != lo(vt_6Player)) { std::puts("fail: table word at call 11"); bad = 1; }
    if (wordAt72AtCall[12] != lo(vt_6Player)) { std::puts("fail: table word at call 12"); bad = 1; }
    if (wordAt72AtCall[13] != lo(vt_6Player)) { std::puts("fail: table word at call 13"); bad = 1; }
    if (wordAt72AtCall[14] != lo(vt_6Player)) { std::puts("fail: table word at call 14"); bad = 1; }
        unsigned w72; std::memcpy(&w72, arena + 72, 4);
        if (w72 != lo(vt_6Player)) { std::puts("fail: final table pointer"); bad = 1; }
        const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{76,76},{80,85},{88,156},{160,173},{176,189},{192,205},{208,233},{236,285},{552,570},{572,573},{580,749},{756,800},{804,887},{892,964},{968,975},{980,999},{1004,1023},{1028,1047},{1052,1071},{1076,1108},{1112,1152},{1156,1189},{1196,1207},{1208,1267},{1268,1327},{1328,1355},{1400,1403}};
        static bool copied[1472];
        std::memset(copied, 0, sizeof copied);
        for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
        for (unsigned i = 0; i < 1472; ++i) {
            bool head = (i >= 0 && i < 4) || (i >= 4 && i < 8) || (i >= 8 && i < 12) || (i >= 288 && i < 552) || (i >= 576 && i < 580) || (i >= 752 && i < 756) || (i >= 888 && i < 892) || (i >= 976 && i < 980) || (i >= 1000 && i < 1004) || (i >= 1024 && i < 1028) || (i >= 1048 && i < 1052) || (i >= 1072 && i < 1076) || (i >= 1192 && i < 1196) || (i >= 1356 && i < 1396) || (i >= 1396 && i < 1400);
            bool light = (i >= 1208 && i < 1216) || (i >= 1268 && i < 1276);
            if (i >= 72 && i < 76) continue;
            if (light) continue;
            if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
            if (!copied[i] && !head && arena[i] != 0xa5) { std::printf("fail: byte %u written but not in a copy range\n", i); bad = 1; break; }
        }
    }
    return bad;
}

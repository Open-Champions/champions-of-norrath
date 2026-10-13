// Observed clone__6Player at 0x00116388, 2280 bytes.
// Allocates 1408 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source, the
// AnimationState8 copy constructor copies 264 bytes; the two 60-byte lights at +1208 and +1268 are each
// copied whole, and then registered only when one of the light's first two words is non-zero: both words
// are cleared and the light's address is passed to lightAdd__FP5Light):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the head at +0
//  - the ListHead copy constructor runs for the head at +4
//  - the ListHead copy constructor runs for the head at +8
//  - source bytes 12..41 are copied
//  - source bytes 44..57 are copied
//  - source bytes 60..71 are copied
//  - the 8Creature table pointer (_vt$8Creature) goes to n+72
//  - source bytes 76..76 are copied
//  - source bytes 80..85 are copied
//  - source bytes 88..156 are copied
//  - source bytes 160..173 are copied
//  - source bytes 176..189 are copied
//  - source bytes 192..205 are copied
//  - source bytes 208..233 are copied
//  - source bytes 236..285 are copied
//  - the AnimationState8 copy constructor runs for the member at +288
//  - source bytes 552..570 are copied
//  - source bytes 572..573 are copied
//  - the ListHead copy constructor runs for the head at +576
//  - source bytes 580..749 are copied
//  - the ListHead copy constructor runs for the head at +752
//  - source bytes 756..800 are copied
//  - source bytes 804..887 are copied
//  - the ListHead copy constructor runs for the head at +888
//  - the 6Player table pointer (_vt$6Player) goes to n+72
//  - source bytes 892..964 are copied
//  - source bytes 968..975 are copied
//  - the ListHead copy constructor runs for the head at +976
//  - source bytes 980..999 are copied
//  - the ListHead copy constructor runs for the head at +1000
//  - source bytes 1004..1023 are copied
//  - the ListHead copy constructor runs for the head at +1024
//  - source bytes 1028..1047 are copied
//  - the ListHead copy constructor runs for the head at +1048
//  - source bytes 1052..1071 are copied
//  - the ListHead copy constructor runs for the head at +1072
//  - source bytes 1076..1108 are copied
//  - source bytes 1112..1152 are copied
//  - source bytes 1156..1189 are copied
//  - the ListHead copy constructor runs for the head at +1192
//  - source bytes 1196..1207 are copied
//  - source bytes 1208..1267 are copied
//  - the light at +1208 is registered (register_light, below)
//  - source bytes 1268..1327 are copied
//  - the light at +1268 is registered (register_light, below)
//  - source bytes 1328..1355 are copied
//  - the AnimationState copy constructor runs for the member at +1356
//  - the ListHead copy constructor runs for the head at +1396
//  - source bytes 1400..1403 are copied.
// Bytes of the new object not named above are left as the allocator returned them (gaps of 1 to 8 bytes
// between fields, 1190..1191 and 1404..1407; 1356..1395 is left to the AnimationState copy constructor and 1396..1399 to
// the last ListHead copy). The new object is returned. Copy ranges and call
// order were measured by running the original on a pattern-filled source (table pointer stores are only
// ordered relative to the calls). Inferred: the parent chain from the table pointers and that the lights
// are registered with the lighting list from the name. Unknown: the meaning of the fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void AnimationState8_copy(void* destination, const void* source) __asm__("__15AnimationState8RC15AnimationState8");
void AnimationState_copy(void* destination, const void* source) __asm__("__14AnimationStateRC14AnimationState");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_8Creature[] __asm__("_vt$8Creature");
extern unsigned char vt_6Player[] __asm__("_vt$6Player");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
typedef unsigned __attribute__((may_alias)) AliasedWord;
inline __attribute__((always_inline)) void register_light(unsigned char* light) {
    AliasedWord* words = reinterpret_cast<AliasedWord*>(light);
    if (words[0] != 0 || words[1] != 0) {
        words[0] = 0;
        words[1] = 0;
        lightAdd__FP5Light(light);
    }
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__6Player(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(1408));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_8Creature);
    copy(n, s, 76, 76);
    copy(n, s, 80, 85);
    copy(n, s, 88, 156);
    copy(n, s, 160, 173);
    copy(n, s, 176, 189);
    copy(n, s, 192, 205);
    copy(n, s, 208, 233);
    copy(n, s, 236, 285);
    AnimationState8_copy(n + 288, s + 288);
    copy(n, s, 552, 570);
    copy(n, s, 572, 573);
    ListHead_copy(n + 576, s + 576);
    copy(n, s, 580, 749);
    ListHead_copy(n + 752, s + 752);
    copy(n, s, 756, 800);
    copy(n, s, 804, 887);
    ListHead_copy(n + 888, s + 888);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_6Player);
    copy(n, s, 892, 964);
    copy(n, s, 968, 975);
    ListHead_copy(n + 976, s + 976);
    copy(n, s, 980, 999);
    ListHead_copy(n + 1000, s + 1000);
    copy(n, s, 1004, 1023);
    ListHead_copy(n + 1024, s + 1024);
    copy(n, s, 1028, 1047);
    ListHead_copy(n + 1048, s + 1048);
    copy(n, s, 1052, 1071);
    ListHead_copy(n + 1072, s + 1072);
    copy(n, s, 1076, 1108);
    copy(n, s, 1112, 1152);
    copy(n, s, 1156, 1189);
    ListHead_copy(n + 1192, s + 1192);
    copy(n, s, 1196, 1207);
    copy(n, s, 1208, 1267);
    register_light(n + 1208);
    copy(n, s, 1268, 1327);
    register_light(n + 1268);
    copy(n, s, 1328, 1355);
    AnimationState_copy(n + 1356, s + 1356);
    ListHead_copy(n + 1396, s + 1396);
    copy(n, s, 1400, 1403);
    return n;
}

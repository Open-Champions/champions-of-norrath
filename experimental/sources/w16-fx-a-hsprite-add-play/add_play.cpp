// Observed: AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene at 0x0105fe50, 644 bytes. Arguments: the sprite (this), an animation id A, a playback
// type T, a float F and the scene S. Offsets are bytes. "Pool P" is this+120, whose element base is the word at P+8 and whose elements are
// 104 bytes; "pool Q" is this+144, with 52-byte elements.
//  1. map = RefMap__7VIScenei(S, the word at this+384); anim = Animation__7VIScenei(S, A).
//  2. i = Add__t6VIPool1Z13VIHSpritePlay(P); a negative result returns -1. ShareAnimation__7VIScenei(S, A) negative returns -1 (the play
//     slot already added is left in place).
//  3. The play record E at P's base + 104 * i is filled: +8 = 1, +0 = A, +4 = T, +12 = 0; then t = GetTime__7VIScene(S) (a 64-bit value) is
//     stored at +16 and +32 = 0, +36 = the float at anim+24, +24 = a zero doubleword, +40 = F, +44, +48, +52, +56 = 0; a second GetTime(S)
//     is stored at +64 after +88 = -1, +80 = 0, +84 = -1 and +72 = 0. The element base is re-read after each GetTime call.
//  4. For k from 0 while k < the word at anim+12 (re-read after each pass): key = the word at (anim+36 pointer) + 8 * k;
//     j = Find__Ct5VIMap2ZiZiRCi(map+8, &key); a value v = -1 when j equals End__Ct5VIMap2ZiZi(map+8), else the word at offset 20 of the
//     24-byte entry j of the map's element array (the word at map+16). A v of -1 is skipped. Otherwise n =
//     Add__t6VIPool1Z17VIHSpritePlayNode(Q): negative returns -1. With E re-derived, +84 = n when +80 is zero, then +88 = n. The 52-byte node n
//     gets +4 = v, +0 = 0 and +8 = the second word of the animation's entry k; finally E+80 is incremented.
//  5. PrecalcPlayback__9VIHSpritei(this, i) runs; the result is -1 when it is negative and i otherwise.
// Inferred: a play record keeps its first and last node numbers (+84, +88) and a node count (+80) for the animation's keyed entries that exist in
// the sprite's map (map entries whose value is -1 are absent); times are scene clock values.
// Unknown: the types of the +16/+24/+64 doublewords' uses, what the map and its keys are, and the field names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
unsigned char* RefMap__7VIScenei(void* scene, int index);
unsigned char* Animation__7VIScenei(void* scene, int id);
int ShareAnimation__7VIScenei(void* scene, int id);
long long GetTime__7VIScene(void* scene);
int Add__t6VIPool1Z13VIHSpritePlay(void* pool);
int Add__t6VIPool1Z17VIHSpritePlayNode(void* pool);
int Find__Ct5VIMap2ZiZiRCi(void* map, const int* key);
int End__Ct5VIMap2ZiZi(void* map);
int PrecalcPlayback__9VIHSpritei(void* self, int index);

static inline unsigned char* Words(unsigned char* base, int offset) {
    return NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(base + offset));
}

int AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(unsigned char* self, int animationId, int playback, float rate, void* scene) {
    unsigned char* map = RefMap__7VIScenei(scene, *reinterpret_cast<int*>(self + 384));
    unsigned char* plays = self + 120;
    unsigned char* animation = Animation__7VIScenei(scene, animationId);
    int index = Add__t6VIPool1Z13VIHSpritePlay(plays);
    if (index < 0) return -1;
    if (ShareAnimation__7VIScenei(scene, animationId) < 0) return -1;
    int offset = index * 104;
    unsigned char* play = Words(plays, 8) + offset;
    *reinterpret_cast<unsigned*>(play + 8) = 1;
    *reinterpret_cast<int*>(play) = animationId;
    *reinterpret_cast<int*>(play + 4) = playback;
    *reinterpret_cast<unsigned*>(play + 12) = 0;
    long long start = GetTime__7VIScene(scene);
    play = Words(plays, 8) + offset;
    *reinterpret_cast<unsigned*>(play + 32) = 0;
    *reinterpret_cast<long long*>(play + 16) = start;
    float nominal = *reinterpret_cast<float*>(animation + 24);
    *reinterpret_cast<long long*>(play + 24) = 0;
    *reinterpret_cast<float*>(play + 40) = rate;
    *reinterpret_cast<float*>(play + 36) = nominal;
    *reinterpret_cast<unsigned*>(play + 56) = 0;
    *reinterpret_cast<unsigned*>(play + 44) = 0;
    *reinterpret_cast<unsigned*>(play + 48) = 0;
    *reinterpret_cast<unsigned*>(play + 52) = 0;
    long long now = GetTime__7VIScene(scene);
    play = Words(plays, 8) + offset;
    *reinterpret_cast<int*>(play + 88) = -1;
    *reinterpret_cast<unsigned*>(play + 80) = 0;
    *reinterpret_cast<int*>(play + 84) = -1;
    *reinterpret_cast<long long*>(play + 64) = now;
    *reinterpret_cast<unsigned*>(play + 72) = 0;
    if (*reinterpret_cast<int*>(animation + 12) > 0) {
        unsigned char* nodes = self + 144;
        int k = 0;
        do {
            int entry = k * 8;
            int key = *reinterpret_cast<int*>(Words(animation, 36) + entry);
            ++k;
            int found = Find__Ct5VIMap2ZiZiRCi(map + 8, &key);
            int value;
            if (found == End__Ct5VIMap2ZiZi(map + 8)) value = -1;
            else value = *reinterpret_cast<int*>(Words(map, 16) + found * 24 + 20);
            if (value != -1) {
                int node = Add__t6VIPool1Z17VIHSpritePlayNode(nodes);
                if (node < 0) return -1;
                play = Words(plays, 8) + offset;
                if (*reinterpret_cast<int*>(play + 80) == 0) *reinterpret_cast<int*>(play + 84) = node;
                unsigned char* slot = Words(nodes, 8) + node * 52;
                *reinterpret_cast<int*>(play + 88) = node;
                *reinterpret_cast<int*>(slot + 4) = value;
                *reinterpret_cast<unsigned*>(slot) = 0;
                *reinterpret_cast<int*>(slot + 8) = *reinterpret_cast<int*>(Words(animation, 36) + entry + 4);
                *reinterpret_cast<int*>(play + 80) += 1;
            }
        } while (k < *reinterpret_cast<int*>(animation + 12));
    }
    int result = PrecalcPlayback__9VIHSpritei(self, index);
    return result < 0 ? -1 : index;
}
}

// Observed playerDamageHandler__6Player at 0x001a2740, 2012 bytes. `self` is the
// Player object; offsets below are bytes from it, and "rec" is the player record
// player + self[960] * 23984 (self[960] is the player index). Pointer fields are
// 32-bit words on the target. Field meanings are not established; the names
// used below are only labels for offsets.
//  1. If self[744] bit 0x20000000 is clear: when cutMode is nonzero or
//     DLG_Active__Fv() is nonzero, clear self[744] and the float self[192]
//     and return.
//  2. tier = 0. If any of bits 0x3c0000 of self[744] is set, the lowest-set of
//     0x40000, 0x80000, 0x100000, 0x200000 gives tier 1, 2, 3, 3 and stores
//     100, 150, 200, 250 (floats) to self[756].
//  3. When tier > 0: if self[1400] == 8 and the signed 16-bit difference of the
//     halfwords self[748] - self[56] is below -24575 or at least 24576, tier = 1.
//     Then self[1400] = 3, self[1088] = frameNm, netForceLocalUpdate = 1 when
//     netPlayerIsLocal(index), and playerPlaySound__Fii(index, 12). With tier >= 2
//     an animation name is built: "knockdown2_" when the byte rec[23582] is 2 or
//     3, else "knockdown1_", followed by "a.anm" (difference below -24575 or at
//     least 24576), "b.anm" (-24575..-8192), "c.anm" (-8191..8192) or "d.anm"
//     (8193..24575). If playerIsAnimPlaying__6PlayerPc(self, name) is zero:
//     when the signed byte self[292] is at least 5, the float at
//     self[284 + 32*self[292]] is set to a constant and the float at
//     self[280 + 32*self[292]] is limited to 1.0; then the lump named by the
//     first word of *(self[36]) / name is found and added with animAddOneShot
//     (flag 2058). With tier < 2 the same slot update runs (different constant)
//     and animAddOneShot is called with the word at *(self[36]) + 104 *
//     rec[23582] + 124 and flag 10.
//  4. damage = float self[192]. If it is not positive, or self[1400] == 4, clear
//     self[744] and return. If cheatInvulnerability is nonzero and bit
//     0x20000000 of self[744] is clear, damage and self[192] become 0. The float
//     rec[172] is reduced by damage. A point (self[16], self[20], self[24] + 36)
//     is built. If (g_BloodLevel > 0 or netNetworkMode != 0) and self[744] &
//     0xd: addEffect(0, point) when g_BloodLevel > 0, then one step of the
//     linear congruential generator on syncRandState (x*1664525+1013904223)
//     and, when (x >> 16) & 7 == 0, a GutChunk of 616 bytes from blockAlloc is
//     constructed with (point, 2, 1, 1.0f). If self[744] & 0x10 and rec[172] <= 0,
//     disintegrationEffect is called with (self, *(self+1332), self+288, a copy
//     of self[16..27], (short)self[56], 1.0f, 0.0f, 0).
//  5. self[192] = 0 and self[12] |= 0x100. If rec[172] <= 0: self[12] |= 0x108 (from
//     the old value), rec[172] = 0, a transition is added with the word at
//     *(self[36]) + 20 when rec[84] is 0, 1, 4, 5, 8 or 9 and rec[23582] == 2, else
//     with the word at *(self[36]) + 12 (constant 0.5, flag 518); then self[12] |= 1,
//     self[1400] = 4, self[1088] = frameNm, enumerateAlivePlayers__Fv(), and
//     when local gameNetKick__Fi(1) with netForceLocalUpdate = 1. With nmPlayers
//     >= 2 and the other player index (self[960] == 0) alive (rec[172] > 0)
//     playerPlaySound__Fii(other, 3) is called. self[744] = 0.
//     Otherwise (still alive): return with self[744] = 0 if self[744] & 0x400000,
//     self[1400] is 3 or 7, or self[1092] == 2. A chance 0.04 - rec[224] * 0.04
//     (limited to 0) times 65536 is compared with the high half of the next
//     generator value; if the value is not below it and self[744] & 0x4000000
//     is clear, return with self[744] = 0. Otherwise self[1400] = 3,
//     self[1088] = frameNm, local update flag, playerPlaySound__Fii(index, 12),
//     the slot update of step 3, and animAddOneShot with flag 10; self[744] = 0.
// Constants that are not exactly representable are stored as the bit patterns
// found in the image (one unit below the nearest float). Whether the hard-coded
// class values 0, 1, 4, 5, 8, 9 have a common meaning is unknown.
struct Point3 {
    float x, y, z;
};

extern "C" {
extern unsigned char player[];
extern signed char cutMode;
extern unsigned char netForceLocalUpdate;
extern signed char netNetworkMode;
extern int frameNm;
extern int cheatInvulnerability;
extern int g_BloodLevel;
extern int nmPlayers;
extern unsigned syncRandState;

int DLG_Active__Fv();
int netPlayerIsLocal__Fi(int index);
void playerPlaySound__Fii(int index, int sound);
int sprintf(char* dest, const char* format, ...);
char* strcat(char* dest, const char* src);
int playerIsAnimPlaying__6PlayerPc(void* self, char* name);
void* lumpFindResource__FPcT0(char* a, char* b);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b,
                                                             int flags);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float a, int flags);
void addEffect__FiG6Point3(int kind, const Point3* where);
void* blockAlloc__Fi(int size);
void __8GutChunkG6Point3iif(void* chunk, const Point3* where, int a, int b, float c);
void disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(
    void* object, void* model, void* state, const Point3* where, short s, float a, float b, int c);
void enumerateAlivePlayers__Fv();
void gameNetKick__Fi(int value);
}

static inline unsigned& W(unsigned char* base, int off) { return *(unsigned*)(base + off); }
static inline float& F(unsigned char* base, int off) { return *(float*)(base + off); }
static inline void* PW(unsigned char* base, int off) { return (void*)(unsigned long)*(unsigned*)(base + off); }
static inline float bitsToFloat(unsigned bits) { return __builtin_bit_cast(float, bits); }

// Floats read from the image as raw words (0xbdaaaaaa, 0x3dcccccc, 0x3d23d70a).
static const unsigned kSlotBitsA = 0xbdaaaaaau;
static const unsigned kBlendBits = 0x3dccccccu;
static const unsigned kChanceBits = 0x3d23d70au;

static void limitSlot(unsigned char* self) {
    int slot = (signed char)self[292];
    if (slot >= 5) {
        F(self, 284 + slot * 32) = bitsToFloat(kSlotBitsA);
        float& limit = F(self, 280 + (signed char)self[292] * 32);
        if (1.0f < limit) limit = 1.0f;
    }
}

extern "C" void playerDamageHandler__6Player(unsigned char* self) {
    unsigned flags = W(self, 744);
    int tier = 0;
    if (!(flags & 0x20000000u)) {
        if (cutMode != 0 || DLG_Active__Fv() != 0) {
            W(self, 744) = 0;
            F(self, 192) = 0.0f;
            return;
        }
        flags = W(self, 744);
    }
    if (flags & 0x3c0000u) {
        if (flags & 0x40000u) { tier = 1; F(self, 756) = 100.0f; }
        else if (flags & 0x80000u) { tier = 2; F(self, 756) = 150.0f; }
        else if (flags & 0x100000u) { tier = 3; F(self, 756) = 200.0f; }
        else if (flags & 0x200000u) { tier = 3; F(self, 756) = 250.0f; }
    }

    if (tier > 0) {
        int turn = (short)(*(unsigned short*)(self + 748) - *(unsigned short*)(self + 56));
        if (W(self, 1400) == 8 && (turn < -24575 || turn >= 24576)) tier = 1;
        W(self, 1400) = 3;
        W(self, 1088) = frameNm;
        if (netPlayerIsLocal__Fi(W(self, 960))) netForceLocalUpdate = 1;
        playerPlaySound__Fii(W(self, 960), 12);
        if (tier >= 2) {
            char name[64];
            unsigned char kind = player[W(self, 960) * 23984 + 23582];
            sprintf(name, (unsigned)(kind - 2) < 2 ? "knockdown2_" : "knockdown1_");
            if (turn < -24575 || turn >= 24576) strcat(name, "a.anm");
            else if ((unsigned short)(turn + 24576) <= 16384) strcat(name, "b.anm");
            else if ((unsigned short)(turn + 8192) <= 16384) strcat(name, "c.anm");
            else strcat(name, "d.anm");
            if (playerIsAnimPlaying__6PlayerPc(self, name) == 0) {
                limitSlot(self);
                unsigned char* table = (unsigned char*)PW(self, 36);
                void* header = lumpFindResource__FPcT0((char*)PW(table, 0), name);
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(self + 288, header, bitsToFloat(kBlendBits),
                                                                       bitsToFloat(kBlendBits), 2058);
            }
        } else {
            limitSlot(self);
            unsigned char* table = (unsigned char*)PW(self, 36);
            int kind = (signed char)player[W(self, 960) * 23984 + 23582];
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(self + 288, PW(table, kind * 104 + 124),
                                                                   bitsToFloat(kBlendBits), bitsToFloat(kBlendBits), 10);
        }
    }

    float damage = F(self, 192);
    if (!(0.0f < damage) || W(self, 1400) == 4) {
        W(self, 744) = 0;
        return;
    }
    flags = W(self, 744);
    if (cheatInvulnerability != 0 && !(flags & 0x20000000u)) {
        F(self, 192) = 0.0f;
        damage = 0.0f;
    }
    int index = W(self, 960);
    F(player + index * 23984, 172) = F(player + index * 23984, 172) - damage;
    Point3 point;
    point.x = F(self, 16);
    point.y = F(self, 20);
    point.z = F(self, 24) + 36.0f;
    if ((g_BloodLevel > 0 || netNetworkMode != 0) && (flags & 0xdu)) {
        if (g_BloodLevel > 0) addEffect__FiG6Point3(0, &point);
        syncRandState = syncRandState * 1664525u + 1013904223u;
        if (((syncRandState >> 16) & 7) == 0) {
            void* chunk = blockAlloc__Fi(616);
            if (chunk != 0) __8GutChunkG6Point3iif(chunk, &point, 2, 1, 1.0f);
        }
        flags = W(self, 744);
        index = W(self, 960);
    }
    if (flags & 0x10u) {
        if (F(player + index * 23984, 172) <= 0.0f) {
            Point3 copy;
            copy.x = F(self, 16);
            copy.y = F(self, 20);
            copy.z = F(self, 24);
            disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(
                self, PW(self, 1332), self + 288, &copy, *(short*)(self + 56), 1.0f, 0.0f, 0);
            index = W(self, 960);
        }
    }

    F(self, 192) = 0.0f;
    float zero = F(self, 192);
    unsigned oldFlags = W(self, 12);
    W(self, 12) = oldFlags | 0x100u;
    unsigned char* rec = player + index * 23984;
    if (F(rec, 172) <= zero) {
        W(self, 12) = oldFlags | 0x108u;
        F(rec, 172) = zero;
        unsigned cls = W(rec, 84);
        unsigned char* table = (unsigned char*)PW(self, 36);
        bool special = cls < 2 || cls == 4 || cls == 5 || cls == 8 || cls == 9;
        if (special && (signed char)player[index * 23984 + 23582] == 2)
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(self + 288, PW(table, 20), 0.5f, 518);
        else
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(self + 288, PW(table, 12), 0.5f, 518);
        unsigned word = W(self, 12) | 1u;
        W(self, 1400) = 4;
        W(self, 1088) = frameNm;
        W(self, 12) = word;
        enumerateAlivePlayers__Fv();
        if (netPlayerIsLocal__Fi(W(self, 960))) {
            gameNetKick__Fi(1);
            netForceLocalUpdate = 1;
        }
        if (nmPlayers >= 2) {
            unsigned other = W(self, 960) < 1u;
            if (0.0f < F(player + other * 23984, 172)) playerPlaySound__Fii(other, 3);
        }
        W(self, 744) = 0;
        return;
    }

    flags = W(self, 744);
    if (flags & 0x400000u) {
        W(self, 744) = 0;
        return;
    }
    unsigned state = W(self, 1400);
    if (state == 3 || state == 7 || W(self, 1092) == 2) {
        W(self, 744) = 0;
        return;
    }
    float chance = bitsToFloat(kChanceBits) - F(rec, 224) * bitsToFloat(kChanceBits);
    if (chance < zero) chance = zero;
    unsigned next = syncRandState * 1664525u + 1013904223u;
    syncRandState = next;
    float threshold = chance * 65536.0f;
    if (!((float)(int)(next >> 16) < threshold) && !(flags & 0x4000000u)) {
        W(self, 744) = 0;
        return;
    }
    W(self, 1088) = frameNm;
    W(self, 1400) = 3;
    if (netPlayerIsLocal__Fi(index)) netForceLocalUpdate = 1;
    playerPlaySound__Fii(W(self, 960), 12);
    limitSlot(self);
    unsigned char* table = (unsigned char*)PW(self, 36);
    int kind = (signed char)player[W(self, 960) * 23984 + 23582];
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(self + 288, PW(table, kind * 104 + 124),
                                                           bitsToFloat(kBlendBits), bitsToFloat(kBlendBits), 10);
    W(self, 744) = 0;
}

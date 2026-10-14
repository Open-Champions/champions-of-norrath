#include <cstring>
extern "C" {
unsigned char player[23984 * 2] __attribute__((aligned(8)));
unsigned char norrath_player_feat_end_name_view[4] = {'E', 'N', 'D', 0};
unsigned char norrath_player_feat_spell_table_view[104 * 6] __attribute__((aligned(8)));
void givePlayerFeat__Fxii(long long, int, int);
char* strcpy(char* d, const char* s) { char* o = d; while ((*d++ = *s++)) {} return o; }
int strcmp(const char* a, const char* b) { while (*a && *a == *b) ++a, ++b; return (unsigned char)*a - (unsigned char)*b; }
}

static unsigned char* row(int i) { return norrath_player_feat_spell_table_view + 104 * i; }
static unsigned char* rec(int p) { return player + p * 23984; }
static unsigned char* slotAt(int p, int i) { return rec(p) + 21976 + 104 * i; }
template <class T> static T& at(unsigned char* b, int o) { return *reinterpret_cast<T*>(b + o); }

// A row with a recognisable value in every byte, a name, a feat word and the halfword at +32.
static void setRow(int i, const char* name, long long feat, short h32) {
    unsigned char* r = row(i);
    for (int k = 0; k < 104; ++k) r[k] = (unsigned char)(0x80 + i * 7 + k);
    std::memset(r, 0, 32); std::strcpy((char*)r, name);
    at<long long>(r, 72) = feat; at<short>(r, 32) = h32;
    at<float>(r, 44) = 1.5f; at<float>(r, 64) = 2.5f;
}
static void reset() {
    std::memset(player, 0, sizeof player);
    for (int p = 0; p < 2; ++p) for (int i = 0; i < 15; ++i) { at<short>(slotAt(p, i), 32) = -1; }
    std::memset(norrath_player_feat_spell_table_view, 0, sizeof norrath_player_feat_spell_table_view);
    setRow(0, "alpha", 1, 7); setRow(1, "beta", 64, 9); setRow(2, "gamma", 128, 29); setRow(3, "delta", 256, 11);
    setRow(4, "END", 0, 0); setRow(5, "after", 2, 5);
}
static bool sameAsRow(unsigned char* slot, int ri) {
    unsigned char* r = row(ri);
    static const struct { int off, len; } copied[] = {{32, 2}, {34, 2}, {36, 1}, {37, 1}, {38, 2}, {40, 2}, {44, 4}, {48, 16}, {64, 4}, {72, 8}, {80, 16}, {96, 4}};
    if (std::strcmp((char*)slot, (char*)r)) return false;
    for (auto& c : copied) if (std::memcmp(slot + c.off, r + c.off, c.len)) return false;
    return true;
}
static bool untouched(unsigned char* slot, int ri) {
    // bytes outside the copied fields and the name are still the original filler
    static const int gaps[][2] = {{42, 2}, {68, 4}, {100, 4}};
    for (auto& g : gaps) for (int k = 0; k < g[1]; ++k) if (slot[g[0] + k] != 0) return false;
    (void)ri;
    return true;
}

int main() {
    // find a slot: slot 0 is free, row 0 matches feat 1
    reset();
    givePlayerFeat__Fxii(1, 0, -1);
    if (!sameAsRow(slotAt(0, 0), 0) || !untouched(slotAt(0, 0), 0)) return 1;
    if (at<long long>(rec(0), 320) != 1) return 2;
    // slots 0..2 busy -> slot 3 used; the same feat again is refused (mask), nothing else touched
    reset(); for (int i = 0; i < 3; ++i) at<short>(slotAt(0, i), 32) = 5;
    givePlayerFeat__Fxii(1, 0, -1);
    if (!sameAsRow(slotAt(0, 3), 0) || at<short>(slotAt(0, 4), 32) != -1 || at<short>(slotAt(0, 0), 32) != 5) return 3;
    unsigned char before[23984]; std::memcpy(before, rec(0), sizeof before);
    givePlayerFeat__Fxii(1, 0, -1);
    if (std::memcmp(before, rec(0), sizeof before)) return 4;
    // all 15 busy: nothing happens
    reset(); for (int i = 0; i < 15; ++i) at<short>(slotAt(1, i), 32) = 5;
    std::memcpy(before, rec(1), sizeof before);
    givePlayerFeat__Fxii(1, 1, -1);
    if (std::memcmp(before, rec(1), sizeof before)) return 5;
    // an explicit slot is used even if busy, and the mask is not consulted
    reset(); at<short>(slotAt(0, 9), 32) = 5; at<long long>(rec(0), 320) = 1;
    givePlayerFeat__Fxii(1, 0, 9);
    if (!sameAsRow(slotAt(0, 9), 0) || at<short>(slotAt(0, 0), 32) != -1) return 6;
    // the player index selects the record
    reset(); givePlayerFeat__Fxii(1, 1, -1);
    if (!sameAsRow(slotAt(1, 0), 0) || at<long long>(rec(1), 320) != 1 || at<long long>(rec(0), 320) != 0) return 7;
    // feat 64: +312 gets 15.0; row copied
    reset(); at<float>(rec(0), 312) = 100.0f;
    givePlayerFeat__Fxii(64, 0, -1);
    if (!sameAsRow(slotAt(0, 0), 1) || at<float>(rec(0), 312) != 115.0f || at<long long>(rec(0), 320) != 64) return 8;
    // feat 128 (row with h32 == 29): slot not written, floats adjusted: 172 clamped to 176
    reset(); at<float>(rec(0), 172) = 50; at<float>(rec(0), 176) = 40;
    givePlayerFeat__Fxii(128, 0, -1);
    if (at<short>(slotAt(0, 0), 32) != -1 || slotAt(0, 0)[0] != 0) return 9;
    if (at<float>(rec(0), 176) != 43.0f || at<float>(rec(0), 172) != 43.0f || at<long long>(rec(0), 320) != 128) return 10;
    reset(); at<float>(rec(0), 172) = 10; at<float>(rec(0), 176) = 40;
    givePlayerFeat__Fxii(128, 0, -1);
    if (at<float>(rec(0), 176) != 43.0f || at<float>(rec(0), 172) != 13.0f) return 11;
    // feat 256: +196/+200 with +5.0
    reset(); at<float>(rec(0), 196) = 30; at<float>(rec(0), 200) = 20;
    givePlayerFeat__Fxii(256, 0, -1);
    if (!sameAsRow(slotAt(0, 0), 3) || at<float>(rec(0), 200) != 25.0f || at<float>(rec(0), 196) != 25.0f) return 12;
    reset(); at<float>(rec(0), 196) = 2; at<float>(rec(0), 200) = 20;
    givePlayerFeat__Fxii(256, 0, -1);
    if (at<float>(rec(0), 200) != 25.0f || at<float>(rec(0), 196) != 7.0f) return 13;
    // other feats: no float changes
    reset(); at<float>(rec(0), 312) = 1; at<float>(rec(0), 172) = 2;
    givePlayerFeat__Fxii(1, 0, -1);
    if (at<float>(rec(0), 312) != 1.0f || at<float>(rec(0), 172) != 2.0f) return 14;
    // unknown feat before END: nothing, mask unchanged; a row after END is never reached
    reset(); givePlayerFeat__Fxii(4, 0, -1);
    if (at<long long>(rec(0), 320) != 0 || at<short>(slotAt(0, 0), 32) != -1) return 15;
    givePlayerFeat__Fxii(2, 0, -1);
    if (at<long long>(rec(0), 320) != 0 || at<short>(slotAt(0, 0), 32) != -1) return 16;
    // only the first matching row is used
    reset(); setRow(3, "dup", 1, 13);
    givePlayerFeat__Fxii(1, 0, -1);
    if (std::strcmp((char*)slotAt(0, 0), "alpha")) return 17;
    // 64-bit match: a feat differing only in the high word is a different feat
    reset(); setRow(0, "big", 0x100000001LL, 7);
    givePlayerFeat__Fxii(1, 0, -1);
    if (at<long long>(rec(0), 320) != 0) return 18;
    givePlayerFeat__Fxii(0x100000001LL, 0, -1);
    if (at<long long>(rec(0), 320) != 0x100000001LL || std::strcmp((char*)slotAt(0, 0), "big")) return 19;
    return 0;
}

// Observed playerRecomputeAbilityModifiers__Fv at 0x001a5480, 1344 bytes. For
// player index p = 0 .. nmPlayers-1, with record R = player + p * 23984 and
// object O = the 32-bit pointer at R+21872 (players whose pointer is zero are
// skipped), the routine does the following in order. Field meanings are not
// established; offsets are relative to R unless written O+.
//  1. Copy the four ints at R+120..R+132 to R+144..R+156 and clear the float at R+224.
//  2. For each of the 16 item pointers at R+21896+4k (k = 0..15, null skipped),
//     look up modifier values: the item has four 8-byte modifier slots starting
//     at item+36 whose halfword at +0 is an id and halfword at +2 a signed
//     value; the first slot with a matching id supplies the value, no match
//     gives 0. Ids 7, 8, 9, 10 are added into the ints R+144, R+148, R+152,
//     R+156; id 30 is converted to float, multiplied by 0.01f and added to R+224.
//  3. Bytes O+1124+j (j = 0..3) = R[144+4j] - 30 (j = 0, 2) or - 5 (j = 1, 3),
//     stored as a signed byte and clamped up to 0 if negative.
//  4. By the class word at R+80 (0..4): signed byte R+192 and float R+200 are set
//     to (18, 4.0), (16, 7.0), (14, 5.0), (14, 6.0), (12, 10.0); other class
//     values leave both unchanged.
//  5. R+176 = (float)(R[192] * (R[89] + 1)); then with n =
//     playerGetFeatRank__Fix(p, 128): R+176 = R+176 + (float)(3 * n) + (float)(R[156] / 2).
//  6. m = 6, 10, 8, 9, 14 for class 0..4 (0 otherwise); R+200 = (float)(m * R[89]);
//     n = playerGetFeatRank__Fix(O+960, 256); R+200 = R+200 + (float)(5 * n) +
//     (float)(R[148] / 2); R+312 = (float)(R[144] + 125) (taken before the next
//     call).
//  7. n = playerGetFeatRank__Fix(O+960, 64); R+312 += (float)n * 15.0f; add 5000.0f if
//     cheatMode is nonzero; clamp to at most 9999.0f.
//  8. If O is still nonzero, the halfword O+232 = recomputePaperDollModifiers__Fi(p).
//  9. R+196 = R+200 if R+200 < R+196; R+172 = R+176 if R+176 < R+172.
// The object pointer is reread where the code rereads it. Integer halving is
// signed division by two. Pointers in the records are 32-bit words.
extern "C" {
extern unsigned char player[];
extern int nmPlayers;
extern int cheatMode;
int playerGetFeatRank__Fix(int index, long long feat);
int recomputePaperDollModifiers__Fi(int index);

static inline int& Int(unsigned char* base, int off) { return *(int*)(base + off); }
static inline float& Flt(unsigned char* base, int off) { return *(float*)(base + off); }
static inline unsigned char* Ptr(unsigned char* record, int off) {
    return (unsigned char*)(unsigned long)*(unsigned*)(record + off);
}

static int modifierValue(const unsigned char* item, int id) {
    for (int j = 0; j < 4; ++j) {
        if (*(const short*)(item + 36 + 8 * j) == id) return *(const short*)(item + 38 + 8 * j);
    }
    return 0;
}

void playerRecomputeAbilityModifiers__Fv() {
    for (int p = 0; p < nmPlayers; ++p) {
        unsigned char* R = player + p * 23984;
        if (Ptr(R, 21872) == 0) continue;

        for (int i = 0; i < 4; ++i) Int(R, 144 + 4 * i) = Int(R, 120 + 4 * i);
        Int(R, 224) = 0;
        for (int k = 0; k < 16; ++k) {
            const unsigned char* item = Ptr(R, 21896 + 4 * k);
            if (item == 0) continue;
            Int(R, 144) += modifierValue(item, 7);
            Int(R, 148) += modifierValue(item, 8);
            Int(R, 152) += modifierValue(item, 9);
            Int(R, 156) += modifierValue(item, 10);
            Flt(R, 224) = Flt(R, 224) + (float)modifierValue(item, 30) * 0.01f;
        }

        for (int j = 0; j < 4; ++j) {
            unsigned char* O = Ptr(R, 21872);
            O[1124 + j] = (signed char)(Int(R, 144 + 4 * j) - ((j == 0 || j == 2) ? 30 : 5));
            O = Ptr(R, 21872);
            if ((signed char)O[1124 + j] < 0) O[1124 + j] = 0;
        }

        int cls = Int(R, 80);
        if (cls == 0) { R[192] = 18; Flt(R, 200) = 4.0f; }
        else if (cls == 2) { R[192] = 14; Flt(R, 200) = 5.0f; }
        else if (cls == 1) { R[192] = 16; Flt(R, 200) = 7.0f; }
        else if (cls == 4) { R[192] = 12; Flt(R, 200) = 10.0f; }
        else if (cls == 3) { R[192] = 14; Flt(R, 200) = 6.0f; }

        Flt(R, 176) = (float)((int)(signed char)R[192] * ((int)(signed char)R[89] + 1));
        int n = playerGetFeatRank__Fix(p, 128LL);
        Flt(R, 176) = Flt(R, 176) + (float)(n * 3) + (float)(Int(R, 156) / 2);

        int m = 0;
        if (cls == 0) m = 6;
        else if (cls == 2) m = 8;
        else if (cls == 1) m = 10;
        else if (cls == 3) m = 9;
        else if (cls == 4) m = 14;
        Flt(R, 200) = (float)(m * (int)(signed char)R[89]);
        n = playerGetFeatRank__Fix(Int(Ptr(R, 21872), 960), 256LL);
        Flt(R, 200) = Flt(R, 200) + (float)(n * 5) + (float)(Int(R, 148) / 2);
        Flt(R, 312) = (float)(Int(R, 144) + 125);

        n = playerGetFeatRank__Fix(Int(Ptr(R, 21872), 960), 64LL);
        Flt(R, 312) = Flt(R, 312) + (float)n * 15.0f;
        if (cheatMode != 0) Flt(R, 312) = Flt(R, 312) + 5000.0f;
        if (9999.0f < Flt(R, 312)) Flt(R, 312) = 9999.0f;

        if (Ptr(R, 21872) != 0) {
            int value = recomputePaperDollModifiers__Fi(p);
            *(short*)(Ptr(R, 21872) + 232) = (short)value;
        }
        if (Flt(R, 200) < Flt(R, 196)) Flt(R, 196) = Flt(R, 200);
        if (Flt(R, 176) < Flt(R, 172)) Flt(R, 172) = Flt(R, 176);
    }
}
}

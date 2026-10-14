// Observed givePlayerFeat__Fxii at 0x001be250, 996 bytes.
// Arguments: a 64-bit feat bit `feat`, a player index `p`, and a slot index `slot`
// (-1 means "find one"). Player records are 23984 bytes apart at `player`
// (0x00408600). Record fields used: a 64-bit mask at +320 (the feats already
// granted); fifteen 104-byte slots from +21976 (slot i at +21976 + 104*i, the
// halfword at slot+32 is -1 when the slot is free); floats at +172, +176, +196,
// +200 and +312.
//  1. slot == -1: if the mask already has any bit of `feat`, nothing happens.
//     Otherwise the first of the 15 slots whose halfword at +32 is -1 is used;
//     if there is none, nothing happens. A slot argument other than -1 is used
//     as given (not range checked).
//  2. The table `norrath_player_feat_spell_table_view` (spellTable.1065, records of
//     104 bytes) is scanned from its first record. A record whose name equals
//     "END" (the end-name view) ends the scan and the function returns; a record
//     whose 64-bit word at +72 differs from `feat` is skipped; for the first
//     record whose +72 word equals `feat`:
//       - if the halfword at record+32 is not 29, the record is copied into the slot
//         field by field: the name with strcpy, bytes +36 and +37, halfwords +32,
//         +34, +38 and +40, float +44, the 16 bytes at +48..+63, float +64, the
//         64-bit word +72, the 16 bytes at +80..+95 and the word +96. The other
//         bytes of the slot are left as they were.
//       - then by feat value: 64 adds 15.0 to the float at +312; 128 adds 3.0 to
//         the floats at +172 and +176 and then lowers +172 to +176 if +176 is the
//         smaller; 256 adds 5.0 to +196 and +200 and then lowers +196 to +200 if
//         +200 is the smaller; other values change none of these.
//       - finally `feat` is OR-ed into the mask at +320 and the function returns.
// The 64, 128 and 256 cases probably adjust resource maxima, but that is inferred
// only. The meaning of the value 29 is not established. The table, end name and
// libc routines are not defined here.
extern "C" {
extern unsigned char player[];
extern const unsigned char norrath_player_feat_spell_table_view[];
extern const unsigned char norrath_player_feat_end_name_view[];
char* strcpy(char*, const char*);
int strcmp(const char*, const char*);
}

template <class T> static inline T& at(unsigned char* base, int offset) { return *reinterpret_cast<T*>(base + offset); }
template <class T> static inline const T& at(const unsigned char* base, int offset) { return *reinterpret_cast<const T*>(base + offset); }

extern "C" void givePlayerFeat__Fxii(long long feat, int p, int slot) {
    unsigned char* record = player + p * 23984;
    if (slot == -1) {
        if ((at<long long>(record, 320) & feat) != 0) return;
        for (int i = 0; i < 15; ++i) {
            if (at<short>(record, 22008 + 104 * i) == -1) {
                slot = i;
                break;
            }
        }
        if (slot == -1) return;
    }
    unsigned char* destination = record + 21976 + 104 * slot;
    for (int i = 0;; ++i) {
        const unsigned char* entry = norrath_player_feat_spell_table_view + 104 * i;
        if (strcmp(reinterpret_cast<const char*>(entry), reinterpret_cast<const char*>(norrath_player_feat_end_name_view)) == 0) return;
        if (at<long long>(entry, 72) != feat) continue;
        if (at<short>(entry, 32) != 29) {
            strcpy(reinterpret_cast<char*>(destination), reinterpret_cast<const char*>(entry));
            at<unsigned char>(destination, 36) = at<unsigned char>(entry, 36);
            at<unsigned char>(destination, 37) = at<unsigned char>(entry, 37);
            at<unsigned short>(destination, 38) = at<unsigned short>(entry, 38);
            at<unsigned short>(destination, 40) = at<unsigned short>(entry, 40);
            at<float>(destination, 44) = at<float>(entry, 44);
            at<unsigned short>(destination, 32) = at<unsigned short>(entry, 32);
            at<unsigned short>(destination, 34) = at<unsigned short>(entry, 34);
            for (int k = 0; k < 4; ++k) at<unsigned short>(destination, 48 + 2 * k) = at<unsigned short>(entry, 48 + 2 * k);
            for (int k = 0; k < 4; ++k) at<unsigned short>(destination, 56 + 2 * k) = at<unsigned short>(entry, 56 + 2 * k);
            at<float>(destination, 64) = at<float>(entry, 64);
            at<long long>(destination, 72) = at<long long>(entry, 72);
            for (int k = 0; k < 4; ++k) at<unsigned>(destination, 80 + 4 * k) = at<unsigned>(entry, 80 + 4 * k);
            at<unsigned>(destination, 96) = at<unsigned>(entry, 96);
        }
        if (feat == 64) {
            at<float>(record, 312) = at<float>(record, 312) + 15.0f;
        } else if (feat == 128) {
            float high = at<float>(record, 176) + 3.0f;
            float low = at<float>(record, 172) + 3.0f;
            at<float>(record, 176) = high;
            at<float>(record, 172) = low;
            if (high < low) at<float>(record, 172) = high;
        } else if (feat == 256) {
            float high = at<float>(record, 200) + 5.0f;
            float low = at<float>(record, 196) + 5.0f;
            at<float>(record, 200) = high;
            at<float>(record, 196) = low;
            if (high < low) at<float>(record, 196) = high;
        }
        at<long long>(record, 320) = at<long long>(record, 320) | feat;
        return;
    }
}

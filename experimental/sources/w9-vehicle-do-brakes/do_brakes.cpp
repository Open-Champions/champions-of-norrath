// Observed doBrakes__7Vehicle at 0x002748a0, 316 bytes. Nothing is returned.
// Nothing here was executed on the EE. Field meanings are not established
// here, so offsets stay numeric. The float at +744 is the value this routine
// moves; call it x. d is the signed word at +956 (-1 means none).
//   - When the word at +2148 is non-zero: x = 1.0 if float(+848) < float(+840),
//     otherwise 0.0; nothing else changes.
//   - Else when the word at +2152 is non-zero: nothing.
//   - Else x goes up by A (float 0x3d088888) or down by B (float 0x3e088888).
//     Up is chosen when d == -1; or when d is a pad index (pad + 88*d + 8 is
//     the word tested, bit 0x80) with that bit clear or with gameControlPaused
//     non-zero, and the word at +1036 is non-zero; or with the bit set,
//     gameControlPaused zero, and either the pointer at player + 23984*d +
//     21872 is null or its word at +12 has bit 0x400000 clear. Down is chosen
//     when the bit is clear (or gameControlPaused is non-zero), or the pointer
//     is set with that bit set, and the word at +1036 is zero.
//     After going up with d != -1: if float(+840) < 2.0 then x = 0.0 and the
//     word at +2152 becomes 1.
//   - Finally x is clamped to [0.0, 1.0] (compare-and-store, so NaN stays).
extern "C" {
extern unsigned char pad[];
extern unsigned char player[];
extern int gameControlPaused;
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

static inline int& word(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedInt*>(base + offset);
}
static inline float& real(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedFloat*>(base + offset);
}

extern "C" void doBrakes__7Vehicle(unsigned char* vehicle) {
    if (word(vehicle, 2148) != 0) {
        if (real(vehicle, 848) < real(vehicle, 840)) real(vehicle, 744) = 1.0f;
        else word(vehicle, 744) = 0;
        return;
    }
    if (word(vehicle, 2152) != 0) return;

    const int driver = word(vehicle, 956);
    bool up;
    bool check_word = false;
    if (driver == -1) {
        up = true;
    } else if ((word(pad, 8 + 88 * driver) & 0x80) == 0) {
        check_word = true;
    } else if (gameControlPaused != 0) {
        check_word = true;
    } else {
        unsigned char* const record = *reinterpret_cast<AliasedPointer*>(player + 23984 * driver + 21872);
        if (record == 0) up = true;
        else if ((word(record, 12) & 0x400000) == 0) up = true;
        else check_word = true;
    }
    if (check_word) up = word(vehicle, 1036) != 0;
    if (up) {
        real(vehicle, 744) = real(vehicle, 744) + 0x1.111110p-5f;
        if (driver != -1 && real(vehicle, 840) < 2.0f) {
            word(vehicle, 744) = 0;
            word(vehicle, 2152) = 1;
        }
    } else {
        real(vehicle, 744) = real(vehicle, 744) - 0x1.111110p-3f;
    }
    if (real(vehicle, 744) < 0.0f) real(vehicle, 744) = 0.0f;
    if (1.0f < real(vehicle, 744)) real(vehicle, 744) = 1.0f;
}

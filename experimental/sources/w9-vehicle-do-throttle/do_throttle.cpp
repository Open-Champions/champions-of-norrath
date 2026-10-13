// Observed doThrottle__7Vehicle at 0x00274588, 788 bytes. Nothing is returned.
// Nothing here was executed on the EE. Field meanings are not established
// here, so offsets stay numeric. x is the float at +740, c the signed byte at
// +109, d the signed word at +956 (-1 = none), P the pointer at
// player + 23984*d + 21872, and pad[d] the word at pad + 88*d + 8. A is the
// float 0x40d55554 and Bt the float 0x3c888888. A Vehicle virtual is called
// through table slot offset 176 (the table entry at that offset is
// eventGasOn) or 184 (eventGasOff): halfword delta, pointer at +4.
//   Auto mode (word +2148 non-zero): x = clamp(x + ((float(+848)/float(+832) -
//   float(+840)/float(+832)) * 0.5) * Bt, 0, 1); nothing else.
//   Otherwise a button state s is chosen: if the signed byte cutMode is
//   non-zero, s = the word at +1040; else s = 0 unless d != -1, the word at
//   +1036 is zero, gameControlPaused is zero, and not (P != null and
//   P[12] & 0x400000): then s = pad[d] & 0x80 when the word at +2152 is
//   non-zero, pad[d] & 0x40 otherwise. (A set 0x400000 bit in P[12] leaves
//   s = 0.)
//   If c != 0 it drops by one first. Then with s set: if c == 0 and x == 0.0
//   the gas-on virtual runs and c = 30; x += A. With s clear: if c == 0 and
//   x == 1.0 the gas-off virtual runs and c = 30; x -= A. x is then clamped
//   to [0.0, 1.0].
//   If the word +2152 is non-zero: when d == -1 it becomes 0; when s is clear
//   and float(+840) < 1.0 it becomes 0; then, unless the word +1036 is
//   non-zero, gameControlPaused is non-zero, or P is set with its 0x400000
//   bit set, and only when pad[d] & 0x40 is set (d is used as is, even -1):
//   float(+840) < 4.0 clears +2152, otherwise float(+744) = 1.0.
extern "C" {
extern unsigned char pad[];
extern unsigned char player[];
extern int gameControlPaused;
extern signed char cutMode;
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));
typedef void (*VirtualFunction)(unsigned char*);
typedef VirtualFunction AliasedFunction __attribute__((may_alias));

static inline int& word(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedInt*>(base + offset);
}
static inline float& real(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedFloat*>(base + offset);
}
static inline void call_virtual(unsigned char* vehicle, int offset) {
    unsigned char* const entry = *reinterpret_cast<AliasedPointer*>(vehicle + 72) + offset;
    (*reinterpret_cast<AliasedFunction*>(entry + 4))(vehicle + *reinterpret_cast<AliasedShort*>(entry));
}

extern "C" void doThrottle__7Vehicle(unsigned char* vehicle) {
    const float step = 0x1.aaaaa8p+2f;
    if (word(vehicle, 2148) != 0) {
        const float a = real(vehicle, 848) / real(vehicle, 832);
        const float b = real(vehicle, 840) / real(vehicle, 832);
        real(vehicle, 740) = real(vehicle, 740) + (a - b) * 0.5f * 0x1.111110p-6f;
        if (real(vehicle, 740) < 0.0f) real(vehicle, 740) = 0.0f;
        if (1.0f < real(vehicle, 740)) real(vehicle, 740) = 1.0f;
        return;
    }
    int pressed = 0;
    const int d = word(vehicle, 956);
    signed char* const counter = reinterpret_cast<signed char*>(vehicle + 109);
    if (cutMode != 0) {
        pressed = word(vehicle, 1040);
    } else if (d != -1 && word(vehicle, 1036) == 0 && gameControlPaused == 0) {
        unsigned char* const record = *reinterpret_cast<AliasedPointer*>(player + 23984 * d + 21872);
        if (record == 0 || (word(record, 12) & 0x400000) == 0) {
            if (word(vehicle, 2152) != 0) pressed = word(pad, 8 + 88 * d) & 0x80;
            else pressed = word(pad, 8 + 88 * d) & 0x40;
        }
    }
    if (*counter != 0) *counter = static_cast<signed char>(*counter - 1);
    if (pressed != 0) {
        if (*counter == 0 && real(vehicle, 740) == 0.0f) {
            call_virtual(vehicle, 176);
            *counter = 30;
        }
        real(vehicle, 740) = real(vehicle, 740) + step;
    } else {
        if (*counter == 0 && real(vehicle, 740) == 1.0f) {
            call_virtual(vehicle, 184);
            *counter = 30;
        }
        real(vehicle, 740) = real(vehicle, 740) - step;
    }
    if (real(vehicle, 740) < 0.0f) real(vehicle, 740) = 0.0f;
    if (1.0f < real(vehicle, 740)) real(vehicle, 740) = 1.0f;

    if (word(vehicle, 2152) == 0) return;
    if (d == -1) word(vehicle, 2152) = 0;
    if (pressed == 0 && real(vehicle, 840) < 1.0f) word(vehicle, 2152) = 0;
    if (word(vehicle, 1036) != 0) return;
    if (gameControlPaused != 0) return;
    unsigned char* const record = *reinterpret_cast<AliasedPointer*>(player + 23984 * d + 21872);
    if (record != 0 && (word(record, 12) & 0x400000) != 0) return;
    if ((word(pad, 8 + 88 * d) & 0x40) == 0) return;
    if (real(vehicle, 840) < 4.0f) word(vehicle, 2152) = 0;
    else real(vehicle, 744) = 1.0f;
}

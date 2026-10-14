// Observed msg_run__7Vehicle at 0x270670, extent 1616. This is a behavior
// reconstruction; the compiled body differs from the image. Offsets are bytes
// from the vehicle. Observed flow, in order:
//  A. objectIsOffGrid(vehicle) nonzero: return.
//  B. If the float at +808 is nonzero it drops by 0x3f7fffff (about 0.99999994,
//     word at 0x482160); a result below zero is stored as 0.0.
//  C. If gameLineUpdatePosition(+2272) is nonzero, the point (0, -f(+896)*0.5, 0)
//     goes through matTransform with the matrix33 at +1116 into +656, and the
//     point at +16 is added to +656 per axis.
//  D. HUD text. G = localNmPlayersAlive (0x484f48). When G >= 2, word +2148 is 0
//     and word +956 is not -1: if word +980 differs from G, word +1036 becomes 1
//     and the text "vehicleWait" (0x475f58) is shown for 180 with word +956,
//     and the rest of this step and step E's text block are skipped; if word
//     +980 equals G, fall through to the clear test. Otherwise (the condition
//     fails) the same clear test runs: a nonzero +1036 shows hudClearInfoMessage and zeroes
//     +1036. Then, if +1036 is 0, +1032 is nonzero, +1024 is 0 and
//     hudInfoMessageShowing is 0: +1032 becomes 0, the text "vehicleHelpDriver"
//     (0x475f68) is formatted with usprintf using gameButtonToIcon(64) and
//     gameButtonToIcon(128) as floats plus six 0.0 floats, and shown for 300
//     with word +956.
//  E. startForces; doAI when word +2148 is nonzero; doSteering; doBrakes;
//     doThrottle. Vtable entry at +232 (halfword adjustment, then function) is
//     called; when it returns 0, doSuspension, doDriveline and doMisc run. The
//     vtable entry at +192 is then called.
//  F. If word +1024 is nonzero, or (word +2144 is 0, float +740 equals 0 and
//     float +840 is below 0.25), floats +1188, +1192, +1196 become 0.0; else
//     getNewRotAndPos(+2004, +1992, 1/60, 0, 0). Word +2144 becomes 0.
//  G. Loop: checkWallCollision; on a hit getNewRotAndPos(.., 1/60, 0, 0), then
//     count++ and repeat while the old count was below 4; backup() runs when the
//     count is 4 or more at exit. Byte +110, if its signed value is nonzero,
//     drops by one. finalizePosition nonzero with fewer than 3 retries gives one
//     more getNewRotAndPos(1/60), resets the count, and restarts the loop.
//     Then checkVehicleCollision runs.
//  H. For i below word +976 (re-read where noted in the code): seat player index
//     p = word at +960+4i, seat record r = word at +2088+4i.
//     p != -1 and word +1024 == 0: the player object is word at
//       player + 23984*p + 21872. removePassenger(vehicle, p) runs when that object
//       is null, or has flag 0x8 at +12, or netPlayerIsConnected(p) is 0, or
//       pad[p] flags (pad + 88*p + 12) has bit 0x10 while word +1004+4p differs
//       from frameNm and gameControlPaused is 0 and the object lacks flag 0x400000;
//       in that last case the byte at player + 23984*p + 23969 is first set to 20.
//       The loop bound word is re-read after this block.
//     r non-null and gameControlPaused 0, the player object null or lacking
//       flag 0x400000 (word at +21872 as above): r[280] += -(pad+88p+20) * 3.0 and
//       r[284] = max(-15.0, min(15.0, r[284] - float(pad+88p+24))).
//     p == -1, r non-null, r[284] nonzero: r[284] *= 0.85 (0x3f599998, word at
//       0x482170); as a double (fptodp) its absolute value (via dpsub 0 - x when
//       dpcmp < 0) below 1.0 (dpcmp against 0x3ff0000000000000) sets r[284] to 0.
// Unknown: what most fields mean; the sizes of the player and pad records
// beyond the strides used; the text buffer size (the frame allows up to 512
// bytes); and the return types of the helper calls whose results are dropped.
// The soft-double helpers take and return the raw 64-bit pattern here.
// When NORRATH_W8_HOST is defined a host hook resolves 32-bit object words.
struct Point3 { float x, y, z; };
extern "C" int objectIsOffGrid__FP10GameObject(void* object);
extern "C" int gameLineUpdatePosition__FP17gameLinePathState(void* state);
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* matrix33, Point3* in);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player, char flag);
extern "C" void hudClearInfoMessage__Fv();
extern "C" int hudInfoMessageShowing__Fv();
extern "C" int gameButtonToIcon__Fi(int button);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* format, float a, float b, float c, float d, float e, float f, float g, float h);
extern "C" void startForces__7Vehicle(void* vehicle);
extern "C" void doAI__7Vehicle(void* vehicle);
extern "C" void doSteering__7Vehicle(void* vehicle);
extern "C" void doBrakes__7Vehicle(void* vehicle);
extern "C" void doThrottle__7Vehicle(void* vehicle);
extern "C" void doSuspension__7Vehicle(void* vehicle);
extern "C" void doDriveline__7Vehicle(void* vehicle);
extern "C" void doMisc__7Vehicle(void* vehicle);
extern "C" void getNewRotAndPos__7VehicleP6Point3T1fii(void* vehicle, void* a, void* b, float step, int c, int d);
extern "C" int checkWallCollision__7Vehicle(void* vehicle);
extern "C" void backup__7Vehicle(void* vehicle);
extern "C" int finalizePosition__7Vehicle(void* vehicle);
extern "C" void checkVehicleCollision__7Vehicle(void* vehicle);
extern "C" void removePassenger__7Vehiclei(void* vehicle, int player);
extern "C" int netPlayerIsConnected__Fi(int player);
extern "C" unsigned long long fptodp(float value);
extern "C" int dpcmp(unsigned long long a, unsigned long long b);
extern "C" unsigned long long dpsub(unsigned long long a, unsigned long long b);
extern "C" int localNmPlayersAlive;
extern "C" int gameControlPaused;
extern "C" unsigned norrath_actor_view_4844a4;
extern "C" unsigned char player[];
extern "C" unsigned char pad[];
#if defined(NORRATH_W8_HOST)
extern "C" unsigned char* norrath_w8_host_resolve(unsigned bits);
#endif

namespace {
typedef unsigned __attribute__((may_alias)) AliasedWord;
typedef int __attribute__((may_alias)) AliasedInt;
typedef float __attribute__((may_alias)) AliasedFloat;
union FloatBits { unsigned bits; float value; };
const FloatBits drain = {0x3f7fffffu};
const FloatBits step = {0x3c888888u};
const FloatBits decay = {0x3f599998u};
char wait_key[] = "vehicleWait";
char help_key[] = "vehicleHelpDriver";
inline unsigned& word(unsigned char* o, unsigned off) { return *reinterpret_cast<AliasedWord*>(o + off); }
inline int& integer(unsigned char* o, unsigned off) { return *reinterpret_cast<AliasedInt*>(o + off); }
inline float& flt(unsigned char* o, unsigned off) { return *reinterpret_cast<AliasedFloat*>(o + off); }
inline unsigned char* resolve(unsigned bits) {
#if defined(NORRATH_W8_HOST)
    return norrath_w8_host_resolve(bits);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
// Calls the vtable entry at the given offset, returning its integer result.
inline int call_slot(unsigned char* o, unsigned offset) {
    const unsigned char* entry = resolve(word(o, 72)) + offset;
    const int adjust = *reinterpret_cast<const short __attribute__((may_alias)) *>(entry);
    typedef int (*Slot)(void*);
    const Slot slot = reinterpret_cast<Slot>(reinterpret_cast<__UINTPTR_TYPE__>(resolve(*reinterpret_cast<const AliasedWord*>(entry + 4))));
    return slot(o + adjust);
}
}

extern "C" void msg_run__7Vehicle(void* vehicle) {
    unsigned char* o = static_cast<unsigned char*>(vehicle);
    if (objectIsOffGrid__FP10GameObject(vehicle) != 0) return;

    if (flt(o, 808) != 0.0f) {
        const float remaining = flt(o, 808) - drain.value;
        flt(o, 808) = remaining;
        if (remaining < 0.0f) flt(o, 808) = 0.0f;
    }
    if (gameLineUpdatePosition__FP17gameLinePathState(o + 2272) != 0) {
        Point3 local = {0.0f, -flt(o, 896) * 0.5f, 0.0f};
        matTransform__FP6Point3P8Matrix33T0(reinterpret_cast<Point3*>(o + 656), o + 1116, &local);
        flt(o, 656) = flt(o, 656) + flt(o, 16);
        flt(o, 660) = flt(o, 660) + flt(o, 20);
        flt(o, 664) = flt(o, 664) + flt(o, 24);
    }

    bool help_allowed = true;
    const int alive = localNmPlayersAlive;
    if (alive >= 2 && word(o, 2148) == 0u && integer(o, 956) != -1) {
        if (integer(o, 980) != alive) {
            word(o, 1036) = 1u;
            hudSetInfoMessage__FPUsiic(textGetString__FPc(wait_key), 180, integer(o, 956), 0);
            help_allowed = false;
        }
    }
    if (help_allowed) {
        if (word(o, 1036) != 0u) {
            hudClearInfoMessage__Fv();
            word(o, 1036) = 0u;
        }
        if (word(o, 1036) == 0u && word(o, 1032) != 0u && word(o, 1024) == 0u && hudInfoMessageShowing__Fv() == 0) {
            unsigned short text[200];
            word(o, 1032) = 0u;
            unsigned short* format = textGetString__FPc(help_key);
            const int first = gameButtonToIcon__Fi(64);
            const int second = gameButtonToIcon__Fi(128);
            usprintf__FPUsT0ffffffff(text, format, static_cast<float>(first), static_cast<float>(second), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            hudSetInfoMessage__FPUsiic(text, 300, integer(o, 956), 0);
        }
    }

    startForces__7Vehicle(vehicle);
    if (word(o, 2148) != 0u) doAI__7Vehicle(vehicle);
    doSteering__7Vehicle(vehicle);
    doBrakes__7Vehicle(vehicle);
    doThrottle__7Vehicle(vehicle);
    if (call_slot(o, 232) == 0) {
        doSuspension__7Vehicle(vehicle);
        doDriveline__7Vehicle(vehicle);
        doMisc__7Vehicle(vehicle);
    }
    call_slot(o, 192);

    if (word(o, 1024) != 0u || (word(o, 2144) == 0u && flt(o, 740) == 0.0f && flt(o, 840) < 0.25f)) {
        flt(o, 1188) = 0.0f;
        flt(o, 1196) = 0.0f;
        flt(o, 1192) = 0.0f;
    } else {
        getNewRotAndPos__7VehicleP6Point3T1fii(vehicle, o + 2004, o + 1992, step.value, 0, 0);
    }
    word(o, 2144) = 0u;

    int count = 0;
    int retries = 0;
    for (;;) {
        if (checkWallCollision__7Vehicle(vehicle) != 0) {
            getNewRotAndPos__7VehicleP6Point3T1fii(vehicle, o + 2004, o + 1992, step.value, 0, 0);
            const int old = count;
            ++count;
            if (old < 4) continue;
        }
        if (count >= 4) backup__7Vehicle(vehicle);
        if (static_cast<signed char>(o[110]) != 0) o[110] = static_cast<unsigned char>(o[110] - 1);
        if (finalizePosition__7Vehicle(vehicle) != 0 && retries < 3) {
            ++retries;
            getNewRotAndPos__7VehicleP6Point3T1fii(vehicle, o + 2004, o + 1992, step.value, 0, 0);
            count = 0;
            continue;
        }
        break;
    }
    checkVehicleCollision__7Vehicle(vehicle);

    int bound = integer(o, 976);
    for (int i = 0; i < bound; ++i) {
        const int p = integer(o, 960u + static_cast<unsigned>(i) * 4u);
        if (p == -1) {
            const unsigned seat_bits = word(o, 2088u + static_cast<unsigned>(i) * 4u);
            if (seat_bits == 0u) continue;
            unsigned char* seat = resolve(seat_bits);
            if (flt(seat, 284) == 0.0f) continue;
            const float scaled = flt(seat, 284) * decay.value;
            flt(seat, 284) = scaled;
            unsigned long long magnitude = fptodp(scaled);
            if (dpcmp(magnitude, 0ull) < 0) magnitude = dpsub(0ull, magnitude);
            if (dpcmp(magnitude, 0x3ff0000000000000ull) < 0) flt(seat, 284) = 0.0f;
            continue;
        }
        unsigned char* slot_player = player + static_cast<unsigned>(p) * 23984u;
        unsigned char* slot_pad = pad + static_cast<unsigned>(p) * 88u;
        if (word(o, 1024) == 0u) {
            unsigned char* body = word(slot_player, 21872) != 0u ? resolve(word(slot_player, 21872)) : 0;
            bool remove = false;
            if (body == 0 || (word(body, 12) & 0x8u) != 0u) {
                remove = true;
            } else if (netPlayerIsConnected__Fi(p) == 0) {
                remove = true;
            } else if ((word(slot_pad, 12) & 0x10u) != 0u && word(o, 1004u + static_cast<unsigned>(p) * 4u) != norrath_actor_view_4844a4 &&
                       gameControlPaused == 0 && (word(body, 12) & 0x400000u) == 0u) {
                slot_player[23969] = 20;
                remove = true;
            }
            if (remove) removePassenger__7Vehiclei(vehicle, p);
            bound = integer(o, 976);
        }
        const unsigned seat_bits = word(o, 2088u + static_cast<unsigned>(i) * 4u);
        if (seat_bits == 0u) continue;
        if (gameControlPaused != 0) continue;
        if (word(slot_player, 21872) != 0u && (word(resolve(word(slot_player, 21872)), 12) & 0x400000u) != 0u) continue;
        unsigned char* seat = resolve(seat_bits);
        flt(seat, 280) = flt(seat, 280) + -flt(slot_pad, 20) * 3.0f;
        const float lowered = flt(seat, 284) - flt(slot_pad, 24);
        const float capped = 15.0f < lowered ? 15.0f : lowered;
        flt(seat, 284) = -15.0f > capped ? -15.0f : capped;
    }
}

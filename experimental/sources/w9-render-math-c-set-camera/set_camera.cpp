// Observed setCamera__Ff at 0x001884f0, 14908 bytes. The argument is the current cut-scene time.
// It reads the cut-scene action list (global pointer `action`, count `nmActions`, hint index
// `displayActionPos`) and evaluates camera keys at that time. A record is 20 bytes: float time at
// 0, signed halfword category at 4 (-100 marks a camera key), signed halfword key kind at 6, and
// three int arguments at 8, 12 and 16. Key kinds (by use):
//   0 position (x, y, z)   1 yaw   2 pitch   8 roll   9 field of view   3 look-at target (actor index)
//   7 screen colour mark   10 blur mark   4 and 5 tangent marks that follow a key of the same time
// (kind 4 is an in-tangent hold, kind 5 a cut marker; their roles are inferred from where they are tested).
//
// Outputs: cutCameraPos (three floats), myYaw, myPitch, myRoll (halfwords), myFOV (float),
// displayCutPos (float, the last cut marker time that fired), and calls to engineSetScreenColor,
// engineSetBlur, coreStartSwitchScene and worldPrecacheWorld.
//
// Each key channel is the same scan, then a cubic Hermite: the latest key of the kind at or before
// the time (cur), the key before it (prev, searched at cur time - 0.01), the next key (next, searched
// from cur time + 0.01) and the one after (nn, searched from next time + 0.01). With dt = next time -
// cur time the tangents are 0.5 * ((next - cur) + (cur - prev) * dt / (cur time - prev time)) at cur and
// 0.5 * ((nn - next) * dt / (nn time - next time) + (next - cur)) at next, and evalHermite__Ffffff
// (u, cur, next, m0, m1) is evaluated at u = (time - cur time) / dt. A tangent is zero when the
// neighbour key is missing, or when a kind 4 or 5 record sits exactly at the key's time. Angles use
// 65536 per turn: the next, prev and nn angles are shifted by 65536 until within half a turn of the
// reference (yaw only for prev and nn; pitch, roll and field of view use no shift for them). Without a
// next key the value is the cur key's argument 0 (roll scaled by 182, field of view as a float). Without
// a cur key the defaults are myYaw = -8192, myPitch = 10000, myRoll = 0, myFOV = 50.0.
// A kind 5 record at the next key's time is a cut: the cur value is held up to that time, and the
// precache of the world is started at the end from the old camera position (and, for yaw and pitch,
// the held angle). A kind 5 record at the cur key's time within 0.04 of the time and later than
// displayCutPos starts scene switch 4 at the end (position and yaw channels only).
// Inferred: the whole function is a cut-scene camera evaluator. Unknown: names of the record fields,
// the meaning of kinds 4 and 5, and the actor slot layout beyond the three fields read.
// Hosts: with NORRATH_HOST_CAMERA the actor slot pointers are offsets into an arena.
#if defined(NORRATH_HOST_CAMERA)
extern unsigned char* norrath_camera_arena;
#endif

struct Action {
    float time;
    short category;
    short kind;
    int arg[3];
};

extern Action* action;
extern int nmActions;
extern int displayActionPos;
extern float displayCutPos;
extern short myYaw;
extern short myPitch;
extern short myRoll;
extern float myFOV;
extern unsigned char* actor;
extern unsigned world;
extern "C" unsigned char worldCameraPos[12];
extern "C" float cutCameraPos[3];

extern "C" {
float evalHermite__Ffffff(float u, float p0, float p1, float m0, float m1);
int iatan2__Fff(float y, float x);
void matIdent__FP8Matrix34(float* matrix);
void matRot__FP8Matrix34is(float* matrix, int axis, short angle);
void matTrans__FP8Matrix34fff(float* matrix, float x, float y, float z);
void worldPrecacheWorld__FP12_worldHeaderP8Matrix34(void* world, float* matrix);
void coreStartSwitchScene__Fi(int scene);
void engineSetScreenColor__Fiiii(int r, int g, int b, int a);
void engineSetBlur__Fi(int amount);
}

namespace {

inline float pattern(unsigned bits) { return __builtin_bit_cast(float, bits); }

struct List {
    const Action* items;
    int count;
};

// First index at or after `start` whose time is not below `t` (the start is kept when it is past the end).
int skip_before(const List& L, int start, float t) {
    int a = start;
    if (start < L.count) {
        if (L.items[start].time < t) {
            ++a;
            while (a < L.count && L.items[a].time < t) ++a;
        }
    }
    return a;
}

// Walking down from `a`: the first camera key of this kind with time <= t. May read index L.count.
const Action* back_key(const List& L, int a, int kind, float t) {
    for (; a >= 0; --a) {
        const Action& r = L.items[a];
        if (r.category == -100 && r.kind == kind && r.time <= t) return &r;
    }
    return nullptr;
}

// Walking down while t <= the record's time; stops at index 0.
int rewind(const List& L, int a, float t) {
    while (a > 0 && t <= L.items[a].time) --a;
    return a;
}

// Walking up from `a`: the first camera key of this kind with time >= t.
const Action* next_key(const List& L, int a, int kind, float t) {
    for (; a < L.count; ++a) {
        const Action& r = L.items[a];
        if (r.category == -100 && r.kind == kind && t <= r.time) return &r;
    }
    return nullptr;
}

inline const unsigned char* actor_object(const unsigned char* slot) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(slot);
#if defined(NORRATH_HOST_CAMERA)
    return norrath_camera_arena + bits;
#else
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}

int index_of(const List& L, const Action* r) { return static_cast<int>(r - L.items); }

struct Keys {
    const Action* cur;
    const Action* prev;
    const Action* next;
    const Action* after;
};

// cur/prev/next/after for one key kind. `epsilon` are the three small offsets of that channel.
Keys find_keys(const List& L, int hint, int kind, float now, float e_prev, float e_next, float e_after) {
    Keys k{};
    int a = skip_before(L, hint, now);
    k.cur = back_key(L, a, kind, now);
    if (k.cur) {
        float f = k.cur->time - e_prev;
        a = skip_before(L, hint, f);
        k.prev = back_key(L, a, kind, f);
    }
    float f = k.cur ? k.cur->time + e_next : now;
    a = rewind(L, hint, f);
    k.next = next_key(L, a, kind, f);
    if (k.next) {
        f = k.next->time + e_after;
        a = rewind(L, hint, f);
        k.after = next_key(L, a, kind, f);
    }
    return k;
}

// Keep a value within half a turn of `reference` by whole turns of 65536.
inline float wrap_to(float value, float reference) {
    float d = value - reference;
    if (pattern(0x47000000u) < d) {
        do {
            value = value - pattern(0x47800000u);
            d = value - reference;
        } while (pattern(0x47000000u) < d);
    }
    if (d < pattern(0xc7000000u)) {
        do {
            value = value + pattern(0x47800000u);
            d = value - reference;
        } while (d < pattern(0xc7000000u));
    }
    return value;
}

struct Flags {
    bool cut = false;
    bool switch_scene = false;
};

enum ChannelKind { kYaw, kPitch, kRoll, kFov };

// Result of one scalar channel (yaw, pitch, roll, field of view). `saved` of the call receives (int) of
// the held-away end angle when the channel cuts.
struct ScalarResult {
    bool has_cur;
    bool has_next;
    float value;
    unsigned short raw_low;
    int raw_arg;
};

ScalarResult scalar_channel(const List& L, int hint, int kind, float now, float e_prev, float e_next, float e_after, ChannelKind which,
                            Flags& flags, int* saved) {
    ScalarResult r{};
    const Keys k = find_keys(L, hint, kind, now, e_prev, e_next, e_after);
    if (!k.cur) return r;
    r.has_cur = true;
    r.raw_arg = k.cur->arg[0];
    r.raw_low = static_cast<unsigned short>(k.cur->arg[0]);
    if (!k.next) return r;
    r.has_next = true;
    const bool wide = (which == kYaw);
    const bool angle = (which != kFov);
    float f13 = static_cast<float>(k.cur->arg[0]);
    float f14 = static_cast<float>(k.next->arg[0]);
    float dnext = f14 - f13;
    if (angle) {
        f14 = wrap_to(f14, f13);
        dnext = f14 - f13;
    }
    const float cur_t = k.cur->time;
    const float next_t = k.next->time;
    const float dt = next_t - cur_t;
    const int cur_i = index_of(L, k.cur);
    int a0 = rewind(L, cur_i, cur_t);
    const Action* sub4_cur = next_key(L, a0, 4, cur_t);
    a0 = rewind(L, cur_i, cur_t);
    const Action* sub5_cur = next_key(L, a0, 5, cur_t);
    const int next_i = index_of(L, k.next);
    a0 = rewind(L, next_i, next_t);
    const Action* sub5_next = next_key(L, a0, 5, next_t);
    if (which == kYaw) {
        if (sub5_cur && sub5_cur->time == cur_t) {
            const float t = sub5_cur->time;
            if (now - t < pattern(0x3d23d70au)) {
                if (displayCutPos < t) {
                    displayCutPos = t;
                    flags.switch_scene = true;
                }
            }
        }
    }
    float m0;
    if (!k.prev || (sub4_cur && sub4_cur->time == cur_t) || (sub5_cur && sub5_cur->time == cur_t) ||
        (sub5_next && sub5_next->time == next_t)) {
        m0 = 0.0f;
    } else {
        float pv = static_cast<float>(k.prev->arg[0]);
        if (wide) pv = wrap_to(pv, f13);
        const float scaled = (f13 - pv) * (dt / (cur_t - k.prev->time));
        m0 = (dnext + scaled) * 0.5f;
    }
    a0 = rewind(L, next_i, next_t);
    const Action* sub4_next = next_key(L, a0, 4, next_t);
    const Action* sub5_after = nullptr;
    if (k.after) {
        a0 = rewind(L, index_of(L, k.after), k.after->time);
        sub5_after = next_key(L, a0, 5, k.after->time);
    }
    float m1;
    if (!k.after || (sub4_next && sub4_next->time == next_t) || (sub5_next && sub5_next->time == next_t)) {
        m1 = 0.0f;
    } else {
        float nv = static_cast<float>(k.after->arg[0]);
        if (wide) nv = wrap_to(nv, f14);
        const float scaled = (nv - f14) * (dt / (k.after->time - next_t));
        m1 = (scaled + dnext) * 0.5f;
    }
    if (sub5_next && sub5_next->time == next_t) {
        if (which != kFov) {
            if (saved) *saved = static_cast<int>(f14);
            flags.cut = true;
        }
        f14 = f13;
    }
    if (sub5_after && sub5_after->time == k.after->time) m1 = 0.0f;
    r.value = evalHermite__Ffffff((now - cur_t) / dt, f13, f14, m0, m1);
    return r;
}


// Yaw and pitch (in 65536-per-turn units) from the camera position to a target actor, or the current
// yaw and pitch when the key is missing or names no actor (argument -1).
void look_at(const Action* key, float& yaw, float& pitch) {
    if (!key || key->arg[0] == -1) {
        yaw = static_cast<float>(myYaw);
        pitch = static_cast<float>(myPitch);
        return;
    }
    const unsigned char* slot = actor + key->arg[0] * 44;
    const unsigned char* object = actor_object(slot);
    const float* position = reinterpret_cast<const float*>(object + 16);
    float local[3] = {position[0], position[1], position[2]};
    const float dy = local[1] - cutCameraPos[1];
    const float dx = local[0] - cutCameraPos[0];
    const unsigned char* object2 = actor_object(actor + key->arg[0] * 44);
    local[2] = local[2] + *reinterpret_cast<const float*>(object2 + 32) * 0.5f;
    yaw = static_cast<float>(iatan2__Fff(dy, dx));
    const float ex = cutCameraPos[0] - local[0];
    const float ey = cutCameraPos[1] - local[1];
    const float distance = __builtin_sqrtf(ex * ex + ey * ey);
    pitch = static_cast<float>(iatan2__Fff(local[2] - cutCameraPos[2], distance));
    yaw = pattern(0x46800000u) - yaw;
    pitch = -pitch;
}

}  // namespace

extern "C" void setCamera__Ff(float now) {
    const List L = {action, nmActions};
    const int hint = displayActionPos;
    const float* camera = reinterpret_cast<const float*>(worldCameraPos);
    float saved_position[3] = {camera[0], camera[1], camera[2]};
    int saved_yaw = myYaw;
    int saved_pitch = myPitch;
    bool switch_scene = false;
    bool cut = false;
    const float small = pattern(0x3c23d70au);

    // Position channel (key kind 0).
    {
        const Keys k = find_keys(L, hint, 0, now, small, small, small);
        if (k.cur) {
            if (!k.next) {
                cutCameraPos[2] = static_cast<float>(k.cur->arg[2]);
                cutCameraPos[0] = static_cast<float>(k.cur->arg[0]);
                cutCameraPos[1] = static_cast<float>(k.cur->arg[1]);
            } else {
                float p0[3] = {static_cast<float>(k.cur->arg[0]), static_cast<float>(k.cur->arg[1]), static_cast<float>(k.cur->arg[2])};
                float p1[3] = {static_cast<float>(k.next->arg[0]), static_cast<float>(k.next->arg[1]), static_cast<float>(k.next->arg[2])};
                const float cur_t = k.cur->time;
                const float next_t = k.next->time;
                const float dt = next_t - cur_t;
                const int cur_i = index_of(L, k.cur);
                int a0 = rewind(L, cur_i, cur_t);
                const Action* sub4_cur = next_key(L, a0, 4, cur_t);
                a0 = rewind(L, cur_i, cur_t);
                const Action* sub5_cur = next_key(L, a0, 5, cur_t);
                const int next_i = index_of(L, k.next);
                a0 = rewind(L, next_i, next_t);
                const Action* sub5_next = next_key(L, a0, 5, next_t);
                if (sub5_cur && sub5_cur->time == cur_t) {
                    const float t = sub5_cur->time;
                    if (now - t < pattern(0x3d23d70au)) {
                        if (displayCutPos < t) {
                            displayCutPos = t;
                            switch_scene = true;
                        }
                    }
                }
                float m0[3];
                if (!k.prev || (sub4_cur && sub4_cur->time == cur_t) || (sub5_cur && sub5_cur->time == cur_t) ||
                    (sub5_next && sub5_next->time == next_t)) {
                    m0[0] = m0[1] = m0[2] = 0.0f;
                } else {
                    const float pm[3] = {static_cast<float>(k.prev->arg[0]), static_cast<float>(k.prev->arg[1]), static_cast<float>(k.prev->arg[2])};
                    const float ratio = dt / (cur_t - k.prev->time);
                    for (int i = 0; i < 3; ++i) {
                        const float dn = p1[i] - p0[i];
                        const float scaled = (p0[i] - pm[i]) * ratio;
                        m0[i] = (dn + scaled) * 0.5f;
                    }
                }
                a0 = rewind(L, next_i, next_t);
                const Action* sub4_next = next_key(L, a0, 4, next_t);
                const Action* sub5_after = nullptr;
                if (k.after) {
                    a0 = rewind(L, index_of(L, k.after), k.after->time);
                    sub5_after = next_key(L, a0, 5, k.after->time);
                }
                float m1[3];
                if (!k.after || (sub4_next && sub4_next->time == next_t) || (sub5_next && sub5_next->time == next_t)) {
                    m1[0] = m1[1] = m1[2] = 0.0f;
                } else {
                    const float pn[3] = {static_cast<float>(k.after->arg[0]), static_cast<float>(k.after->arg[1]), static_cast<float>(k.after->arg[2])};
                    const float ratio = dt / (k.after->time - next_t);
                    for (int i = 0; i < 3; ++i) {
                        const float scaled = (pn[i] - p1[i]) * ratio;
                        const float dn = p1[i] - p0[i];
                        m1[i] = (scaled + dn) * 0.5f;
                    }
                }
                if (sub5_next && sub5_next->time == next_t) {
                    saved_position[0] = p1[0];
                    saved_position[1] = p1[1];
                    saved_position[2] = p1[2];
                    cut = true;
                    p1[0] = p0[0];
                    p1[1] = p0[1];
                    p1[2] = p0[2];
                }
                if (sub5_after && sub5_after->time == k.after->time) m1[0] = m1[1] = m1[2] = 0.0f;
                const float u = (now - cur_t) / dt;
                cutCameraPos[0] = evalHermite__Ffffff(u, p0[0], p1[0], m0[0], m1[0]);
                cutCameraPos[1] = evalHermite__Ffffff(u, p0[1], p1[1], m0[1], m1[1]);
                cutCameraPos[2] = evalHermite__Ffffff(u, p0[2], p1[2], m0[2], m1[2]);
            }
        }
    }

    // Yaw channel (key kind 1).
    myYaw = -8192;
    {
        Flags f;
        int saved = 0;
        const ScalarResult r = scalar_channel(L, hint, 1, now, small, small, small, kYaw, f, &saved);
        if (f.switch_scene) switch_scene = true;
        if (r.has_cur) {
            if (!r.has_next) {
                myYaw = static_cast<short>(r.raw_low);
            } else {
                if (f.cut) { saved_yaw = saved; cut = true; }
                myYaw = static_cast<short>(static_cast<int>(r.value));
            }
        }
    }

    // Pitch channel (key kind 2).
    myPitch = 10000;
    {
        Flags f;
        int saved = 0;
        const ScalarResult r = scalar_channel(L, hint, 2, now, small, small, small, kPitch, f, &saved);
        if (r.has_cur) {
            if (!r.has_next) {
                myPitch = static_cast<short>(r.raw_low);
            } else {
                if (f.cut) { saved_pitch = saved; cut = true; }
                myPitch = static_cast<short>(static_cast<int>(r.value));
            }
        }
    }

    // Roll channel (key kind 8).
    myRoll = 0;
    {
        Flags f;
        const ScalarResult r = scalar_channel(L, hint, 8, now, small, small, small, kRoll, f, nullptr);
        if (r.has_cur) {
            if (!r.has_next) {
                myRoll = static_cast<short>(r.raw_arg * 182);
            } else {
                if (f.cut) cut = true;
                myRoll = static_cast<short>(static_cast<int>(r.value * pattern(0x43360000u)));
            }
        }
    }

    // Field of view channel (key kind 9).
    myFOV = pattern(0x42480000u);
    {
        Flags f;
        const ScalarResult r = scalar_channel(L, hint, 9, now, small, small, small, kFov, f, nullptr);
        if (r.has_cur) {
            if (!r.has_next) myFOV = static_cast<float>(r.raw_arg);
            else myFOV = r.value;
        }
    }

    // Look-at channel (key kind 3): blends yaw and pitch toward an actor's position.
    {
        int a = skip_before(L, hint, now);
        const Action* cur = back_key(L, a, 3, now);
        float f = cur ? cur->time + small : now;
        a = rewind(L, hint, f);
        const Action* next = next_key(L, a, 3, f);
        if (cur || next) {
            const float start_t = cur ? cur->time : 0.0f;
            const float end_t = next ? next->time : pattern(0x49742400u);
            float from_yaw, from_pitch, to_yaw, to_pitch;
            look_at(cur, from_yaw, from_pitch);
            look_at(next, to_yaw, to_pitch);
            const float h = evalHermite__Ffffff((now - start_t) / (end_t - start_t), 0.0f, 1.0f, 0.0f, 0.0f);
            to_yaw = wrap_to(to_yaw, from_yaw);
            const float yaw = from_yaw + (to_yaw - from_yaw) * h;
            const float pitch = from_pitch + (to_pitch - from_pitch) * h;
            myYaw = static_cast<short>(static_cast<int>(yaw));
            myPitch = static_cast<short>(static_cast<int>(pitch));
        }
    }

    // Screen colour channel (key kind 7): fade by the distance in time to the nearest mark.
    {
        int a = skip_before(L, hint, now);
        const Action* before = back_key(L, a, 7, now);
        a = rewind(L, hint, now);
        const Action* after = next_key(L, a, 7, now);
        float distance = pattern(0x47c35000u);
        if (before) {
            const float d = __builtin_fabsf(before->time - now);
            distance = distance < d ? distance : d;
        }
        if (after) {
            const float d = __builtin_fabsf(after->time - now);
            distance = distance < d ? distance : d;
        }
        int amount = static_cast<int>(distance * pattern(0x43480000u));
        if (!(amount < 128)) amount = 128;
        amount -= 128;
        engineSetScreenColor__Fiiii(amount, amount, amount, 0);
    }

    // Blur channel (key kind 10).
    {
        int a = skip_before(L, hint, now);
        const Action* before = back_key(L, a, 10, now);
        a = rewind(L, hint, now);
        const Action* after = next_key(L, a, 10, now);
        float distance = pattern(0x47c35000u);
        if (before) {
            const float d = __builtin_fabsf(before->time - now);
            distance = distance < d ? distance : d;
        }
        if (after) {
            const float d = __builtin_fabsf(after->time - now);
            distance = distance < d ? distance : d;
        }
        int amount = 150 - static_cast<int>(distance * pattern(0x42c80000u));
        if (amount < 0) amount = 0;
        if (110 < amount) amount = 110;
        engineSetBlur__Fi(amount);
    }

    if (switch_scene) coreStartSwitchScene__Fi(4);
    if (cut) {
        float matrix[12];
        matIdent__FP8Matrix34(matrix);
        matRot__FP8Matrix34is(matrix, 0, static_cast<short>(saved_pitch + 16339));
        matRot__FP8Matrix34is(matrix, 2, static_cast<short>(saved_yaw));
        matTrans__FP8Matrix34fff(matrix, -saved_position[0], -saved_position[1], -saved_position[2]);
        worldPrecacheWorld__FP12_worldHeaderP8Matrix34(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(world)), matrix);
    }
}

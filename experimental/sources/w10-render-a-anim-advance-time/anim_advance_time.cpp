#include "../level-anim-load-common/mem.hpp"
// Observed animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps at 0x00160830,
// 3100 bytes. Arguments: animation state, a float step "dt", a Point3 pointer used only as the
// position handed to sound playback (may be null), a Point2 pointer that receives accumulated
// planar motion (read and written without a null test in some paths), and a short pointer that
// receives an accumulated turn (also dereferenced without a test in some paths, but zeroed only if
// non-null). Records are addressed by byte offset; nothing here claims the original declarations.
// State: +0 result record pointer, +4 signed byte component count, +6 halfword that receives the
// low 16 bits of the OR of all component flags, +8 + 32*i components. Component (32 bytes):
// +0 header pointer, +4 link record pointer, +8 and +16 float weights with float rates at +12
// and +20, +24 float time, +28 halfword flags, +30 signed halfword event index.
// Header: +4 word length (converted to float), four 8-byte trigger entries from +20 (float time,
// byte value at +24+8k), +56 event table pointer (28-byte records whose first word is a time),
// +60 byte sound group, +72 64-bit word whose zero-ness decides whether the component consumes
// blend weight.
// Order of work, as in the image:
//  1. step = dt * 0.99999994; the result record at state+0 (if any) is released (its +1552 cleared
//     and state+0 cleared); *motion is zeroed (both floats) and *turn set to 0.
//  2. Components are visited from the last to the first. For each: the flags are OR-ed (sign
//     extended) into a running mask; trigger entries whose time lies in [time/60, (time+step)/60)
//     OR their byte into the return value (sign extended from the low byte each time); unless flag
//     0x2000 the time advances by step (by 0.99999994 when flag 0x40 is set); a negative time skips
//     the component. For the last component only, with an event table and a sound position, the
//     event index is repaired against the time before the step, a per-frame limiter on the sound
//     group (soundLimit pairs of bytes, soundGroupLimit bytes, drawFrameNm) may suppress sounds, and
//     events passed during the step are played in runs of equal first word with
//     playSoundGroup__FP15AnimationHeaderiiP6Point3 (and, if the time passes the end of a looping
//     animation, the events of the wrapped portion). Then the effective weight is min(weights,
//     remaining) limited to [0,1] and, unless header+72 is nonzero, subtracted from the remaining
//     weight. If the time reached the length: looping components (flag 0x1) optionally apply root
//     motion (flag 0x800) or a turn extracted from a quaternion with iatan2 (flag 0x4000) for the
//     remainder of the animation and subtract the length until below it; non-looping ones clamp
//     the time (flag 0x4) and set return/mask bits 0x80 and, for the last component, 0x400, or zero
//     both weights. The weights then change by their rates (times dt unless flag 0x40). A component
//     whose second weight is not positive and which lacks flag 0x200 is removed and the later ones
//     are shifted down; one with flag 0x2 whose weights both reach 1 removes the unprotected
//     (flag 0x1000 clear) components below it and ends the walk.
//  3. When the mask has 0x800, the components are walked again from the last: the same weight
//     rule is applied and the root motion accumulated in the link record is added to *motion scaled
//     by the weight.
//  4. When the stored halfword has 0x4000, every component with that flag extracts a turn from its
//     quaternion and adds the change to *turn.
// The return value is the OR of the trigger bytes (and 0x80/0x400 end bits are in the mask, not the
// return). Inferred: the names and meanings of the flags and fields above. Unknown: what the
// result record is, what the link record fields beyond those used are, and the units of
// the trigger and event times.
extern "C" {
extern int drawFrameNm;
extern unsigned char soundLimit[];
extern signed char soundGroupLimit[];
void playSoundGroup__FP15AnimationHeaderiiP6Point3(unsigned char* header, int first, int last,
                                                  unsigned char* position);
void updateAnimation__FP13animComponent(unsigned char* component);
int iatan2__Fff(float y, float x);
}

namespace {
using namespace norrath_lal;

inline float minimum(float a, float b) { return a < b ? a : b; }

inline float frameWord(const unsigned char* events, int index) {
    return static_cast<float>(static_cast<int>(load_word(events + index * 28)));
}

// Weight limits shared by the two walks: returns the effective weight and reduces *remaining.
__attribute__((always_inline)) inline float takeWeight(unsigned char* component, const unsigned char* header, float* remaining) {
    float weight = minimum(load_float(component + 8), load_float(component + 16));
    float raw = weight;
    weight = minimum(weight, *remaining);
    if (1.0f < weight) weight = 1.0f;
    if (weight < 0.0f) weight = 0.0f;
    if (raw < 0.0f) raw = 0.0f;
    if ((load_word(header + 72) | load_word(header + 76)) == 0) {
        *remaining = *remaining - raw;
        if (*remaining < 0.0f) *remaining = 0.0f;
    }
    return weight;
}

// Turn extraction from the quaternion kept in the link record; the component was just updated.
__attribute__((always_inline)) inline void addTurn(unsigned char* component, unsigned char* link, short* turn) {
    const float elapsed = load_float(component + 24) - static_cast<float>(load_signed_half(link + 30));
    const unsigned char* rates = link + 72;
    const unsigned char* base = link + 56;
    const float d2 = load_float(rates + 8) * elapsed;
    const float d1 = load_float(rates + 4) * elapsed;
    const float d3 = load_float(rates + 12) * elapsed;
    const float d0 = load_float(rates + 0) * elapsed;
    const float q0 = load_float(base + 0) + d0;
    const float q1 = load_float(base + 4) + d1;
    const float q2 = load_float(base + 8) + d2;
    const float q3 = load_float(base + 12) + d3;
    const float q1q1 = q1 * q1;
    const float q2q2 = q2 * q2;
    const float q2q3 = q2 * q3;
    float x = q0 * q0;
    float y = q0 * q1;
    const float q3q3 = q3 * q3;
    x = x - q1q1;
    y = y + q2q3;
    x = x - q2q2;
    y = y + y;
    x = x + q3q3;
    const int angle = iatan2__Fff(y, x);
    const unsigned previous = load_half(link + 24);
    const unsigned delta = static_cast<unsigned>(angle) - previous;
    *turn = static_cast<short>(static_cast<unsigned short>(*turn) + delta);
    store_half(link + 24, static_cast<unsigned>(angle));
}
}  // namespace

extern "C" int animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(
    unsigned char* state, float dt, unsigned char* soundPosition, float* motion, short* turn) {
    const float step = dt * 0x1.fffffep-1f;
    float remaining = 1.0f;
    int result = 0;
    int mask = 0;
    unsigned char* released = load_pointer(state);
    if (released != nullptr) {
        store_word(released + 1552, 0);
        store_word(state, 0);
    }
    if (motion != nullptr) {
        motion[0] = 0.0f;
        motion[1] = 0.0f;
    }
    if (turn != nullptr) *turn = 0;
    for (int i = load_signed_byte(state + 4) - 1; i >= 0; --i) {
        unsigned char* component = state + 8 + i * 32;
        float time = load_float(component + 24);
        const float before = time;
        unsigned char* header = load_pointer(component);
        unsigned flags = load_half(component + 28);
        mask |= static_cast<short>(flags);
        {
            const float low = before * 0x1.11111p-6f;
            const float high = low + step * 0x1.11111p-6f;
            for (int k = 0; k != 4; ++k) {
                const float trigger = load_float(header + 4 + 16 + 8 * k);
                if (low <= trigger && trigger < high)
                    result = static_cast<signed char>(result | header[16 + 8 * k + 8]);
            }
        }
        if ((flags & 0x2000) == 0) {
            time = (flags & 0x40) != 0 ? before + 0x1.fffffep-1f : before + step;
            store_float(component + 24, time);
        }
        if (time < 0.0f) continue;
        unsigned char* const soundHeader = header;
        unsigned char* events = load_pointer(header + 56);
        bool quiet = false;
        if (events != nullptr && soundPosition != nullptr && i == load_signed_byte(state + 4) - 1) {
            int index = load_signed_half(component + 30);
            unsigned scan = load_half(component + 30);
            if (index > 0) {
                if (before <= frameWord(events, index - 1)) {
                    store_half(component + 30, 0);
                    scan = 0;
                }
            }
            int cursor = static_cast<short>(scan);
            if (frameWord(events, cursor) < before) {
                unsigned next;
                do {
                    next = scan + 1;
                    cursor = static_cast<short>(next);
                    scan = next;
                } while (frameWord(events, cursor) < before);
                store_half(component + 30, next);
            }
            const unsigned group = header[60];
            if (group != 0) {
                const unsigned char frame = static_cast<unsigned char>(drawFrameNm);
                if (soundLimit[group * 2] != frame) {
                    soundLimit[group * 2] = frame;
                    soundLimit[group * 2 + 1] = 1;
                    time = load_float(component + 24);
                    events = load_pointer(header + 56);
                } else if (soundGroupLimit[group] - 2 < static_cast<int>(soundLimit[group * 2 + 1])) {
                    quiet = true;
                } else {
                    soundLimit[group * 2 + 1] = static_cast<unsigned char>(soundLimit[group * 2 + 1] + 1);
                    time = load_float(component + 24);
                    events = load_pointer(header + 56);
                }
            }
            int at = load_signed_half(component + 30);
            while (frameWord(events, at) < time) {
                const int first = at;
                int last = first + 1;
                if (load_word(events + last * 28) == load_word(events + first * 28)) {
                    const unsigned reference = load_word(events + first * 28);
                    const unsigned char* walker = events + last * 28;
                    unsigned value;
                    do {
                        walker += 28;
                        value = load_word(walker);
                        ++last;
                    } while (value == reference);
                }
                if (!quiet) {
                    playSoundGroup__FP15AnimationHeaderiiP6Point3(header, first, last, soundPosition);
                    time = load_float(component + 24);
                    events = load_pointer(header + 56);
                }
                store_half(component + 30, static_cast<unsigned>(last));
                at = static_cast<short>(last);
            }
            header = load_pointer(component);
            const float length = static_cast<float>(static_cast<int>(load_word(header + 4)));
            flags = load_half(component + 28);
            if (length <= time) {
                if ((flags & 1) != 0) {
                    const float excess = time - length;
                    const bool replays = frameWord(events, 0) < excess;
                    store_half(component + 30, 0);
                    if (replays) {
                        float replay;
                        do {
                            const int first = load_signed_half(component + 30);
                            int last = first + 1;
                            if (load_word(events + last * 28) == load_word(events + first * 28)) {
                                const unsigned reference = load_word(events + first * 28);
                                const unsigned char* walker = events + last * 28;
                                unsigned value;
                                do {
                                    walker += 28;
                                    value = load_word(walker);
                                    ++last;
                                } while (value == reference);
                            }
                            if (!quiet) {
                                playSoundGroup__FP15AnimationHeaderiiP6Point3(soundHeader, first, last, soundPosition);
                                events = load_pointer(soundHeader + 56);
                            }
                            store_half(component + 30, static_cast<unsigned>(last));
                            replay = frameWord(events, static_cast<short>(last));
                        } while (replay < excess);
                        flags = load_half(component + 28);
                        time = load_float(component + 24);
                        header = load_pointer(component);
                    }
                }
            }
        }
        const float weight = takeWeight(component, header, &remaining);
        const float length = static_cast<float>(static_cast<int>(load_word(header + 4)));
        if (length <= time) {
            if ((flags & 1) != 0) {
                if ((flags & 0x800) != 0) {
                    store_float(component + 24, length);
                    updateAnimation__FP13animComponent(component);
                    unsigned char* link = load_pointer(component + 4);
                    const float stamp = load_float(component + 24);
                    const float vx = load_float(link + 44);
                    const float vy = load_float(link + 48);
                    const float ax = load_float(link + 32);
                    const float ay = load_float(link + 36);
                    store_word(link + 32, 0);
                    store_word(link + 36, 0);
                    flags = load_half(component + 28);
                    const float elapsed = stamp - static_cast<float>(load_signed_half(link + 28));
                    header = load_pointer(component);
                    const float py = vy * elapsed;
                    const float px = vx * elapsed;
                    const float my = (ay + py) * weight;
                    const float mx = (ax + px) * weight;
                    const float newY = motion[1] + my;
                    const float newX = motion[0] + mx;
                    motion[1] = newY;
                    motion[0] = newX;
                    store_float(component + 24, time);
                }
                if ((flags & 0x4000) != 0) {
                    store_float(component + 24, static_cast<float>(static_cast<int>(load_word(header + 4))));
                    updateAnimation__FP13animComponent(component);
                    unsigned char* link = load_pointer(component + 4);
                    addTurn(component, link, turn);
                    header = load_pointer(component);
                    store_float(component + 24, time);
                    flags = load_half(component + 28);
                }
                float current;
                float span;
                do {
                    span = static_cast<float>(static_cast<int>(load_word(header + 4)));
                    current = time - span;
                    store_float(component + 24, current);
                    time = current;
                } while (span <= current);
            } else if ((flags & 4) != 0) {
                store_float(component + 24, length);
                mask |= 0x80;
                if (i == load_signed_byte(state + 4) - 1) mask |= 0x400;
            } else {
                store_word(component + 16, 0);
                store_word(component + 20, 0);
            }
        }
        {
            float rateA = load_float(component + 12);
            float rateB = load_float(component + 20);
            if ((flags & 0x40) == 0) {
                rateA = rateA * dt;
                rateB = rateB * dt;
            }
            const float weightA = load_float(component + 8) + rateA;
            const float weightB = load_float(component + 16) + rateB;
            store_float(component + 8, weightA);
            store_float(component + 16, weightB);
        }
        const float second = load_float(component + 16);
        if (second <= 0.0f && (flags & 0x200) == 0) {
            unsigned char* link = load_pointer(component + 4);
            if (link != nullptr) {
                store_word(link + 20, 0);
                store_word(component + 4, 0);
            }
            const int count = load_signed_byte(state + 4);
            for (int j = i; j < count - 1; ++j) {
                unsigned char* destination = state + 8 + j * 32;
                for (int b = 0; b != 32; ++b) destination[b] = destination[32 + b];
                unsigned char* moved = load_pointer(destination + 4);
                if (moved != nullptr) store_pointer(moved + 20, destination);
            }
            store_word(state + 8 + (count - 1) * 32 + 4, 0);
            state[4] = static_cast<unsigned char>(state[4] - 1);
            continue;
        }
        if ((flags & 2) == 0) continue;
        if (!(1.0f <= minimum(load_float(component + 8), second))) continue;
        if (i <= 0) continue;
        {
            const int count = load_signed_byte(state + 4);
            int kept = 0;
            for (int j = 0; j < count; ++j) {
                unsigned char* source = state + 8 + j * 32;
                unsigned char* target = state + 8 + kept * 32;
                if (j < i && (load_half(source + 28) & 0x1000) == 0) {
                    unsigned char* link = load_pointer(source + 4);
                    if (link != nullptr) {
                        store_word(link + 20, 0);
                        store_word(source + 4, 0);
                    }
                } else {
                    for (int b = 0; b != 32; ++b) target[b] = source[b];
                    if (load_word(source + 4) != 0 && kept != j) {
                        unsigned char* link = load_pointer(target + 4);
                        store_word(source + 4, 0);
                        store_pointer(link + 20, target);
                    }
                    ++kept;
                }
            }
            state[4] = static_cast<unsigned char>(kept);
        }
        break;
    }
    store_half(state + 6, static_cast<unsigned>(mask));
    if ((mask & 0x800) != 0) {
        float left = 1.0f;
        for (int i = load_signed_byte(state + 4) - 1; i >= 0; --i) {
            unsigned char* component = state + 8 + i * 32;
            unsigned char* header = load_pointer(component);
            if (header == nullptr) continue;
            const float weight = takeWeight(component, header, &left);
            if ((load_half(component + 28) & 0x800) == 0) continue;
            updateAnimation__FP13animComponent(component);
            unsigned char* link = load_pointer(component + 4);
            const float stamp = load_float(component + 24);
            const float vx = load_float(link + 44);
            const float vy = load_float(link + 48);
            const float ax = load_float(link + 32);
            const float ay = load_float(link + 36);
            const float elapsed = stamp - static_cast<float>(load_signed_half(link + 28));
            const float py = vy * elapsed;
            const float px = vx * elapsed;
            const float sumY = ay + py;
            const float sumX = ax + px;
            const float restY = ay - sumY;
            const float restX = ax - sumX;
            const float outY = sumY * weight;
            const float outX = sumX * weight;
            store_float(link + 36, restY);
            store_float(link + 32, restX);
            motion[0] = motion[0] + outX;
            motion[1] = motion[1] + outY;
        }
    }
    if ((load_half(state + 6) & 0x4000) != 0) {
        for (int i = 0; i < load_signed_byte(state + 4); ++i) {
            unsigned char* component = state + 8 + i * 32;
            if (load_word(component) == 0) continue;
            if ((load_half(component + 28) & 0x4000) == 0) continue;
            updateAnimation__FP13animComponent(component);
            addTurn(component, load_pointer(component + 4), turn);
        }
    }
    return result;
}

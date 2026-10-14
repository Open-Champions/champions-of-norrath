// Observed: ProcessPlayTiming__9VIHSpriteiR7VIScene at 0x010626a0, 1324 bytes. Arguments: the sprite (this), a play index i and the scene S. Offsets
// are bytes. The play pool is this+120 (104-byte records at the word at +8 of the pool); E is record i. anim = Animation__7VIScenei(S, the
// word at E+0), and C is the signed word at anim+16. R is this+88: a count at +0 and the word at +4 pointing at 16-byte records.
//  1. When the word at E+4 is 2 or 3 nothing but step 6 happens.
//  2. now = GetTime__7VIScene(S); dt = (float) the low 32 bits of (now - the doubleword at E+64). F is the float at E+40, G the float at +72
//     and H the float at +76. pos0 = (float)(the int at +44) + the float at +56 when 0 < F, else (float)(the int at +44) - the float at +56.
//  3. When G is not zero: acc = the float at +52 + dt; n = (int)(acc * G); the float at +52 is stored as acc - (float)n * H. The int at +44 is
//     advanced by n when 0 < F (reduced by n otherwise) and the frame fields are fixed up. The playback word at +4 is 1 for clamped play and
//     anything else for looping.
//       F > 0, clamped: +44 is limited to C - 1; +48 = +44 + 1; if C - 1 < +48 then +48 = +44, +52 = 0.0 and +4 = 3.
//       F > 0, looping: +44 = (+44) % C (signed remainder), +48 = +44 + 1, set to 0 when C - 1 < +48.
//       F <= 0, clamped: +44 is limited below at 0; +48 = +44 - 1; if that is negative then +48 = +44, +52 = 0.0 and +4 = 3.
//       F <= 0, looping: +44 = (+44) % C, plus C when negative; +48 = +44 - 1, set to C - 1 when negative.
//     Then +56 = (the float at +52) * G. When the word at anim+20 (the node count M) is non-zero, pos1 = (float)(+44) + (+56) when 0 < F, wrapped
//     by fmodf(pos1, C) + C when C < pos1; or (float)(+44) - (+56) when F <= 0, wrapped by fmodf(pos1, C) - C when below -C.
//     With M positive, for each node k below M (re-read each pass): the entry is at the word at anim+44 plus 8 * k; the first R record whose
//     first word equals the entry's second word is found (index j; none skips the node); t is the float at the entry's start. Forward: the node
//     fires when pos0 < t <= pos1, else when pos0 < t + C <= pos1 (t takes the shifted value); amount = pos1 - t. Backward: when t < pos0 and
//     pos1 <= t, else with t - C; amount = t - pos1. Firing reads GetTime(S), and in R record j sets the word at +4 to 1 and the doubleword at +8
//     to (time - (long long)(amount * H)), the conversion being the library __fixsfdi call.
//  4. Step 5 follows whether or not step 3 ran (G zero skips straight to it). With the word at E+12: zero stores 0 at E+32; one reads GetTime(S);
//     elapsed = that - the doubleword at +16; if the doubleword at +24 is below elapsed the word at +12 becomes 0, otherwise the float at +32 is
//     (float)(int)elapsed / (float)(the int at +24); any other value does nothing.
//  5. (end of step 4)  6. E+64 = GetTime(S); returns 0.
// Inferred: a per-play clock that turns elapsed ticks into a frame index and fraction, loops or ends the play (state 3 is a finished
// play), and arms trigger records (events at animation times) in R when the play crosses their time; the F, G, H names are mine.
// Unknown: what the R records are beyond the id/state/time layout used here, the exact meaning of the play states 0 and 1 in E+4/E+12, and
// why the wrap adds C after fmodf.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
unsigned char* Animation__7VIScenei(void* scene, int id);
long long GetTime__7VIScene(void* scene);
float fmodf(float x, float y);
long long __fixsfdi(float value);

static inline unsigned char* Words(unsigned char* base, int offset) {
    return NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(base + offset));
}
static inline int& Int(unsigned char* base, int offset) { return *reinterpret_cast<int*>(base + offset); }
static inline float& Float(unsigned char* base, int offset) { return *reinterpret_cast<float*>(base + offset); }

int ProcessPlayTiming__9VIHSpriteiR7VIScene(unsigned char* self, int index, void* scene) {
    unsigned char* plays = self + 120;
    int offset = index * 104;
    unsigned char* animation = Animation__7VIScenei(scene, Int(Words(plays, 8) + offset, 0));
    unsigned char* play = Words(plays, 8) + offset;
    if (static_cast<unsigned>(Int(play, 4)) - 2u >= 2u) {
        long long now = GetTime__7VIScene(scene);
        long long last = *reinterpret_cast<long long*>(play + 64);
        float rate = Float(play, 40);
        float dt = static_cast<float>(static_cast<int>(now - last));
        int frame = Int(play, 44);
        float position0;
        if (0.0f < rate) position0 = static_cast<float>(frame) + Float(play, 56);
        else position0 = static_cast<float>(frame) - Float(play, 56);
        float scale = Float(play, 72);
        if (!(scale == 0.0f)) {
            float step = Float(play, 76);
            float accumulated = Float(play, 52);
            accumulated = accumulated + dt;
            int whole = static_cast<int>(accumulated * scale);
            Float(play, 52) = accumulated - static_cast<float>(whole) * step;
            if (0.0f < rate) {
                int moved = frame + whole;
                Int(play, 44) = moved;
                if (Int(play, 4) == 1) {
                    int lastFrame = Int(animation, 16) - 1;
                    if (lastFrame < moved) Int(play, 44) = lastFrame;
                    int current = Int(play, 44);
                    int next = current + 1;
                    Int(play, 48) = next;
                    if (Int(animation, 16) - 1 < next) {
                        Int(play, 48) = current;
                        Float(play, 52) = 0.0f;
                        Int(play, 4) = 3;
                    }
                } else {
                    int remainder = moved % Int(animation, 16);
                    Int(play, 44) = remainder;
                    int next = remainder + 1;
                    Int(play, 48) = next;
                    if (Int(animation, 16) - 1 < next) Int(play, 48) = 0;
                }
            } else {
                int moved = frame - whole;
                Int(play, 44) = moved;
                if (Int(play, 4) == 1) {
                    if (moved < 0) Int(play, 44) = 0;
                    int current = Int(play, 44);
                    int previous = current - 1;
                    Int(play, 48) = previous;
                    if (previous < 0) {
                        Int(play, 48) = current;
                        Float(play, 52) = 0.0f;
                        Int(play, 4) = 3;
                    }
                } else {
                    int remainder = moved % Int(animation, 16);
                    Int(play, 44) = remainder;
                    if (remainder < 0) Int(play, 44) = remainder + Int(animation, 16);
                    int previous = Int(play, 44) - 1;
                    Int(play, 48) = previous;
                    if (previous < 0) Int(play, 48) = Int(animation, 16) - 1;
                }
            }
            float fraction = Float(play, 52) * scale;
            Float(play, 56) = fraction;
            int nodeCount = Int(animation, 20);
            if (nodeCount != 0) {
                float position1;
                if (0.0f < rate) {
                    float current = static_cast<float>(Int(play, 44));
                    float frames = static_cast<float>(Int(animation, 16));
                    position1 = current + fraction;
                    if (frames < position1) {
                        position1 = fmodf(position1, frames) + static_cast<float>(Int(animation, 16));
                        nodeCount = Int(animation, 20);
                    }
                } else {
                    float current = static_cast<float>(Int(play, 44));
                    float frames = static_cast<float>(Int(animation, 16));
                    position1 = current - fraction;
                    if (position1 < -frames) {
                        position1 = fmodf(position1, frames) - static_cast<float>(Int(animation, 16));
                        nodeCount = Int(animation, 20);
                    }
                }
                if (nodeCount > 0) {
                    unsigned char* triggers = self + 88;
                    int k = 0;
                    do {
                        unsigned char* entries = Words(animation, 44);
                        int count = Int(triggers, 0);
                        unsigned char* entry = entries + k * 8;
                        int slot = 0;
                        bool found = false;
                        if (count > 0) {
                            int wanted = Int(entry, 4);
                            unsigned char* records = Words(triggers, 4);
                            for (int j = 0; j < count; ++j) {
                                if (Int(records + j * 16, 0) == wanted) {
                                    slot = j;
                                    found = true;
                                    break;
                                }
                            }
                        }
                        if (found) {
                            float time = Float(entry, 0);
                            bool fire = false;
                            float amount = 0.0f;
                            if (0.0f < Float(play, 40)) {
                                if (position0 < time && time <= position1) {
                                    fire = true;
                                    amount = position1 - time;
                                } else {
                                    time = time + static_cast<float>(Int(animation, 16));
                                    if (position0 < time && time <= position1) {
                                        fire = true;
                                        amount = position1 - time;
                                    }
                                }
                            } else {
                                if (time < position0 && position1 <= time) {
                                    fire = true;
                                    amount = time - position1;
                                } else {
                                    time = time - static_cast<float>(Int(animation, 16));
                                    if (time < position0 && position1 <= time) {
                                        fire = true;
                                        amount = time - position1;
                                    }
                                }
                            }
                            if (fire) {
                                long long stamp = GetTime__7VIScene(scene);
                                unsigned char* record = Words(triggers, 4) + slot * 16;
                                long long elapsed = __fixsfdi(amount * Float(play, 76));
                                Int(record, 4) = 1;
                                *reinterpret_cast<long long*>(record + 8) = stamp - elapsed;
                            }
                        }
                        ++k;
                    } while (k < Int(animation, 20));
                }
            }
        }
        unsigned mode = *reinterpret_cast<unsigned*>(play + 12);
        if (mode == 0) {
            *reinterpret_cast<unsigned*>(play + 32) = 0;
        } else if (mode == 1) {
            long long stamp = GetTime__7VIScene(scene);
            long long elapsed = stamp - *reinterpret_cast<long long*>(play + 16);
            float seconds = static_cast<float>(static_cast<int>(elapsed));
            if (*reinterpret_cast<long long*>(play + 24) < elapsed) *reinterpret_cast<unsigned*>(play + 12) = 0;
            else Float(play, 32) = seconds / static_cast<float>(Int(play, 24));
        }
    }
    *reinterpret_cast<long long*>(play + 64) = GetTime__7VIScene(scene);
    return 0;
}
}

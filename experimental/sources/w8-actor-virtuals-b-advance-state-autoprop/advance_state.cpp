// Observed advanceState__8AutoProp at 0x00220bc0, 876 bytes.
// Offsets are into the object. The word at +376 is a state index, +288 a state count,
// +384 a 64-bit chunk mask, +200 a 64-bit mask result, and each state has an 8-byte
// record at +296 + 8*state (a word at +296 and a flags word at +300).
//  1. Remember the state s0 = word +376. If it is zero, set bit 0 of the mask at +384 and
//     store modelGetActiveChunks(model +144, low word of +384) at +200.
//  2. before = modelGetActiveChunks(model, low word of +384).
//  3. If state < count - 1, the state becomes state + 1, the word at +380 becomes 0, and
//     the 32-bit value (1 << state) sign-extended to 64 bits is OR-ed into +384.
//     Else, if bit 7 (0x80) of the word at +284 is set, state and +380 become 0.
//  4. +200 = modelGetActiveChunks(model, low word of +384). The table entry at byte
//     offset 152 (setPassableState in the AutoProp family tables) is called through the
//     table at the word at +72 with (word at +300 + 8*state) & 1.
//  5. changed = before & ~(+200). When changed equals the +200 value nothing more is done
//     in this step (the image compares them directly). Otherwise bit 63 is cleared and
//     for each set bit index i, lowest first, a PhProp is built: blockAlloc(584), then
//     the PhProp constructor with the 12-byte position at +16, the texts at +109, +216,
//     +248, the value 1<<i (64-bit), flags and a null description, and the float 1.0.
//     Flags are 0x1020e when bit 3 (value 8) of the word at +284 is set, else 0x22e, with
//     0x100 added when bit 8 (0x100) of that word is set. When bit 3 is set an effect
//     (type 2) is added at the 12 bytes at offset 416 of the new PhProp. Bit 36 is
//     skipped when the text at +76 equals one short name, ignoring case.
//  6. If the state differs from s0: when the word at +296 + 8*state is nonzero, the byte at
//     +156 becomes 0 and animAdd(+152, that word, 3 if bit 1 of the flags word is set else
//     6). Then, if the (re-read) state still differs from s0, "action<state>" is formatted,
//     and if the object has such a tag, currentRunningObject is the object while
//     scriptStart runs on each tag of that name (index 0, 1, ...), then it is cleared.
//  7. The object is added to the run list. If the text at +76 equals a second short name
//     (ignoring case), a sound sequence from lump text at +109 and a resource name is
//     played for the object at volume 1.0.
// The texts (names and formats) live in the image's shared string area; this source holds
// its own copies. Unknown: the meaning of the masks, flags and the name comparisons.

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);
int strcasecmp(const char* left, const char* right);
unsigned char* norrath_block_alloc(int requested_size);
void phprop_construct(void* self, const void* position, const char* name_a, const char* name_b, const char* name_c,
                      unsigned long long mask, int flags, void* description, float scale)
    asm("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
void addEffect__FiG6Point3(int kind, const void* position);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int mode);
int format_text(char* buffer, const char* format, ...) asm("sprintf");
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int scriptStart__FPc(char* script);
void objectAddToRunList__FP10GameObject(void* object);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
extern void* currentRunningObject;
}

namespace {
typedef int Word __attribute__((may_alias));
typedef unsigned UWord __attribute__((may_alias));
typedef unsigned long long DWord __attribute__((may_alias));

void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
}

extern "C" void advanceState__8AutoProp(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    Word& state_word = *reinterpret_cast<Word*>(object + 376);
    DWord& mask = *reinterpret_cast<DWord*>(object + 384);
    DWord& result = *reinterpret_cast<DWord*>(object + 200);
    void* model = pointer_from_word(*reinterpret_cast<UWord*>(object + 144));

    const int old_state = state_word;
    if (old_state == 0) {
        mask |= 1ull;
        result = modelGetActiveChunks__FP12_modelHeaderi(model, static_cast<int>(mask));
    }
    unsigned long long before = modelGetActiveChunks__FP12_modelHeaderi(model, static_cast<int>(mask));
    int state = state_word;
    if (state < *reinterpret_cast<Word*>(object + 288) - 1) {
        ++state;
        state_word = state;
        *reinterpret_cast<Word*>(object + 380) = 0;
        mask |= static_cast<unsigned long long>(static_cast<long long>(static_cast<int>(1u << (state & 31))));
    } else if ((*reinterpret_cast<UWord*>(object + 284) & 0x80u) != 0) {
        state_word = 0;
        *reinterpret_cast<Word*>(object + 380) = 0;
    }
    result = modelGetActiveChunks__FP12_modelHeaderi(model, static_cast<int>(mask));
    w8avb::call_slot(self, 152, *reinterpret_cast<Word*>(object + 12 + 8 * state_word + 288) & 1);

    unsigned long long changed = before & ~result;
    if (changed != result) {
        const bool special = strcasecmp(reinterpret_cast<const char*>(object + 76), "hateicon") == 0;
        changed &= 0x7fffffffffffffffull;
        if (changed != 0) {
            for (int bit = 0; changed != 0; ++bit, changed = static_cast<unsigned long long>(static_cast<long long>(changed) >> 1)) {
                if (special && bit == 36) continue;
                if ((static_cast<int>(changed & 1ull)) == 0) continue;
                const unsigned options = *reinterpret_cast<UWord*>(object + 284);
                int flags = (options & 8u) != 0 ? 0x1020e : 0x22e;
                if ((options & 0x100u) != 0) flags |= 0x100;
                unsigned char* created = norrath_block_alloc(584);
                if (created == nullptr) continue;
                float position[3];
                position[0] = *reinterpret_cast<float*>(object + 16);
                position[1] = *reinterpret_cast<float*>(object + 20);
                position[2] = *reinterpret_cast<float*>(object + 24);
                phprop_construct(created, position, reinterpret_cast<const char*>(object + 109),
                                 reinterpret_cast<const char*>(object + 216), reinterpret_cast<const char*>(object + 248),
                                 1ull << bit, flags, nullptr, 1.0f);
                if ((*reinterpret_cast<UWord*>(object + 284) & 8u) != 0) {
                    float effect_position[3];
                    effect_position[0] = *reinterpret_cast<float*>(created + 416);
                    effect_position[1] = *reinterpret_cast<float*>(created + 420);
                    effect_position[2] = *reinterpret_cast<float*>(created + 424);
                    addEffect__FiG6Point3(2, effect_position);
                }
            }
        }
    }

    int new_state = state_word;
    if (new_state != old_state) {
        void* header = pointer_from_word(*reinterpret_cast<UWord*>(object + 296 + 8 * new_state));
        if (header != nullptr) {
            object[156] = 0;
            const int mode = (*reinterpret_cast<UWord*>(object + 300 + 8 * new_state) & 2u) != 0 ? 3 : 6;
            animAdd__FP14AnimationStateP15AnimationHeaderi(object + 152, header, mode);
            new_state = state_word;
        }
        if (new_state != old_state) {
            char tag[16];
            format_text(tag, "action%d", new_state);
            if (findTagString__10GameObjectPci(self, tag, 0) != nullptr) {
                currentRunningObject = self;
                for (int index = 0;; ++index) {
                    char* script = findTagString__10GameObjectPci(self, tag, index);
                    if (script == nullptr) break;
                    scriptStart__FPc(script);
                }
                currentRunningObject = nullptr;
            }
        }
    }
    objectAddToRunList__FP10GameObject(self);
    if (strcasecmp(reinterpret_cast<const char*>(object + 76), "dt_brazier") == 0)
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(reinterpret_cast<const char*>(object + 109), "spel_brazier_out.anm"), self, 1.0f);
}

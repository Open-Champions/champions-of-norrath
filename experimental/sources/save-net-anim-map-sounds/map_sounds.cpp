#include "map_sounds.hpp"

// Observed string bytes used as strcmp operands. They are not separate imports.
// "*bodyfall*" selects signed index -3. "*blank*" selects signed index -4.
// Unknown: which other animation names exist outside this routine.

extern "C" int norrath_string_compare(const char* left, const char* right);
extern "C" int soundMapSound__FPci(const char* name, int mode);
extern "C" float soundComputeSequenceLength__FP15AnimationHeader(void* header);

// The host test compiles this same routine with an arena so 4-byte address
// words remain address words on a 64-bit machine. The EE build does not.
#ifdef NORRATH_HOST_ARENA
extern unsigned char* norrath_host_arena;
#endif

namespace {
using Word = unsigned;

#ifdef NORRATH_HOST_ARENA
Word address(const void* pointer) {
    return static_cast<Word>(static_cast<unsigned char*>(const_cast<void*>(pointer)) -
                             norrath_host_arena);
}
unsigned char* bytes(Word at) { return norrath_host_arena + at; }
#else
Word address(const void* pointer) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned char* bytes(Word at) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(at));
}
#endif
const char* text(Word at) { return reinterpret_cast<const char*>(bytes(at)); }
Word read_word(Word at) {
    return Word(bytes(at)[0]) | (Word(bytes(at)[1]) << 8) | (Word(bytes(at)[2]) << 16) |
           (Word(bytes(at)[3]) << 24);
}
void write_word(Word at, Word value) {
    bytes(at)[0] = static_cast<unsigned char>(value);
    bytes(at)[1] = static_cast<unsigned char>(value >> 8);
    bytes(at)[2] = static_cast<unsigned char>(value >> 16);
    bytes(at)[3] = static_cast<unsigned char>(value >> 24);
}
int read_half_signed(Word at) {
    const unsigned half = unsigned(bytes(at)[0]) | (unsigned(bytes(at)[1]) << 8);
    return static_cast<int>(static_cast<short>(half));
}
void write_half(Word at, int value) {
    const unsigned half = static_cast<unsigned>(value) & 0xffffu;
    bytes(at)[0] = static_cast<unsigned char>(half);
    bytes(at)[1] = static_cast<unsigned char>(half >> 8);
}

const char kBodyfall[] = "*bodyfall*";
const char kBlank[] = "*blank*";
constexpr Word kSentinel = 0x7fffffffu;
constexpr Word kSoundList = 52u;
constexpr Word kEvents = 56u;
constexpr Word kMode = 61u;
constexpr Word kFrameCount = 64u;
}  // namespace

extern "C" void norrath_archive_map_animation_sounds(void* header) {
    const Word base = address(header);
    const Word sounds = read_word(base + kSoundList);
    if (sounds == 0u) return;

    int sound_count = 0;
    if (read_word(sounds) != 0u) {
        Word offset = 0u;
        for (;;) {
            const Word name = read_word(sounds + offset);
            const int mapped = soundMapSound__FPci(text(name), 0);
            ++sound_count;
            // The list pointer is read again after the call returns.
            const Word fresh = read_word(base + kSoundList);
            write_word(fresh + offset + 4u, static_cast<Word>(mapped));
            offset += 8u;
            if (read_word(fresh + offset) == 0u) break;
        }
    }

    Word events = read_word(base + kEvents);
    if (read_word(events) != kSentinel) {
        Word offset = 0u;
        for (;;) {
            const int index = read_half_signed(events + offset + 4u);
            if (index >= 0) {
                const Word sound_list = read_word(base + kSoundList);
                const Word name = read_word(sound_list + static_cast<Word>(index) * 8u);
                if (norrath_string_compare(text(name), kBodyfall) == 0) {
                    events = read_word(base + kEvents);
                    write_half(events + offset + 4u, -3);
                } else {
                    events = read_word(base + kEvents);
                    const int fresh_index = read_half_signed(events + offset + 4u);
                    const Word sound_list_again = read_word(base + kSoundList);
                    const Word fresh_name =
                        read_word(sound_list_again + static_cast<Word>(fresh_index) * 8u);
                    // The event pointer is read again after the blank compare,
                    // whether or not the names match.
                    if (norrath_string_compare(text(fresh_name), kBlank) == 0) {
                        events = read_word(base + kEvents);
                        write_half(events + offset + 4u, -4);
                    } else {
                        events = read_word(base + kEvents);
                    }
                }
            }
            offset += 28u;
            if (read_word(events + offset) == kSentinel) break;
        }
    }

    // Mode is read before the clear and again before the repair.
    if (bytes(base + kMode)[0] == 0u) {
        events = read_word(base + kEvents);
        if (read_word(events) != kSentinel) {
            Word offset = 0u;
            for (;;) {
                bytes(events + offset + 10u)[0] = 0;
                bytes(events + offset + 11u)[0] = 0;
                bytes(events + offset + 12u)[0] = 0;
                bytes(events + offset + 13u)[0] = 0;
                bytes(events + offset + 14u)[0] = 0;
                offset += 28u;
                events = read_word(base + kEvents);
                if (read_word(events + offset) == kSentinel) break;
            }
        }
    }
    if (bytes(base + kMode)[0] < 2u) {
        events = read_word(base + kEvents);
        if (read_word(events) != kSentinel) {
            Word offset = 0u;
            for (;;) {
                const int index = read_half_signed(events + offset + 4u);
                write_half(events + offset + 4u, index);
                const bool marked_or_past_end = index < -2 || !(index < sound_count);
                if (marked_or_past_end)
                    write_half(events + offset + 4u, sound_count != 0 ? 0 : -1);
                offset += 28u;
                if (read_word(events + offset) == kSentinel) break;
            }
        }
    }

    const float length = soundComputeSequenceLength__FP15AnimationHeader(header);
    const float scaled = length * 60.0f;
    write_word(base + kFrameCount, static_cast<Word>(static_cast<int>(scaled)));
}

#define NORRATH_HOST_CONTRACT 1
#include "animation_state_copy_ctor.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

static std::vector<const void*> handles;
extern "C" unsigned norrath_host_word_for_pointer(const void* pointer) {
    handles.push_back(pointer);
    return static_cast<unsigned>(0x1000 + handles.size() - 1);
}
extern "C" unsigned char* norrath_host_pointer_for_word(unsigned word) {
    return const_cast<unsigned char*>(static_cast<const unsigned char*>(handles.at(word - 0x1000)));
}

constexpr unsigned kFrameSize = 3868;
static unsigned char pool[3][kFrameSize];
static int acquired;
static int acquire_order[4];

extern "C" void* norrath_animation_frame_state_acquire() {
    acquire_order[acquired] = acquired;
    return pool[acquired++];
}

static unsigned word_at(const unsigned char* base, unsigned offset) {
    unsigned value;
    std::memcpy(&value, base + offset, 4);
    return value;
}
static void put_word(unsigned char* base, unsigned offset, unsigned value) { std::memcpy(base + offset, &value, 4); }

int main() {
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    alignas(16) unsigned char source[8 + 32 * 3];
    alignas(16) unsigned char destination[sizeof source];
    static unsigned char source_frames[2][kFrameSize];
    std::memset(source, 0, sizeof source);
    std::memset(destination, 0xcc, sizeof destination);
    for (unsigned n = 0; n < 40; ++n) source[n] = static_cast<unsigned char>(0x30 + n);
    put_word(source, 0, 0xdeadbeefu);
    source[5] = 3;  // three slots; only slot 0 lies inside the 40 copied header bytes
    for (int f = 0; f < 2; ++f)
        for (unsigned n = 0; n < kFrameSize; ++n) source_frames[f][n] = static_cast<unsigned char>(n * 7 + f * 13 + 1);
    // Slot 0: flag 0x0800 in the header copied from the source (slot flags halfword at 36).
    const unsigned short slot0_flags = 0x0800;
    std::memcpy(source + 36, &slot0_flags, 2);
    put_word(source, 12, norrath_host_word_for_pointer(source_frames[0]));
    put_word(source, 12 + 32, 0x5555u);  // slot 1 source state: never read, its flags say none
    put_word(source, 12 + 64, norrath_host_word_for_pointer(source_frames[1]));
    // Slots 1 and 2 lie past the copied header, so their flags are whatever the destination already holds.
    const unsigned short slot1_flags = 0x0001, slot2_flags = 0x4000;
    std::memcpy(destination + 36 + 32, &slot1_flags, 2);
    std::memcpy(destination + 36 + 64, &slot2_flags, 2);
    put_word(destination, 12 + 32, 0x7777u);
    put_word(destination, 12 + 64, 0x8888u);

    __14AnimationStateRC14AnimationState(destination, source);

    expect(word_at(destination, 0) == 0u, "word 0 cleared");
    expect(std::memcmp(destination + 4, source + 4, 4) == 0 && destination[5] == 3, "header bytes after word 0 copied");
    expect(std::memcmp(destination + 8, source + 8, 28) == 0 || word_at(destination, 12) != word_at(source, 12),
           "slot 0 header bytes copied before the pointer is replaced");
    expect(destination[39] == source[39], "last header byte copied");
    expect(acquired == 2, "frame states acquired for slots 0 and 2 only");
    const unsigned char* slot0 = destination + 8;
    expect(norrath_host_pointer_for_word(word_at(slot0, 4)) == pool[0], "slot 0 holds the first fresh state");
    expect(norrath_host_pointer_for_word(word_at(pool[0], 20)) == slot0, "fresh state points back at its slot");
    expect(word_at(pool[0], 8) == word_at(source_frames[0], 8) && word_at(pool[0], 0) == word_at(source_frames[0], 0) &&
               word_at(pool[0], 4) == word_at(source_frames[0], 4), "words 8, 0 and 4 copied");
    expect(std::memcmp(pool[0] + 28, source_frames[0] + 28, 3840) == 0, "3840 bytes copied from +28");
    expect(word_at(destination, 8 + 32 + 4) == 0u, "slot 1 pointer cleared");
    const unsigned char* slot2 = destination + 8 + 64;
    expect(norrath_host_pointer_for_word(word_at(slot2, 4)) == pool[1], "slot 2 holds the second fresh state");
    expect(norrath_host_pointer_for_word(word_at(pool[1], 20)) == slot2, "slot 2 back pointer");
    expect(word_at(pool[1], 8) == word_at(source_frames[1], 8) && word_at(pool[1], 4) == word_at(source_frames[1], 4),
           "slot 2 words copied");
    expect(std::memcmp(pool[1] + 28, source_frames[1] + 28, 3840) == 0, "slot 2 bytes copied");
    // Bytes the routine does not write stay as they were (the pool starts zeroed).
    for (int s = 0; s < 2; ++s) {
        for (unsigned n = 12; n < 20; ++n) expect(pool[s][n] == 0, "bytes 12..19 untouched");
        for (unsigned n = 24; n < 28; ++n) expect(pool[s][n] == 0, "bytes 24..27 untouched");
        expect(word_at(pool[s], 12) == 0u && word_at(pool[s], 16) == 0u, "words 12 and 16 untouched");
        expect(pool[s][3868 - 1] == source_frames[s][3868 - 1], "last copied byte");
    }

    // A zero or negative slot count does nothing beyond the header.
    for (int count : {0, -1}) {
        acquired = 0;
        std::memset(destination, 0xcc, sizeof destination);
        source[5] = static_cast<unsigned char>(count);
        __14AnimationStateRC14AnimationState(destination, source);
        expect(acquired == 0 && destination[5] == static_cast<unsigned char>(count) && word_at(destination, 0) == 0u,
               "no slots processed");
    }
    if (bad) return 1;
    std::puts("animation state copy constructor contract passed");
    return 0;
}

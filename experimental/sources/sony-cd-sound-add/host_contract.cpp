#include <cstdint>
#include <cstdio>
#include <map>
#define NORRATH_HOST_CONTRACT 1
#include "sound_add.cpp"

static int converted = 32;
static int pitch = 9;
static int transfers = 0;
static int transfer_address = 0;
static std::map<unsigned, unsigned char*> objects;
extern "C" int convertEnd__Fi(int value) { return value == 12 ? converted : -1; }
extern "C" int getStockPitch__FP10_vagHeader(void* header) { return header == nullptr ? -1 : pitch; }
extern "C" int strcasecmp(const char* left, const char* right) { return left == right ? 0 : 1; }
extern "C" void transferVAG__FP10_vagHeaderi(void*, int address) {
    transfers += 1;
    transfer_address = address;
}
extern "C" unsigned char* norrath_sony_cd_resolve_sound(unsigned bits) { return objects[bits]; }
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned bits_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
int main() {
    unsigned char header[80] = {};
    put_word(header + 12, 12);
    char name[] = "wave";
    objects[bits_of(name)] = reinterpret_cast<unsigned char*>(name);
    soundAddSound__FPcP10_vagHeaderb(name, header, 0);
    if (word_at(header + 44) != 32u || word_at(header + 32) != 9u) return 1;
    if (word_at(header + 36) != 0x8040u || header[40] != 0 || transfers != 1 || transfer_address != 0x8040) return 2;
    if (nmSounds != 1 || word_at(soundList + 8) != 0x8040u + 32u) return 3;
    if (word_at(soundList) != bits_of(name) || word_at(soundList + 4) != bits_of(header)) return 4;
    converted = 48;
    char second[] = "other";
    objects[bits_of(second)] = reinterpret_cast<unsigned char*>(second);
    unsigned char again[96] = {};
    again[49] = 4;
    put_word(again + 12, 12);
    soundAddSound__FPcP10_vagHeaderb(second, again, 0);
    if (again[40] != 1 || word_at(again + 36) != ((0x8060u + 63u) & ~63u)) return 5;
    if (nmSounds != 2 || transfers != 2) return 6;
    unsigned char previous[48] = {};
    previous[40] = 7;
    put_word(previous + 36, 99);
    put_word(soundList, bits_of(name));
    put_word(soundList + 4, bits_of(previous));
    objects[bits_of(name)] = reinterpret_cast<unsigned char*>(name);
    objects[bits_of(previous)] = previous;
    nmSounds = 1;
    unsigned char fresh[48] = {};
    put_word(fresh + 12, 12);
    const int before = transfers;
    soundAddSound__FPcP10_vagHeaderb(name, fresh, 0);
    if (fresh[40] != 7 || word_at(fresh + 36) != 99u || transfers != before || nmSounds != 1) return 7;
    soundAddSound__FPcP10_vagHeaderb(second, fresh, 1);
    if (nmSounds != 1) return 8;
    effectsBufferSize = 0x200000 - 50;
    nmSounds = 0;
    soundAddSound__FPcP10_vagHeaderb(second, fresh, 0);
    if (word_at(fresh + 36) != 20480u || fresh[40] != 0 || nmSounds != 1) return 9;
    std::puts("sound add contract passed");
    return 0;
}

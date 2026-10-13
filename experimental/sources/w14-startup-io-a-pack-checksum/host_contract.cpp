#define strlen norrath_host_strlen
#include "pack_checksum.cpp"
#undef strlen
#include <cstdlib>
#include <cstring>
#include <string>

unsigned char ramSlot[0xb29c8];
unsigned char* gameCam;
static unsigned char camera[8];
static int foot1, foot2, strlen_calls;

extern "C" int GetSaveFootPrint__6Camera(void* c) { return c == camera ? foot1 : -99999; }
extern "C" int jumpGateSaveFootPrint__Fv() { return foot2; }
extern "C" unsigned norrath_host_strlen(const char* t) { ++strlen_calls; unsigned n = 0; while (t[n]) ++n; return n; }

// Independent model: 32-bit accumulator reduced to 16 bits at the end of every step.
static int sext8(unsigned char b) { return b >= 128 ? int(b) - 256 : int(b); }
static short model() {
    int sum = 0;
    auto fix = [](int v) { return int(short(v)); };
    int length = foot1 + foot2 + 28102;
    long base = 112;
    for (int i = 0; i < length; ++i) sum = fix(sum + sext8(ramSlot[base + i]) - i);
    long at = base + length + 3 * 23998;
    while (at % 4) ++at;
    int span = 36;
    for (long p = at; sext8(ramSlot[p]) != 0 && (p += 36, true); span += 36) {}
    for (int i = 0; i < span; ++i) sum = fix(sum + sext8(ramSlot[at + i]) - i);
    int size = 0;
    for (int e = 0; e < 26 && sext8(ramSlot[0x1b610 + 24 * e]) != 0; ++e) {
        const unsigned char* entry = ramSlot + 0x1b610 + 24 * e;
        int len = int(std::strlen(reinterpret_cast<const char*>(entry)));
        for (int i = 0; i < len; ++i) sum = fix(i * sext8(entry[i]) + sum);
        unsigned word; std::memcpy(&word, entry + 20, 4);
        sum = fix(sum - int(word & 0xffff));
        size += int(word);
    }
    for (int i = 0; i < size; ++i) sum = fix(i * sext8(ramSlot[0x1b898 + i]) + sum);
    return short(sum);
}

int main() {
    srand(5);
    gameCam = camera;
    for (int round = 0; round < 60; ++round) {
        std::memset(ramSlot, 0, sizeof ramSlot);
        for (int i = 0; i < 0x1b898 + 26 * 60; ++i) ramSlot[i] = static_cast<unsigned char>(rand());
        foot1 = (round % 5) * 7 - (round % 3 == 0 ? 28102 : 0);
        foot2 = round % 4;
        if (foot1 + foot2 + 28102 > 40000) return 90;
        long at = 112 + (foot1 + foot2 + 28102 > 0 ? foot1 + foot2 + 28102 : foot1 + foot2 + 28102) + 3 * 23998;
        while (at % 4) ++at;
        int records = round % 4;
        ramSlot[at + 36 * records] = 0;
        for (int e = 0; e < 26; ++e) {
            unsigned char* entry = ramSlot + 0x1b610 + 24 * e;
            int len = 1 + rand() % 19;
            for (int i = 0; i < len; ++i) entry[i] = static_cast<unsigned char>(1 + rand() % 255);
            entry[len] = 0;
            unsigned size = rand() % 30;
            std::memcpy(entry + 20, &size, 4);
        }
        int known = round % 7 == 0 ? 26 : round % 7 * 3;
        if (known < 26) ramSlot[0x1b610 + 24 * known] = 0;
        strlen_calls = 0;
        short got = getPackChecksum__Fv();
        if (got != model()) return 1;
    }
    return 0;
}

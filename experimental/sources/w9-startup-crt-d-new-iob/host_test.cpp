// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
int _fs_iob_semid = 5;
unsigned char _iob[512];
}
static std::vector<int> events;   // 0 = make, 1 = wait, 2 = signal
static std::vector<int> ids;
extern "C" void _sceFsIobSemaMK() { events.push_back(0); _fs_iob_semid = 9; }
extern "C" int WaitSema(int s) { events.push_back(1); ids.push_back(s); return 0; }
extern "C" int SignalSema(int s) { events.push_back(2); ids.push_back(s); return 0; }
#include "new_iob.cpp"

int main() {
    int failures = 0;
    std::memset(_iob, 0, sizeof _iob);
    // Slots 0..2 busy (word at +4 nonzero, any bit), slot 3 free.
    for (int i = 0; i < 3; ++i) _iob[i * 16 + 4 + i] = 1;
    unsigned char* got = new_iob();
    if (got != _iob + 48) { std::printf("first free slot expected\n"); ++failures; }
    unsigned v; std::memcpy(&v, _iob + 52, 4);
    if (v != 0x10000000u) { std::printf("marker wrong\n"); ++failures; }
    if (events != std::vector<int>{0, 1, 2} || ids != std::vector<int>{9, 9}) { std::printf("call order wrong\n"); ++failures; }
    // Words other than +4 stay untouched.
    for (int i = 0; i < 512; ++i) if (i / 16 == 3 && i % 16 != 4 && i % 16 != 7 && _iob[i] != 0) { std::printf("stray write\n"); ++failures; break; }
    // Fill everything: no free slot gives null and still signals.
    for (int i = 0; i < 32; ++i) _iob[i * 16 + 4] = 1;
    events.clear(); ids.clear();
    if (new_iob() != nullptr || events != std::vector<int>{0, 1, 2}) { std::printf("full table must return null after signal\n"); ++failures; }
    // Last slot only.
    for (int i = 0; i < 31; ++i) _iob[i * 16 + 4] = 1;
    _iob[31 * 16 + 4] = 0;
    if (new_iob() != _iob + 31 * 16) { std::printf("last slot expected\n"); ++failures; }
    if (failures) return 2;
    std::printf("new_iob cases passed\n");
    return 0;
}

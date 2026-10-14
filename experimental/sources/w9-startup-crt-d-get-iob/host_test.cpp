// Host comparison only. Not an EE build input.
#include <cstdio>
#include <vector>

extern "C" {
int _fs_iob_semid = 5;
unsigned char _iob[512];
}
static std::vector<int> events;
static std::vector<int> ids;
extern "C" void _sceFsIobSemaMK() { events.push_back(0); _fs_iob_semid = 9; }
extern "C" int WaitSema(int s) { events.push_back(1); ids.push_back(s); return 0; }
extern "C" int SignalSema(int s) { events.push_back(2); ids.push_back(s); return 0; }
#include "get_iob.cpp"

int main() {
    int failures = 0;
    for (unsigned i = 0; i < 40; ++i) {
        events.clear(); ids.clear();
        unsigned char* got = get_iob(i);
        unsigned char* want = i < 32 ? _iob + i * 16 : nullptr;
        if (got != want) { std::printf("index %u wrong\n", i); ++failures; }
        if (events != std::vector<int>{0, 1, 2} || ids != std::vector<int>{9, 9}) { std::printf("index %u order wrong\n", i); ++failures; }
    }
    events.clear();
    if (get_iob(0xffffffffu) != nullptr || events.size() != 3) { std::printf("negative index must fail\n"); ++failures; }
    if (failures) return 2;
    std::printf("get_iob cases passed\n");
    return 0;
}

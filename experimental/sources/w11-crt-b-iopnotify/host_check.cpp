// Host comparison only. Not an EE build input.
#include <cstdio>
#include <vector>
extern "C" {
int _sceSifMInitCheck;
static std::vector<std::pair<int, int>> calls;
int sceSifSetSreg(int i, int v) { calls.push_back({i, v}); return 0; }
}
#include "iopnotify.cpp"
int main() {
    int failures = 0;
    for (int mode : {0, 2, -1, 3, 0x7fffffff}) {
        _sceSifMInitCheck = 7; calls.clear();
        _iopnotify(mode, nullptr);
        if (_sceSifMInitCheck != 7 || !calls.empty()) { std::printf("mode %d acted\n", mode); ++failures; }
    }
    _sceSifMInitCheck = 7; calls.clear();
    _iopnotify(1, nullptr);
    if (_sceSifMInitCheck != 0 || calls.size() != 1 || calls[0].first != 1 || calls[0].second != 0) { std::puts("mode 1 wrong"); ++failures; }
    if (failures) return 2;
    std::puts("_iopnotify cases passed");
    return 0;
}

#include <cstdio>
#include <vector>
static std::vector<int> exits;
extern "C" void Exit(int status) { exits.push_back(status); }
#include "kill.cpp"
int main() {
    int bad = 0;
    if (kill(1, 15) != 0 || exits != std::vector<int>{15}) { std::puts("pid 1 wrong"); ++bad; }
    exits.clear();
    for (int pid : {0, 2, -1, 1000, 0x7fffffff}) if (kill(pid, 9) != 0 || !exits.empty()) { std::puts("other pid wrong"); ++bad; }
    if (kill(1, 0) != 0 || exits != std::vector<int>{0}) { std::puts("signal 0 wrong"); ++bad; }
    if (bad) return 1;
    std::puts("kill contract passed");
    return 0;
}

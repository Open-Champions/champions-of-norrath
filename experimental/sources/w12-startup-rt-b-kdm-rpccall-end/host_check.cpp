// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <cstdio>
#include "kdm_rpccall_end.cpp"
alignas(8) unsigned char GLibData[2048];
static int calls, last;
extern "C" int iSignalSema(int semaphore) { ++calls; last = semaphore; return 0; }
int main() {
    int fails = 0;
    int* words = reinterpret_cast<int*>(GLibData);
    words[10] = 77;   // +40: semaphore
    words[14] = 0;    // +56: switch clear
    words[11] = 55;   // +44: a different semaphore must not be used
    int dummy;
    kdm_rpccall_end(&dummy);
    if (calls != 1 || last != 77) { std::printf("signal count/semaphore wrong\n"); ++fails; }
    words[14] = 1;
    kdm_rpccall_end(&dummy);
    if (calls != 1) { std::printf("non-zero switch must not signal\n"); ++fails; }
    words[14] = (int)0x80000000;
    kdm_rpccall_end(nullptr);
    if (calls != 1) { std::printf("high-bit switch must not signal\n"); ++fails; }
    if (fails) return 1;
    std::printf("kdm_rpccall_end cases passed\n");
    return 0;
}

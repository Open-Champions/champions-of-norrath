#include "bounce.cpp"

#include <cstdio>

int sid = 0;
static int seen = -1;
static int result = 0;
extern "C" int iSignalSema(int semaphore) { seen = semaphore; return result; }

int main() {
    sid = 23;
    result = 0;
    if (Bounce__FiUsPv(1, 2, nullptr) != 0 || seen != 23) return 1;
    sid = 5;
    result = -7;
    if (Bounce__FiUsPv(0, 0, &sid) != -7 || seen != 5) return 2;
    std::puts("bounce contract passed");
    return 0;
}

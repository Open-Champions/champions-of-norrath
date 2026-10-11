#include <cstdio>
extern "C" unsigned char nmTasks[4];
extern "C" unsigned char currentTask[4];
int main() {
    for (int index = 0; index < 4; ++index) {
        if (nmTasks[index] != 0 || currentTask[index] != 0) {
            std::fprintf(stderr, "fail: task word byte %d\n", index);
            return 1;
        }
    }
    nmTasks[0] = 1;
    if (currentTask[0] != 0) {
        std::fprintf(stderr, "fail: current shares storage with count\n");
        return 1;
    }
    nmTasks[0] = 0;
    std::printf("task words host check passed\n");
    return 0;
}

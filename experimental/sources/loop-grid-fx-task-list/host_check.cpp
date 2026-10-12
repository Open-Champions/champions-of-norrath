// Host contract for taskList. Not an EE plan input.
#include <cstdio>

extern "C" unsigned char taskList[512];

int main() {
    for (unsigned n = 0; n < sizeof taskList; ++n)
        if (taskList[n] != 0) {
            std::fprintf(stderr, "fail: taskList starts zero\n");
            return 1;
        }
    taskList[511] = 1;   // the whole extent is writable
    std::printf("task-list contract passed\n");
    return 0;
}

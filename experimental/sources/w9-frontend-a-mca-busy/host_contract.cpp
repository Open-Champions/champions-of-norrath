#include "mca_busy.cpp"

#include <cstdio>

int checkResults;
static int sync_result;
static int queue_empty;
static int syncs;
static int queue_calls;

extern "C" int sceMcSync(int mode, int* command, int* result) {
    syncs += 1;
    if (mode != 1 || command != nullptr || result != nullptr) return -1;
    return sync_result;
}
extern "C" int commandQueueEmpty__Fv() {
    queue_calls += 1;
    return queue_empty;
}

int main() {
    checkResults = 1;
    sync_result = 1;
    queue_empty = 1;
    if (MCA_Busy__Fv() != 1 || syncs != 0 || queue_calls != 0) return 1;
    checkResults = 0;
    sync_result = 0;
    if (MCA_Busy__Fv() != 1 || syncs != 1 || queue_calls != 0) return 2;
    sync_result = 2;
    queue_empty = 1;
    if (MCA_Busy__Fv() != 0 || syncs != 2 || queue_calls != 1) return 3;
    queue_empty = 0;
    if (MCA_Busy__Fv() != 1) return 4;
    checkResults = 2;
    if (MCA_Busy__Fv() != 1) return 5;
    queue_empty = 3;
    if (MCA_Busy__Fv() != 0) return 6;
    std::puts("mca busy passed");
    return 0;
}

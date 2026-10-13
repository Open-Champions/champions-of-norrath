// Host contract for cutRunFrameTask__Fv. Not an EE plan input.
#include "cut_run_frame_task.cpp"

#include <cstdio>

static int calls;
static int last_arg = -1;
extern "C" void cutRunFrame__Fb(bool skip) {
    ++calls;
    last_arg = skip ? 1 : 0;
}

int main() {
    int failures = 0;
    cutRunFrameTask__Fv();
    if (calls != 1) { std::fprintf(stderr, "fail: one call\n"); ++failures; }
    if (last_arg != 0) { std::fprintf(stderr, "fail: argument is false\n"); ++failures; }
    cutRunFrameTask__Fv();
    if (calls != 2) { std::fprintf(stderr, "fail: one call per task run\n"); ++failures; }
    return failures ? 1 : 0;
}

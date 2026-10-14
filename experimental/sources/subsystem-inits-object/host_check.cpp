// Host contract for objectInit__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void objectInit__Fv();
extern "C" void runObjectsTask__Fv();
extern "C" void drawObjectsTask__Fv();
extern "C" unsigned objectGrid[120 * 120] = {};
extern "C" unsigned ghostObjectGrid[120 * 120] = {};
extern "C" unsigned stringHash[512] = {};
extern "C" unsigned objectHash[512] = {};
extern "C" unsigned offTheGrid = 0;
extern "C" unsigned objectStrings = 0;

namespace {
int failures = 0;
int tasks = 0;
void (*task_fn[2])();
int task_order[2];
int task_flag[2];
const char* task_name[2];
int destroys = 0;
unsigned destroyed_head = 1;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
}

extern "C" void runObjectsTask__Fv() {}
extern "C" void drawObjectsTask__Fv() {}
extern "C" void engineAddTask__FPFv_viPci(void (*task)(), int order, char* name, int flag) {
    expect(tasks < 2, "task capacity");
    if (tasks >= 2) return;
    task_fn[tasks] = task;
    task_order[tasks] = order;
    task_flag[tasks] = flag;
    task_name[tasks] = name;
    ++tasks;
}
extern "C" void listDestroy__FP8ListHead(void* head) {
    ++destroys;
    destroyed_head = *static_cast<unsigned*>(head);
}

int main() {
    std::memset(objectGrid, 0xa5, sizeof objectGrid);
    std::memset(ghostObjectGrid, 0x5a, sizeof ghostObjectGrid);
    std::memset(stringHash, 0x3c, sizeof stringHash);
    std::memset(objectHash, 0x11, sizeof objectHash);
    offTheGrid = 7;
    objectStrings = 9;
    objectInit__Fv();
    expect(tasks == 2, "two tasks");
    expect(task_fn[0] == runObjectsTask__Fv && task_order[0] == 10 && task_flag[0] == 0, "run task");
    expect(task_name[0] != nullptr && std::strcmp(task_name[0], "runObjects") == 0, "run name");
    expect(task_fn[1] == drawObjectsTask__Fv && task_order[1] == 21 && task_flag[1] == 1, "draw task");
    expect(task_name[1] != nullptr && std::strcmp(task_name[1], "drawObjects") == 0, "draw name");
    expect(destroys == 1 && destroyed_head == 0, "empty list");
    for (unsigned word : objectGrid) expect(word == 0, "object grid");
    for (unsigned word : ghostObjectGrid) expect(word == 0, "ghost grid");
    for (unsigned word : stringHash) expect(word == 0, "string hash");
    for (unsigned word : objectHash) expect(word == 0, "object hash");
    expect(offTheGrid == 0 && objectStrings == 0, "cleared words");
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    return 0;
}

// Host contract for DLG_Init. Not an EE plan input.
#include "dlg_init.cpp"

#include <cstdio>
#include <cstring>

extern "C" char* boxTex = nullptr;
extern "C" unsigned curDlg = 9u;

static int failures = 0;
static int finds = 0;
static int tasks = 0;
static char seen_lump[32];
static char seen_name[32];
static char* find_result = nullptr;
static char* box_at_task[2];
static unsigned cur_at_task[2];
static void (*task_fn[2])();
static int task_priority[2];
static char task_name[2][32];
static int task_flag[2];

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    ++finds;
    std::snprintf(seen_lump, sizeof seen_lump, "%s", lump);
    std::snprintf(seen_name, sizeof seen_name, "%s", name);
    return find_result;
}

extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag) {
    if (tasks < 2) {
        box_at_task[tasks] = boxTex;
        cur_at_task[tasks] = curDlg;
        task_fn[tasks] = callback;
        task_priority[tasks] = priority;
        std::snprintf(task_name[tasks], sizeof task_name[tasks], "%s", name);
        task_flag[tasks] = flag;
    }
    ++tasks;
}

extern "C" void runDialog__Fv() {}
extern "C" void drawDialog__Fv() {}

int main() {
    find_result = reinterpret_cast<char*>(static_cast<unsigned long>(0x51u));
    boxTex = reinterpret_cast<char*>(static_cast<unsigned long>(0x22u));
    curDlg = 9u;
    DLG_Init__Fv();
    expect(finds == 1, "one resource lookup");
    expect(std::strcmp(seen_lump, "dlgbox.lmp") == 0, "lump name");
    expect(std::strcmp(seen_name, "box.tex") == 0, "texture name");
    expect(boxTex == find_result, "boxTex receives the lookup");
    expect(tasks == 2, "two tasks");
    expect(box_at_task[0] == find_result && box_at_task[1] == find_result, "boxTex visible to both tasks");
    expect(cur_at_task[0] == 9u && cur_at_task[1] == 9u, "curDlg still uncleared during registration");
    expect(curDlg == 0u, "curDlg cleared after registration");
    expect(task_fn[0] == runDialog__Fv && task_priority[0] == 110, "run task");
    expect(std::strcmp(task_name[0], "Dialog Demon Run") == 0 && task_flag[0] == 0, "run arguments");
    expect(task_fn[1] == drawDialog__Fv && task_priority[1] == 110, "draw task");
    expect(std::strcmp(task_name[1], "Dialog Demon Draw") == 0 && task_flag[1] == 1, "draw arguments");

    find_result = nullptr;
    finds = 0;
    tasks = 0;
    DLG_Init__Fv();
    expect(boxTex == nullptr && box_at_task[0] == nullptr, "null resource is stored");
    if (failures != 0) return 1;
    std::puts("dlg init contract passed");
    return 0;
}

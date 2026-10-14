#include "floor_init.cpp"

#include <cstdio>
#include <cstring>

static int clear_calls;
static int task_calls;
static void (*seen_callback)();
static int seen_order;
static int seen_mode;
static char* seen_label;

extern "C" void norrath_floor_clear() { ++clear_calls; }
extern "C" void floorDraw__Fv() {}
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int order,
                                          char* label, int mode) {
    if (clear_calls != 1) task_calls = -100;
    ++task_calls;
    seen_callback = callback;
    seen_order = order;
    seen_label = label;
    seen_mode = mode;
}

int main() {
    floorInit__Fv();
    if (clear_calls != 1 || task_calls != 1) return 1;
    if (seen_callback != floorDraw__Fv) return 2;
    if (seen_order != 28 || seen_mode != 1) return 3;
    if (seen_label != norrath_floor_draw_task_label) return 4;
    if (std::strcmp(norrath_floor_draw_task_label,
                    "drawFloorSprites (must be before drawworld)") != 0)
        return 5;
    if (std::strlen(norrath_floor_draw_task_label) != 43) return 6;
    std::puts("floor init call order passed");
    return 0;
}

// Host contract for lightInit__Fv. Not an EE plan input.
#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" void lightInit__Fv();
extern "C" void transformSceneLightTask__Fv();
extern "C" unsigned char scene[56] = {};

namespace {
int failures = 0;
int direct_calls = 0;
int reset_calls = 0;
int tasks = 0;
int scene_ready_at_task = 0;
const char* task_name = nullptr;
int task_order = -1;
int task_flag = -1;

unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

bool scene_ready() {
    const float expected = 1.0f / std::sqrt(3.0f);
    union {
        float value;
        unsigned bits;
    } x, y, z;
    x.bits = load_word(scene);
    y.bits = load_word(scene + 4);
    z.bits = load_word(scene + 8);
    return std::fabs(x.value + expected) < 0.00001f && std::fabs(y.value + expected) < 0.00001f &&
           std::fabs(z.value - expected) < 0.00001f && scene[36] == 64 && scene[52] == 128;
}
}

extern "C" void transformSceneLightTask__Fv() { ++direct_calls; }
extern "C" void lightResetOptionalParams__Fv() {
    ++reset_calls;
    expect(direct_calls == 1, "reset after transform");
    expect(scene_ready(), "scene before reset");
}
extern "C" void engineAddTask__FPFv_viPci(void (*task)(), int order, char* name, int flag) {
    ++tasks;
    task_order = order;
    task_flag = flag;
    task_name = name;
    scene_ready_at_task = scene_ready();
    expect(task == transformSceneLightTask__Fv, "task function");
    expect(reset_calls == 1, "task after reset");
}

int main() {
    std::memset(scene, 0xa5, sizeof scene);
    lightInit__Fv();
    expect(direct_calls == 1 && reset_calls == 1 && tasks == 1, "call counts");
    expect(task_order == 12 && task_flag == 0, "task arguments");
    expect(task_name != nullptr && std::strcmp(task_name, "transformSceneLight") == 0, "task name");
    expect(scene_ready_at_task, "scene before task");
    for (int byte = 12; byte < 36; ++byte) expect(scene[byte] == 0xa5, "untouched direction gap");
    const unsigned char tail[] = {64, 70, 80, 0, 32, 32, 32, 0, 64, 70, 80, 0, 32, 32, 32, 0};
    for (int index = 0; index < 16; ++index) expect(scene[36 + index] == tail[index], "scene tail bytes");
    expect(load_word(scene + 52) == 128u, "scene word 52");
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    return 0;
}

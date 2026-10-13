// Host contract for showHiddenObjects__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct GameObject;
static int calls;
static GameObject* last;
extern "C" void objectUpdateInGrid__FP10GameObject(GameObject* o) { ++calls; last = o; }
extern "C" void showHiddenObjects__FP10GameObject(GameObject*);
alignas(8) static unsigned char obj[640];
static void run(int flags, unsigned char state) {
    std::memset(obj, 0, sizeof obj);
    *reinterpret_cast<int*>(obj + 12) = flags;
    obj[570] = state;
    showHiddenObjects__FP10GameObject(reinterpret_cast<GameObject*>(obj));
}
int main() {
    run(0x20, 0x08);
    if (calls != 1 || last != reinterpret_cast<GameObject*>(obj)) return 1;
    run(0x00, 0x08);                    // flag bit clear
    run(0x20, 0x07);                    // state bit clear
    run(~0x20, 0xff);
    if (calls != 1) return 2;
    run(0x20 | 0x100, 0x0c);
    if (calls != 2) return 3;
    if (obj[570] != 0x0c || *reinterpret_cast<int*>(obj + 12) != 0x120) return 4;   // untouched
    std::printf("show hidden objects contract passed\n");
    return 0;
}

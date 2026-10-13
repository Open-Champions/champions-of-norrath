#include "world_clear.cpp"

static int results[3];
static void* seen[3];
static int calls;
extern "C" int Clear__t7VIArray1Z6VIZone(void* a) { seen[calls] = a; return results[calls++]; }
extern "C" int Clear__11VIWorldTree(void* a) { seen[calls] = a; return results[calls++]; }
extern "C" int Clear__t7VIArray1Z13VIWorldRegion(void* a) { seen[calls] = a; return results[calls++]; }

int main() {
    alignas(8) static char world[128];
    struct Case { int r0, r1, r2, calls, ret; } cases[] = {
        {0, 0, 0, 3, 0}, {5, 7, 9, 3, 0}, {-1, 0, 0, 1, -1}, {0, -3, 0, 2, -1}, {0, 0, -9, 3, -1},
        {1, 0, -1, 3, -1}, {-5, -5, -5, 1, -1},
    };
    for (auto& c : cases) {
        calls = 0; results[0] = c.r0; results[1] = c.r1; results[2] = c.r2;
        if (Clear__7VIWorld(world) != c.ret) return 1;
        if (calls != c.calls) return 2;
        if (seen[0] != world + 72) return 3;
        if (calls > 1 && seen[1] != world + 52) return 4;
        if (calls > 2 && seen[2] != world + 80) return 5;
    }
    return 0;
}

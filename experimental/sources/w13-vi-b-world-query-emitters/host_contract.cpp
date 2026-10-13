#include "query_emitters.cpp"
#include <string.h>

static int calls, nextResult, seenMode, atCall;
static void* seenWorld; static void* seenVars;
extern "C" int QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars(void* world, int mode, void* vars) {
    ++calls; seenWorld = world; seenMode = mode; seenVars = vars;
    atCall = *reinterpret_cast<int*>(vars);
    return nextResult;
}

int main() {
    static unsigned char world[128];
    int vars[8];
    struct Case { int w52; int result; int expectMode; int expectRet; int expectSerial; } cases[] = {
        {0, 0, 0x40000000, 0, 8}, {7, 5, 0, 0, 8}, {-1, -1, 0, -1, 7}, {0, -3, 0x40000000, -1, 7}, {0, 0x7fffffff, 0x40000000, 0, 8},
    };
    for (const Case& c : cases) {
        memset(world, 0x33, sizeof world);
        memset(vars, 0x44, sizeof vars);
        memcpy(world + 52, &c.w52, 4);
        int serial = 7; memcpy(world + 68, &serial, 4);
        nextResult = c.result; calls = 0;
        if (QueryEmitters__7VIWorldR23VISceneEmitterQueryVars(world, vars) != c.expectRet || calls != 1) return 1;
        if (seenWorld != world || seenVars != vars || seenMode != c.expectMode || atCall != 7) return 2;
        int got; memcpy(&got, world + 68, 4);
        if (got != c.expectSerial) return 3;
        if (vars[0] != 7) return 4;
        for (int i = 1; i < 8; ++i) if (vars[i] != 0x44444444) return 5;
    }
    return 0;
}

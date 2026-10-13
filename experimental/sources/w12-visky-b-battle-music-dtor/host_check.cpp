#include "dtor.cpp"

static int order[4];
static void* seen[4];
static int n;
extern "C" void Free__13VIBattleMusic(void* p) { order[n] = 1; seen[n++] = p; }
extern "C" void __builtin_delete(void* p) { order[n] = 2; seen[n++] = p; }

int main() {
    static char object[16];
    const int flag_values[] = {0, 1, 2, 3, 0x10001, -1, -2};
    for (int flags : flag_values) {
        n = 0;
        _$_13VIBattleMusic(object, flags);
        if (n != ((flags & 1) ? 2 : 1)) return 1;
        if (order[0] != 1 || seen[0] != object) return 2;
        if ((flags & 1) && (order[1] != 2 || seen[1] != object)) return 3;
    }
    return 0;
}

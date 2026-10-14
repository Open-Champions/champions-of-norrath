#include "dtor.cpp"

static void* log_ptr[8];
static int log_kind[8];
static int log_flag[8];
static int n;
static void rec(int kind, void* p, int f) { log_kind[n] = kind; log_ptr[n] = p; log_flag[n] = f; ++n; }
extern "C" int Clear__14VIQueryTriList(void* o) { rec(1, o, 0); return 0; }
extern "C" void _$_t7VIArray1Z7VIVect3(void* a, int f) { rec(2, a, f); }
extern "C" void __builtin_delete(void* p) { rec(3, p, 0); }

int main() {
    unsigned char object[160];
    for (int flags = 0; flags < 4; ++flags) {
        n = 0;
        _$_14VIQueryTriList(object, flags);
        int expect = (flags & 1) ? 4 : 3;
        if (n != expect) return 1 + flags;
        if (log_kind[0] != 1 || log_ptr[0] != object) return 10;
        if (log_kind[1] != 2 || log_ptr[1] != object + 20 || log_flag[1] != 2) return 11;
        if (log_kind[2] != 2 || log_ptr[2] != object + 12 || log_flag[2] != 2) return 12;
        if ((flags & 1) && (log_kind[3] != 3 || log_ptr[3] != object)) return 13;
    }
    return 0;
}

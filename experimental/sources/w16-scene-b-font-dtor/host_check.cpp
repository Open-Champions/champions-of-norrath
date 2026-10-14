#include <initializer_list>
#include <string.h>
static int clears, deletes;
static void* clear_arg;
static void* delete_arg;
extern "C" int Clear__7VIRFont(void* self) { ++clears; clear_arg = self; return 0; }
extern "C" void __builtin_delete(void* p) { ++deletes; delete_arg = p; }
#include "font_dtor.cpp"

int main() {
    int o;
    for (int flags : {0, 1, 2, 3, -1}) {
        clears = deletes = 0;
        clear_arg = delete_arg = 0;
        _$_7VIRFont(&o, flags);
        if (clears != 1 || clear_arg != &o) return 1;
        if (deletes != ((flags & 1) ? 1 : 0)) return 2;
        if ((flags & 1) && delete_arg != &o) return 3;
    }
    return 0;
}

#include "dtor.cpp"

struct Call { int kind; void* pointer; int flags; };
static Call calls[8];
static int count;
static void add(int kind, void* pointer, int flags) { calls[count++] = Call{kind, pointer, flags}; }

extern "C" int Close__9VICollide(void* object) { add(1, object, 0); return 0; }
extern "C" void _$_t6VIPool1ZP12VICollBuffer(void* pool, int flags) { add(2, pool, flags); }
extern "C" void _$_10VIMatrix44(void* matrix, int flags) { add(3, matrix, flags); }
extern "C" void __builtin_delete(void* pointer) { add(4, pointer, 0); }

int main() {
    unsigned char object[600];
    _$_9VICollide(object, 0);
    if (count != 4) return 1;
    if (calls[0].kind != 1 || calls[0].pointer != object) return 2;
    if (calls[1].kind != 2 || calls[1].pointer != object + 528 || calls[1].flags != 2) return 3;
    if (calls[2].kind != 3 || calls[2].pointer != object + 80 || calls[2].flags != 2) return 4;
    if (calls[3].kind != 3 || calls[3].pointer != object + 16 || calls[3].flags != 2) return 5;
    count = 0;
    _$_9VICollide(object, 2);
    if (count != 4) return 6;
    count = 0;
    _$_9VICollide(object, 3);
    if (count != 5 || calls[4].kind != 4 || calls[4].pointer != object) return 7;
    return 0;
}

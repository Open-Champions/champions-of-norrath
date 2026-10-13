#include "dtor.cpp"

static int order[4];
static int count;
static void* closed;
static void* deleted;
extern "C" void Close__5VIPad(void* object) { closed = object; order[count++] = 1; }
extern "C" void __builtin_delete(void* pointer) { deleted = pointer; order[count++] = 2; }

int main() {
    int object;
    _$_5VIPad(&object, 0);
    if (count != 1 || closed != &object || deleted) return 1;
    count = 0;
    _$_5VIPad(&object, 2);
    if (count != 1 || deleted) return 2;
    count = 0;
    _$_5VIPad(&object, 1);
    if (count != 2 || order[0] != 1 || order[1] != 2 || deleted != &object) return 3;
    return 0;
}

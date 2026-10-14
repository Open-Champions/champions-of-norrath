#include "dtor.cpp"

static void* deleted;
static int calls;
extern "C" void __builtin_delete(void* pointer) {
    deleted = pointer;
    ++calls;
}

int main() {
    int object;
    _$_10VIMatrix44(&object, 0);
    if (calls != 0) return 1;
    _$_10VIMatrix44(&object, 2);
    if (calls != 0) return 2;
    _$_10VIMatrix44(&object, 1);
    if (calls != 1 || deleted != &object) return 3;
    deleted = nullptr;
    _$_10VIMatrix44(&object, 0x11);
    if (calls != 2 || deleted != &object) return 4;
    return 0;
}

#include "launcher_dtor.cpp"

static int deletes;
static void* deleted;
static int error;

extern "C" void __builtin_delete(void* pointer) {
    ++deletes;
    deleted = pointer;
}

int main() {
    unsigned char object[8];
    object[0] = 0x11;
    _$_10VILauncher(reinterpret_cast<VILauncher*>(object), 0);
    _$_10VILauncher(reinterpret_cast<VILauncher*>(object), 2);
    if (deletes != 0 || object[0] != 0x11) return 1;
    _$_10VILauncher(reinterpret_cast<VILauncher*>(object), 1);
    if (deletes != 1 || deleted != object || object[0] != 0x11) return 2;
    deletes = 0;
    _$_10VILauncher(reinterpret_cast<VILauncher*>(object), 3);
    if (deletes != 1 || deleted != object) return 3;
    (void)error;
    return 0;
}

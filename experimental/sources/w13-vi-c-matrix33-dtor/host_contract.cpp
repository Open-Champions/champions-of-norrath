// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "dtor.cpp"

static void* deleted;
static int deletes;
extern "C" void __builtin_delete(void* pointer) { deleted = pointer; ++deletes; }

int main() {
    unsigned char object[48];
    _$_10VIMatrix33(object, 0);
    _$_10VIMatrix33(object, 2);
    if (deletes != 0) return 1;
    _$_10VIMatrix33(object, 1);
    if (deletes != 1 || deleted != object) return 2;
    _$_10VIMatrix33(object, 3);
    if (deletes != 2) return 3;
    return 0;
}

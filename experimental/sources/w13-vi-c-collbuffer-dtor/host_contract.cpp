// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "dtor.cpp"

struct Call { int kind; void* pointer; };
static Call calls[8];
static int count;
extern "C" void Clear__12VICollBuffer(void* object) { calls[count++] = Call{1, object}; }
extern "C" void __builtin_delete(void* pointer) { calls[count++] = Call{2, pointer}; }

int main() {
    unsigned char object[64];
    _$_12VICollBuffer(object, 0);
    if (count != 1 || calls[0].kind != 1 || calls[0].pointer != object) return 1;
    count = 0;
    _$_12VICollBuffer(object, 2);
    if (count != 1) return 2;
    count = 0;
    _$_12VICollBuffer(object, 1);
    if (count != 2 || calls[0].kind != 1 || calls[1].kind != 2 || calls[1].pointer != object) return 3;
    return 0;
}

// Host contract for __builtin_vec_delete. Not an EE plan input.
#include "builtin_vec_delete.cpp"
static void* seen = reinterpret_cast<void*>(1); static int calls; static char block[4];
extern "C" void __builtin_delete(void* pointer) { seen = pointer; ++calls; }
int main() {
    __builtin_vec_delete(block);
    if (seen != block || calls != 1) return 1;
    __builtin_vec_delete(0);                          // the null test belongs to __builtin_delete
    if (seen != 0 || calls != 2) return 2;
    return 0;
}

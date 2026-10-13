#include "free.cpp"

extern "C" void* sMessagePool = nullptr;
static void* freed;
static int calls;
extern "C" void __wrap_free(void* pointer) {
    freed = pointer;
    ++calls;
}

int main() {
    int pool;
    sMessagePool = &pool;
    VIFreeUSocketExApi__Fv();
    if (calls != 1 || freed != &pool || sMessagePool != &pool) return 1;
    sMessagePool = nullptr;
    VIFreeUSocketExApi__Fv();
    if (calls != 2 || freed != nullptr) return 2;
    return 0;
}

// Host contract for __get_eh_context. Not an EE plan input.
#include "get_eh_context.cpp"
static int slot_a, slot_b, calls;
static void* provider_a() { ++calls; return &slot_a; }
static void* provider_b() { ++calls; return &slot_b; }
extern "C" void* (*get_eh_context)() = provider_a;
int main() {
    if (__get_eh_context() != &slot_a || calls != 1) return 1;
    get_eh_context = provider_b;                       // the pointer is read on every call
    if (__get_eh_context() != &slot_b || calls != 2) return 2;
    if (__get_eh_context() != &slot_b || calls != 3) return 3;
    get_eh_context = 0;
    return 0;
}

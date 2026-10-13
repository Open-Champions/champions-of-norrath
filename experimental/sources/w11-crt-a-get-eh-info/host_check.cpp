// Host contract for __get_eh_info. Not an EE plan input.
#include "get_eh_info.cpp"
static unsigned char context[32];
static void* provider() { return context; }
static void* provider_null() { return 0; }
extern "C" void* (*get_eh_context)() = provider;
int main() {
    if (__get_eh_info() != context + 8) return 1;
    get_eh_context = provider_null;
    if (__get_eh_info() != reinterpret_cast<void*>(8)) return 2;
    return 0;
}

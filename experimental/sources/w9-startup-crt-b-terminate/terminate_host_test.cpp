// Host check only; not an EE build input.
#include <csetjmp>
#include <cstdio>
extern "C" void (*__terminate_func)() __attribute__((noreturn));
extern "C" void __terminate() __attribute__((noreturn));
static std::jmp_buf env;
static int hits;
static void handler_a() __attribute__((noreturn));
static void handler_a() { ++hits; std::longjmp(env, 1); }
static void handler_b() __attribute__((noreturn));
static void handler_b() { hits += 10; std::longjmp(env, 1); }
void (*__terminate_func)() __attribute__((noreturn)) = nullptr;
int main() {
    __terminate_func = handler_a;
    if (setjmp(env) == 0) __terminate();
    if (hits != 1) return 1;
    // The pointer is re-read on every call, so replacing it changes the target.
    __terminate_func = handler_b;
    if (setjmp(env) == 0) __terminate();
    if (hits != 11) return 2;
    std::puts("terminate ok");
    return 0;
}

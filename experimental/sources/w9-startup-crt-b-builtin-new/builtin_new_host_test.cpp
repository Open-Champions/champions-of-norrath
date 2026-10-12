// Host check only; not an EE build input. Build with -D__builtin_new=w9_builtin_new.
#include <csetjmp>
#include <cstdio>
extern "C" {
void* w9_builtin_new(unsigned size);
void (*__new_handler)() = nullptr;
unsigned char _vt$9bad_alloc[32];
void _$_9bad_alloc() {}
}
static int mallocs, fail_count, handler_calls, thrown, pushes, tf_calls, eh_size;
static unsigned last_size;
static unsigned char eh_record[8], type_obj[8];
static void* pushed_obj; static void* pushed_type; static void (*pushed_dtor)();
static std::jmp_buf env;
static unsigned char heap[16];
extern "C" void* __wrap_malloc(unsigned size) { ++mallocs; last_size = size; return fail_count > 0 ? (--fail_count, nullptr) : heap; }
extern "C" void* __eh_alloc(unsigned size) { eh_size = (int)size; return eh_record; }
extern "C" void* __tf9bad_alloc() { ++tf_calls; return type_obj; }
extern "C" void __cp_push_exception(void* o, void* t, void (*d)()) { ++pushes; pushed_obj = o; pushed_type = t; pushed_dtor = d; }
extern "C" void __throw() __attribute__((noreturn));
extern "C" void __throw() { ++thrown; std::longjmp(env, 1); }
static void handler() { ++handler_calls; if (handler_calls == 3) __new_handler = nullptr; }
#define CHECK(c, n) do { if (!(c)) { std::printf("fail %d\n", n); return n; } } while (0)
int main() {
    CHECK(w9_builtin_new(40) == heap && mallocs == 1 && last_size == 40 && handler_calls == 0, 1);
    CHECK(w9_builtin_new(0) == heap && last_size == 1, 2);          // zero becomes one
    // Handler installed: retried until malloc succeeds, same size each time.
    mallocs = 0; fail_count = 2; __new_handler = handler;
    CHECK(w9_builtin_new(64) == heap && handler_calls == 2 && mallocs == 3 && last_size == 64, 3);
    // Handler word re-read each pass: handler clears itself on its 3rd call, then bad_alloc.
    fail_count = 100; handler_calls = 0; mallocs = 0; __new_handler = handler;
    if (setjmp(env) == 0) { w9_builtin_new(8); CHECK(false, 4); }
    CHECK(handler_calls == 3 && thrown == 1 && mallocs == 4, 5);
    CHECK(eh_size == 4 && pushes == 1 && pushed_obj == eh_record && pushed_type == type_obj && pushed_dtor == _$_9bad_alloc, 6);
    CHECK(*(void**)eh_record == (void*)_vt$9bad_alloc, 7);          // vtable word set before registering
    // No handler at all: throws on first failure without calling malloc again.
    mallocs = 0; fail_count = 1; thrown = 0; __new_handler = nullptr;
    if (setjmp(env) == 0) { w9_builtin_new(8); CHECK(false, 8); }
    CHECK(thrown == 1 && mallocs == 1, 9);
    std::puts("builtin_new ok");
    return 0;
}

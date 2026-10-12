// Host comparison only. Not an EE build input.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" const char norrath_assert_format_view[] = "Assertion \"%s\" failed in %s:%d  context:%s (%x)\n";
static const char first_context[] = "first", second_context[] = "second", third_context[] = "third";
extern "C" const char* norrath_assert_context_word_view = first_context;

struct Call { const char* format; const char* text; const char* file; unsigned line; const char* context; int caller; };
static std::vector<Call> calls;
static int faults = 0;

extern "C" int scePrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    Call call;
    call.format = format;
    call.text = va_arg(args, const char*);
    call.file = va_arg(args, const char*);
    call.line = va_arg(args, unsigned);
    call.context = va_arg(args, const char*);
    call.caller = va_arg(args, int);
    va_end(args);
    calls.push_back(call);
    // The context word is re-read before every repetition.
    if (calls.size() == 1) norrath_assert_context_word_view = second_context;
    if (calls.size() == 2) norrath_assert_context_word_view = third_context;
    return 0;
}
extern "C" void norrath_assert_host_fault_probe() { ++faults; }

#include "assert.cpp"

int main() {
    int failures = 0;
    __assert__FPCcUiT0i("world.cc", 2326u, "x > 0", 0x1234);
    if (calls.size() != 3) { std::printf("expected three prints\n"); return 1; }
    const char* contexts[3] = {first_context, second_context, third_context};
    for (int i = 0; i < 3; ++i) {
        const Call& c = calls[i];
        if (c.format != norrath_assert_format_view || std::strcmp(c.text, "x > 0") != 0 || std::strcmp(c.file, "world.cc") != 0 ||
            c.line != 2326u || c.caller != 0x1234 || c.context != contexts[i]) {
            std::printf("call %d wrong\n", i);
            ++failures;
        }
    }
    if (faults != 1) { std::printf("fault store must run exactly once, after the prints\n"); ++failures; }
    if (failures) return 2;
    std::printf("__assert cases passed\n");
    return 0;
}

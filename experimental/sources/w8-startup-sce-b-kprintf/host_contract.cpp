#include "kprintf.cpp"

#include <cstdio>
#include <cstring>

static const char* seen_format;
static int seen_int;
static const char* seen_string;
static int calls;

extern "C" void _printf(const char* format, __builtin_va_list arguments) {
    ++calls;
    seen_format = format;
    seen_int = __builtin_va_arg(arguments, int);
    seen_string = __builtin_va_arg(arguments, const char*);
}

int main() {
    const char* format = "%d %s";
    kprintf(format, 1234, "text");
    if (calls != 1 || seen_format != format || seen_int != 1234 || std::strcmp(seen_string, "text") != 0)
        return 1;
    kprintf(format, -5, "");
    if (calls != 2 || seen_int != -5 || seen_string[0] != 0) return 2;
    std::puts("kprintf passed");
    return 0;
}

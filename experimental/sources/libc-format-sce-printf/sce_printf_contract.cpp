#include <cstdio>

extern "C" int norrath_sce_printf(const char*, ...);
extern "C" int serialPutchar(int) { return 0; }
extern "C" int deci2Putchar(int) { return 0; }
extern "C" int other_putchar(int) { return 0; }
extern "C" int (*_putchar)(int);

static const char* g_format;
static int g_hook_during_call;
static int g_calls;

extern "C" int _printf(const char* format, const void* arguments) {
    ++g_calls;
    g_format = format;
    g_hook_during_call = _putchar == deci2Putchar;
    if (arguments == nullptr) return -1;
    return 7;
}

int main() {
    _putchar = other_putchar;
    if (norrath_sce_printf("score %d", 4) != 7) return 1;
    if (g_calls != 1 || g_format == nullptr || g_format[0] != 's') return 2;
    if (!g_hook_during_call) return 3;
    if (_putchar != other_putchar) return 4;
    std::printf("scePrintf cases passed\n");
    return 0;
}

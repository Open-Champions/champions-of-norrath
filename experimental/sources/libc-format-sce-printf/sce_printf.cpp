// Observed scePrintf at 0x002d70d0. It installs deci2Putchar over the
// current _putchar hook, calls _printf, then restores the previous hook.
// The initial hook target is serialPutchar.
extern "C" int serialPutchar(int character);
extern "C" int deci2Putchar(int character);
extern "C" int _printf(const char* format, const void* arguments);
extern "C" int (*_putchar)(int) = serialPutchar;

static int norrath_sce_printf_body(const char* format, __builtin_va_list arguments) {
    int (*saved)(int) = _putchar;
    _putchar = deci2Putchar;
    const int result = _printf(format, reinterpret_cast<const void*>(arguments));
    _putchar = saved;
    return result;
}

extern "C" int norrath_sce_printf(const char* format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    const int result = norrath_sce_printf_body(format, arguments);
    __builtin_va_end(arguments);
    return result;
}

// Same body as norrath_sce_printf. The startup callers use this name.
#if defined(__mips__)
extern "C" int scePrintf(const char* format, ...) __attribute__((alias("norrath_sce_printf")));
#else
extern "C" int scePrintf(const char* format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    const int result = norrath_sce_printf_body(format, arguments);
    __builtin_va_end(arguments);
    return result;
}
#endif

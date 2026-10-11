// Observed scePrintf at 0x002d70d0. It installs deci2Putchar over the
// current _putchar hook, calls _printf, then restores the previous hook.
// The initial hook target is serialPutchar.
extern "C" int serialPutchar(int character);
extern "C" int deci2Putchar(int character);
extern "C" int _printf(const char* format, const void* arguments);
extern "C" int (*_putchar)(int) = serialPutchar;

extern "C" int norrath_sce_printf(const char* format, ...) {
    int (*saved)(int) = _putchar;
    _putchar = deci2Putchar;
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    const int result = _printf(format, reinterpret_cast<const void*>(arguments));
    __builtin_va_end(arguments);
    _putchar = saved;
    return result;
}

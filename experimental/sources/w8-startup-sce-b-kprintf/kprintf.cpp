// Observed kprintf at 0x002d7098, 56 bytes. It spills its register arguments
// and passes the format and a pointer to the variable arguments to _printf,
// whose own return value is not used.
extern "C" {
void _printf(const char* format, __builtin_va_list arguments);

void kprintf(const char* format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    _printf(format, arguments);
    __builtin_va_end(arguments);
}
}

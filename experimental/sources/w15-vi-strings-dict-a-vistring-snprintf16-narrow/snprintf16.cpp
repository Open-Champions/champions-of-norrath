// Observed: snprintf16__8VIStringPUsiPCce at 0x0101a088 is 88 bytes. A variadic function: wide
// buffer (a0), size (a1), narrow format (a2), then variable arguments. It builds a pointer to the
// variable-argument save area and calls vsnprintf16__8VIStringPUsiPCcPv(buffer, size, format,
// args). It returns nothing of its own.
// Inferred: formats a narrow format string into a 16-bit buffer.
// Unknown: the callee's formatting rules and whether the size is bytes or units.
extern "C" {
void vsnprintf16__8VIStringPUsiPCcPv(unsigned short* buffer, int size, const char* format, void* args);

void snprintf16__8VIStringPUsiPCce(unsigned short* buffer, int size, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    vsnprintf16__8VIStringPUsiPCcPv(buffer, size, format, ap);
    __builtin_va_end(ap);
}
}

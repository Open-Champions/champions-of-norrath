// Observed: snprintf__8VIStringPciPCce at 0x0101a030 is 88 bytes. A variadic function: buffer
// (a0), size (a1), format (a2), then variable arguments. It builds a pointer to the variable-
// argument save area and calls the instantiation VI_vsnprintf<char>(buffer, size, format, args);
// it returns nothing of its own (the callee's mangled name ends in a void return).
// Inferred: the narrow-character formatted-print front end of VIString.
// Unknown: the callee's formatting rules (not part of this unit).
extern "C" {
void VI_vsnprintf__H1Zc_PX01iPCX01Pv_v(char* buffer, int size, const char* format, void* args);

void snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    VI_vsnprintf__H1Zc_PX01iPCX01Pv_v(buffer, size, format, ap);
    __builtin_va_end(ap);
}
}

// Observed: snprintf16__8VIStringPUsiPCUse at 0x0101a0e0 is 88 bytes. A variadic function: wide
// buffer (a0), size (a1), wide format (a2), then variable arguments. It builds a pointer to the
// variable-argument save area and calls the instantiation VI_vsnprintf<unsigned short>(buffer,
// size, format, args). It returns nothing of its own.
// Inferred: the wide-character formatted-print front end of VIString.
// Unknown: the callee's formatting rules and whether the size is bytes or units.
extern "C" {
void VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(unsigned short* buffer, int size, const unsigned short* format, void* args);

void snprintf16__8VIStringPUsiPCUse(unsigned short* buffer, int size, const unsigned short* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(buffer, size, format, ap);
    __builtin_va_end(ap);
}
}

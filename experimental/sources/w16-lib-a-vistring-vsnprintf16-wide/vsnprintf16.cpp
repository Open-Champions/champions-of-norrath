// Observed: vsnprintf16__8VIStringPUsiPCUsPv at 0x0101a7e0, 24 bytes, pinned SLUS_205.65. It calls the instantiation
// VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v (0x011f59d0) with its four arguments unchanged (buffer, size, wide format, argument
// pointer) and returns nothing of its own.
// Inferred: the wide front end of VIString's formatter. Unknown: the callee's formatting rules.
extern "C" {
void VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(unsigned short* buffer, int size, const unsigned short* format, void* args);

void vsnprintf16__8VIStringPUsiPCUsPv(unsigned short* buffer, int size, const unsigned short* format, void* args) {
    VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v(buffer, size, format, args);
}
}

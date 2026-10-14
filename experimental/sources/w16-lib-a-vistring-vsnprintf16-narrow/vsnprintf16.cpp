// Observed: vsnprintf16__8VIStringPUsiPCcPv at 0x01019f80, 172 bytes, pinned SLUS_205.65. Arguments: wide output buffer (a0),
// size (a1), narrow format (a2) and the variable-argument pointer (a3). With n = strlen(format):
//  1. units = 2 * n + 2. A block of 2 * units bytes, rounded up to a multiple of 16, is taken from the stack
//     (the stack pointer is lowered and restored on return).
//  2. str8to16__8VIStringPUsUiPCc(block, units, format) widens the format into the block.
//  3. vsnprintf16__8VIStringPUsiPCUsPv(buffer, size, block, args) formats with the widened format.
// It returns nothing of its own; the original's result register still holds the last callee's value.
// Inferred: the narrow front end of the wide formatter; the block is deliberately larger than the widened format needs
// (str8to16 stops after n units when its limit is 2 * n + 2).
// Unknown: the callee's formatting rules.
extern "C" {
unsigned strlen(const char* text);
unsigned short* str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void vsnprintf16__8VIStringPUsiPCUsPv(unsigned short* buffer, int size, const unsigned short* format, void* args);

void vsnprintf16__8VIStringPUsiPCcPv(unsigned short* buffer, int size, const char* format, void* args) {
    const unsigned units = strlen(format) * 2 + 2;
    unsigned short* wide = static_cast<unsigned short*>(__builtin_alloca((units * 2 + 15) & ~15u));
    str8to16__8VIStringPUsUiPCc(wide, units, format);
    vsnprintf16__8VIStringPUsiPCUsPv(buffer, size, wide, args);
}
}

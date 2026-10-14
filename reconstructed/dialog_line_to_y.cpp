// Independently authored from the hash-pinned Norrath ELF behavior.
// Original symbol: lineToY__Fi, address 0x001d3990, complete extent 20 bytes.
// Observed dialog callers pass 1 or 6 and use the result for font positioning.
// Arithmetic wraps at 32 bits; the recorded -fwrapv option is required.
// This C entry point is a comparison adapter, not whole-program ABI proof.
static_assert(sizeof(int) == 4, "The comparison requires a 32-bit int.");

extern "C" int norrath_dialog_line_to_y(int line) {
    return line * 18 + 295;
}

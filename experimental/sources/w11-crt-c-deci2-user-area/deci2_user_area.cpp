/* Observed: the symbol table names a file-local .bss object userarea at
 * 0x008e4748; sceDeci2Open passes its address (with bit 29 set) as the fourth
 * word of the open request. The symbol has no recorded size. The next symbol
 * (EER00) is at 0x008e5780, so the span is at most 4152 bytes. The symbol is
 * .bss; the object starts 8-byte aligned (not 16).
 * Unknown: how much of the span the kernel really uses.
 */
extern "C" {
alignas(8) unsigned char norrath_deci2_user_area_view[4152];
}

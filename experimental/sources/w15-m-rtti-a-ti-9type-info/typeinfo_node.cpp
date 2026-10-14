// Observed __ti9type_info at 0x00486de0, 8 bytes of zero-initialized (bss) type-info node storage.
// It is the base node for classes registered against type_info, for example by __tf19__builtin_type_info.
// Inferred from the registration calls that name it as their base. Unknown: the node fields after the first word,
// and the getter __tf9type_info, which is not in this unit.
extern "C" {
unsigned char __ti9type_info[8] __attribute__((aligned(4))) = {};
}

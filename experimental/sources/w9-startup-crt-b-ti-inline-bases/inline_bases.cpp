// Observed type_info nodes that are only ever initialized inline, by the
// accessor of a derived class, and have no accessor function of their own.
// Each is 12 bytes of zero-initialized storage (the image places them in BSS):
//   __ti4Prop          0x008eadb8  base of the 6PhProp accessor
//   __ti9AutoChest     0x008eb4f8  base of 11AutoChestLP
//   __ti10AutoObject   0x008eb4e8  base of 9AutoChest and 13AutoContainer
//   __ti13AutoContainer 0x008eb508 base of 15AutoContainerLP
// Initialization is by __tf6PhProp, __tf11AutoChestLP and __tf15AutoContainerLP
// (authored beside this unit). Inferred: no other accessor exists for these
// four because none of the image's function symbols is named for them.
// Unknown: whether other code reads the nodes.
extern "C" {
unsigned char __ti4Prop[12] __attribute__((aligned(4)));
unsigned char __ti9AutoChest[12] __attribute__((aligned(4)));
unsigned char __ti10AutoObject[12] __attribute__((aligned(4)));
unsigned char __ti13AutoContainer[12] __attribute__((aligned(4)));
}

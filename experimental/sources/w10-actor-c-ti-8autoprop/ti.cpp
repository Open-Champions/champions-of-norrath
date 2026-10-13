// Observed __ti8AutoProp at 0x008eb518, 12 bytes of zero-initialized storage (BSS).
// The image has an accessor __tf8AutoProp (weak, 0x00314ef0) that fills it; that accessor is
// not part of this unit. The node is also filled inline by the 10AutoPropLP accessor.
// Unknown: whether other code reads the node.
extern "C" {
unsigned char __ti8AutoProp[12] __attribute__((aligned(4)));
}

// Observed _3map$iconList at 0x006023c8 (.bss), 1344 bytes. The span to the next symbol (skillWizardBeam) is 1344 bytes. The authored map-add-icon routine indexes it in 28-byte records (48 records fit).
// Inferred: A static member list of map icons whose fill level is _3map$iconListCount.
// Unknown: The record fields beyond what map-add-icon writes, and the capacity limit (the image has no check).
extern "C" {
unsigned char _3map$iconList[1344];
}

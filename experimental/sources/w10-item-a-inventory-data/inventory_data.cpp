// Observed small inventory-related data objects in the image, defined here with
// their observed sizes and start values (the image keeps all of them in initialized
// data, here the zero ones may be placed in bss by the compiler):
//   armorDebug          0x00483f40, 4 bytes, signed bytes, start 0 (flags read by
//                       the default-equipment setup; armorDebug[3] is the fourth)
//   buySellVertSpacing  0x00484920, one int, start 34 (0x22); multiplied by a row
//                       count in the shop/inventory exit layout
//   inventoryDrawDelay  0x00484830, one int, start 0 (a unit-local label in the
//                       image; counted down by the per-frame inventory run)
//   healthPotionsLeft   0x0034b8a8, four ints, start 0 (one per player)
//   manaPotionsLeft     0x0034b8b8, four ints, start 0 (one per player)
//   goldAnm             0x00600d20, 16 bytes of bss, the first of the gold
//                       resource-map labels (goldModel and goldTex follow it at
//                       16-byte steps); its layout is unknown
// Unknown: whether inventoryDrawDelay is a static member or a file-local, and the
// resource-map layout of goldAnm.
extern "C" {
signed char armorDebug[4];
int buySellVertSpacing = 34;
int inventoryDrawDelay;
int healthPotionsLeft[4];
int manaPotionsLeft[4];
unsigned char goldAnm[16];
}

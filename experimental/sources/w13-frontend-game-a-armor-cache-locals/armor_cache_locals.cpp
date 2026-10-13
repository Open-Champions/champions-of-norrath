// Observed file-local data of the armor-cache allocator (playerAllocateArmorCache__Fii):
//  equipmentSlot2Cache.1137 at 0x00422a38, 72 bytes: eighteen ints, indexed by an item's equipment
//  kind (the signed byte at item-row offset 28): 7 8 6 9 0, then -1 four times, 2 1 3 4 5, then
//  -1 four times. The value is a row (0 to 9) of the armor cache, or -1 for none.
//  armorName.1138 at 0x00422a80, 40 bytes: ten pointers to the labels given to the ten cache
//  allocations, in cache row order.
// Section placement follows the build profile, not the original .data split. Names that carry a
// dot in the image are given through assembler labels.
extern "C" {
int norrath_equipment_slot_to_cache[18] __asm__("equipmentSlot2Cache.1137") = {7, 8, 6, 9, 0, -1, -1, -1, -1, 2, 1, 3, 4, 5, -1, -1, -1, -1};
const char* norrath_armor_name[10] __asm__("armorName.1138") = {
    "armor(body)",  "armor(boots)",     "armor(gloves)",     "armor(helmet)",     "armor(leggings)",
    "armor(quiver)", "armor(lefthand)", "armor(righthand)", "armor(righthand2)", "armor(shield)"};
}

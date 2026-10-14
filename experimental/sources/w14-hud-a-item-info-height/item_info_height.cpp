// Observed: itemInfoHeight is the 16-byte bss object at 0x007d03d8 (the next
// symbol starts 16 bytes later). The authored shop info-box code reads and adds
// to one int per player index. Inferred: four ints. Unknown: units.
extern "C" int itemInfoHeight[4] = {};
static_assert(sizeof(itemInfoHeight) == 16, "itemInfoHeight");

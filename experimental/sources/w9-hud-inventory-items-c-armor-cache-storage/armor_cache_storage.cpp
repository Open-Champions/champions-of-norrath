// Observed table at 0x007cb770, 3520 bytes of .bss (the gap to the next symbol,
// cacheLoadName at 0x007cc530). The image lists only a local symbol `cache` there.
// playerDeallocateArmorCaches and preCacheArmor index it in rows of 88 bytes:
// element = base + 352 * slot + 88 * player index. The name below is authored here.
// Unknown: the layout of an element.
extern "C" {
unsigned char norrath_player_armor_cache_view_7cb770[3520];
}

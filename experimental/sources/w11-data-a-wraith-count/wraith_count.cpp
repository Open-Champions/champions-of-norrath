// _6Wraith$s_wraithCount at 0x004852a8, 4 bytes, global .data symbol (static member s_wraithCount of
// Wraith). The file image holds 0. wraithIsEnemy walks that many slots of the Wraith list, so the
// word is a signed count. The reviewed -G0 profile places a zero object in BSS.
extern "C" {
int _6Wraith$s_wraithCount;
}
static_assert(sizeof(_6Wraith$s_wraithCount) == 4u, "Wraith s_wraithCount extent");

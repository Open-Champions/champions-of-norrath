// objectHash at 0x00518ac0, 2048 bytes, to _args.
// 512 words. BSS is zero. objectInit clears them later.
// object-infra publishes the same bytes as norrath_constructor_string_objects_view.
// This is the original name, so the link has two objects.
extern "C" {
unsigned objectHash[512];
}
static_assert(sizeof objectHash == 2048u, "objectHash extent");

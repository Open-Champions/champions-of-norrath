// stringHash at 0x005182c0, 2048 bytes, to objectHash.
// 512 words. BSS is zero. objectInit clears them later.
// object-infra publishes the same bytes as norrath_constructor_string_names_view.
// This is the original name, so the link has two objects.
extern "C" {
unsigned stringHash[512];
}
static_assert(sizeof stringHash == 2048u, "stringHash extent");

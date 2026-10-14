// offTheGrid at 0x00486890, four .sbss bytes, next userCreateByNameFunction.
// The image value is zero. objectInit stores zero here later.
// Another local offTheGrid at 0x00486848 is a different object and is not
// this word. object-infra publishes 0x00486890 as
// norrath_constructor_default_head_view. This is the original name.
extern "C" {
unsigned offTheGrid;
}
static_assert(sizeof offTheGrid == 4u, "offTheGrid width");

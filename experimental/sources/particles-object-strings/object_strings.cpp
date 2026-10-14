// objectStrings at 0x00486898, four .sbss bytes, next poolStart.
// The image value is zero. objectInit stores zero here later.
// object-infra publishes the same address as
// norrath_constructor_string_count_view. This is the original name.
extern "C" {
unsigned objectStrings;
}
static_assert(sizeof objectStrings == 4u, "objectStrings width");

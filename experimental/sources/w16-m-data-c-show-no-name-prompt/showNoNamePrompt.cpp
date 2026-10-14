// showNoNamePrompt: zeroed storage for an unresolved import. Closes the link name and extent only.
// Observed (source level):
// - Reference image: a bss byte at 0x00486d2f (reviewed EE nm; the symbol has no size in the table).
// - Callers: w15-gameloop-a-do-name-pad/do_name_pad.cpp and w15-gameloop-a-switch-to-name-pad/switch_to_name_pad.cpp
//   declare it as unsigned char and clear it.
// Inferred:
// - One byte, from the declared type.
// Unknown:
// - Its value before the first write. This unit supplies zero.
// This unit does not establish any field value.
extern "C" {
unsigned char showNoNamePrompt = 0;
}
static_assert(sizeof showNoNamePrompt == 1u, "showNoNamePrompt extent");

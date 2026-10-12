// Observed .sdata symbol syncRandState at 0x483e5c, four file-backed zero bytes.
// FireBomb reads this word, stores state * 0x00019660d + 0x3c6ef35f, and
// shifts the updated word right 16. The next symbol, asyncRandState, starts
// four bytes later.
// object-infra publishes these same bytes as norrath_constructor_random_state_view.
// natives-a references the ELF name syncRandState. This unit defines only the
// actor-view name and does not alias either of those symbols.
// Unknown: whether the three names are required to be one object.
extern "C" {
unsigned norrath_actors_a_view_483e5c = 0u;
}

// Observed `context` at 0x004840f0, a 4-byte initialized word in small data.
// Its initial value is 0x004840e8, the address of the text "none" stored in
// a padded block starting at 0x004840e8 (the object at 0x004840e0 is an
// unrelated table). The assertion routine reads the word and prints it with
// %s, so it is a text pointer.
// Inferred: the word is meant to be updated by other code to name what the
// game was doing; no writer is claimed here. Unknown: all writers.
static const char kNorrathAssertNoneText[] = "none";

extern "C" {
const char* norrath_assert_context_word_view = kNorrathAssertNoneText;
}

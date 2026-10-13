// Observed .data symbol _6Wraith$s_rWraithList at 0x42bd18, 240 bytes, all zero.
// Observed: the Wraith constructor links an object at this address plus four
// times a count held in another word, so the view is read as 60 words.
// Unknown: the element type and what the count is bounded by. The reviewed
// profile places these zeros in .bss, not .data.
extern "C" {
unsigned norrath_ctors_lava_wraith_view_42bd18[60];
}

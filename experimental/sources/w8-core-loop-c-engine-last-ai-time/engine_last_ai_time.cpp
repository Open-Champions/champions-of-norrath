// engineLastAITime at 0x004843e8, 4 bytes, in SLUS_205.65.
// Observed: zero-initial word in .data. Another unit stores a timer 0 difference here. Unknown: the unit of that value.
extern "C" {
int engineLastAITime = 0;
}

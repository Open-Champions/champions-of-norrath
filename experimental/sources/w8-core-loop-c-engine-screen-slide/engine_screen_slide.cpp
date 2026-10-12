// engineScreenSlide at 0x00484410, 4 bytes, in SLUS_205.65.
// Observed: zero-initial word in .data. Unknown: its use beyond being read for the Y offset.
extern "C" {
int engineScreenSlide = 0;
}

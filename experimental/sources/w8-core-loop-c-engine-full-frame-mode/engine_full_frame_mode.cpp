// engineFullFrameMode at 0x00484474, 4 bytes, in SLUS_205.65.
// Observed: zero-initial word in .data. Authored code elsewhere stores 40 and other values here. Unknown: what the values select.
extern "C" {
int engineFullFrameMode = 0;
}

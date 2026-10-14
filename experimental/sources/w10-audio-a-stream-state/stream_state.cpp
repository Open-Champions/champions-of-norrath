// Observed: stream globals with the image's initial values.
//   stream 0x007dd290 (.bss; three records of 452 bytes = 1356, the symbol after it starts 1360
//   bytes on), streamVolume 0x0042de48 (three floats, all 1.0), streaming 0x00483ffd (1 byte, 0),
//   streamOutput 0x00484008 (4 bytes, 0), streamSizeLeft 0x0048400c (4 bytes, 0),
//   streamPaused 0x00484010 (1 byte, 0).
// Inferred: nothing.
// Unknown: the pointee type of streamOutput (the cd stream code treats it as a byte cursor), and
// the four bytes between the end of `stream` and its neighbour.
extern "C" {
alignas(16) unsigned char stream[3 * 452];
float streamVolume[3] = { 1.0f, 1.0f, 1.0f };
unsigned char streaming = 0;
char* streamOutput = 0;
int streamSizeLeft = 0;
unsigned char streamPaused = 0;
}

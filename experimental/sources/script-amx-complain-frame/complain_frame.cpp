// Observed complainFrame.672, four .sdata bytes at 0x0048508c.
// The file bytes are f0 d8 ff ff, which is signed -10000.
extern "C" {
unsigned char norrath_script_complain_frame_word_view[4] = {
    0xf0, 0xd8, 0xff, 0xff
};
}

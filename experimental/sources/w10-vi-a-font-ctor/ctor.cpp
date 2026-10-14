// Observed: __6VIFont at 0x01012130 is 28 bytes and returns nothing. It stores
// float 1.0 (0x3f800000) at offsets 0, 4, 8 and 12 and nothing else.
// Unknown: what the four floats are (they could be a scale or a color) and the
// size of VIFont.
extern "C" {
void __6VIFont(void* object) {
    float* words = static_cast<float*>(object);
    words[3] = 1.0f;
    words[0] = 1.0f;
    words[1] = 1.0f;
    words[2] = 1.0f;
}
}

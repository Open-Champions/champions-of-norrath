// Observed msg_run__7Lantern at 0x00231160, 220 bytes.
// Takes n = ef1Noise(0x7efe41 + object * 1664525 + (frameNm << 4)), with the object address itself
// (32-bit wrapping) as one term. With s = (float)((int)(n * 22.0f) + 160), stores s at the Light
// member (object+76) offsets 28 and 36, and 1.0f / (s * s) at offsets 40 and 32. When strstr finds
// the 4-character string "mine" in the text at levelName+81, stores zero at the word at object+112.
// Then, when the signed byte at object+149 is 31 or more, removes the object from the run list and
// clears the byte at object+148. Finally the byte at object+149 is increased by one (modulo 256).
// Inferred: the Light member holds the flicker radius terms; byte 149 counts frames since the last
// draw. Unknown: what "mine" tests for in the level name and what the word at +112 means.
// Nothing here ran on the EE.
extern "C" {
extern int frameNm;
extern char levelName[];
float ef1Noise__Fi(int seed);
char* strstr(const char* haystack, const char* needle);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

extern "C" void msg_run__7Lantern(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const float noise = ef1Noise__Fi(0x7efe41 + static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(self)) * 0x19660d + (frameNm << 4));
    const float size = static_cast<float>(static_cast<int>(noise * 22.0f) + 160);
    float* light = reinterpret_cast<float*>(object + 76);
    light[7] = size;
    light[9] = size;
    const float inverse = 1.0f / (size * size);
    light[10] = inverse;
    light[8] = inverse;
    if (strstr(levelName + 81, "mine") != nullptr) *reinterpret_cast<unsigned*>(object + 112) = 0u;
    if (static_cast<signed char>(object[149]) >= 31) {
        objectRemoveFromRunList__FP10GameObject(object);
        object[148] = 0;
    }
    object[149] = static_cast<unsigned char>(object[149] + 1);
}

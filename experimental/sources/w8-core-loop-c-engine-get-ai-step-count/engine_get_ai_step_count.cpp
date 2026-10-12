// engineGetAIStepCount at 0x001913d8, 84 bytes, in SLUS_205.65.
// Observed: reads the word trippleBufferMode (0x004843e0). When it is zero the candidate is the
// word lastFrameVBlankCount (0x00486a38). Otherwise the candidate is the signed byte
// nextFrameDrop (0x004843f0) plus one. The result is the candidate limited to 1..2, signed.
// Inferred: the value is a number of AI steps for the frame. Unknown: the meaning of the
// two inputs beyond their use here, and whether nextFrameDrop is signed in the source (the
// image loads it with a signed byte load).
extern "C" int trippleBufferMode;
extern "C" signed char nextFrameDrop;
extern "C" int lastFrameVBlankCount;

extern "C" int engineGetAIStepCount__Fv() {
    int steps;
    if (trippleBufferMode == 0)
        steps = lastFrameVBlankCount;
    else
        steps = nextFrameDrop + 1;
    if (steps < 1) steps = 1;
    if (steps > 2) steps = 2;
    return steps;
}

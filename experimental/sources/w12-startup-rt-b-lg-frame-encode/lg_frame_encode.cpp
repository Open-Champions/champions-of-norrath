// Observed lgFrameEncode at 0x002eb2b8, 484 bytes, eleven arguments (eight in registers, three on the
// stack). It slices a run of 16-bit samples into fixed-size frames, runs a per-frame callback on each
// full frame and appends the callback's results to an output buffer. Seen from the one caller,
// lgCodecGSMEncode (frame buffer = object + 28, 320 bytes; fill counter = object + 348; 160 samples
// per frame; 33 output bytes per frame; callback gsm_frame_encode; object passed through).
//
// Arguments, in order: samples, sample byte count, output buffer, pointer to the output capacity in
// bytes (in) and the bytes produced (out), frame buffer, size of the frame buffer in bytes, pointer to
// the count of samples already in the frame buffer, samples per frame, output bytes per frame,
// callback fn(frame buffer, output position, object), object.
//
// Observed behaviour:
//  * Input or output pointer null, or an odd byte count: *capacity is set to 0 and 0x80000004 is
//    returned. A zero byte count or a zero capacity: *capacity = 0, result 0.
//  * Otherwise frames = (count / 2) / samplesPerFrame, plus one when the fill counter is non-zero and
//    capacity % outputBytesPerFrame is non-zero (the original tests the capacity here, not the sample
//    count; kept as observed). If capacity < frames * outputBytesPerFrame, printf of the "bytes provided
//    / Need" diagnostic, *capacity = 0 and 0x80000004.
//  * The output buffer is cleared for `capacity` bytes and *capacity set to 0. Then, until the samples
//    are used up: min(samplesPerFrame - fill, samples left) samples are copied to the frame buffer at
//    sample offset fill, fill grows; when it reaches samplesPerFrame the callback runs with the output
//    position (advanced afterwards by outputBytesPerFrame), the fill returns to 0, the callback result
//    is added to the produced total and the frame buffer is cleared for frameBufferBytes.
//  * On exit *capacity holds the produced total (the sum of callback results) and 0 is returned.
// Inferred only: sample/frame naming from the 160-sample, 33-byte values of the single caller.
// The remaining-sample count is checked after the loop body, so a copy of zero samples ends the loop.
// Nothing here was run on an EE.
extern "C" {
int min(int a, int b);
void* memset(void* dst, int value, unsigned count);
void* memcpy(void* dst, const void* src, unsigned count);
int printf(const char* format, ...);

int lgFrameEncode(const short* samples, int count, unsigned char* out, int* capacity, short* frame,
                  int frameBufferBytes, int* fill, int samplesPerFrame, int outputBytesPerFrame,
                  int (*fn)(void*, void*, void*), void* object) {
    int produced = 0;
    if (!samples || !out || (count & 1)) {
        *capacity = 0;
        return 0x80000004;
    }
    if (count == 0) {
        *capacity = 0;
        return 0;
    }
    int cap = *capacity;
    if (cap == 0) {
        *capacity = 0;
        return 0;
    }
    int remaining = count / 2;
    int frames = remaining / samplesPerFrame;
    if (*fill != 0 && cap % outputBytesPerFrame != 0) frames = frames + 1;
    int needed = frames * outputBytesPerFrame;
    if (cap < needed) {
        printf("ERROR: bytes provided = %d. Need=%d\n", cap, needed);
        *capacity = 0;
        return 0x80000004;
    }
    memset(out, 0, cap);
    *capacity = 0;
    for (;;) {
        int n = min(samplesPerFrame - *fill, remaining);
        memcpy(frame + *fill, samples, n * 2);
        samples += n;
        remaining -= n;
        *fill += n;
        if (*fill != samplesPerFrame) {
            *capacity = produced;
            return 0;
        }
        unsigned char* position = out;
        out += outputBytesPerFrame;
        int result = fn(frame, position, object);
        *fill = 0;
        produced += result;
        memset(frame, 0, frameBufferBytes);
    }
}
}

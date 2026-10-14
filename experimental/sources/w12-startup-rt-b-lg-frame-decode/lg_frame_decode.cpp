// Observed lgFrameDecode at 0x002eb4a0, 464 bytes, eleven arguments (seven used in registers, three
// on the stack; the sixth register argument is never read). The input-to-output twin of lgFrameEncode:
// it gathers encoded frames from a byte stream, runs a per-frame callback on each complete frame and
// leaves the decoded samples in an output buffer. Seen from the one caller, lgCodecGSMDecode.
//
// Arguments, in order: input bytes, input byte count, output buffer, pointer to the output capacity
// in bytes (in) and the bytes produced (out), frame buffer, an argument that is not used, pointer to
// the count of bytes already in the frame buffer, samples per frame, input bytes per frame, callback
// fn(frame buffer, output position, object), object.
//
// Observed behaviour:
//  * Input or output null, or an odd capacity: *capacity = 0 and 0x80000004. A zero input count, a
//    zero capacity, or fewer input bytes than one whole frame (count / bytesPerFrame <= 0):
//    *capacity = 0 and 0.
//  * Otherwise the input actually consumed is limited to
//      min(2 * min(capacity / (2 * samplesPerFrame), count / bytesPerFrame) * samplesPerFrame, count)
//    (the original mixes output and input units here; kept as observed), the output is cleared for
//    `capacity` bytes and *capacity set to 0.
//  * Loop: min(bytesPerFrame - fill, input left) bytes are appended to the frame buffer at offset
//    fill; fill grows. When fill is short of bytesPerFrame, or the remaining capacity is smaller than
//    2 * samplesPerFrame bytes, the loop stops. Otherwise the callback runs with the current output
//    position, which then advances by 2 * samplesPerFrame bytes; the running sample total grows by
//    samplesPerFrame, the input left drops by bytesPerFrame, fill returns to 0, and the remaining
//    capacity drops by 2 * total (the running total, not the frame, as observed).
//  * On exit *capacity = 2 * total and 0 is returned. The callback result is ignored.
// Inferred only: the byte/sample naming from the values of the single caller.
// Nothing here was run on an EE.
extern "C" {
int min(int a, int b);
void* memset(void* dst, int value, unsigned count);
void* memcpy(void* dst, const void* src, unsigned count);

int lgFrameDecode(const unsigned char* in, int count, unsigned char* out, int* capacity, unsigned char* frame,
                  int unused, int* fill, int samplesPerFrame, int bytesPerFrame,
                  int (*fn)(void*, void*, void*), void* object) {
    int total = 0;
    if (!in || !out || (*capacity & 1)) {
        *capacity = 0;
        return 0x80000004;
    }
    int cap = *capacity;
    if (count == 0 || cap == 0) {
        *capacity = 0;
        return 0;
    }
    unsigned remainingCap = cap;
    int left = count;
    int frames = left / bytesPerFrame;
    if (frames <= 0) {
        *capacity = 0;
        return 0;
    }
    unsigned frameOutBytes = samplesPerFrame * 2;
    frames = min(cap / (int)frameOutBytes, frames);
    left = min(frames * samplesPerFrame * 2, left);
    memset(out, 0, *capacity);
    *capacity = 0;
    for (;;) {
        int n = min(bytesPerFrame - *fill, left);
        memcpy(frame + *fill, in, n);
        in += n;
        unsigned char* position = out;
        out += frameOutBytes;
        *fill += n;
        if (*fill != bytesPerFrame || remainingCap < frameOutBytes) {
            *capacity = total * 2;
            return 0;
        }
        total += samplesPerFrame;
        left -= bytesPerFrame;
        fn(frame, position, object);
        *fill = 0;
        remainingCap -= total * 2;
    }
}
}

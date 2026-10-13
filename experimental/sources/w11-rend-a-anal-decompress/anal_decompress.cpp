#include "../w11-rend-a-common/rend.hpp"
// Observed analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii at
// 0x00133f60, 1100 bytes. Arguments: the CDecompress object (a0, not read), a bit reader, an
// analState, an output byte pointer, and two ints named here count (t0) and skip (t1). It is the
// counterpart of analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci.
//
// Bit reader (observed): the word at +0 is a pointer to 16-bit little-endian units, the word at
// +4 a bit position. read(n) takes the 32-bit value (unit[pos >> 4] << 16 | unit[(pos >> 4) + 1]),
// shifts it left by pos & 15, keeps the top n bits (logical shift right by 32 - n) and then adds n
// to the position.
// analState (observed use): word +0 index 0..8, word +4 level 0..255, float +8, float +12.
//
// With total = count + skip a scratch buffer of ((total + 15) >> 4) << 4 bytes is used on the
// stack (not cleared). Three passes:
//  1. Fill buf[0..total). State mode (0 or non-zero) and flag. In mode 0: n = read(5); n zero bytes
//     are written (no bounds test); stop when the index equals total; with n != 31 a byte 1 is
//     written and mode becomes 1 with flag 0 (n == 31 leaves mode 0). In non-zero mode: bit = read(1)
//     is written as the byte, mode increases by one, and a set bit sets flag; when mode reaches 6
//     (after five bits) mode returns to 1 if flag is set, else 0, and flag is cleared. Repeats while
//     the index is below total.
//  2. Sign pass over the bytes whose bit 0 is set ("marked"). Starting at the first marked byte,
//     sign = read(1); then repeat while the index is below total: c = read(3); c times, OR (sign << 1)
//     into the current byte, step the index to the next marked byte (or past total); then if c != 7
//     sign flips. (With the index past total the OR still runs and goes past total: the original does
//     not check; the scratch size rounds up to 16, so a few bytes of slack exist.)
//  3. For indices skip .. total-1 (nothing at all when count <= 0): marker = byte & 1, negative =
//     (byte >> 1) & 1, step = stepTable[state index] (nine halfwords 4, 6, 8, 12, 16, 24, 32, 48, 64),
//     level = state level + or - marker * step clamped to 0..255; the index becomes index + (marker ?
//     2 : -1) clamped to 0..8; both go back to the state. With target = (float)level and the two
//     float constants c0 = 0x3e4ccccc (0.2 truncated) and c1 = 0x3d4ccccc (0.05 truncated), k = 1.0f -
//     sqrt(c0) (the image runs sqrt.s on the constant for every sample, and so does this source: the
//     constant is hidden from the compiler so that it is not folded with different rounding), the
//     pair (position = float +8, velocity = float +12) is advanced ten times:
//     position += velocity; velocity = (velocity + (target - position) * c1) * k.  The output byte is
//     (int)position (truncating) clamped to 0..255.
// Nothing is returned.
// Inferred: the stream is an adaptive-step coder for 8-bit samples smoothed by a second-order low
// pass. Unknown: what the callers decode (run_prediction decodes four states), the meaning of the
// sign-flip codes, and the stream format beyond the reads described. The stepChange table in the
// image holds only {-1, 2}, used here as the marker 0/1 deltas. Nothing here was executed on the EE.
extern "C" {
void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(
    void* decompression, unsigned char* reader, unsigned char* state, unsigned char* output, int count,
    int skip) {
    using namespace rend;
    (void)decompression;
    static const short stepTable[9] = {4, 6, 8, 12, 16, 24, 32, 48, 64};
    static const short stepChange[2] = {-1, 2};
    const int total = count + skip;
    const int size = total > 0 ? ((total + 15) >> 4) << 4 : 16;
    unsigned char buf[size];
    struct Reader {
        unsigned char* r;
        unsigned take(int n) {
            const int pos = at<i32>(r, 4);
            const unsigned char* units = ptr(r, 0) + (pos >> 4) * 2;
            unsigned word = (static_cast<unsigned>(at<u16>(units, 0)) << 16) | at<u16>(units, 2);
            word <<= (pos & 15);
            at<i32>(r, 4) = pos + n;
            return word >> (32 - n);
        }
    } in = {reader};
    int mode = 0;
    int flag = 0;
    int i = 0;
    while (i < total) {
        if (mode != 0) {
            const unsigned bit = in.take(1);
            ++mode;
            buf[i++] = static_cast<unsigned char>(bit);
            if (bit != 0) flag = 1;
            if (mode == 6) {
                mode = flag != 0 ? 1 : 0;
                flag = 0;
            }
        } else {
            const int n = static_cast<int>(in.take(5));
            for (int k = 0; k < n; ++k) buf[i++] = 0;
            if (i == total) break;
            if (n == 31) continue;
            buf[i++] = 1;
            mode = 1;
            flag = 0;
        }
    }
    if (total > 0) {
        i = 0;
        bool reached = true;
        while ((buf[i] & 1) == 0) {
            ++i;
            if (i >= total) {
                reached = false;
                break;
            }
        }
        if (reached) {
            unsigned sign = in.take(1);
            do {
                const unsigned c = in.take(3);
                if (c != 0) {
                    const unsigned char mask = static_cast<unsigned char>(sign << 1);
                    unsigned left = c;
                    do {
                        buf[i] = static_cast<unsigned char>(buf[i] | mask);
                        ++i;
                        if (i < total && (buf[i] & 1) == 0) {
                            do {
                                ++i;
                                if (i >= total) break;
                            } while ((buf[i] & 1) == 0);
                        }
                        --left;
                    } while (left != 0);
                }
                if (c != 7) sign = sign == 0 ? 1 : 0;
            } while (i < total);
        }
    }
    if (!(skip < count + skip)) return;
    for (int j = skip; j < total; ++j) {
        const unsigned e = buf[j];
        const int marker = static_cast<int>(e & 1);
        const int negative = static_cast<int>((e >> 1) & 1);
        const int index = at<i32>(state, 0);
        const int step = stepTable[index];
        int level = negative != 0 ? at<i32>(state, 4) - marker * step : at<i32>(state, 4) + marker * step;
        if (255 < level) level = 255;
        if (level < 0) level = 0;
        int next = index + stepChange[marker];
        if (next < 0) next = 0;
        if (8 < next) next = 8;
        at<i32>(state, 4) = level;
        at<i32>(state, 0) = next;
        float c0 = 0x1.999998p-3f;
#ifndef NORRATH_HOST_ARENA
        __asm__("" : "+f"(c0));
#endif
        const float k = 1.0f - __builtin_sqrtf(c0);
        const float c1 = 0x1.999998p-5f;
        const float target = static_cast<float>(level);
        for (int pass = 0; pass != 10; ++pass) {
            const float velocity = at<f32>(state, 12);
            const float position = at<f32>(state, 8) + velocity;
            at<f32>(state, 8) = position;
            at<f32>(state, 12) = (velocity + (target - position) * c1) * k;
        }
        int value = static_cast<int>(at<f32>(state, 8));
        if (!(0 < value)) value = 0;
        if (!(value < 255)) value = 255;
        *output++ = static_cast<unsigned char>(value);
    }
}
}

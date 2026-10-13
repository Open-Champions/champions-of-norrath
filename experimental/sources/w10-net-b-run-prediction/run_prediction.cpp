// Observed runPrediction__Fi at 0x0012a248, 3920 bytes in the pinned
// SLUS_205.65 image; the int argument is not read, no result. Offsets named
// "record" are relative to a partner record in `partners` (stride 17632) or a
// player record in `player` (stride 23984), as stated.
//  1. When localPredictedPlayer is null: Update__6Camera(gameCam) is called
//     once for each step of engineGetAIStepCount() (reread every pass), and the
//     function returns.
//  2. Otherwise local = netGetLocalPlayerNm(0), objectIgnoreFlags = 0x200000.
//     When frameNm is below localNetFrameNm, the word at player record
//     (local) + 21872 is remembered and, for each frame from frameNm up to
//     localNetFrameNm (the end test rereads localNetFrameNm):
//       a. netPrediction and netLogDisable are set to 1 and the 24-byte ring
//          entry (frame & 255) of the partner record `local` (ring at record
//          + 172) is copied; bytes 4..15 of it are the frame data.
//       b. The frame data is encoded with the same bit-stream layout as the
//          send path of netGameStep (16-bit button halfword, run/bit-index
//          codes, four analCompress calls over bytes 2..5, the yaw coder, the
//          yaw-bit runs, the marker 0xa5a5) into a 512-byte buffer using the
//          global compImmediateCompressor (word +72 counts frames), and then
//          decoded again from bit 0 with analDecompress and the global
//          compImmediateDecompressor, so the frame the game steps with is the
//          one the remote peers would see. The decoder mirrors the encoder: the
//          button halfword toggles one bit per code, the four byte columns come
//          from analDecompress (states +32, +48, +0, +16), the yaw word comes
//          from run counts, 1-bit/16-bit absolute values or 1-bit/9-bit signed
//          deltas on the halfwords +66/+68, the yaw bit toggles per run, the
//          marker is skipped (16 bits) and word +72 of the decompressor counts.
//       c. padProcessData(pad + 88 * local, frame data, localPlayerPadState)
//          runs; the low halfword of the decoded yaw word is stored in
//          netCameraYaw[local]; the player record word +21872, if nonzero, is
//          set to localPredictedPlayer; syncRandState and the floats at player
//          record +172 and +196 are saved around the step.
//       d. netPredictionFastForward is set when frameNm is below
//          localNetFrameNm - engineGetAIStepCount(), and then bit 0x04000000
//          of the word at +12 of localPredictedPlayer is set (otherwise
//          cleared). The object's virtual entry at (table at +72) + 32 is
//          called with the object adjusted by its halfword at +0; when
//          netPredictionFastForward is zero, Update__6Camera(gameCam) follows.
//          The saved floats and syncRandState are restored, frameNm becomes
//          frameNm + 1 (reread) and the remembered word is written back.
//     After the loop netLogDisable and netPrediction are cleared.
//  3. If the word at player record (local) + 21872 and localPredictedPlayer are
//     both nonzero, 16 bytes at +140 and 24 bytes at +856 are copied from the
//     former to the latter.
// Inferred: this replays unacknowledged local input through the same
// compression the network path applies, to keep the local prediction in step
// with what peers see. The name of the virtual entry (step/update) is unknown.
// Unknown: the meaning of the player record words (+140..+155, +172, +196,
// +856..+879, +21872), and the content of the 512-byte buffer past the bits
// used (stack storage, only the first unit is cleared as in the image).
typedef int __attribute__((may_alias)) Int;
typedef unsigned short __attribute__((may_alias)) Half;
typedef float __attribute__((may_alias)) Float;

struct Camera;

extern "C" {
extern int localNetFrameNm;
extern int frameNm;
extern int syncRandState;
extern int objectIgnoreFlags;
extern unsigned char* localPredictedPlayer;
extern signed char netPrediction;
extern signed char netLogDisable;
extern signed char netPredictionFastForward;
extern unsigned char partners[];
extern unsigned char player[];
extern unsigned char pad[];
extern unsigned char localPlayerPadState[];
extern unsigned char netCameraYaw[];
extern unsigned char compImmediateCompressor[];
extern unsigned char compImmediateDecompressor[];
extern Camera* gameCam;

int netGetLocalPlayerNm__Fi(int index);
int engineGetAIStepCount__Fv();
void Update__6Camera(Camera* camera);
void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void* pad_record, void* data, void* state);
void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(void* compression, void* writer,
                                                                          void* state, unsigned char* bytes,
                                                                          int count);
void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(void* decompression, void* reader,
                                                                                    void* state, unsigned char* bytes,
                                                                                    int count, int flag);
}

namespace {
struct BitWriter {
    unsigned short* data;
    int position;
};

struct BitReader {
    unsigned short* data;
    int position;
};

struct Entry {
    unsigned char bytes[24];
};

inline Int& word(unsigned char* base, int offset) { return *reinterpret_cast<Int*>(base + offset); }

inline void put(BitWriter* writer, unsigned value, int width) {
    unsigned window = (value & ((1u << width) - 1u)) << (16 - width);
    int position = writer->position;
    unsigned short* unit = writer->data + (position >> 4);
    int shift = position & 15;
    unsigned short old = unit[0];
    writer->position = position + width;
    unit[0] = static_cast<unsigned short>(old | (window >> shift));
    unit[1] = static_cast<unsigned short>(window << (16 - shift));
}

// The 32 bits starting at the reader position, left aligned.
inline unsigned window(const BitReader* reader) {
    int position = reader->position;
    const unsigned short* unit = reader->data + (position >> 4);
    return ((static_cast<unsigned>(unit[0]) << 16) | unit[1]) << (position & 15);
}

inline unsigned get(BitReader* reader, int width) {
    unsigned value = window(reader) >> (32 - width);
    reader->position += width;
    return value;
}

inline int get_signed(BitReader* reader, int width) {
    int value = static_cast<int>(window(reader)) >> (32 - width);
    reader->position += width;
    return value;
}
}

extern "C" void runPrediction__Fi(int) {
    if (localPredictedPlayer == 0) {
        for (int i = 0; i < engineGetAIStepCount__Fv(); ++i) Update__6Camera(gameCam);
        return;
    }
    int local = netGetLocalPlayerNm__Fi(0);
    objectIgnoreFlags = 0x200000;
    int current = frameNm;
    unsigned char* record = player + local * 23984;
    int* remembered_slot = reinterpret_cast<int*>(record + 21872);
    int remembered = *remembered_slot;
    if (current < localNetFrameNm) {
        do {
            netPrediction = 1;
            netLogDisable = 1;
            Entry entry = *reinterpret_cast<const Entry*>(partners + local * 17632 + 172 + (current & 0xff) * 24);
            unsigned char* frame = entry.bytes + 4;
            unsigned short buffer[256];
            BitWriter writer;
            BitReader reader;
            buffer[0] = 0;
            buffer[1] = 0;
            writer.data = buffer;
            writer.position = 0;
            reader.data = buffer;
            reader.position = 0;
            unsigned char* compression = compImmediateCompressor;
            word(compression, 72) = word(compression, 72) + 1;
            put(&writer, *reinterpret_cast<Half*>(frame), 16);
            {
                unsigned previous = *reinterpret_cast<Half*>(frame);
                int run = 0;
                unsigned now = *reinterpret_cast<Half*>(frame);
                if (now == previous) {
                    ++run;
                } else {
                    do {
                        put(&writer, run & 31, 5);
                        run = 0;
                        int bit = 0;
                        while (bit < 16 && !(((now ^ previous) >> bit) & 1)) ++bit;
                        if (bit < 16) {
                            put(&writer, bit & 15, 4);
                            previous ^= 1u << bit;
                        }
                    } while (now != previous);
                    ++run;
                }
                if (run == 31) {
                    put(&writer, 31, 5);
                    run = 0;
                }
                if (run != 0) put(&writer, run & 31, 5);
            }
            static const int kColumn[4] = {2, 3, 4, 5};
            static const int kState[4] = {32, 48, 0, 16};
            for (int c = 0; c < 4; ++c) {
                unsigned char column[1];
                column[0] = frame[kColumn[c]];
                analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(
                    compression, &writer, compression + kState[c], column, 1);
            }
            {
                int run = 0;
                int yaw_in = word(frame, 8);
                int target = yaw_in & -2;
                int delta = target - static_cast<short>(*reinterpret_cast<Half*>(compression + 64));
                int magnitude = delta < 0 ? -delta : delta;
                *reinterpret_cast<Half*>(compression + 64) = static_cast<unsigned short>(target);
                bool escape = true;
                int second = 0;
                if (magnitude < 1001) {
                    second = target - static_cast<short>(*reinterpret_cast<Half*>(compression + 66));
                    int second_magnitude = second < 0 ? -second : second;
                    if (second_magnitude < 3001) escape = false;
                }
                if (escape) {
                    put(&writer, run & 31, 5);
                    put(&writer, 0, 1);
                    put(&writer, yaw_in & 0xfffe, 16);
                    run = 0;
                    *reinterpret_cast<Half*>(compression + 66) = static_cast<unsigned short>(target);
                } else {
                    int second_magnitude = second < 0 ? -second : second;
                    int smooth = delta;
                    if (!(second_magnitude < 201)) smooth = delta + second / 15;
                    int previous_step = static_cast<short>(*reinterpret_cast<Half*>(compression + 68));
                    int difference = smooth - previous_step;
                    if (difference < 0) difference = -difference;
                    unsigned short sum;
                    if (difference < 31 && previous_step * delta >= 0) {
                        ++run;
                        if (run == 31) {
                            put(&writer, 31, 5);
                            run = 0;
                        }
                        sum = static_cast<unsigned short>(*reinterpret_cast<Half*>(compression + 66) +
                                                          *reinterpret_cast<Half*>(compression + 68));
                    } else {
                        put(&writer, run & 31, 5);
                        put(&writer, 1, 1);
                        int clamped = smooth;
                        if (clamped < -255) clamped = -255;
                        if (clamped > 255) clamped = 255;
                        put(&writer, clamped & 0x1ff, 9);
                        run = 0;
                        *reinterpret_cast<Half*>(compression + 68) = static_cast<unsigned short>(clamped);
                        sum = static_cast<unsigned short>(*reinterpret_cast<Half*>(compression + 66) + clamped);
                    }
                    *reinterpret_cast<Half*>(compression + 66) = sum;
                }
                if (run != 0) put(&writer, run & 31, 5);
            }
            {
                int run = 0;
                int bit = word(frame, 8) & 1;
                put(&writer, bit, 1);
                if ((word(frame, 8) & 1) != bit) {
                    put(&writer, run & 31, 5);
                    run = 0;
                    bit = word(frame, 8) & 1;
                }
                ++run;
                if (run == 31) {
                    put(&writer, 31, 5);
                    run = 0;
                }
                if (run != 0) put(&writer, run & 31, 5);
            }
            put(&writer, 0xa5a5, 16);

            // Decode what was just written back into the frame data.
            unsigned char* decompression = compImmediateDecompressor;
            {
                unsigned previous = get(&reader, 16);
                int run = static_cast<int>(get(&reader, 5));
                int k = 0;
                for (;;) {
                    for (int r = 0; r < run; ++r) {
                        *reinterpret_cast<Half*>(frame + 12 * k) = static_cast<unsigned short>(previous);
                        ++k;
                    }
                    if (k == 1) break;
                    if (run != 31) {
                        int bit = static_cast<int>(get(&reader, 4));
                        previous ^= 1u << bit;
                    }
                    run = static_cast<int>(get(&reader, 5));
                }
            }
            for (int c = 0; c < 4; ++c) {
                unsigned char column[16];
                analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(
                    decompression, &reader, decompression + kState[c], column, 1, 0);
                frame[kColumn[c]] = column[0];
            }
            {
                int k = 0;
                for (;;) {
                    int run = static_cast<int>(get(&reader, 5));
                    for (int r = 0; r < run; ++r) {
                        unsigned short sum = static_cast<unsigned short>(*reinterpret_cast<Half*>(decompression + 66) +
                                                                         *reinterpret_cast<Half*>(decompression + 68));
                        *reinterpret_cast<Half*>(decompression + 66) = sum;
                        word(frame + 12 * k, 8) = static_cast<short>(sum);
                        ++k;
                    }
                    if (k == 1) break;
                    if (run != 31) {
                        if (get(&reader, 1) != 0) {
                            int delta = get_signed(&reader, 9);
                            unsigned short sum = static_cast<unsigned short>(*reinterpret_cast<Half*>(decompression + 66) + delta);
                            *reinterpret_cast<Half*>(decompression + 68) = static_cast<unsigned short>(delta);
                            *reinterpret_cast<Half*>(decompression + 66) = sum;
                        } else {
                            int value = get_signed(&reader, 16);
                            *reinterpret_cast<Half*>(decompression + 66) = static_cast<unsigned short>(value);
                        }
                        word(frame + 12 * k, 8) = static_cast<short>(*reinterpret_cast<Half*>(decompression + 66));
                        ++k;
                    }
                    if (!(k < 1)) break;
                }
            }
            {
                int bit = static_cast<int>(get(&reader, 1));
                int k = 0;
                for (;;) {
                    int run = static_cast<int>(get(&reader, 5));
                    for (int r = 0; r < run; ++r) {
                        word(frame + 12 * k, 8) = (word(frame + 12 * k, 8) & -2) | bit;
                        ++k;
                    }
                    if (k == 1) break;
                    if (run != 31) bit ^= 1;
                }
            }
            reader.position += 16;
            word(decompression, 72) = word(decompression, 72) + 1;

            padProcessData__FP3padP18padUnprocessedDataP12padStateData(pad + 88 * local, frame, localPlayerPadState);
            *reinterpret_cast<Half*>(netCameraYaw + 2 * local) = *reinterpret_cast<Half*>(frame + 8);
            if (*remembered_slot != 0) *remembered_slot = static_cast<int>(reinterpret_cast<unsigned long>(localPredictedPlayer));
            int saved_random = syncRandState;
            float saved_a = *reinterpret_cast<Float*>(record + 172);
            float saved_b = *reinterpret_cast<Float*>(record + 196);
            int steps = engineGetAIStepCount__Fv();
            if (frameNm < localNetFrameNm - steps) {
                netPredictionFastForward = 1;
                word(localPredictedPlayer, 12) = word(localPredictedPlayer, 12) | 0x04000000;
            } else {
                netPredictionFastForward = 0;
                word(localPredictedPlayer, 12) = word(localPredictedPlayer, 12) & 0xfbffffff;
            }
            {
                unsigned char* object = localPredictedPlayer;
                unsigned char* entry_at = *reinterpret_cast<unsigned char**>(object + 72) + 32;
                short adjust = *reinterpret_cast<short*>(entry_at);
                void (*step)(void*) = *reinterpret_cast<void (**)(void*)>(entry_at + 4);
                step(object + adjust);
            }
            if (netPredictionFastForward == 0) Update__6Camera(gameCam);
            *reinterpret_cast<Float*>(record + 172) = saved_a;
            *reinterpret_cast<Float*>(record + 196) = saved_b;
            current = frameNm + 1;
            syncRandState = saved_random;
            *remembered_slot = remembered;
            int limit = localNetFrameNm;
            frameNm = current;
            if (!(current < limit)) break;
        } while (true);
        netLogDisable = 0;
        netPrediction = 0;
    }
    unsigned char* source = reinterpret_cast<unsigned char*>(*remembered_slot);
    if (source != 0 && localPredictedPlayer != 0) {
        for (int i = 0; i < 16; ++i) localPredictedPlayer[140 + i] = source[140 + i];
        for (int i = 0; i < 24; ++i) localPredictedPlayer[856 + i] = source[856 + i];
    }
}

// Observed advanceRealState__Fi at 0x001317d0, 5488 bytes, a local (static)
// function. The argument is a step count; for each step the lock-step state is
// advanced by one frame. Offsets named "record" are relative to a partner record
// in `partners` (stride 17632); "pad data" is the 12-byte block of a frame entry
// at entry + 4 (a button halfword at +0, four analog bytes at +2..+5 and a yaw
// word at +8). Per step, unless the first byte of levelName is nonzero (the
// step is then skipped but objectIgnoreFlags stays 0x00020000):
//  1. objectIgnoreFlags = 0x00020000 and frameNm = trueNetFrameNm. The local
//     index is the first partner whose byte 17620 is nonzero (0 when
//     netNetworkMode is zero, -1 when none).
//  2. For every partner whose byte 17621 is zero: entry = record + 172 +
//     (trueNetFrameNm & 255) * 24. For the local partner a 24-byte copy of the
//     entry is passed through the local compressor and decompressor (one frame:
//     16-bit buttons, run/bit-change codes, four analCompress and analDecompress
//     calls, the yaw coder, a bit-0 run coder, the 0xa5a5 marker; the same coding
//     netGameStep uses to send) and the decoded copy replaces the entry for the
//     rest of the step. Then padProcessData(pad + 88 * index, entry + 4,
//     record + 17588), netCameraYaw[index] = the low halfword of the yaw word, and
//     the message ring is replayed: when the entry's word at +20 is nonzero,
//     messages (type byte, length byte, payload; ring of 8192 bytes at
//     record + 9388, start offset = entry word +16) are handed to
//     msgHandler[type](payload, length) until that many bytes are consumed; for a
//     non-local partner the ring position is stored at record + 17584.
//  3. When (frameNm & 63) == 0, three words per player at player + 4936 + 4616 *
//     k are each counted down in an empty loop (no effect other than time).
//  4. netTrueStep = 1. Partners whose byte 17622 is nonzero and whose word 17624
//     equals trueNetFrameNm lose bit 0x00400000 of their player object's word at
//     +12, receive virtual entry (+72) + 16 (this, 0, 0x20000040, 1000000.0f) and
//     then playerDamageHandler. coreGameStep(1, 2) runs. If localPredictedPlayer
//     is set, bit 0x100 of the local player object's word +12 moves to it.
//     netTrueStep = 0, worldPlotRouteProcess(1).
//  5. A three-word log entry at record(local) + 6316 + (new frame & 255) * 12 is
//     written: new trueNetFrameNm (incremented here), the frameNm value saved at the
//     start of the step, and logChecksum + syncRandState plus the integer parts
//     of each player object's position (+16, +20, +24, via cvt.w.s). frameNm is
//     restored. For every partner with bytes 17620 and 17621 zero whose log
//     entry for that frame carries the same frame number,
//     compareStatePackets(own entry, theirs, index) is called.
//  6. The words at +12 and +16 of the four netLocalPad records are zeroed.
// Afterwards (also when steps < 1) the saved words are restored to the four pad
// records and, when the local player object exists: objectIgnoreFlags =
// 0x00200000, netForceLocalUpdate counts down toward zero, frameNm and
// derivedFrameNm = trueNetFrameNm, netLogDisable = netPrediction = 1, the
// previous predicted player gets virtual entry (+72) + 8 with 3, the 76-byte
// local compressor and decompressor are copied to their "immediate" twins,
// localPlayerPadState takes the eight bytes at record(local) + 17588, a clone
// (virtual entry (+72) + 120) of the local player object becomes
// localPredictedPlayer (syncRandState preserved, word +12 = (word & ~0x00200000) |
// 0x00020000 | (old predicted word & 0x100)), the local object gets bit
// 0x00200000 and objectAddToGrid runs on the clone; netLogDisable and
// netPrediction are cleared.
// Inferred: this re-simulates the confirmed frames and rebuilds the predicted
// local player; names of the fields are the image's own where a symbol exists.
// Unknown: the purpose of the step-3 loops, the type behind the three words they
// read, what bit 0x40 flags in the damage call, the exact meaning of word 12 bits,
// and the behaviour for a local index of -1 (the image indexes player and partners
// with it).
typedef int __attribute__((may_alias)) Int;
typedef unsigned __attribute__((may_alias)) Word;
typedef unsigned short __attribute__((may_alias)) Half;
typedef unsigned char Byte;

extern "C" {
extern signed char netNetworkMode;
extern int nmPartners;
extern int nmPlayers;
extern int trueNetFrameNm;
extern int frameNm;
extern int derivedFrameNm;
extern int syncRandState;
extern int logChecksum;
extern int objectIgnoreFlags;
extern signed char netTrueStep;
extern signed char netPrediction;
extern signed char netLogDisable;
extern signed char netForceLocalUpdate;
extern unsigned localPredictedPlayer;
extern unsigned short netCameraYaw[];
extern unsigned char localPlayerPadState[];
extern unsigned char partners[];
extern unsigned char player[];
extern unsigned char pad[];
extern unsigned char netLocalPad[];
extern unsigned msgHandler[];
extern char levelName[];
extern unsigned char compLocalCompressor[];
extern unsigned char compLocalDecompressor[];
extern unsigned char compImmediateCompressor[];
extern unsigned char compImmediateDecompressor[];

void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void* pad_record, void* data, void* state);
void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(void* compression, void* writer,
                                                                          void* state, unsigned char* bytes,
                                                                          int count);
void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(
    void* decompression, void* reader, void* state, unsigned char* bytes, int count, int extra);
void coreGameStep__Fii(int first, int second);
void worldPlotRouteProcess__Fi(int value);
void playerDamageHandler__6Player(void* player_object);
void objectAddToGrid__FP10GameObject(void* object);
void compareStatePackets__FP11statePacketT0i(void* first, void* second, int partner);
}

#ifdef NORRATH_W10NA_HOST
extern "C" Byte* norrath_w10na_word_to_pointer(unsigned word);
extern "C" unsigned norrath_w10na_pointer_to_word(const void* pointer);
#define NORRATH_W10NA_POINTER(word) norrath_w10na_word_to_pointer(word)
#define NORRATH_W10NA_WORD(pointer) norrath_w10na_pointer_to_word(pointer)
inline int norrath_w10na_float_to_int(float value) { return static_cast<int>(value); }
#else
#define NORRATH_W10NA_POINTER(word) reinterpret_cast<Byte*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_W10NA_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
// The image converts with cvt.w.s, which the EE truncates.
inline int norrath_w10na_float_to_int(float value) {
    int result;
    float converted;
    __asm__ volatile("cvt.w.s %1,%2\n\tmfc1 %0,%1" : "=&r"(result), "=&f"(converted) : "f"(value));
    return result;
}
#endif

namespace {
const int kRecord = 17632;
const int kPlayer = 23984;
const int kPad = 88;

struct BitWriter {
    unsigned short* data;
    int position;
};

struct BitReader {
    unsigned short* data;
    int position;
};

struct VirtualEntry {
    short adjust;
    short unused;
    unsigned function;
};

inline Int& word(Byte* base, int offset) { return *reinterpret_cast<Int*>(base + offset); }
inline Half& half(Byte* base, int offset) { return *reinterpret_cast<Half*>(base + offset); }
inline short signed_half(Byte* base, int offset) { return static_cast<short>(half(base, offset)); }

// Writes the low `width` bits of value at the writer position.
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

// Reads `width` bits at the reader position as an unsigned value.
inline unsigned get(BitReader* reader, int width) {
    int position = reader->position;
    unsigned short* unit = reader->data + (position >> 4);
    unsigned window = (static_cast<unsigned>(unit[0]) << 16) | unit[1];
    reader->position = position + width;
    return (window << (position & 15)) >> (32 - width);
}

// Reads `width` bits at the reader position as a signed value.
inline int get_signed(BitReader* reader, int width) {
    int position = reader->position;
    unsigned short* unit = reader->data + (position >> 4);
    unsigned window = (static_cast<unsigned>(unit[0]) << 16) | unit[1];
    reader->position = position + width;
    return static_cast<int>(window << (position & 15)) >> (32 - width);
}

// Index of the first partner whose byte 17620 is nonzero, 0 without a network
// mode, -1 when there is none.
inline int find_local_partner() {
    if (netNetworkMode == 0) return 0;
    int count = nmPartners;
    for (int i = 0; i < count; ++i)
        if (static_cast<signed char>(partners[i * kRecord + 17620]) != 0) return i;
    return -1;
}

// Codes one frame of pad data with the local compressor into `buffer` (the
// same coding netGameStep sends).
void encode_frame(Byte* data, Byte* compression, BitWriter* writer) {
    const int count = 1;
    word(compression, 72) = word(compression, 72) + count;
    put(writer, half(data, 0), 16);
    {
        unsigned previous = half(data, 0);
        int run = 0;
        for (int k = 0; k < count; ++k) {
            unsigned current = half(data + k * 12, 0);
            if (current == previous) {
                ++run;
            } else {
                do {
                    put(writer, run & 31, 5);
                    run = 0;
                    int bit = 0;
                    while (bit < 16 && !(((current ^ previous) >> bit) & 1)) ++bit;
                    if (bit < 16) {
                        put(writer, bit & 15, 4);
                        previous ^= 1u << bit;
                    }
                } while (current != previous);
                ++run;
            }
            if (run == 31) {
                put(writer, 31, 5);
                run = 0;
            }
        }
        if (run != 0) put(writer, run & 31, 5);
    }
    static const int kColumn[4] = {2, 3, 4, 5};
    static const int kState[4] = {32, 48, 0, 16};
    for (int c = 0; c < 4; ++c) {
        unsigned char column[count];
        for (int k = 0; k < count; ++k) column[k] = data[k * 12 + kColumn[c]];
        analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(
            compression, writer, compression + kState[c], column, count);
    }
    {
        int run = 0;
        for (int k = 0; k < count; ++k) {
            int yaw_in = word(data + k * 12, 8);
            int target = yaw_in & -2;
            int delta = target - signed_half(compression, 64);
            int magnitude = delta < 0 ? -delta : delta;
            half(compression, 64) = static_cast<unsigned short>(target);
            bool escape = true;
            int second = 0;
            if (magnitude < 1001) {
                second = target - signed_half(compression, 66);
                int second_magnitude = second < 0 ? -second : second;
                if (second_magnitude < 3001) escape = false;
            }
            if (escape) {
                put(writer, run & 31, 5);
                put(writer, 0, 1);
                put(writer, yaw_in & 0xfffe, 16);
                run = 0;
                half(compression, 66) = static_cast<unsigned short>(target);
                continue;
            }
            int second_magnitude = second < 0 ? -second : second;
            int smooth = delta;
            if (!(second_magnitude < 201)) smooth = delta + second / 15;
            int previous_step = signed_half(compression, 68);
            int difference = smooth - previous_step;
            if (difference < 0) difference = -difference;
            unsigned short sum;
            if (difference < 31 && previous_step * delta >= 0) {
                ++run;
                if (run == 31) {
                    put(writer, 31, 5);
                    run = 0;
                }
                sum = static_cast<unsigned short>(half(compression, 66) + half(compression, 68));
            } else {
                put(writer, run & 31, 5);
                put(writer, 1, 1);
                int clamped = smooth;
                if (clamped < -255) clamped = -255;
                if (clamped > 255) clamped = 255;
                put(writer, clamped & 0x1ff, 9);
                run = 0;
                half(compression, 68) = static_cast<unsigned short>(clamped);
                sum = static_cast<unsigned short>(half(compression, 66) + clamped);
            }
            half(compression, 66) = sum;
        }
        if (run != 0) put(writer, run & 31, 5);
    }
    {
        int run = 0;
        int current = word(data, 8) & 1;
        put(writer, current, 1);
        for (int k = 0; k < count; ++k) {
            if ((word(data + k * 12, 8) & 1) != current) {
                put(writer, run & 31, 5);
                run = 0;
                current = word(data + k * 12, 8) & 1;
            }
            ++run;
            if (run == 31) {
                put(writer, 31, 5);
                run = 0;
            }
        }
        if (run != 0) put(writer, run & 31, 5);
    }
    put(writer, 0xa5a5, 16);
}

// Decodes one frame written by encode_frame back into `data` using the local
// decompressor.
void decode_frame(Byte* data, Byte* decompression, BitReader* reader) {
    const int count = 1;
    reader->position = 0;
    unsigned current = get(reader, 16);
    {
        int k = 0;
        unsigned run = get(reader, 5);
        for (;;) {
            for (unsigned j = 0; j < run; ++j) {
                half(data + k * 12, 0) = static_cast<unsigned short>(current);
                ++k;
            }
            if (k == count) break;
            if (run != 31) current ^= 1u << get(reader, 4);
            run = get(reader, 5);
        }
    }
    static const int kColumn[4] = {2, 3, 4, 5};
    static const int kState[4] = {32, 48, 0, 16};
    for (int c = 0; c < 4; ++c) {
        unsigned char column[count];
        analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(
            decompression, reader, decompression + kState[c], column, count, 0);
        for (int k = 0; k < count; ++k) data[k * 12 + kColumn[c]] = column[k];
    }
    {
        int k = 0;
        for (;;) {
            unsigned run = get(reader, 5);
            for (unsigned j = 0; j < run; ++j) {
                half(decompression, 66) = static_cast<unsigned short>(half(decompression, 66) + half(decompression, 68));
                word(data + k * 12, 8) = signed_half(decompression, 66);
                ++k;
            }
            if (k == count) break;
            if (run != 31) {
                if (get(reader, 1) != 0) {
                    int delta = get_signed(reader, 9);
                    half(decompression, 68) = static_cast<unsigned short>(delta);
                    half(decompression, 66) = static_cast<unsigned short>(half(decompression, 66) + delta);
                } else {
                    half(decompression, 66) = static_cast<unsigned short>(get_signed(reader, 16));
                }
                word(data + k * 12, 8) = signed_half(decompression, 66);
                ++k;
            }
            if (!(k < count)) break;
        }
    }
    {
        int k = 0;
        unsigned bit = get(reader, 1);
        for (;;) {
            unsigned run = get(reader, 5);
            for (unsigned j = 0; j < run; ++j) {
                word(data + k * 12, 8) = (word(data + k * 12, 8) & -2) | static_cast<int>(bit);
                ++k;
            }
            if (k == count) break;
            if (run != 31) bit ^= 1;
        }
    }
    reader->position += 16;
    word(decompression, 72) = word(decompression, 72) + 1;
}

// Replays the queued messages of one partner entry.
void replay_messages(Byte* record, Byte* entry, bool local_partner) {
    int total = word(entry, 20);
    if (total == 0) return;
    int start = word(entry, 16);
    Byte* ring = record + 12 + 9376;
    unsigned char message[256];
    do {
        int length = ring[(start + 1) & 0x1fff];
        for (int j = 0; j < length; ++j) message[j] = ring[(start + 2 + j) & 0x1fff];
        typedef void (*Handler)(unsigned char*, int);
        Handler handler = reinterpret_cast<Handler>(NORRATH_W10NA_POINTER(msgHandler[ring[start]]));
        handler(message, length);
        total = total - length - 2;
        start = (start + length + 2) & 0x1fff;
    } while (total != 0);
    if (!local_partner) word(record, 17584) = start;
}
}

extern "C" void advanceRealState__Fi(int steps) {
    int local = find_local_partner();
    int saved_pad_a[4];
    int saved_pad_b[4];
    for (int i = 0; i < 4; ++i) {
        saved_pad_a[i] = word(netLocalPad, i * kPad + 12);
        saved_pad_b[i] = word(netLocalPad, i * kPad + 16);
    }
    for (int step = 0; step < steps; ++step) {
        int local_offset = local * kRecord;
        objectIgnoreFlags = 0x20000;
        if (levelName[0] != 0) continue;
        int saved_frame = frameNm;
        frameNm = trueNetFrameNm;
        for (int p = 0; p < nmPartners; ++p) {
            Byte* record = partners + p * kRecord;
            if (static_cast<signed char>(record[17621]) != 0) continue;
            Byte* entry = record + 172 + (trueNetFrameNm & 0xff) * 24;
            Word copy[6];
            if (p == local) {
                for (int i = 0; i < 6; ++i) copy[i] = reinterpret_cast<Word*>(entry)[i];
                Byte* data = reinterpret_cast<Byte*>(copy) + 4;
                unsigned short buffer[256];
                BitWriter writer;
                BitReader reader;
                buffer[0] = 0;
                buffer[1] = 0;
                writer.data = buffer;
                writer.position = 0;
                encode_frame(data, compLocalCompressor, &writer);
                reader.data = buffer;
                decode_frame(data, compLocalDecompressor, &reader);
                entry = reinterpret_cast<Byte*>(copy);
            }
            padProcessData__FP3padP18padUnprocessedDataP12padStateData(pad + p * kPad, entry + 4, record + 17588);
            netCameraYaw[p] = half(entry, 12);
            replay_messages(record, entry, p == local);
        }
        if ((frameNm & 63) == 0) {
            for (int i = 0; i < nmPlayers; ++i) {
                for (int k = 0; k < 3; ++k) {
                    for (int n = word(player + i * kPlayer, 4936 + k * 4616); n > 0; --n) __asm__ volatile("");
                }
            }
        }
        netTrueStep = 1;
        for (int p = 0; p < nmPartners; ++p) {
            Byte* record = partners + p * kRecord;
            if (static_cast<signed char>(record[17622]) == 0) continue;
            if (word(record, 17624) != trueNetFrameNm) continue;
            Byte* object = NORRATH_W10NA_POINTER(word(player + p * kPlayer, 21872));
            if (object == 0) continue;
            word(object, 12) &= ~0x400000;
            const VirtualEntry* entry =
                reinterpret_cast<const VirtualEntry*>(NORRATH_W10NA_POINTER(word(object, 72)) + 16);
            typedef void (*Damage)(Byte*, int, unsigned, float);
            reinterpret_cast<Damage>(NORRATH_W10NA_POINTER(entry->function))(object + entry->adjust, 0, 0x20000040u,
                                                                              1000000.0f);
            playerDamageHandler__6Player(object);
        }
        coreGameStep__Fii(1, 2);
        if (localPredictedPlayer != 0) {
            Byte* object = NORRATH_W10NA_POINTER(word(player + local * kPlayer, 21872));
            if (object != 0) {
                Byte* predicted = NORRATH_W10NA_POINTER(localPredictedPlayer);
                word(predicted, 12) |= word(object, 12) & 0x100;
                word(object, 12) &= ~0x100;
            }
        }
        netTrueStep = 0;
        worldPlotRouteProcess__Fi(1);
        {
            int next_frame = trueNetFrameNm + 1;
            int checksum = logChecksum + syncRandState;
            Byte* log = partners + 6316 + local_offset + (next_frame & 0xff) * 12;
            trueNetFrameNm = next_frame;
            frameNm = saved_frame;
            word(log, 0) = next_frame;
            word(log, 4) = saved_frame;
            word(log, 8) = checksum;
            for (int i = 0; i < nmPlayers; ++i) {
                Byte* object = NORRATH_W10NA_POINTER(word(player + i * kPlayer, 21872));
                if (object == 0) continue;
                float x = *reinterpret_cast<float*>(object + 16);
                float y = *reinterpret_cast<float*>(object + 20);
                float z = *reinterpret_cast<float*>(object + 24);
                word(log, 8) = word(log, 8) + norrath_w10na_float_to_int(x) + norrath_w10na_float_to_int(y) +
                               norrath_w10na_float_to_int(z);
            }
            for (int p = 0; p < nmPartners; ++p) {
                Byte* record = partners + p * kRecord;
                if (static_cast<signed char>(record[17620]) != 0) continue;
                if (static_cast<signed char>(record[17621]) != 0) continue;
                Byte* theirs = record + 6316 + (word(log, 0) & 0xff) * 12;
                if (word(theirs, 0) == word(log, 0))
                    compareStatePackets__FP11statePacketT0i(log, theirs, p);
            }
        }
        for (int i = 0; i < 4; ++i) {
            word(netLocalPad, i * kPad + 12) = 0;
            word(netLocalPad, i * kPad + 16) = 0;
        }
    }
    for (int i = 0; i < 4; ++i) {
        word(netLocalPad, i * kPad + 12) = saved_pad_a[i];
        word(netLocalPad, i * kPad + 16) = saved_pad_b[i];
    }
    objectIgnoreFlags = 0x200000;
    if (word(player + local * kPlayer, 21872) == 0) return;
    unsigned old_flags = 0;
    if (localPredictedPlayer != 0) old_flags = word(NORRATH_W10NA_POINTER(localPredictedPlayer), 12);
    if (netForceLocalUpdate > 0) netForceLocalUpdate = static_cast<signed char>(netForceLocalUpdate - 1);
    frameNm = trueNetFrameNm;
    netLogDisable = 1;
    derivedFrameNm = frameNm;
    netPrediction = 1;
    int predicted_index = find_local_partner();
    if (localPredictedPlayer != 0) {
        Byte* predicted = NORRATH_W10NA_POINTER(localPredictedPlayer);
        const VirtualEntry* entry =
            reinterpret_cast<const VirtualEntry*>(NORRATH_W10NA_POINTER(word(predicted, 72)) + 8);
        typedef void (*Destroy)(Byte*, int);
        reinterpret_cast<Destroy>(NORRATH_W10NA_POINTER(entry->function))(predicted + entry->adjust, 3);
    }
    for (int i = 0; i < 76; ++i) {
        compImmediateCompressor[i] = compLocalCompressor[i];
    }
    for (int i = 0; i < 76; ++i) {
        compImmediateDecompressor[i] = compLocalDecompressor[i];
    }
    for (int i = 0; i < 8; ++i) localPlayerPadState[i] = partners[local * kRecord + 17588 + i];
    int saved_random = syncRandState;
    Byte* source = NORRATH_W10NA_POINTER(word(player + predicted_index * kPlayer, 21872));
    const VirtualEntry* entry = reinterpret_cast<const VirtualEntry*>(NORRATH_W10NA_POINTER(word(source, 72)) + 120);
    typedef Byte* (*Clone)(Byte*);
    Byte* clone = reinterpret_cast<Clone>(NORRATH_W10NA_POINTER(entry->function))(source + entry->adjust);
    syncRandState = saved_random;
    word(clone, 12) = (word(clone, 12) & ~0x200000) | 0x20000 | (old_flags & 0x100);
    localPredictedPlayer = NORRATH_W10NA_WORD(clone);
    Byte* local_object = NORRATH_W10NA_POINTER(word(player + predicted_index * kPlayer, 21872));
    word(local_object, 12) |= 0x200000;
    objectAddToGrid__FP10GameObject(clone);
    netLogDisable = 0;
    netPrediction = 0;
}

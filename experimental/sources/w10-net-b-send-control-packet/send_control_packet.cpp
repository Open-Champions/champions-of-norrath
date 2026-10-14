// Observed sendControlPacket__FiiiiiR9CCompressb at 0x0012f840, 5532 bytes in
// the pinned SLUS_205.65 image; arguments (partner, first, second, header,
// local, compression, flag), no result. `partners` records are 17632 bytes;
// the ring of 256 entries of 24 bytes sits at record + 172 (frame data in bytes
// 4..15 of an entry, a message length at +20 and a message start at +16 of the
// entry, with the message ring of 8192 bytes at record + 9388).
// It builds one datagram in a stack buffer (about 5000 bytes) and sends it to
// partner `partner`:
//  1. Header: the word `header`, the word `second`, the byte `local`.
//  2. One byte: when flag is false, second - (low byte of trueNetFrameNm);
//     when flag is true, 0 if netNetworkMode is zero, else the index of the
//     first partner whose record byte +17620 is nonzero, or -1 if none.
//  3. Two bytes: second - first + 1 and (record word +17596) - second.
//  4. The compression object is copied (76 bytes) to a local one. When flag is
//     false and first differs from the record word +92, the frames of the
//     ring of partner `local` from record word +92 up to first-1 are packed with
//     the encoder below into the buffer at the current position; that output
//     is then overwritten by the next stage, and only the local compression
//     object keeps the effect.
//  5. Frames first through second of the ring of partner `local` (bytes 4..15 of
//     each entry): when flag is true they are copied raw (12 bytes each); when
//     false they are packed with the encoder, starting at the current position,
//     and the position advances by (bits used + 7) / 8.
//  6. The position is rounded up to a multiple of 4. When sentStatePacketFrameNm
//     differs from trueNetFrameNm it is set to it and the word 0xbd40 is written
//     followed by 12 bytes at record(local) + 6316 + 12 * (low byte of
//     trueNetFrameNm); otherwise the word -1 is written.
//  7. For each frame f from first through second whose ring entry has a nonzero
//     message length n: the bytes (second - f), the low byte of n, then n bytes
//     from the message ring starting at the entry word +16 (wrapping at 8192).
//  8. The datagram is sent with sceInetSendTo(cd, g_net_buf,
//     broadcastConnection, packet, length, 0, block, <record word +4>, 1000)
//     after broadcastConnectionFlags is cleared; the block is 16 zero bytes
//     with bytes 4..7 copied from the record's first four bytes.
// Encoder (the same coder the authored netGameStep uses): the counter word at
// compression + 72 grows by the frame count; the first button halfword in 16
// bits; run/bit-index codes; four analCompress calls over frame bytes 2..5 with
// states +32, +48, +0, +16; the yaw coder on frame bytes 8..11; the yaw-bit
// runs; the marker 0xa5a5.
// Inferred: this is the packing of the lock-step input stream; stage 4 only
// advances the local compression state. Unknown: why stage 4's bits are
// discarded; behavior when second < first - 1 (the image sizes a stack array
// from the difference), and the content of the buffer past the written bytes.
typedef int __attribute__((may_alias)) Int;
typedef unsigned short __attribute__((may_alias)) Half;

struct CCompress {
    unsigned words[20];
};

extern "C" {
extern signed char netNetworkMode;
extern int nmPartners;
extern unsigned char partners[];
extern int trueNetFrameNm;
extern int sentStatePacketFrameNm;
extern int broadcastConnection;
extern int broadcastConnectionFlags;
extern unsigned char cd[];
extern unsigned char g_net_buf[];
void* memcpy(void* destination, const void* source, unsigned long length);
void* memset(void* destination, int value, unsigned long length);
int sceInetSendTo(void* object, void* buffer, int connection, void* data, int length, int flags,
                  void* address, int first_limit, int second_limit);
void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(void* compression, void* writer,
                                                                          void* state, unsigned char* bytes,
                                                                          int count);
}

namespace {
struct BitWriter {
    unsigned short* data;
    int position;
};

struct Frame {
    unsigned char bytes[12];
} __attribute__((aligned(4)));

inline const Frame& frame_at(const unsigned char* base) { return *reinterpret_cast<const Frame*>(base); }

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

void encode(unsigned char* out, unsigned char* compression, const Frame* frames, int count, BitWriter* writer) {
    writer->data = reinterpret_cast<unsigned short*>(out);
    writer->data[0] = 0;
    writer->data[1] = 0;
    writer->position = 0;
    word(compression, 72) = word(compression, 72) + count;
    put(writer, *reinterpret_cast<const Half*>(frames[0].bytes), 16);
    {
        unsigned previous = *reinterpret_cast<const Half*>(frames[0].bytes);
        int run = 0;
        for (int k = 0; k < count; ++k) {
            unsigned current = *reinterpret_cast<const Half*>(frames[k].bytes);
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
        for (int k = 0; k < count; ++k) column[k] = frames[k].bytes[kColumn[c]];
        analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(compression, writer,
                                                                              compression + kState[c], column, count);
    }
    {
        int run = 0;
        for (int k = 0; k < count; ++k) {
            int yaw_in = *reinterpret_cast<const Int*>(frames[k].bytes + 8);
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
                put(writer, run & 31, 5);
                put(writer, 0, 1);
                put(writer, yaw_in & 0xfffe, 16);
                run = 0;
                *reinterpret_cast<Half*>(compression + 66) = static_cast<unsigned short>(target);
                continue;
            }
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
                    put(writer, 31, 5);
                    run = 0;
                }
                sum = static_cast<unsigned short>(*reinterpret_cast<Half*>(compression + 66) +
                                                  *reinterpret_cast<Half*>(compression + 68));
            } else {
                put(writer, run & 31, 5);
                put(writer, 1, 1);
                int clamped = smooth;
                if (clamped < -255) clamped = -255;
                if (clamped > 255) clamped = 255;
                put(writer, clamped & 0x1ff, 9);
                run = 0;
                *reinterpret_cast<Half*>(compression + 68) = static_cast<unsigned short>(clamped);
                sum = static_cast<unsigned short>(*reinterpret_cast<Half*>(compression + 66) + clamped);
            }
            *reinterpret_cast<Half*>(compression + 66) = sum;
        }
        if (run != 0) put(writer, run & 31, 5);
    }
    {
        int run = 0;
        int current = *reinterpret_cast<const Int*>(frames[0].bytes + 8) & 1;
        put(writer, current, 1);
        for (int k = 0; k < count; ++k) {
            if ((*reinterpret_cast<const Int*>(frames[k].bytes + 8) & 1) != current) {
                put(writer, run & 31, 5);
                run = 0;
                current = *reinterpret_cast<const Int*>(frames[k].bytes + 8) & 1;
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
}

extern "C" void sendControlPacket__FiiiiiR9CCompressb(int partner, int first, int second, int header, int local,
                                                      CCompress& compression, bool flag) {
    unsigned char* record = partners + partner * 17632;
    unsigned char* local_record = partners + local * 17632;
    unsigned char packet[5008];
    unsigned char* out = packet;
    *reinterpret_cast<Int*>(out) = header;
    *reinterpret_cast<Int*>(out + 4) = second;
    out[8] = static_cast<unsigned char>(local);
    out += 9;
    if (!flag) {
        *out++ = static_cast<unsigned char>(second - static_cast<unsigned char>(trueNetFrameNm));
    } else {
        int value;
        if (netNetworkMode == 0) {
            value = 0;
        } else {
            value = -1;
            for (int i = 0; i < nmPartners; ++i) {
                if (static_cast<signed char>(partners[17632 * i + 17620]) != 0) {
                    value = i;
                    break;
                }
            }
        }
        *out++ = static_cast<unsigned char>(value);
    }
    *out++ = static_cast<unsigned char>(second - first + 1);
    *out++ = static_cast<unsigned char>(word(record, 17596) - second);
    CCompress local_compression = compression;
    unsigned char* object = reinterpret_cast<unsigned char*>(&local_compression);
    BitWriter writer;
    if (!flag) {
        int count = first - word(record, 92);
        if (count != 0) {
            Frame frames[count];
            for (int f = word(record, 92); f < first; ++f)
                frames[f - word(record, 92)] = frame_at(local_record + 176 + (f & 0xff) * 24);
            encode(out, object, frames, count, &writer);
        }
    }
    int count = second - first + 1;
    Frame frames[count];
    for (int f = first; f <= second; ++f) frames[f - first] = frame_at(local_record + 176 + (f & 0xff) * 24);
    if (flag) {
        memcpy(out, frames, count * 12);
        out += count * 12;
    } else {
        encode(out, object, frames, count, &writer);
        out += (writer.position + 7) >> 3;
    }
    out = reinterpret_cast<unsigned char*>((reinterpret_cast<unsigned long>(out) + 3) & ~3ul);
    if (sentStatePacketFrameNm != trueNetFrameNm) {
        sentStatePacketFrameNm = trueNetFrameNm;
        *reinterpret_cast<Int*>(out) = 0xbd40;
        out += 4;
        *reinterpret_cast<Frame*>(out) = frame_at(local_record + 6316 + 12 * static_cast<unsigned char>(trueNetFrameNm));
        out += 12;
    } else {
        *reinterpret_cast<Int*>(out) = -1;
        out += 4;
    }
    for (int f = first; f <= second; ++f) {
        unsigned char* entry = local_record + 172 + (f & 0xff) * 24;
        if (word(entry, 20) == 0) continue;
        *out++ = static_cast<unsigned char>(second - f);
        *out++ = entry[20];
        for (int j = 0; j < word(entry, 20); ++j) out[j] = local_record[9388 + ((word(entry, 16) + j) & 0x1fff)];
        out += word(entry, 20);
    }
    unsigned char block[16];
    broadcastConnectionFlags = 0;
    int length = static_cast<int>(out - packet);
    memset(block, 0, 16);
    int port = word(record, 4);
    for (int k = 0; k < 4; ++k) block[4 + k] = record[k];
    sceInetSendTo(cd, g_net_buf, broadcastConnection, packet, length, 0, block, port, 1000);
}

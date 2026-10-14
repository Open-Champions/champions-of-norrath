// Observed netGameStep at 0x0012b250, 5796 bytes, returning nothing. The two
// arguments are bools (the first is stored as flagA below, the second as
// flagB). Offsets named "record" are relative to a partner record in
// `partners` (stride 17632, four records). The record fields used here are:
//   +0..3 address bytes, +4 word, +92 word (first frame not yet sent),
//   +96 compression state (see below), +172 ring of 256 frame entries of 24
//   bytes (frame word, 12 bytes of pad data, a word at +16, a word at +20),
//   +17580 word, +17596/+17600/+17604/+17608/+17612 words, +17616 word,
//   +17620/+17621/+17622 flag bytes, +17624 and +17628 words.
// Sequence (all offsets are bytes):
//  1. localPlayer = netGetLocalPlayerNm(0); netNmPartners = nmPartners as a
//     byte; receivePackets(). n is then nmPartners.
//  2. With flagA clear, for each partner whose word at +17628 is nonzero: the
//     word is incremented (back to 0 once it was 121 or more) and, when the
//     old value is a multiple of 10, a four-byte ping 0xbd43 is sent with
//     sceInetSendTo to the partner's address bytes (block bytes 4..7), using
//     the partner word at +4 as the limit and 1000 as the last argument.
//  3. With localPlayer zero: every partner whose three flag bytes are clear
//     counts up its word at +17612; at 901 or more it is marked (byte 17622
//     = 1, word 17624 = -1) and kickState is set to 1 (the 8 bytes cleared).
//     When kickState is 1 and drawFrameNm % 30 == 0, a mask packet 0xbd41 (the
//     header and a mask whose bit k is set where partner k has byte 17622 equal
//     to 1) goes to each partner whose flags and kickState byte (4 + index) are
//     clear; if none was sent, kickState becomes 2 and marked partners take
//     word 17596 + 1 in word 17624. When kickState is 2 and drawFrameNm % 30
//     == 0, each unflagged partner sends a control packet (0xbd42) to every
//     other unflagged partner whose byte 17622 is 1 and whose word 17624 is
//     not below its own word 17608, carrying a default-initialised
//     compression object; if none was sent, partners with byte 17622 equal to
//     1 become 2 and kickState becomes 0.
//  4. ahead = localNetFrameNm - trueNetFrameNm; smallest word 17604 and 17608
//     over unflagged partners bound it and the frame number.
//  5. Local input: padGetUnprocessedData; a camera yaw word, pad blanking,
//     padProcessData into netLocalPad.
//  6. A stall flag is derived from frame distance, counters and flagB. When it
//     is clear, one 24-byte frame entry is built for the local player's ring
//     for each engineGetAIStepCount() step.
//  7. Per-partner send cycle: for partners that are unflagged, when
//     packetSendCycleTime matches the slot, an encoded packet is built from
//     the local ring (see below) and sent with sendControlPacket(..., 0xbd3f,
//     ...); packetSendCycleTime then wraps at packetSendCycleLength.
//  8. A leaver message is shown for partners marked as gone, frame availability
//     decides how many real steps advanceRealState performs, netTrouble is
//     maintained, runPrediction(stall) runs, and the local pad record is
//     restored from netLocalPad.
// Encoding of one packet: a bit writer over a 16-bit-unit buffer puts the
// first button halfword in 16 bits, then run-length/changed-bit codes for the
// button halfword of each frame (5-bit run count, 4-bit bit index), then four
// analCompress calls over the analog bytes at offsets 2, 3, 4 and 5, then a
// delta coder for the yaw words (5-bit run, 1-bit escape, 16-bit absolute or
// 9-bit clamped delta), then a run-length code for yaw bit 0 and the 16-bit
// marker 0xa5a5.
// Inferred: the loop as a whole is the lock-step network frame step. The meaning
// of most fields is not established.
// Unknown: what is uninitialised in the image (word 16 of each new ring entry is
// not written here), the exact types behind the compression object, and the
// receive-side layout.
typedef int __attribute__((may_alias)) Int;
typedef unsigned short __attribute__((may_alias)) Half;

struct CCompress;
struct Camera;

extern "C" {
extern int nmPartners;
extern unsigned char netNmPartners;
extern unsigned char partners[];
extern int trueNetFrameNm;
extern int localNetFrameNm;
extern int localNetSentFrameNm;
extern int noAdvanceTime;
extern int msgBufferSent;
extern unsigned char kickState[];
extern int broadcastConnection;
extern int broadcastConnectionFlags;
extern int drawFrameNm;
extern unsigned char cd[];
extern unsigned char g_net_buf[];
extern int packetSendCycleLength;
extern int packetSendCycleTime;
extern signed char isModem;
extern signed char totallyDisconnected;
extern int netTrouble;
extern unsigned char pad[];
extern unsigned char netLocalPad[];
extern unsigned char netLocalPadState[];
extern unsigned char padBlank[];
extern Camera* gameCam;
extern unsigned char player[];

void* memset(void* destination, int value, unsigned long length);
int netGetLocalPlayerNm__Fi(int index);
void receivePackets__Fv();
int sceInetSendTo(void* object, void* buffer, int connection, void* data, int length, int flags,
                  void* address, int first_limit, int second_limit);
void sendControlPacket__FiiiiiR9CCompressb(int partner, int first, int second, int header, int local,
                                           CCompress& compression, bool flag);
int padGetUnprocessedData__FiP18padUnprocessedData(int index, void* data);
int GetYaw__6Camera(Camera* camera);
void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void* pad_record, void* data, void* state);
int engineGetAIStepCount__Fv();
void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(void* compression, void* writer,
                                                                          void* state, unsigned char* bytes,
                                                                          int count);
void ustrcpy__FPUsPc(unsigned short* destination, char* source);
char* textGetString__FPc(char* key);
void ustrcat__FPUsT0(unsigned short* destination, char* source);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int color, char flag);
void advanceRealState__Fi(int steps);
void runPrediction__Fi(int stall);
}

namespace {
const int kRecord = 17632;
const int kEntries = 172;
const int kEntry = 24;

struct BitWriter {
    unsigned short* data;
    int position;
};

struct Frame {
    unsigned char bytes[12];
};

struct PadRecord {
    unsigned char bytes[88];
};

struct Slot {
    unsigned first;
    unsigned second;
    float value;
    unsigned fourth;
};

struct Compression {
    Slot slot[4];
    unsigned tail[3];
};

struct StackTemplate {
    Compression object;
    unsigned spare;
};

const int kTable[7] = {6, 2, 4, 6, 6, 6, 6};
int count_state;
int min_steps_to_take = 100;
int min_steps_timer;

inline Int& word(unsigned char* base, int offset) { return *reinterpret_cast<Int*>(base + offset); }
inline unsigned char* record_at(int index) { return partners + index * kRecord; }

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
}

extern "C" void netGameStep__FbT0(bool flagA, bool flagB) {
    int localPlayer = netGetLocalPlayerNm__Fi(0);
    netNmPartners = static_cast<unsigned char>(nmPartners);
    receivePackets__Fv();
    int n = nmPartners;

    // 2. pings
    if (!flagA) {
        for (int i = 0; i < n; ++i) {
            unsigned char* record = record_at(i);
            int old = word(record, 17628);
            if (old == 0) continue;
            word(record, 17628) = old + 1;
            if (!(old < 121)) word(record, 17628) = 0;
            if (old % 10 != 0) continue;
            int header = 0xbd43;
            unsigned char block[16];
            int limit = word(record, 4);
            broadcastConnectionFlags = 0;
            memset(block, 0, 16);
            for (int k = 0; k < 4; ++k) block[4 + k] = record[k];
            sceInetSendTo(cd, g_net_buf, broadcastConnection, &header, 4, 0, block, limit, 1000);
            n = nmPartners;
        }
    }

    if (localPlayer == 0) {
        // 3a. timeouts
        for (int i = 0; i < n; ++i) {
            unsigned char* record = record_at(i);
            if (static_cast<signed char>(record[17620]) != 0) continue;
            if (static_cast<signed char>(record[17621]) != 0) continue;
            if (static_cast<signed char>(record[17622]) != 0) continue;
            int old = word(record, 17612);
            word(record, 17612) = old + 1;
            if (old < 901) continue;
            record[17622] = 1;
            word(record, 17624) = -1;
            memset(kickState, 0, 8);
            word(kickState, 0) = 1;
            n = nmPartners;
        }
        // 3b. mask packets
        if (word(kickState, 0) == 1 && drawFrameNm % 30 == 0) {
            bool none_sent = true;
            for (int index = 0; index < n; ++index) {
                unsigned char* record = record_at(index);
                if (static_cast<signed char>(record[17620]) != 0) continue;
                if (static_cast<signed char>(record[17621]) != 0) continue;
                if (static_cast<signed char>(record[17622]) != 0) continue;
                if (static_cast<signed char>(kickState[4 + index]) != 0) continue;
                alignas(16) unsigned char packet[16];
                word(packet, 0) = 0xbd41;
                int mask = 0;
                int count = nmPartners;
                for (int k = 0; k < count; ++k) {
                    int flag = static_cast<signed char>(record_at(k)[17622]);
                    if (flag == 1) mask |= flag << k;
                }
                word(packet, 4) = mask;
                unsigned char block[16];
                broadcastConnectionFlags = 0;
                memset(block, 0, 16);
                int limit = word(record, 4);
                for (int k = 0; k < 4; ++k) block[4 + k] = record[k];
                sceInetSendTo(cd, g_net_buf, broadcastConnection, packet, 8, 0, block, limit, 1000);
                none_sent = false;
                n = nmPartners;
            }
            if (none_sent) {
                word(kickState, 0) = 2;
                for (int i = 0; i < n; ++i) {
                    unsigned char* record = record_at(i);
                    if (static_cast<signed char>(record[17622]) != 0) word(record, 17624) = word(record, 17596) + 1;
                }
            }
        }
        // 3c. control packets
        if (word(kickState, 0) == 2 && drawFrameNm % 30 == 0) {
            bool none_sent = true;
            for (int i = 0; i < n; ++i) {
                unsigned char* record = record_at(i);
                if (static_cast<signed char>(record[17620]) != 0) continue;
                if (static_cast<signed char>(record[17621]) != 0) continue;
                if (static_cast<signed char>(record[17622]) != 0) continue;
                for (int j = 0; j < n; ++j) {
                    unsigned char* other = record_at(j);
                    if (static_cast<signed char>(other[17620]) != 0) continue;
                    if (static_cast<signed char>(other[17622]) != 1) continue;
                    if (word(other, 17624) < word(record, 17608)) continue;
                    StackTemplate stack;
                    memset(&stack.object.slot[0], 0, 16);
                    none_sent = false;
                    memset(&stack.object.slot[1], 0, 16);
                    memset(&stack.object.slot[2], 0, 16);
                    memset(&stack.object.slot[3], 0, 16);
                    memset(&stack.object.tail, 0, 16);
                    int latest = word(other, 17596);
                    int earlier = latest - 40;
                    if (earlier < 0) earlier = 0;
                    stack.object.slot[0].value = 128.0f;
                    stack.object.slot[1].value = 128.0f;
                    stack.object.slot[2].value = 128.0f;
                    stack.object.slot[3].value = 128.0f;
                    stack.object.tail[2] = 0;
                    sendControlPacket__FiiiiiR9CCompressb(i, earlier, latest, 0xbd42, j,
                                                           *reinterpret_cast<CCompress*>(&stack.object), true);
                    n = nmPartners;
                }
            }
            if (none_sent) {
                if (n <= 0) {
                    word(kickState, 0) = 0;
                } else {
                    for (int i = 0; i < n; ++i) {
                        unsigned char* record = record_at(i);
                        if (static_cast<signed char>(record[17622]) == 1) record[17622] = 2;
                    }
                    word(kickState, 0) = 0;
                }
            }
        }
    }

    // 4. bounds over unflagged partners
    int ahead = localNetFrameNm - trueNetFrameNm;
    int low_frame = trueNetFrameNm;
    int bound = ahead;
    for (int i = 0; i < n; ++i) {
        unsigned char* record = record_at(i);
        if (static_cast<signed char>(record[17620]) != 0) continue;
        if (static_cast<signed char>(record[17621]) != 0) continue;
        if (static_cast<signed char>(record[17622]) != 0) continue;
        int first = word(record, 17604);
        int second = word(record, 17608);
        if (!(bound < first)) bound = first;
        if (!(low_frame < second)) low_frame = second;
    }

    // 5. local input
    alignas(8) unsigned char data[16];
    int has_input = padGetUnprocessedData__FiP18padUnprocessedData(0, data);
    int since = ahead - bound;
    int* pad_counter = reinterpret_cast<int*>(pad + 4);
    if (has_input != 0) {
        *pad_counter = 30;
    } else {
        int v = *pad_counter - 1;
        if (v < 0) v = 0;
        *pad_counter = v;
    }
    int yaw = GetYaw__6Camera(gameCam);
    int yaw_word = static_cast<short>(yaw & -2);
    word(data, 8) = yaw_word;
    if ((*reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(gameCam) + 52) ^ 1) != 0) word(data, 8) = yaw_word | 1;
    padProcessData__FP3padP18padUnprocessedDataP12padStateData(netLocalPad + localPlayer * 88, data,
                                                               netLocalPadState + localPlayer * 8);
    {
        unsigned char* blank = padBlank + localPlayer * 12;
        int remaining = word(blank, 0);
        if (remaining != 0) {
            word(blank, 0) = remaining - 1;
            unsigned short buttons = *reinterpret_cast<Half*>(data);
            unsigned short mask = *reinterpret_cast<Half*>(blank + 4);
            int flag = word(blank, 8);
            data[6] = 0;
            data[7] = 0;
            *reinterpret_cast<Half*>(data) = static_cast<unsigned short>(buttons & ~mask);
            if (flag != 0) {
                data[3] = 0x80;
                data[4] = 0x80;
                data[5] = 0x80;
                data[2] = 0x80;
            }
        }
    }

    // 6. stall flag and local ring
    int local_frame = localNetFrameNm;
    int stall = !((local_frame - low_frame) < 39);
    if (since < 5) {
        count_state = 0;
    } else {
        int next = count_state + 1;
        if (next < 31) {
            count_state = next;
        } else {
            count_state = 0;
            stall = 1;
        }
    }
    if (flagB) stall = 1;
    int timer = min_steps_timer;
    min_steps_timer = timer + 1;
    if (!(timer < 61)) {
        int previous = min_steps_to_take;
        min_steps_to_take = 100;
        min_steps_timer = 0;
        if (!(previous < 3)) stall = 1;
    }
    if (local_frame < 3) stall = 0;

    unsigned char* local_record = record_at(localPlayer);
    if (!stall) {
        for (int step = 0; step < engineGetAIStepCount__Fv(); ++step) {
            int frame = localNetFrameNm;
            unsigned char* entry = local_record + kEntries + (frame & 0xff) * kEntry;
            word(entry, 0) = frame;
            for (int k = 0; k < 12; ++k) entry[4 + k] = data[k];
            word(entry, 20) = 0;
            localNetFrameNm = frame + 1;
        }
    }

    // message bookkeeping
    local_frame = localNetFrameNm;
    if (localNetSentFrameNm < local_frame - 1) {
        int sent = msgBufferSent;
        if (word(local_record, 17580) != sent) {
            int next = localNetSentFrameNm + 1;
            if (next < trueNetFrameNm) next = trueNetFrameNm;
            unsigned char* entry = local_record + kEntries + (next & 0xff) * kEntry;
            int before = word(entry, 20);
            if (before == 0) word(entry, 16) = sent;
            word(entry, 20) = before + ((word(local_record, 17580) - sent) & 0x1fff);
            msgBufferSent = word(local_record, 17580);
        }
    }
    n = nmPartners;

    // send cycle
    int active = 0;
    for (int i = 0; i < n; ++i)
        if ((word(record_at(i), 17620) & 0xffffff) == 0) ++active;
    int length = kTable[active];
    if (isModem != 0) length += length >> 1;
    packetSendCycleLength = length;
    int sent_index = 0;
    for (int i = 0; i < n; ++i) {
        length = packetSendCycleLength;
        int cycle_time = packetSendCycleTime;
        unsigned char* record = record_at(i);
        if ((word(record, 17620) & 0xffffff) != 0) continue;
        if (cycle_time != (sent_index * length) / active) {
            ++sent_index;
            continue;
        }
        BitWriter writer;
        unsigned short buffer[2048];
        int end_frame = word(record, 17600);
        buffer[0] = 0;
        int start_frame = word(record, 92);
        int count = end_frame - start_frame;
        writer.position = 0;
        ++sent_index;
        buffer[1] = 0;
        writer.data = buffer;
        unsigned char* compression = record + 96;
        if (count != 0) {
            Frame frames[count];
            for (int f = start_frame; f < end_frame; ++f) {
                const unsigned char* source = local_record + (f & 0xff) * kEntry + 176;
                for (int k = 0; k < 12; ++k) frames[f - start_frame].bytes[k] = source[k];
            }
            word(compression, 72) = word(compression, 72) + count;
            put(&writer, *reinterpret_cast<Half*>(frames[0].bytes), 16);
            {
                unsigned previous = *reinterpret_cast<Half*>(frames[0].bytes);
                int run = 0;
                for (int k = 0; k < count; ++k) {
                    unsigned current = *reinterpret_cast<Half*>(frames[k].bytes);
                    if (current == previous) {
                        ++run;
                    } else {
                        do {
                            put(&writer, run & 31, 5);
                            run = 0;
                            int bit = 0;
                            while (bit < 16 && !(((current ^ previous) >> bit) & 1)) ++bit;
                            if (bit < 16) {
                                put(&writer, bit & 15, 4);
                                previous ^= 1u << bit;
                            }
                        } while (current != previous);
                        ++run;
                    }
                    if (run == 31) {
                        put(&writer, 31, 5);
                        run = 0;
                    }
                }
                if (run != 0) put(&writer, run & 31, 5);
            }
            static const int kColumn[4] = {2, 3, 4, 5};
            static const int kState[4] = {32, 48, 0, 16};
            for (int c = 0; c < 4; ++c) {
                unsigned char column[count];
                for (int k = 0; k < count; ++k) column[k] = frames[k].bytes[kColumn[c]];
                analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(
                    compression, &writer, compression + kState[c], column, count);
            }
            {
                int run = 0;
                for (int k = 0; k < count; ++k) {
                    int yaw_in = word(frames[k].bytes, 8);
                    int target = yaw_in & -2;
                    int delta = target - static_cast<short>(*reinterpret_cast<Half*>(compression + 64));
                    int magnitude = delta < 0 ? -delta : delta;
                    *reinterpret_cast<Half*>(compression + 64) = static_cast<unsigned short>(target);
                    bool escape = true;
                    int smooth = 0;
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
                        continue;
                    }
                    int second_magnitude = second < 0 ? -second : second;
                    smooth = delta;
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
                int current = word(frames[0].bytes, 8) & 1;
                put(&writer, current, 1);
                for (int k = 0; k < count; ++k) {
                    if ((word(frames[k].bytes, 8) & 1) != current) {
                        put(&writer, run & 31, 5);
                        run = 0;
                        current = word(frames[k].bytes, 8) & 1;
                    }
                    ++run;
                    if (run == 31) {
                        put(&writer, 31, 5);
                        run = 0;
                    }
                }
                if (run != 0) put(&writer, run & 31, 5);
            }
            put(&writer, 0xa5a5, 16);
            word(record, 92) = end_frame;
        }
        sendControlPacket__FiiiiiR9CCompressb(i, end_frame, localNetFrameNm - 1, 0xbd3f, localPlayer,
                                               *reinterpret_cast<CCompress*>(compression), false);
        localNetSentFrameNm = localNetFrameNm;
        n = nmPartners;
    }
    int next_time = packetSendCycleTime + 1;
    if (!(next_time < packetSendCycleLength)) next_time = 0;
    packetSendCycleTime = next_time;

    // leavers
    for (int i = 0; i < n; ++i) {
        unsigned char* record = record_at(i) + 17616;
        if (static_cast<signed char>(record[6]) == 0) continue;
        if (static_cast<signed char>(record[5]) != 0) continue;
        if (word(record, 8) != trueNetFrameNm) continue;
        unsigned short text[256];
        ustrcpy__FPUsPc(text, reinterpret_cast<char*>(player + i * 23984));
        char* message = textGetString__FPc(const_cast<char*>("netMessagePlayerExit"));
        ustrcat__FPUsT0(text, message);
        hudSetInfoMessage__FPUsiic(text, 300, -1, 0);
        record[5] = 1;
        n = nmPartners;
    }

    // frame availability and real steps
    int available = (localNetFrameNm - trueNetFrameNm) - 1;
    int steps = 0;
    if (available > 0) {
        for (; steps < available; ++steps) {
            int frame = trueNetFrameNm + steps;
            int a1 = 0;
            for (; a1 < n; ++a1) {
                unsigned char* record = record_at(a1);
                if (static_cast<signed char>(record[17621]) != 0) continue;
                if (word(record, kEntries + (frame & 0xff) * kEntry) != frame) break;
            }
            if (a1 < n) break;
        }
    }
    {
        int minimum = min_steps_to_take;
        if (!(minimum < steps)) minimum = steps;
        min_steps_to_take = minimum;
    }
    {
        int ai = engineGetAIStepCount__Fv();
        if (!(steps < ai)) steps = ai;
    }
    if (flagA) steps = 0;
    if (totallyDisconnected == 0) {
        if (static_cast<signed char>(record_at(0)[17621]) != 0) steps = 0;
    }
    if (steps > 0) {
        noAdvanceTime = 0;
        advanceRealState__Fi(steps);
    } else {
        noAdvanceTime = noAdvanceTime + 1;
    }
    if (static_cast<unsigned char>(totallyDisconnected) != 0) {
        netTrouble = 0;
    } else if (noAdvanceTime < 301 && static_cast<signed char>(record_at(0)[17621]) == 0) {
        netTrouble = 0;
    } else {
        netTrouble = netTrouble + 1;
    }
    runPrediction__Fi(stall);

    // restore the local pad record, keeping its counter word
    PadRecord* destination = reinterpret_cast<PadRecord*>(pad + localPlayer * 88);
    int kept = *reinterpret_cast<Int*>(pad + localPlayer * 88 + 4);
    *destination = *reinterpret_cast<PadRecord*>(netLocalPad + localPlayer * 88);
    *reinterpret_cast<Int*>(pad + localPlayer * 88 + 4) = kept;
}

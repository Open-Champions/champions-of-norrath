// Observed commProcessReal__Fv at 0x002cc4a8, 1992 bytes, no arguments, no result.
// Outline of the observed behavior. It returns at once when the signed byte
// netNetworkMode is zero or the word frontEndRunning is nonzero. It updates
// commEnable: if it was nonzero the new value is 1 when netIsModem() returns
// zero and 0 otherwise. lastlastCE takes the old lastCE; lastCE and commEnable
// take the new value.
// While comm_handle is -1 and commEnable is nonzero, it calls lgAudEnumHint into
// a local buffer and, if the first word is nonzero or lastCE differs from
// lastlastCE, walks lgAudEnumerate(i, info) until it returns 0x80000002. The
// info buffer holds a count byte, a pad byte and 6-byte entries; a second list
// follows 98 bytes after the first count. An entry is 'eligible' when its first
// two bytes are 1 and 16, its first halfword is at most 8000 and its second is at
// least 8000; the last eligible index of each list is kept. When both lists have
// one, it calls lgAudOpen(i, params, &comm_handle, 200) where params holds the
// byte 3 and two 6-byte records {1, 16, 8000, 200} and {1, 16, 8000, 100}; on
// failure comm_handle is set back to -1 and the walk continues. Once a handle
// exists it applies lgAudSetRecordingVolume(handle, 0, low byte of commRVInternal),
// starts recording and playback, clears the 64 flag bytes of packetbuffer, sends
// the one-byte message ((1 << netGetLocalPlayerNmSafe(0)) | 0x80) through
// commStatusHandle, and clears the stream counters (framestart takes drawFrameNm).
// With a handle it re-applies the recording volume when commRVInternal differs
// from commRVInternalLast, then dispatches on buffrmode (0..4, else returns):
//   0: if commEnable, request 1280 - buffrfull bytes with lgAudARead into buffr;
//      a nonzero result (or commEnable zero) closes the device. Then as 1.
//   1: lgAudASync; busy returns, nonzero status closes. A short read stores the
//      new fill and returns to 0. A complete 1280-byte block adds to readcount,
//      takes the largest absolute value of the 640 halfwords doubled, stores
//      commCVol = max * 2^-15 * 0.5, sets timeout to 5 when commRVThresh is below
//      max, and while timeout is nonzero decrements it, encodes the block with
//      lgCodecEncode and sends 133 bytes (132 encoded plus the local player byte)
//      with netSendPacket(commHandle, commSendmask, ..., 133). Then as 2.
//   2: for each of 4 senders, if the flag byte of slot packetshift % 16 is set it
//      sets that sender's bit in commNameMask, decodes the packet into buffr and
//      adds the 640 halfwords into a 32-bit accumulator. No sender: buffr is
//      zeroed. Otherwise each sum times commPV is divided by 32768 (truncating)
//      and clamped to 16 bits into buffr. All four flags of the slot are cleared,
//      buffrmode is 3 and packetshift advances modulo 16. Then as 3.
//   3: lgAudAWrite the remaining bytes; zero starts mode 4 at once, nonzero closes.
//   4: lgAudASync; as 1 for busy and status; a short write stores the fill and
//      goes to 3; a complete block adds to writecount, clears buffrfull, zeroes
//      buffr, sets buffrmode 0 and continues as 0.
// Closing is lgAudClose, comm_handle = -1, a status message ((1 << local player))
// and commNameMask = 0.
// Inferred: this is a voice-chat pump. Unknown: names of the record fields, the
// meaning of timeout beyond 5 passes, and the owners of every global used here.
extern "C" {
extern signed char netNetworkMode;
extern int frontEndRunning;
extern int commEnable;
extern int lastCE;
extern int lastlastCE;
extern int comm_handle;
extern int commRVInternal;
extern int commRVInternalLast;
extern int commRVThresh;
extern int commPV;
extern int commSendmask;
extern int commHandle;
extern int commStatusHandle;
extern int commNameMask;
extern int codec;
extern int timeout;
extern int packetshift;
extern int buffrmode;
extern int buffrfull;
extern int buffrout;
extern int readcount;
extern int writecount;
extern int framestart;
extern int drawFrameNm;
extern float commCVol;
extern unsigned char buffr[1280];
extern unsigned char packetbuffer[8512];

int netIsModem__Fv();
int netGetLocalPlayerNmSafe__Fi(int index);
void netSendMessage__FiPUci(int handler, unsigned char* data, int size);
void netSendPacket__FiiPUci(int handle, int mask, unsigned char* data, int size);
void lgAudEnumHint(unsigned char* hint);
unsigned lgAudEnumerate(int index, unsigned char* info);
int lgAudOpen(int index, unsigned char* params, int* handle, int value);
int lgAudSetRecordingVolume(int handle, int channel, int volume);
int lgAudStartRecording(int handle);
int lgAudStartPlayback(int handle);
int lgAudASync(int wait, int* status);
int lgAudARead(int handle, int flag, unsigned char* buffer, int* count);
int lgAudAWrite(int handle, int flag, unsigned char* buffer, int* count);
int lgAudClose(int handle);
int lgCodecEncode(int coder, unsigned char* in, int inSize, unsigned char* out, int* outSize);
int lgCodecDecode(int coder, unsigned char* in, int inSize, unsigned char* out, int* outSize);
void* memset(void* destination, int value, unsigned long size);
}

namespace {
struct Format {
    unsigned char first;
    unsigned char second;
    unsigned short rate;
    unsigned short other;
};
struct OpenParams {
    unsigned char kind;
    Format input;
    Format output;
};

unsigned short half_at(const unsigned char* base, int offset) {
    return *reinterpret_cast<const unsigned short*>(base + offset);
}

int last_eligible(const unsigned char* count_at, int first_entry) {
    int found = -1;
    for (int i = 0; i < count_at[0]; ++i) {
        const unsigned char* entry = count_at + first_entry + 6 * i;
        if (entry[0] == 1 && entry[1] == 16 && half_at(entry, 2) <= 8000 && half_at(entry, 4) >= 8000) found = i;
    }
    return found;
}

void send_status(int bits) {
    unsigned char message = static_cast<unsigned char>(bits);
    netSendMessage__FiPUci(commStatusHandle, &message, 1);
}

void close_device() {
    lgAudClose(comm_handle);
    comm_handle = -1;
    int local = netGetLocalPlayerNmSafe__Fi(0);
    send_status(1 << local);
    commNameMask = 0;
}
}

extern "C" void commProcessReal__Fv() {
    if (netNetworkMode == 0) return;
    if (frontEndRunning != 0) return;

    int enable = 0;
    if (commEnable != 0) enable = netIsModem__Fv() == 0;
    lastlastCE = lastCE;
    commEnable = enable;
    lastCE = enable;

    if (comm_handle == -1) {
        if (enable == 0) return;
        alignas(16) unsigned char hint[16];
        alignas(16) unsigned char info[212];
        lgAudEnumHint(hint);
        if (*reinterpret_cast<int*>(hint) == 0 && lastCE == lastlastCE) return;
        int index = 0;
        for (;;) {
            if (lgAudEnumerate(index, info) == 0x80000002u) break;
            int first = last_eligible(info, 2);
            int second = last_eligible(info + 98, 2);
            if (second == -1) {
                ++index;
                continue;
            }
            if (first != -1) {
                OpenParams params;
                params.kind = 3;
                params.input.first = 1;
                params.input.second = 16;
                params.input.rate = 8000;
                params.input.other = 200;
                params.output = params.input;
                params.output.other = 100;
                if (lgAudOpen(index, reinterpret_cast<unsigned char*>(&params), &comm_handle, 200) == 0) break;
                comm_handle = -1;
            }
            ++index;
        }
        if (comm_handle == -1) return;
        lgAudSetRecordingVolume(comm_handle, 0, static_cast<unsigned char>(commRVInternal));
        lgAudStartRecording(comm_handle);
        lgAudStartPlayback(comm_handle);
        for (int sender = 0; sender < 4; ++sender)
            for (int slot = 15; slot >= 0; --slot) packetbuffer[sender * 2128 + slot * 133 + 132] = 0;
        timeout = 0;
        int local = netGetLocalPlayerNmSafe__Fi(0);
        send_status((1 << local) | 0x80);
        commNameMask = 0;
        buffrout = 0;
        framestart = drawFrameNm;
        buffrmode = 0;
        readcount = 0;
        writecount = 0;
        buffrfull = 0;
    }

    if (commRVInternal != commRVInternalLast) {
        int volume = commRVInternal;
        commRVInternalLast = volume;
        lgAudSetRecordingVolume(comm_handle, 0, static_cast<unsigned char>(commRVInternal));
    }

    if (static_cast<unsigned>(buffrmode) >= 5) return;
    int status[4];
    int mode = buffrmode;
    for (;;) {
        if (mode == 4) {
            if (lgAudASync(1, status) != 0) return;
            if (status[0] != 0) break;
            int fill = buffrfull + buffrout;
            if (fill != 1280) {
                buffrfull = fill;
                buffrmode = 3;
                return;
            }
            writecount = writecount + fill;
            buffrfull = 0;
            memset(buffr, 0, 1280);
            buffrmode = 0;
            mode = 0;
        }
        if (mode == 0) {
            int result = 0x80000000;
            if (commEnable != 0) {
                buffrout = 1280 - buffrfull;
                result = lgAudARead(comm_handle, 1, buffr + buffrfull, &buffrout);
            }
            if (result != 0) break;
            buffrmode = 1;
            mode = 1;
        }
        if (mode == 1) {
            if (lgAudASync(1, status) != 0) return;
            if (status[0] != 0) break;
            int fill = buffrfull + buffrout;
            if (fill != 1280) {
                buffrfull = fill;
                buffrmode = 0;
                return;
            }
            buffrfull = 0;
            readcount = readcount + fill;
            int largest = 0;
            for (int i = 0; i < 640; ++i) {
                int value = reinterpret_cast<short*>(buffr)[i] * 2;
                if (value < 0) value = -value;
                if (value >= largest) largest = value;
            }
            commCVol = static_cast<float>(largest) * __builtin_bit_cast(float, 0x38000000u) *
                       __builtin_bit_cast(float, 0x3f000000u);
            if (commRVThresh < largest) timeout = 5;
            if (timeout != 0) {
                timeout = timeout - 1;
                unsigned char packet[136];
                int size = 132;
                lgCodecEncode(codec, buffr, 1280, packet, &size);
                packet[132] = static_cast<unsigned char>(netGetLocalPlayerNmSafe__Fi(0));
                netSendPacket__FiiPUci(commHandle, commSendmask, packet, 133);
            }
            buffrmode = 2;
            mode = 2;
        }
        if (mode == 2) {
            commNameMask = 0;
            int mixed = 0;
            int sums[640];
            for (int sender = 0; sender < 4; ++sender) {
                unsigned char* slot = packetbuffer + sender * 2128 + (packetshift % 16) * 133;
                if (static_cast<signed char>(slot[132]) != 0) {
                    commNameMask = commNameMask | (1 << sender);
                    int size = 1280;
                    lgCodecDecode(codec, slot, 132, buffr, &size);
                    if (mixed == 0) memset(sums, 0, 2560);
                    for (int i = 0; i < 640; ++i) sums[i] += reinterpret_cast<short*>(buffr)[i];
                    ++mixed;
                }
            }
            if (mixed == 0) {
                memset(buffr, 0, 1280);
            } else {
                for (int i = 0; i < 640; ++i) {
                    int scaled = sums[i] * commPV;
                    if (scaled < 0) scaled += 32767;
                    int out = scaled >> 15;
                    if (out < -32768) out = -32768;
                    else if (out > 32767) out = 32767;
                    reinterpret_cast<short*>(buffr)[i] = static_cast<short>(out);
                }
            }
            for (int sender = 0; sender < 4; ++sender)
                packetbuffer[sender * 2128 + (packetshift % 16) * 133 + 132] = 0;
            buffrmode = 3;
            packetshift = packetshift + 1;
            packetshift = packetshift % 16;
            mode = 3;
        }
        // mode 3
        int fill = buffrfull;
        buffrout = 1280 - fill;
        if (lgAudAWrite(comm_handle, 1, buffr + fill, &buffrout) != 0) break;
        buffrmode = 4;
        mode = 4;
    }
    close_device();
}

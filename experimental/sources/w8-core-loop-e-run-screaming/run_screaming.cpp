// Observed runScreaming__Fv at 0x0024b090, 1244 bytes.
// It drives the three 452-byte stream records (`stream`, 0x007dd290) through
// the IOP streaming service. The file-local words are loadDone (a byte,
// image value 1), loadInProgress (byte), loadVoice, loadAmount,
// loadSampleRate, loadStereo and preloadAmount (words, zero at load). The
// record words used are: byte +0 (a file name), +16 position, +20 remaining,
// +24 end, +28 length, +32 flags, +36 offset, +44 a destination, +48 chunk
// size, +52 sample rate, +56 stereo, +60 loop size, +64 a second file name,
// +80 and +84 its offset and remaining size, +448 the second-file count.
// 1. Returns when cdIsBusy is nonzero or loadDone is zero.
// 2. With loadInProgress set (cleared here): the finished load's voice, amount
//    and preload amount are taken. The record's position gains the amount and
//    its remaining drops by it; atEnd is whether the end word equalled the old
//    position. A positive preload amount with the first name set, or with a
//    second-file count and second name set, is cleared and added to the
//    record's offset word. A record with a first name sends the 16-byte
//    message {voice, amount, sampleRate, stereo (16 bits), atEnd (16 bits)}
//    through sbuffin with RPC 64 when the amount is positive. Then, for the
//    voice re-read, a record whose remaining is zero and whose flags have
//    bit 0 set restarts: position = end + ((loop size << 1) & -2048) and
//    remaining = length - that value.
// 3. RPC 80 is called with no payload and sbuff receives three 16-byte
//    records, kept as local copies. runStreamQueing may end the call early.
// 4. For each record with a nonzero +44, the copied first two words give a
//    position that is reduced modulo the chunk size to s0 (a multiple of 2048,
//    capped at the bytes to the chunk end); a value below half a chunk skips
//    the record. A first-file load (name set, remaining positive, present on
//    the current layer) is started with cdLoadAsyncIOP for the smaller of
//    remaining and s0, and the function ends after recording the voice, amount,
//    rate, stereo and a zero preload. Otherwise a record with flags bit 0
//    clear, a second-file count, a second name, a positive second remaining
//    and the file present on the layer starts the second-file load; its
//    offset and remaining advance by the amount, the preload amount becomes
//    the amount, and loadAmount is zero.
// Unknown: the word meanings above are inferred from use only. The RPC
// numbers 64 and 80 are copied. What the three received records hold beyond
// their first two words is unknown.
extern "C" {
int cdIsBusy__Fv();
int runStreamQueing__Fv();
int cdCheckForFileInCurrentLayer__FPc(char* name);
void cdLoadAsyncIOP__FPciT0ii(char* name, int destination, char* done, int size, int offset);
int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                  void* receive, int receive_size, void* completion, void* completion_argument);
extern unsigned char stream[];
extern unsigned char gCd[];
extern unsigned char sbuff[];
extern unsigned char sbuffin[];
}

namespace {
__attribute__((always_inline)) inline int word_at(const unsigned char* p) {
    return static_cast<int>(p[0] | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) |
                            (unsigned(p[3]) << 24));
}
__attribute__((always_inline)) inline void set_word(unsigned char* p, int value) {
    const unsigned v = static_cast<unsigned>(value);
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16);
    p[3] = static_cast<unsigned char>(v >> 24);
}
signed char loadDone = 1;
signed char loadInProgress;
int loadVoice;
int loadAmount;
int loadSampleRate;
int loadStereo;
int preloadAmount;
}

extern "C" void runScreaming__Fv() {
    if (cdIsBusy__Fv() != 0) return;
    if (loadDone == 0) return;
    if (loadInProgress != 0) {
        int voice = loadVoice;
        loadInProgress = 0;
        const int amount = loadAmount;
        const int preload = preloadAmount;
        unsigned char* record = stream + voice * 452;
        const int old_position = word_at(record + 16);
        set_word(record + 16, old_position + amount);
        const int at_end = word_at(record + 24) == old_position ? 1 : 0;
        set_word(record + 20, word_at(record + 20) - amount);
        if (preload > 0) {
            if (record[0] != 0 || (word_at(record + 448) != 0 && record[64] != 0)) {
                preloadAmount = 0;
                set_word(record + 36, word_at(record + 36) + preload);
            }
        }
        record = stream + voice * 452;
        if (record[0] != 0) {
            unsigned char message[16];
            set_word(message, voice);
            set_word(message + 4, amount);
            set_word(message + 8, loadSampleRate);
            message[12] = static_cast<unsigned char>(loadStereo);
            message[13] = static_cast<unsigned char>(loadStereo >> 8);
            message[14] = static_cast<unsigned char>(at_end);
            message[15] = static_cast<unsigned char>(at_end >> 8);
            if (amount > 0) {
                for (int i = 0; i < 16; ++i) sbuffin[i] = message[i];
                sceSifCallRpc(gCd, 64, 0, sbuffin, 16, sbuff, 64, nullptr, nullptr);
                voice = loadVoice;
            }
            record = stream + voice * 452;
            if (word_at(record + 20) == 0 && (word_at(record + 32) & 1) != 0) {
                const int loop = (word_at(record + 60) << 1) & -2048;
                set_word(record + 16, word_at(record + 24) + loop);
                set_word(record + 20, word_at(record + 28) - loop);
            }
        }
    }
    sceSifCallRpc(gCd, 80, 0, sbuffin, 0, sbuff, 64, nullptr, nullptr);
    int received[3][4];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j) received[i][j] = word_at(sbuff + i * 16 + j * 4);
    if (runStreamQueing__Fv() != 0) return;
    for (int voice = 0; voice < 3; ++voice) {
        unsigned char* const record = stream + voice * 452;
        if (word_at(record + 44) == 0) continue;
        const int chunk = word_at(record + 48);
        const int position = (received[voice][0] + word_at(record + 36)) % chunk;
        int available = chunk - ((position - received[voice][1] + chunk) % chunk) - 2048;
        if (available < (chunk >> 1)) continue;
        const int to_end = chunk - position;
        if (to_end < available) available = to_end;
        available &= -2048;
        if (available <= 0) continue;
        if (record[0] != 0 && word_at(record + 20) > 0 &&
            cdCheckForFileInCurrentLayer__FPc(reinterpret_cast<char*>(record)) != 0) {
            if (word_at(record + 20) < available) available = word_at(record + 20);
            cdLoadAsyncIOP__FPciT0ii(reinterpret_cast<char*>(record), word_at(record + 44) + position,
                                     reinterpret_cast<char*>(&loadDone), (available + 2047) & -2048,
                                     word_at(record + 16));
            loadInProgress = 1;
            loadSampleRate = word_at(record + 52);
            loadStereo = word_at(record + 56);
            loadVoice = voice;
            loadAmount = available;
            preloadAmount = 0;
            return;
        }
        if ((word_at(record + 32) & 1) != 0) continue;
        if (word_at(record + 448) <= 0) continue;
        if (record[64] == 0) continue;
        if (word_at(record + 84) <= 0) continue;
        if (cdCheckForFileInCurrentLayer__FPc(reinterpret_cast<char*>(record + 64)) == 0) continue;
        if (word_at(record + 84) < available) available = word_at(record + 84);
        cdLoadAsyncIOP__FPciT0ii(reinterpret_cast<char*>(record + 64), word_at(record + 44) + position,
                                 reinterpret_cast<char*>(&loadDone), (available + 2047) & -2048,
                                 word_at(record + 80));
        set_word(record + 80, word_at(record + 80) + available);
        set_word(record + 84, word_at(record + 84) - available);
        loadInProgress = 1;
        loadVoice = voice;
        loadSampleRate = word_at(record + 52);
        loadStereo = word_at(record + 56);
        preloadAmount = available;
        loadAmount = 0;
        return;
    }
}

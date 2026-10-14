// Observed: AllocateChannels__13VISoundDeviceii at 0x01125bc0, 412 bytes. Arguments: the device, a count and a priority number. The
// doubleword at device+0 is a mask of channels in use, bits 0 to 45. It returns a 64-bit mask (the image's v0 holds all 64 bits).
//  1. free = 46 - ChannelCount__13VISoundDeviceUl(device, mask). If free is not less than count, go to step 4.
//  2. need = count - free. First pass over the playback records (FirstPlayback__13VISoundDevice, NextPlayback__13VISoundDeviceP10VIPlayback):
//     skip a record when priority < the word at record+24; otherwise subtract ChannelCount(device, doubleword at record+16) from need
//     and stop the pass as soon as the remainder is not positive. If it is still positive after the pass, return 0 and change nothing.
//  3. Second pass with the same selection: the next record is fetched first; for each record with priority >= word at +24, call
//     ChannelCount for its doubleword at +16, then Stop__13VISoundDevicei(device, word at record+32), subtract, and stop once the
//     remainder is not positive.
//  4. Re-read the mask. Let result = 0 and found = 0. If bit 0 is clear: found = 1, result = 1, and if count is 1 go to the store.
//     Then for bit 1 to bit 45 in order, each clear bit is added to result and counted; the loop ends when found equals count (so a
//     count of zero or less takes every clear bit, and a count larger than what is free takes all that exist). Store mask | result at
//     device+0 and return result.
// Inferred: this reserves count channels from a 46-channel pool, evicting the playbacks whose priority number is at or below the
// requested one (smaller numbers appear to be stronger or equal) until enough channels are free. The record doubleword at +16 is the
// channel mask the playback holds and the word at +32 its stop handle, as in SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf.
// Unknown: priority numbering, why the first pass is separate from the stopping pass (it keeps the device untouched when the request
// cannot be met), and the meaning of the playback record fields beyond those used here.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
#endif

static inline unsigned long long mask_at(const unsigned* record, int byteOffset) {
    return static_cast<unsigned long long>(record[byteOffset / 4]) | (static_cast<unsigned long long>(record[byteOffset / 4 + 1]) << 32);
}

extern "C" {
unsigned FirstPlayback__13VISoundDevice(void* device);
unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void* device, unsigned playback);
int ChannelCount__13VISoundDeviceUl(void* device, unsigned long long mask);
void Stop__13VISoundDevicei(void* device, int voice);

unsigned long long AllocateChannels__13VISoundDeviceii(void* device, int count, int priority) {
    unsigned long long* inUse = static_cast<unsigned long long*>(device);
    int available = 46 - ChannelCount__13VISoundDeviceUl(device, *inUse);
    if (available < count) {
        int missing = count - available;
        int remaining = missing;
        for (unsigned p = FirstPlayback__13VISoundDevice(device); p; p = NextPlayback__13VISoundDeviceP10VIPlayback(device, p)) {
            const unsigned* record = words_at(p);
            if (priority < static_cast<int>(record[24 / 4])) continue;
            remaining -= ChannelCount__13VISoundDeviceUl(device, mask_at(record, 16));
            if (remaining <= 0) break;
        }
        if (remaining > 0) return 0;
        remaining = missing;
        for (unsigned p = FirstPlayback__13VISoundDevice(device); p;) {
            unsigned next = NextPlayback__13VISoundDeviceP10VIPlayback(device, p);
            const unsigned* record = words_at(p);
            if (priority >= static_cast<int>(record[24 / 4])) {
                int held = ChannelCount__13VISoundDeviceUl(device, mask_at(record, 16));
                Stop__13VISoundDevicei(device, static_cast<int>(record[32 / 4]));
                remaining -= held;
                if (remaining <= 0) break;
            }
            p = next;
        }
    }
    unsigned long long mask = *inUse;
    unsigned long long result = 0;
    int found = 0;
    if (!(mask & 1)) {
        found = 1;
        result = 1;
        if (count == 1) {
            *inUse = mask | result;
            return result;
        }
    }
    for (int i = 1; i < 46; ++i) {
        unsigned long long bit = 1ull << i;
        if (mask & bit) continue;
        ++found;
        result |= bit;
        if (found == count) break;
    }
    *inUse = mask | result;
    return result;
}
}

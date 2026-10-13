// Observed: SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf at 0x01126690, 136 bytes. Arguments: the device, a class number and a
// float. It stores the float at device + 3424 + 4 * class. Then it walks the playback records with FirstPlayback__13VISoundDevice and
// NextPlayback__13VISoundDeviceP10VIPlayback (the next record is requested after the record has been handled); for every record whose
// word at +52 equals the class number it calls SetVolume__13VISoundDeviceif(device, record word at +32, float at record+40).
// Inferred: device+3424 is a per-class volume table (Reset__13VISoundDevice at 0x011248a8 fills a table that ends at +3428 with
// 1.0 as well); a playback record keeps its class at +52, its voice id at +32 and its own volume at +40, and the device volume
// setter is re-applied so the new class volume takes effect. Whether SetVolume multiplies by the class table is not examined.
// Unknown: the class numbering (the enum is only known by name), the table length and the record layout beyond those three words.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned FirstPlayback__13VISoundDevice(void* device);
unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void* device, unsigned playback);
void SetVolume__13VISoundDeviceif(void* device, int voice, float volume);

void SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(void* device, int soundClass, float volume) {
    unsigned char* base = static_cast<unsigned char*>(device);
    reinterpret_cast<float*>(base + 3424)[soundClass] = volume;
    for (unsigned p = FirstPlayback__13VISoundDevice(device); p; p = NextPlayback__13VISoundDeviceP10VIPlayback(device, p)) {
        const unsigned* record = words_at(p);
        if (static_cast<int>(record[52 / 4]) == soundClass) {
            SetVolume__13VISoundDeviceif(device, static_cast<int>(record[32 / 4]), __builtin_bit_cast(float, record[40 / 4]));
        }
    }
}
}

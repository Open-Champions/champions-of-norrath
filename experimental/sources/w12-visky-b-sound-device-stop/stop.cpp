// Observed: Stop__13VISoundDevicei at 0x011252a0, 260 bytes. Arguments: the device and a playback
// id. Nothing is returned.
//   1. pb = Playback__13VISoundDevicei(device, id). A null pb ends the call.
//   2. The word at pb+28 selects a sound: unless it is -1, the sound pointer is the word at
//      device + 2584 + 4 * that value; otherwise there is none.
//   3. If there is a sound and Type__C7VISound(sound) is nonzero, Type__C7VISound(sound) is called
//      a second time (the image calls it twice), and when that second result is 1 the 64-bit word
//      at pb+16 is split into two words of sIopFunctionData and SetIopFunction__FUii(9, 2) is
//      called: data[0] = the low 24 bits of the 64-bit word (zero-extended into an int),
//      data[1] = bits 24..55 of it (arithmetic shift, so bit 55 supplies the sign). data[1] is
//      written first, data[0] in the delay slot of the call.
//   4. StopChannels__13VISoundDeviceUl(device, *(64-bit)(pb+16)) and
//      DeallocateChannels__13VISoundDeviceUl(device, *(64-bit)(pb+16)).
//   5. device[3100 + 4 * (byte at pb+32 % 46)] = 0 (the table Playback searches), then
//      DeletePlayback__13VISoundDeviceP10VIPlayback(device, pb).
// Inferred: pb+16 is a 64-bit channel mask, pb+32 the playback id (its low byte is the table key
// and Playback compares the whole word), and the 9/2 IOP call forwards a stream sound's mask.
// Unknown: the name of IOP function 9, and why Type is queried twice.
extern "C" {
void* Playback__13VISoundDevicei(void* device, int id);
int Type__C7VISound(const void* sound);
void SetIopFunction__FUii(unsigned function, int count);
void StopChannels__13VISoundDeviceUl(void* device, unsigned long long channels);
void DeallocateChannels__13VISoundDeviceUl(void* device, unsigned long long channels);
void DeletePlayback__13VISoundDeviceP10VIPlayback(void* device, void* playback);
extern int norrath_overlay_sIopFunctionData_11edf08[];

#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

void Stop__13VISoundDevicei(void* device, int id) {
    unsigned char* dev = static_cast<unsigned char*>(device);
    unsigned char* pb = static_cast<unsigned char*>(Playback__13VISoundDevicei(device, id));
    if (!pb) return;
    int slot = *reinterpret_cast<int*>(pb + 28);
    void* sound = 0;
    if (slot != -1) sound = NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(dev + 2584 + slot * 4));
    if (sound && Type__C7VISound(sound) != 0) {
        if (Type__C7VISound(sound) == 1) {
            long long channels = *reinterpret_cast<long long*>(pb + 16);
            norrath_overlay_sIopFunctionData_11edf08[1] = static_cast<int>(channels << 8 >> 32);
            norrath_overlay_sIopFunctionData_11edf08[0] = static_cast<int>(channels & 0xffffff);
            SetIopFunction__FUii(9, 2);
        }
    }
    unsigned long long channels = *reinterpret_cast<unsigned long long*>(pb + 16);
    StopChannels__13VISoundDeviceUl(device, channels);
    DeallocateChannels__13VISoundDeviceUl(device, channels);
    *reinterpret_cast<unsigned*>(dev + 3100 + (pb[32] % 46) * 4) = 0;
    DeletePlayback__13VISoundDeviceP10VIPlayback(device, pb);
}
}

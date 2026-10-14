// Observed: SoundPlay__4VIUI11VIUISoundId at 0x0102fad8, 108 bytes. Arguments: the UI object and a sound id.
// It calls GetString__13VIStringTableii on the string table at offset 32 of the object with (id, 0), converts
// that 16-bit string into a 64-byte local buffer with str16to8__8VIStringPcUiPCUs(buffer, 64, string), then
// calls GetUISound__F11VIUISoundId(id). When that returns zero the result is -1. Otherwise it calls
// SoundPlay__4VIUIPCcf(ui, buffer, f) with f the float at offset 4 of the returned record and returns that
// call's result.
// Inferred: the string table at +32 is the UISound.txt table loaded by Init__4VIUI, so the string is the sound
// file name and the record gives a volume or pitch; the id is looked up in both.
// Unknown: the meaning of the float, the return value of the inner call (an int is assumed), and the types
// behind the id.
extern "C" {
struct VIStringTable;
struct VIUI;
const unsigned short* GetString__13VIStringTableii(VIStringTable* table, int id, int flag);
void str16to8__8VIStringPcUiPCUs(char* destination, unsigned size, const unsigned short* source);
const float* GetUISound__F11VIUISoundId(int id);
int SoundPlay__4VIUIPCcf(VIUI* ui, const char* name, float value);

int SoundPlay__4VIUI11VIUISoundId(VIUI* ui, int id) {
    char name[64];
    str16to8__8VIStringPcUiPCUs(
        name, 64,
        GetString__13VIStringTableii(reinterpret_cast<VIStringTable*>(reinterpret_cast<char*>(ui) + 32), id, 0));
    const float* record = GetUISound__F11VIUISoundId(id);
    if (!record) return -1;
    return SoundPlay__4VIUIPCcf(ui, name, record[1]);
}
}

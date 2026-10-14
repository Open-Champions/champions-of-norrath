#include "sound_play.cpp"
#include <cstring>
#include <string>

static std::string trace;
static char* table_base;
static float volume_record[2] = {0.0f, 0.75f};
static const float* sound_record;
static const unsigned short text[3] = {'a', 'b', 0};

extern "C" const unsigned short* GetString__13VIStringTableii(VIStringTable* table, int id, int flag) {
    trace += "G" + std::to_string(reinterpret_cast<char*>(table) - table_base) + ":" +
             std::to_string(id) + "," + std::to_string(flag) + ";";
    return text;
}
extern "C" void str16to8__8VIStringPcUiPCUs(char* destination, unsigned size, const unsigned short* source) {
    trace += "S" + std::to_string(size) + ";";
    for (unsigned i = 0; i < size; ++i) {
        destination[i] = static_cast<char>(source[i]);
        if (!source[i]) break;
    }
}
extern "C" const float* GetUISound__F11VIUISoundId(int) { trace += "U;"; return sound_record; }
static std::string played;
static float played_value;
extern "C" int SoundPlay__4VIUIPCcf(VIUI*, const char* name, float value) {
    played = name; played_value = value; trace += "P;"; return 42;
}

int main() {
    alignas(16) unsigned char ui[64] = {};
    table_base = reinterpret_cast<char*>(ui);
    sound_record = nullptr;
    if (SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<VIUI*>(ui), 5) != -1) return 1;
    if (!played.empty()) return 2;
    sound_record = volume_record;
    if (SoundPlay__4VIUI11VIUISoundId(reinterpret_cast<VIUI*>(ui), 5) != 42) return 3;
    if (played != "ab" || played_value != 0.75f) return 4;
    // order: string lookup, conversion, record lookup, play
    if (trace.find("G32:5,0;S64;U;P;") == std::string::npos) return 5;
    return 0;
}

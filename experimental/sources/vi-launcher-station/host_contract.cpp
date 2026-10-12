#define strcpy norrath_host_strcpy
#include "station.cpp"
#undef strcpy

#include <cstring>

static int options_calls;
static void* options_value[2];
static void* username_options;
static void* password_options;
static int station_mode;
static const char* station_name;
static char* station_username;
static char* station_password;
static const char* station_135;
static const char* station_copied;
static int station_byte;
static int station_result = 100;

extern "C" char* norrath_host_strcpy(char* destination, const char* source) {
    char* cursor = destination;
    while ((*cursor++ = *source++) != '\0') {
    }
    return destination;
}
extern "C" void* VIGetGameOptions__Fv() {
    void* value = options_calls == 0 ? reinterpret_cast<void*>(0x11) : reinterpret_cast<void*>(0x22);
    if (options_calls < 2) options_value[options_calls] = value;
    ++options_calls;
    return value;
}
extern "C" char* Username__C13VIGameOptions(void* options) {
    username_options = options;
    return const_cast<char*>("user");
}
extern "C" char* Password__C13VIGameOptions(void* options) {
    password_options = options;
    return const_cast<char*>("secret");
}
extern "C" int station_main__FiPCcPcT2T1T1i(
    int mode, const char* name, char* username, char* password,
    const char* text_135, const char* copied, int launch_byte) {
    station_mode = mode;
    station_name = name;
    station_username = username;
    station_password = password;
    station_135 = text_135;
    station_copied = copied;
    station_byte = launch_byte;
    return station_result;
}

int main() {
    unsigned char object[280];
    std::memset(object, 0xa5, sizeof object);
    const char* name = "alpha";
    const char* text_135 = "beta";
    const char* copied = "gamma";
    int result = Station__10VILauncheriPCcN22i(
        reinterpret_cast<VILauncher*>(object), 2, name, text_135, copied, 9);
    if (result != 100) return 1;
    if (options_calls != 2 || username_options != options_value[0] || password_options != options_value[1])
        return 2;
    if (std::strcmp(reinterpret_cast<char*>(object + 8), "user") != 0) return 3;
    if (std::strcmp(reinterpret_cast<char*>(object + 264), "secret") != 0) return 4;
    if (object[7] != 0xa5 || object[13] != 0xa5) return 5;
    if (station_mode != 2 || station_name != name || station_135 != text_135 || station_copied != copied)
        return 6;
    if (station_byte != 9) return 7;
    if (station_username != reinterpret_cast<char*>(object + 8)) return 8;
    if (station_password != reinterpret_cast<char*>(object + 264)) return 9;
    return 0;
}

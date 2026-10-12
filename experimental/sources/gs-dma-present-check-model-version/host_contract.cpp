#include "check_model_version.cpp"

#include <cstdio>
#include <cstring>
#include <string>

static int open_result;
static const char* open_path;
static int open_mode;
static int reads;
static int closes;
static std::string file_bytes;
static const char* atoi_text;
static int atoi_result;

extern "C" int sceOpen(const char* path, int mode) {
    open_path = path;
    open_mode = mode;
    return open_result;
}
extern "C" int sceRead(int descriptor, void* buffer, int count) {
    if (descriptor != open_result || count != 1) return -1;
    reads += 1;
    const unsigned index = static_cast<unsigned>(reads - 1);
    const char byte = index < file_bytes.size() ? file_bytes[index] : '\0';
    *static_cast<unsigned char*>(buffer) = static_cast<unsigned char>(byte);
    return 1;
}
extern "C" int sceClose(int descriptor) {
    if (descriptor != open_result) return -1;
    closes += 1;
    return 0;
}
extern "C" int atoi(const char* text) {
    atoi_text = text;
    return atoi_result;
}

static void reset(int descriptor, const char* bytes) {
    open_result = descriptor;
    file_bytes = bytes;
    reads = 0;
    closes = 0;
    atoi_text = nullptr;
    atoi_result = 0;
}

int main() {
    reset(-1, "");
    if (checkModelVersion() != -1 || reads != 0 || closes != 0) return 1;
    if (std::strcmp(open_path, "rom0:ROMVER") != 0 || open_mode != 1) return 2;

    reset(4, "ABC020010608");
    atoi_result = 0x01315670;
    if (checkModelVersion() != 0 || closes != 1 || reads != 13) return 3;
    if (atoi_text == nullptr || std::strcmp(atoi_text, "020010608") != 0) return 4;

    reset(4, "ABC020010609");
    atoi_result = 0x01315671;
    if (checkModelVersion() != 1) return 5;

    reset(4, "");
    atoi_result = 1;
    if (checkModelVersion() != 0 || reads != 1) return 6;
    if (atoi_text == nullptr) return 7;

    reset(4, std::string(256, '9').c_str());
    atoi_result = 0x01315670;
    if (checkModelVersion() != 0 || reads != 256 || closes != 1) return 8;
    if (atoi_text == nullptr || std::memcmp(atoi_text, "999999999", 9) != 0) return 9;
    std::puts("check model version contract passed");
    return 0;
}

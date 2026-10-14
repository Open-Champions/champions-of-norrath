#define NORRATH_HOST_CONTRACT 1
#include "comm.cpp"

#include <cstdio>

extern "C" unsigned char enableMallocWrapper = 0;
static int aud_calls;
static int aud_left;
static int aud_right;
static unsigned char wrapper_at_aud;
static int codec_inits;
static unsigned char wrapper_at_codec_init;
static int opens;
static int opened_id;
static int* opened_codec;
static unsigned char wrapper_at_open;
static int enum_index[8];
static int enums;

struct Record {
    unsigned char bytes[8];
    const char* name;
};
static Record records[4];
static int record_count;

extern "C" const char* norrath_comm_name(const void* record) {
    for (int index = 0; index < record_count; ++index)
        if (record == records[index].bytes) return records[index].name;
    return "";
}
extern "C" void lgAudInit(int left, int right) {
    ++aud_calls;
    aud_left = left;
    aud_right = right;
    wrapper_at_aud = enableMallocWrapper;
}
extern "C" void lgCodecInit() {
    ++codec_inits;
    wrapper_at_codec_init = enableMallocWrapper;
}
extern "C" void* lgCodecEnumerate(int index) {
    enum_index[enums] = index;
    ++enums;
    if (index < record_count) return records[index].bytes;
    return nullptr;
}
extern "C" int strcmp(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        ++left;
        ++right;
    }
    return static_cast<unsigned char>(*left) - static_cast<unsigned char>(*right);
}
extern "C" void lgCodecOpen(int identifier, int* codec_word) {
    ++opens;
    opened_id = identifier;
    opened_codec = codec_word;
    wrapper_at_open = enableMallocWrapper;
}

static void reset() {
    aud_calls = 0;
    codec_inits = 0;
    opens = 0;
    enums = 0;
    enableMallocWrapper = 9;
    audinitted = 0;
    codec = 4;
    record_count = 0;
}

static void put_id(Record* record, unsigned value) {
    record->bytes[0] = static_cast<unsigned char>(value);
    record->bytes[1] = static_cast<unsigned char>(value >> 8);
    record->bytes[2] = static_cast<unsigned char>(value >> 16);
    record->bytes[3] = static_cast<unsigned char>(value >> 24);
}

int main() {
    reset();
    records[0].name = "PCM";
    put_id(&records[0], 3);
    records[1].name = "GSM";
    put_id(&records[1], 9);
    records[2].name = "GSM";
    put_id(&records[2], 4);
    record_count = 3;
    commSystemInit__Fv();
    if (aud_calls != 1 || aud_left != 0 || aud_right != 0 || wrapper_at_aud != 1) return 1;
    if (audinitted != 1 || codec_inits != 1 || wrapper_at_codec_init != 1) return 2;
    if (enums != 4 || enum_index[0] != 0 || enum_index[3] != 3) return 3;
    if (opens != 1 || opened_id != 4 || opened_codec != &codec || wrapper_at_open != 1) return 4;
    if (enableMallocWrapper != 0) return 5;

    reset();
    audinitted = 6;
    commSystemInit__Fv();
    if (aud_calls != 0 || audinitted != 6 || codec_inits != 1 || wrapper_at_codec_init != 1) return 6;
    if (opened_id != -1 || enableMallocWrapper != 0) return 7;
    std::puts("commSystemInit passed");
    return 0;
}

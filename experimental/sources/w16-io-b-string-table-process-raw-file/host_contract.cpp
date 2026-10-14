static unsigned char arena[1 << 14];
#define NORRATH_WORD_ADDRESS(word) static_cast<void*>(arena + static_cast<unsigned>(word))
#define NORRATH_ADDRESS_WORD(pointer) ((pointer) == nullptr ? 0u : static_cast<unsigned>(static_cast<const unsigned char*>(static_cast<const void*>(pointer)) - arena))
#include "process_raw_file.cpp"

#include <cstdio>
#include <cstring>
#include <string>

static int vec_new_calls, last_new_size, fail_new, strlen_calls, convert_result_delta, convert_count, strlen_result;
static int str8_calls;
static unsigned str8_size;
static std::string str8_text;
static std::string strlen_text;

extern "C" void* __builtin_vec_new(unsigned size) {
    ++vec_new_calls;
    last_new_size = static_cast<int>(size);
    std::memset(arena + 0x2000, 0xcc, 256);
    return fail_new ? nullptr : arena + 0x2000;
}
extern "C" int UTF8_strlen__FPCci(const char* text, int count) {
    ++strlen_calls;
    strlen_text.assign(text, count);
    return strlen_result;
}
extern "C" int UTF8_convertToUTF16__FPCcPUsi(const char*, unsigned short* out, int count) {
    convert_count = count;
    out[0] = 0x41;
    return count + convert_result_delta;
}
extern "C" unsigned short* str8to16__8VIStringPUsUiPCc(unsigned short* out, unsigned size, const char* text) {
    ++str8_calls;
    str8_size = size;
    str8_text = text;
    out[0] = 0x42;
    return out;
}

static int run(const std::string& body, unsigned char* table) {
    std::memset(table, 0, 16);
    std::memcpy(arena + 0x1000, body.data(), body.size());
    arena[0x1000 + body.size()] = 0;
    *reinterpret_cast<int*>(arena + 0x400) = static_cast<int>(body.size());
    *reinterpret_cast<unsigned*>(arena + 0x404) = 0x1000;
    vec_new_calls = strlen_calls = str8_calls = 0;
    return ProcessRawFile__13VIStringTableRCt7VIArray1Zc(table, arena + 0x400);
}

int main() {
    unsigned char* table = arena + 0x600;
    strlen_result = 4;
    // UTF-8 with byte order mark.
    if (run(std::string("\xef\xbb\xbf" "abcdef", 9), table) != 0) return 1;
    if (strlen_text != "abcdef" || last_new_size != 10 || convert_count != 4) return 2;
    if (*reinterpret_cast<unsigned*>(table + 8) != 0x2000u) return 3;
    convert_result_delta = -1;
    if (run(std::string("\xef\xbb\xbf" "abcdef", 9), table) != 1) return 4;
    convert_result_delta = 0;
    fail_new = 1;
    if (run(std::string("\xef\xbb\xbf" "abcdef", 9), table) != -1 || *reinterpret_cast<unsigned*>(table + 8) != 0u) return 5;
    fail_new = 0;
    // UTF-16 little-endian with mark: 2 mark bytes + 6 text bytes.
    if (run(std::string("\xff\xfe" "A\0B\0C\0", 8), table) != 0 || last_new_size != 8) return 6;
    if (std::memcmp(arena + 0x2000, "A\0B\0C\0\0\0", 8) != 0) return 7;
    // Byte-swapped.
    if (run(std::string("\xfe\xff" "\0A\0B\0C", 8), table) != 0 || last_new_size != 8) return 8;
    if (std::memcmp(arena + 0x2000, "A\0B\0C\0\0\0", 8) != 0) return 9;
    // Only the mark: empty text.
    if (run(std::string("\xff\xfe", 2), table) != 0 || last_new_size != 2 || arena[0x2000] != 0 || arena[0x2001] != 0) return 10;
    if (run(std::string("\xfe\xff", 2), table) != 0 || last_new_size != 2 || arena[0x2000] != 0) return 11;
    // Plain text, including short and mark-like starts.
    if (run(std::string("hello", 5), table) != 0 || str8_calls != 1 || str8_size != 12 || str8_text != "hello") return 12;
    if (run(std::string("\xef\xbb", 2), table) != 0 || str8_calls != 1 || str8_size != 6) return 13;
    if (run(std::string("", 0), table) != 0 || str8_size != 2 || strlen_calls != 0) return 14;
    fail_new = 1;
    if (run(std::string("abc", 3), table) != -1 || str8_calls != 0) return 15;
    std::puts("string-table process-raw-file passed");
    return 0;
}

// Host comparison only. Not an EE build input.
#include "factory_sscanf.cpp"

#include <cstdio>
#include <cstring>

static unsigned recorded_words[24];
static const char* recorded_format = nullptr;
static int scan_calls = 0;

extern "C" unsigned norrath_factory_strlen(const char* text) {
    unsigned length = 0;
    while (text[length] != '\0') ++length;
    return length;
}

static unsigned char fake_impure[8] = {0x11, 0x22, 0x33, 0x44, 0, 0, 0, 0};
extern "C" unsigned char norrath_factory_impure_pointer_word[4] = {};

extern "C" int norrath_player_string_eof_read(unsigned char*, char*, int) {
    return 0;
}

extern "C" int norrath_player_stream_scan(unsigned char* stream, const char* format,
                                          const unsigned char* argument_cursor) {
    (void)argument_cursor;
    ++scan_calls;
    for (int i = 0; i < 24; ++i) recorded_words[i] = read_word(stream + i * 4u);
    recorded_format = format;
    return -7;
}

static unsigned word_of_host(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

int main() {
    write_word(norrath_factory_impure_pointer_word, word_of_host(fake_impure));
    const char input[] = " (12, 34)";
    const char format[] = " (%d, %d)";
    scan_calls = 0;
    const int result = norrath_factory_sscanf(input, format, nullptr);
    if (scan_calls != 1) {
        std::printf("scan was not called once\n");
        return 1;
    }
    if (result != -7) {
        std::printf("result was not passed through\n");
        return 2;
    }
    if (recorded_format != format) {
        std::printf("format was not forwarded\n");
        return 3;
    }
    const unsigned input_bits = word_of_host(input);
    const unsigned length = static_cast<unsigned>(std::strlen(input));
    // Stream words 0, 4, 5(base/cursor/size), 8(eofread), 12, 21(owner).
    if (recorded_words[0] != input_bits || recorded_words[5] != length) return 4;
    if (recorded_words[4] != input_bits) return 5;
    if (recorded_words[8] != word_of_host(reinterpret_cast<const void*>(norrath_player_string_eof_read)))
        return 6;
    if (recorded_words[21] != word_of_host(fake_impure)) return 7;
    if (recorded_words[1] != length) return 8;
    if (recorded_words[12] != 0u || recorded_words[17] != 0u) return 9;
    // Halfword flag at byte 12 must be 4 (read-only string stream).
    if ((recorded_words[3] & 0xffffu) != 4u) return 10;
    return 0;
}

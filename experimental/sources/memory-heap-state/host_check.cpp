#include <cstdio>
#include <cstring>

extern "C" {
extern unsigned char norrath_heap_ring_word_view[4];
extern unsigned char norrath_heap_next_fit_view[4];
extern unsigned char norrath_heap_fragment_threshold_view[4];
extern unsigned char norrath_heap_free_bytes_view[4];
extern unsigned char norrath_heap_allocation_tag_view[4];
extern unsigned char norrath_heap_total_bytes_view[4];
extern unsigned char norrath_heap_dump_count_view[4];
extern const unsigned char norrath_heap_check_file_view[];
extern const unsigned char norrath_heap_failure_format_view[];
extern const unsigned char norrath_heap_dump_border_view[];
extern const unsigned char norrath_heap_dump_count_format_view[];
extern const unsigned char norrath_heap_dump_free_format_view[];
extern const unsigned char norrath_heap_dump_used_format_view[];
extern const unsigned char norrath_heap_dump_totals_format_view[];
extern const unsigned char norrath_heap_dump_serial_format_view[];
}

namespace {
unsigned failures;
void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
void expect_text(const unsigned char* actual, const char* hex, const char* message) {
    unsigned count = 0;
    while (hex[count] != 0 && hex[count + 1] != 0) {
        auto nybble = [](char c) {
            if (c >= '0' && c <= '9') return c - '0';
            return c - 'a' + 10;
        };
        unsigned char byte = static_cast<unsigned char>((nybble(hex[count]) << 4) | nybble(hex[count + 1]));
        if (actual[count / 2] != byte) {
            std::fprintf(stderr, "fail: %s at %u\n", message, count / 2);
            ++failures;
            return;
        }
        count += 2;
    }
    if (actual[count / 2] != 0 && hex[count] == 0) {
        // hex includes the terminating 00, so count/2 is one past the last compared byte.
    }
}
}

int main() {
    unsigned mutable_bits = 0;
    for (unsigned char byte : norrath_heap_ring_word_view) mutable_bits |= byte;
    for (unsigned char byte : norrath_heap_next_fit_view) mutable_bits |= byte;
    for (unsigned char byte : norrath_heap_fragment_threshold_view) mutable_bits |= byte;
    for (unsigned char byte : norrath_heap_free_bytes_view) mutable_bits |= byte;
    for (unsigned char byte : norrath_heap_allocation_tag_view) mutable_bits |= byte;
    for (unsigned char byte : norrath_heap_total_bytes_view) mutable_bits |= byte;
    for (unsigned char byte : norrath_heap_dump_count_view) mutable_bits |= byte;
    expect(mutable_bits == 0u, "heap words start at zero");
    expect(sizeof norrath_heap_ring_word_view == 4, "ring word is one word");
    expect_text(norrath_heap_check_file_view, "6d656d2e636300", "check file");
    expect_text(norrath_heap_failure_format_view,
                "6d656d416c6c6f63206661696c65642120202872613d25782c2073697a653d2564290a00",
                "failure format");
    expect_text(norrath_heap_dump_border_view,
                "2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d0a00",
                "border");
    expect_text(norrath_heap_dump_count_format_view,
                "2020202020537461727420486561702044756d7020282564290a00",
                "count format");
    expect_text(norrath_heap_dump_free_format_view,
                "467265652061742025782c2025642062797465732e0a00",
                "free format");
    expect_text(norrath_heap_dump_used_format_view,
                "557365642061742025782027257327202872613d2578292c2025642062797465732e0a00",
                "used format");
    expect_text(norrath_heap_dump_totals_format_view,
                "546f74616c3d25642c20557365643d25642c20467265653d25640a00",
                "totals format");
    expect_text(norrath_heap_dump_serial_format_view,
                "202020202020456e6420486561702044756d702028256429200a00",
                "serial format");
    // A dropped space in the failure text changes a compared byte.
    expect(norrath_heap_failure_format_view[16] == ' ' && norrath_heap_failure_format_view[17] == ' ',
           "failure text has both spaces before the return address");
    if (failures != 0) return 1;
    std::printf("heap state checks passed\n");
    return 0;
}

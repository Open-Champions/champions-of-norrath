#include "specialparse.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <strings.h>
#include <vector>

unsigned char norrath_megatile_counters_view[20];
unsigned char norrath_megatile_pm_pointers_view[0x400];
unsigned char norrath_megatile_dynlevels_view[0x400];
unsigned char norrath_megatile_level_name_view[0x142];
const unsigned char norrath_megatile_closest_limit_view[4] = {0xeb, 0x78, 0xad, 0x60};
unsigned char norrath_factory_ctype_minimum_view[256];

const unsigned char norrath_megatile_trigger_text[] = "trigger";
const unsigned char norrath_megatile_width_key[] = "w=";
const unsigned char norrath_megatile_height_key[] = "h=";
const unsigned char norrath_megatile_bang_text[] = "!!!";
const unsigned char norrath_megatile_dynamic_new_world_text[] = "!!!dynamicnewworld";
const unsigned char norrath_megatile_scan_pair_format[] = "%[^,],%d";
const unsigned char norrath_megatile_suffix_number_format[] = "%s%d";
const unsigned char norrath_megatile_level_assignment_format[] = "level=%s";
const unsigned char norrath_megatile_dynamic_shift_text[] = "!!!dynamicshift";
const unsigned char norrath_megatile_pair_join_format[] = "%s_%s";
const unsigned char norrath_megatile_decimal_format[] = "%d";
const unsigned char norrath_megatile_dynamic_orientation_text[] = "!!!dynamicorientation";
const unsigned char norrath_megatile_orientation_format[] = "orientation=%d";
const unsigned char norrath_megatile_closest_level_text[] = "!!!closestdynamiclevel";
const unsigned char norrath_megatile_from_format[] = "from=%s";
const unsigned char norrath_megatile_width_format[] = "w=%d";
const unsigned char norrath_megatile_height_format[] = "h=%d";

namespace {
struct Event {
    enum Kind { Word, Store } kind;
    const unsigned char* cell;
    unsigned word;
    const void* pointer;
};
std::vector<Event> events;
std::vector<std::string> pooled;
int cap_calls = 0;
char* cap_sentinel = nullptr;
int failures = 0;

void fail(const char* name, const std::string& detail) {
    std::fprintf(stderr, "FAIL %s: %s\n", name, detail.c_str());
    failures += 1;
}

unsigned read_word(const unsigned char* cell) {
    return unsigned(cell[0]) | (unsigned(cell[1]) << 8) |
           (unsigned(cell[2]) << 16) | (unsigned(cell[3]) << 24);
}
void write_word(unsigned char* cell, unsigned value) {
    cell[0] = static_cast<unsigned char>(value);
    cell[1] = static_cast<unsigned char>(value >> 8);
    cell[2] = static_cast<unsigned char>(value >> 16);
    cell[3] = static_cast<unsigned char>(value >> 24);
}
void write_float(unsigned char* cell, float value) {
    unsigned word;
    std::memcpy(&word, &value, 4);
    write_word(cell, word);
}

void reset() {
    std::memset(norrath_megatile_counters_view, 0, sizeof norrath_megatile_counters_view);
    std::memset(norrath_megatile_pm_pointers_view, 0, sizeof norrath_megatile_pm_pointers_view);
    std::memset(norrath_megatile_dynlevels_view, 0, sizeof norrath_megatile_dynlevels_view);
    std::memset(norrath_megatile_level_name_view, 0, sizeof norrath_megatile_level_name_view);
    std::memset(norrath_factory_ctype_minimum_view, 0, sizeof norrath_factory_ctype_minimum_view);
    for (int character = '0'; character <= '9'; character += 1)
        norrath_factory_ctype_minimum_view[character + 128] = 0x4;
    events.clear();
    pooled.clear();
    cap_calls = 0;
}
}

extern "C" void norrath_megatile_host_note_word(const unsigned char* cell, unsigned value) {
    events.push_back(Event{Event::Word, cell, value, nullptr});
}
extern "C" void norrath_megatile_host_note_store(const unsigned char* cell, const void* value) {
    events.push_back(Event{Event::Store, cell, 0u, value});
}
extern "C" int norrath_factory_strcasecmp(const char* left, const char* right) {
    return ::strcasecmp(left, right);
}
extern "C" int norrath_factory_strncmp(const char* left, const char* right, unsigned count) {
    return std::strncmp(left, right, count);
}
extern "C" int norrath_factory_atoi(const char* text) { return std::atoi(text); }
extern "C" char* norrath_factory_strstr(const char* text, const char* needle) {
    return const_cast<char*>(std::strstr(text, needle));
}
extern "C" char* norrath_factory_strchr(const char* text, int character) {
    return const_cast<char*>(std::strchr(text, character));
}
extern "C" int norrath_factory_sscanf(const char* text, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int count = ::vsscanf(text, format, args);
    va_end(args);
    return count;
}
extern "C" int norrath_factory_sprintf(char* output, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int count = std::vsnprintf(output, 96, format, args);
    va_end(args);
    return count;
}
extern "C" char* norrath_factory_strcpy(char* destination, const char* source) {
    return std::strcpy(destination, source);
}
extern "C" char* norrath_megatile_add_pm_list(char* text) {
    pooled.emplace_back(text);
    return const_cast<char*>(pooled.back().c_str());
}
extern "C" char** norrath_megatile_cap_pm_pointers() {
    cap_calls += 1;
    return &cap_sentinel;
}
extern "C" void norrath_swap_int(int* left, int* right) {
    const int saved = *left;
    *left = *right;
    *right = saved;
}

namespace {
const unsigned char* point_bytes() {
    static const unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    return point;
}

void expect_true(const char* name, bool condition, const std::string& detail) {
    if (!condition) fail(name, detail);
}

void check_trigger_pair(const char* name, int selector, int width, int height,
                        const char* expect_w, const char* expect_h) {
    reset();
    char width_arg[32];
    char height_arg[32];
    std::snprintf(width_arg, sizeof width_arg, "w=%d", width);
    std::snprintf(height_arg, sizeof height_arg, "h=%d", height);
    char* args[] = {width_arg, height_arg, nullptr};
    char entity[] = "TrIgGeR";
    char** result = norrath_megatile_specialparse(args, selector, 0, point_bytes(), entity);
    expect_true(name, result == &cap_sentinel, "cap return");
    expect_true(name, cap_calls == 1, "one cap");
    expect_true(name, pooled.size() == 2, "two published measures");
    if (pooled.size() == 2) {
        expect_true(name, pooled[0] == expect_w, pooled[0]);
        expect_true(name, pooled[1] == expect_h, pooled[1]);
    }
    expect_true(name, read_word(norrath_megatile_counters_view + 4) == 2u, "pointer count");
    std::vector<Event> ordered;
    for (const Event& event : events) {
        if (event.kind == Event::Store || event.cell == norrath_megatile_counters_view + 4)
            ordered.push_back(event);
    }
    expect_true(name, ordered.size() == 4, "two publishes");
    if (ordered.size() == 4) {
        expect_true(name, ordered[0].kind == Event::Store && ordered[1].kind == Event::Word &&
                               ordered[1].word == 1u,
                    "width pointer before its count");
        expect_true(name, ordered[2].kind == Event::Word && ordered[2].word == 2u &&
                               ordered[3].kind == Event::Store,
                    "height count before its pointer");
    }
}

void test_plain_append_order() {
    const char* name = "plain append";
    reset();
    char hello[] = "hello";
    char* args[] = {hello, nullptr};
    char entity[] = "crate";
    char** result = norrath_megatile_specialparse(args, 0, 1, point_bytes(), entity);
    expect_true(name, result == &cap_sentinel, "cap return");
    expect_true(name, pooled.empty(), "raw pointer is not copied");
    std::vector<Event> ordered;
    for (const Event& event : events) {
        if (event.kind == Event::Store || event.cell == norrath_megatile_counters_view + 4)
            ordered.push_back(event);
    }
    expect_true(name, ordered.size() == 2, "count then pointer");
    if (ordered.size() == 2) {
        expect_true(name, ordered[0].kind == Event::Word && ordered[0].word == 1u, "count published first");
        expect_true(name, ordered[1].kind == Event::Store && ordered[1].pointer == hello, "original pointer stored");
    }
}

void test_unknown_bang_dropped() {
    const char* name = "unknown bang";
    reset();
    char text[] = "!!!nope";
    char* args[] = {text, nullptr};
    char entity[] = "crate";
    norrath_megatile_specialparse(args, 4, 1, point_bytes(), entity);
    expect_true(name, pooled.empty(), "dropped");
    expect_true(name, read_word(norrath_megatile_counters_view + 4) == 0u, "no pointer");
    expect_true(name, cap_calls == 1, "still capped");
}

void test_uppercase_width_is_raw() {
    const char* name = "uppercase width";
    reset();
    char text[] = "W=2";
    char* args[] = {text, nullptr};
    char entity[] = "trigger";
    norrath_megatile_specialparse(args, 0, 1, point_bytes(), entity);
    expect_true(name, pooled.size() == 2, "default measures");
    if (pooled.size() == 2) {
        expect_true(name, pooled[0] == "w=-1", pooled[0]);
        expect_true(name, pooled[1] == "h=-1", pooled[1]);
    }
    bool saw_raw = false;
    for (const Event& event : events)
        if (event.kind == Event::Store && event.pointer == text) saw_raw = true;
    expect_true(name, saw_raw, "raw W= argument");
}

void test_orientation() {
    const char* name = "orientation";
    reset();
    char text[] = "prefix!!!dynamicorientation";
    char* args[] = {text, nullptr};
    char entity[] = "crate";
    norrath_megatile_specialparse(args, 5, 9, point_bytes(), entity);
    expect_true(name, pooled.size() == 1 && pooled[0] == "orientation=5",
                pooled.empty() ? "missing" : pooled[0]);
}

void test_closest_aborts() {
    const char* name = "closest abort";
    reset();
    char first[] = "hello";
    char second[] = "!!!closestdynamiclevel";
    char* args[] = {first, second, nullptr};
    char entity[] = "crate";
    char** result = norrath_megatile_specialparse(args, 1, 0, point_bytes(), entity);
    expect_true(name, result == nullptr, "null return");
    expect_true(name, cap_calls == 0, "cap skipped");
    expect_true(name, pooled.empty(), "no from= string");
    bool saw_hello = false;
    for (const Event& event : events)
        if (event.kind == Event::Store && event.pointer == first) saw_hello = true;
    expect_true(name, saw_hello, "earlier argument kept");
}

void test_closest_picks_first_equal_then_nearer() {
    const char* name = "closest";
    reset();
    std::strcpy(reinterpret_cast<char*>(norrath_megatile_dynlevels_view), "far");
    std::strcpy(reinterpret_cast<char*>(norrath_megatile_dynlevels_view + 28), "near");
    write_float(norrath_megatile_dynlevels_view + 16, 50.f);
    write_float(norrath_megatile_dynlevels_view + 20, 0.f);
    write_float(norrath_megatile_dynlevels_view + 28 + 16, 1.f);
    write_float(norrath_megatile_dynlevels_view + 28 + 20, 0.f);
    write_word(norrath_megatile_counters_view + 12, 2u);
    unsigned char point[12] = {};
    write_float(point, 0.f);
    write_float(point + 4, 0.f);
    char text[] = "!!!closestdynamiclevel";
    char* args[] = {text, nullptr};
    char entity[] = "crate";
    norrath_megatile_specialparse(args, 0, 1, point, entity);
    expect_true(name, pooled.size() == 1 && pooled[0] == "from=near",
                pooled.empty() ? "missing" : pooled[0]);

    reset();
    std::strcpy(reinterpret_cast<char*>(norrath_megatile_dynlevels_view), "first");
    std::strcpy(reinterpret_cast<char*>(norrath_megatile_dynlevels_view + 28), "second");
    write_float(norrath_megatile_dynlevels_view + 16, 3.f);
    write_float(norrath_megatile_dynlevels_view + 20, 0.f);
    write_float(norrath_megatile_dynlevels_view + 28 + 16, 3.f);
    write_float(norrath_megatile_dynlevels_view + 28 + 20, 0.f);
    write_word(norrath_megatile_counters_view + 12, 2u);
    norrath_megatile_specialparse(args, 0, 1, point, entity);
    expect_true(name, pooled.size() == 1 && pooled[0] == "from=first",
                pooled.empty() ? "missing" : pooled[0]);
}

void test_new_world() {
    const char* name = "new world";
    reset();
    write_word(norrath_megatile_counters_view, 7u);
    char text[] = "!!!dynamicnewworld(alpha,3)";
    char* args[] = {text, nullptr};
    char entity[] = "crate";
    unsigned char point[12];
    for (unsigned index = 0; index < 12; index += 1) point[index] = static_cast<unsigned char>(0x10 + index);
    norrath_megatile_specialparse(args, 0, 1, point, entity);
    expect_true(name, pooled.size() == 1 && pooled[0] == "level=alpha7",
                pooled.empty() ? "missing" : pooled[0]);
    expect_true(name, read_word(norrath_megatile_counters_view) == 10u, "suffix plus parsed count");
    expect_true(name, std::strcmp(reinterpret_cast<char*>(norrath_megatile_dynlevels_view), "alpha7") == 0,
                "recorded suffix");
    expect_true(name, std::memcmp(norrath_megatile_dynlevels_view + 16, point, 12) == 0, "recorded point");
    expect_true(name, read_word(norrath_megatile_counters_view + 12) == 1u, "dynamic count");
}

void test_shift() {
    const char* name = "shift";
    reset();
    std::strcpy(reinterpret_cast<char*>(norrath_megatile_level_name_view + 81), "ab_12");
    std::strcpy(reinterpret_cast<char*>(norrath_megatile_level_name_view + 161), "zz");
    char text[] = "!!!dynamicshift(9)";
    char* args[] = {text, nullptr};
    char entity[] = "crate";
    unsigned char point[12] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2};
    norrath_megatile_specialparse(args, 0, 1, point, entity);
    expect_true(name, pooled.size() == 1 && pooled[0] == "level=ab_21",
                pooled.empty() ? "missing" : pooled[0]);
    expect_true(name, std::strcmp(reinterpret_cast<char*>(norrath_megatile_dynlevels_view), "ab_21") == 0,
                "joined name overwritten at the digit");
    expect_true(name, std::memcmp(norrath_megatile_dynlevels_view + 16, point, 12) == 0, "shift point");
}
}

int main() {
    check_trigger_pair("selector 0", 0, 4, 7, "w=4", "h=7");
    check_trigger_pair("selector 1", 1, 4, 7, "w=7", "h=-4");
    check_trigger_pair("selector 2", 2, 4, 7, "w=-4", "h=-7");
    check_trigger_pair("selector 3", 3, 4, 7, "w=-7", "h=4");
    check_trigger_pair("selector 4", 4, 4, 7, "w=4", "h=7");
    check_trigger_pair("selector negative", -1, 4, 7, "w=4", "h=7");
    test_plain_append_order();
    test_unknown_bang_dropped();
    test_uppercase_width_is_raw();
    test_orientation();
    test_closest_aborts();
    test_closest_picks_first_equal_then_nearer();
    test_new_world();
    test_shift();
    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::printf("specialparse host checks passed\n");
    return 0;
}

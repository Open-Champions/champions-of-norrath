#include "dispatch.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <strings.h>
#include <vector>

unsigned char norrath_megatile_regions_view[64];
unsigned char norrath_megatile_counters_view[20];
unsigned char norrath_megatile_pm_pointers_view[64];
unsigned char norrath_restore_filter_word[4];
unsigned char norrath_factory_ctype_minimum_view[256];
const unsigned char norrath_megatile_mspawn_text[] = "MSpawn";
const unsigned char norrath_megatile_bang_text[] = "!!!";
const unsigned char norrath_megatile_trigger_text[] = "trigger";
const unsigned char norrath_megatile_push_trigger_text[] = "PushTrigger";
const unsigned char norrath_megatile_kitten_text[] = "kitten";
const unsigned char norrath_megatile_kitten_name_format[] = "name=kitten%d";

namespace {
struct Spawn {
    char** arguments;
    unsigned char point[12];
    short type_word;
    bool filter;
};
struct Created {
    char* name;
    unsigned char point[12];
    int type_word;
    char** arguments;
};
struct Parsed {
    char** arguments;
    int selector;
    int mode;
    unsigned char point[12];
    char* name;
};
std::vector<void*> registry;
std::vector<Spawn> spawns;
std::vector<Created> created;
std::vector<Parsed> parsed;
std::vector<std::string> pooled;
int cap_calls = 0;
int filter_result = 1;
unsigned filter_seen = 0;
char* cap_sentinel = nullptr;
char** parse_result = nullptr;
int failures = 0;

void fail(const char* name, const std::string& detail) {
    std::fprintf(stderr, "FAIL %s: %s\n", name, detail.c_str());
    failures += 1;
}
void expect(const char* name, bool condition, const std::string& detail) {
    if (!condition) fail(name, detail);
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
void write_half(unsigned char* cell, unsigned value) {
    cell[0] = static_cast<unsigned char>(value);
    cell[1] = static_cast<unsigned char>(value >> 8);
}
void write_float(unsigned char* cell, float value) {
    unsigned word = 0;
    std::memcpy(&word, &value, 4);
    write_word(cell, word);
}
float read_float(const unsigned char* cell) {
    unsigned word = read_word(cell);
    float value;
    std::memcpy(&value, &word, 4);
    return value;
}

void reset() {
    std::memset(norrath_megatile_regions_view, 0, sizeof norrath_megatile_regions_view);
    std::memset(norrath_megatile_counters_view, 0, sizeof norrath_megatile_counters_view);
    std::memset(norrath_megatile_pm_pointers_view, 0, sizeof norrath_megatile_pm_pointers_view);
    std::memset(norrath_restore_filter_word, 0, sizeof norrath_restore_filter_word);
    std::memset(norrath_factory_ctype_minimum_view, 0, sizeof norrath_factory_ctype_minimum_view);
    for (int character = 'A'; character <= 'Z'; character += 1)
        norrath_factory_ctype_minimum_view[character + 128] = 0x1;
    registry.clear();
    registry.push_back(nullptr);
    spawns.clear();
    created.clear();
    parsed.clear();
    pooled.clear();
    cap_calls = 0;
    filter_result = 1;
    filter_seen = 0;
    parse_result = &cap_sentinel;
}

unsigned enroll(void* pointer) {
    registry.push_back(pointer);
    return static_cast<unsigned>(registry.size() - 1u);
}

void fill_row(int index, const unsigned char bytes[10]) {
    std::memcpy(norrath_megatile_regions_view + 10 * index, bytes, 10);
}

struct Record {
    unsigned char bytes[32];
    char* name;
    char** arguments;
};

Record make_record(char* name, char** arguments, unsigned type_half, float x, float y, float z) {
    Record record{};
    record.name = name;
    record.arguments = arguments;
    write_word(record.bytes, enroll(name));
    write_half(record.bytes + 6, type_half);
    write_float(record.bytes + 8, x);
    write_float(record.bytes + 12, y);
    write_float(record.bytes + 16, z);
    write_word(record.bytes + 20, enroll(arguments));
    return record;
}
}

extern "C" void* norrath_megatile_host_pointer(unsigned id) {
    return id < registry.size() ? registry[id] : nullptr;
}
extern "C" void norrath_megatile_host_note_word(const unsigned char*, unsigned) {}
extern "C" void norrath_megatile_host_note_store(const unsigned char*, const void*) {}
extern "C" int norrath_megatile_host_filter(char*, unsigned code_word) {
    filter_seen = code_word;
    return filter_result;
}
extern "C" int norrath_factory_strncmp(const char* left, const char* right, unsigned count) {
    return std::strncmp(left, right, count);
}
extern "C" int norrath_factory_strcasecmp(const char* left, const char* right) {
    return ::strcasecmp(left, right);
}
extern "C" char* norrath_factory_strstr(const char* text, const char* needle) {
    return const_cast<char*>(std::strstr(text, needle));
}
extern "C" int norrath_factory_sprintf(char* output, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const int count = std::vsnprintf(output, 96, format, args);
    va_end(args);
    return count;
}
extern "C" char* norrath_megatile_add_pm_list(char* text) {
    pooled.emplace_back(text);
    return const_cast<char*>(pooled.back().c_str());
}
extern "C" char** norrath_megatile_cap_pm_pointers() {
    cap_calls += 1;
    return &cap_sentinel;
}
extern "C" char** norrath_megatile_specialparse(char** arguments, int selector, int mode,
                                                const unsigned char* point, char* name) {
    Parsed call{};
    call.arguments = arguments;
    call.selector = selector;
    call.mode = mode;
    std::memcpy(call.point, point, 12);
    call.name = name;
    parsed.push_back(call);
    return parse_result;
}
extern "C" void norrath_monster_spawn(char** arguments, const void* point, short type_word, bool filter) {
    Spawn call{};
    call.arguments = arguments;
    std::memcpy(call.point, point, 12);
    call.type_word = type_word;
    call.filter = filter;
    spawns.push_back(call);
}
extern "C" unsigned char* norrath_object_create_by_name(char* name, const unsigned char* point,
                                                       int type_word, char** arguments) {
    Created call{};
    call.name = name;
    std::memcpy(call.point, point, 12);
    call.type_word = type_word;
    call.arguments = arguments;
    created.push_back(call);
    return nullptr;
}

namespace {
void expect_point(const char* name, const unsigned char* point, float x, float y, float z) {
    expect(name, read_float(point) == x, "x");
    expect(name, read_float(point + 4) == y, "y");
    expect(name, read_float(point + 8) == z, "z");
}

void test_region_stride_and_kind0() {
    const char* name = "kind 0";
    reset();
    unsigned char wrong[10] = {9, 9, 9, 9, 9, 9, 9, 9, 9, 9};
    unsigned char row[10] = {1, 2, 0, 0, 0, 0, 0, 3, 4, 5};
    fill_row(0, wrong);
    fill_row(1, row);
    char entity[] = "crate";
    char arg0[] = "plain";
    char* args[] = {arg0, nullptr};
    Record record = make_record(entity, args, 5, 1000.f, 2000.f, 3000.f);
    norrath_megatile_do_entity_dispatch(record.bytes, 1, nullptr, nullptr, false);
    expect(name, spawns.empty() && parsed.empty() && created.size() == 1, "direct create");
    if (created.size() == 1) {
        expect(name, created[0].name == entity, "name");
        expect(name, created[0].arguments == args, "original arguments");
        expect(name, created[0].type_word == 5, "type half plus zero kind");
        expect_point(name, created[0].point, 2440.f, 3440.f, 3300.f);
    }
}

void test_kind_rotations() {
    const char* name = "rotations";
    unsigned char base[10] = {1, 2, 1, 6, 2, 0, 0, 0, 0, 0};
    const int kinds[] = {1, 2, 3};
    const float xs[] = {880.f, 1160.f, 560.f};
    const float ys[] = {280.f, 3760.f, 4040.f};
    for (int sample = 0; sample < 3; sample += 1) {
        reset();
        base[2] = static_cast<unsigned char>(kinds[sample]);
        fill_row(0, base);
        char entity[] = "crate";
        char* args[] = {entity, nullptr};
        Record record = make_record(entity, args, 0, 1000.f, 2000.f, 3000.f);
        norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, false);
        expect(name, created.size() == 1, "created");
        if (created.size() == 1) expect_point(name, created[0].point, xs[sample], ys[sample], 3000.f);
    }
}

void test_mspawn_prefix() {
    const char* name = "mspawn";
    reset();
    unsigned char row[10] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    fill_row(0, row);
    char entity[] = "MSpawned";
    char* args[] = {entity, nullptr};
    Record record = make_record(entity, args, 5, 0.f, 0.f, 0.f);
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, true);
    expect(name, created.empty() && parsed.empty() && spawns.size() == 1, "spawn only");
    if (spawns.size() == 1) {
        expect(name, spawns[0].arguments == args, "args");
        expect(name, spawns[0].filter, "filter passed through");
        expect(name, spawns[0].type_word == static_cast<short>(5 + (1 << 14)), "combined type");
    }
    expect(name, record.bytes[6] == 5 && record.bytes[7] == 0, "type half unchanged");
}

void test_push_trigger_zeroes_type() {
    const char* name = "push trigger";
    reset();
    unsigned char row[10] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
    fill_row(0, row);
    char entity[] = "pushtrigger";
    char* args[] = {entity, nullptr};
    Record record = make_record(entity, args, 9, 0.f, 0.f, 0.f);
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, false);
    expect(name, record.bytes[6] == 0 && record.bytes[7] == 0, "type half cleared");
    expect(name, created.size() == 1 && created[0].type_word == (1 << 14), "create uses cleared type");
}

void test_specialparse_and_filter() {
    const char* name = "specialparse";
    reset();
    unsigned char row[10] = {0, 0, 4, 0, 0, 0, 0, 0, 0, 0};
    fill_row(0, row);
    char entity[] = "crate";
    char bang[] = "see!!!this";
    char* args[] = {bang, nullptr};
    Record record = make_record(entity, args, 3, 10.f, 20.f, 30.f);
    write_word(norrath_restore_filter_word, 0u);
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, true);
    expect(name, parsed.size() == 1 && created.empty() && spawns.empty(), "filter word blocks create");
    if (parsed.size() == 1) {
        expect(name, parsed[0].selector == 4 && parsed[0].mode == 0, "selector and mode");
        expect(name, parsed[0].name == entity && parsed[0].arguments == args, "parse inputs");
        expect(name, read_float(parsed[0].point) == 10.f, "point x");
    }

    reset();
    fill_row(0, row);
    record = make_record(entity, args, 3, 10.f, 20.f, 30.f);
    write_word(norrath_restore_filter_word, 9u);
    filter_result = 0;
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, true);
    expect(name, filter_seen == 9u && created.empty(), "rejected filter");

    reset();
    fill_row(0, row);
    record = make_record(entity, args, 3, 10.f, 20.f, 30.f);
    write_word(norrath_restore_filter_word, 9u);
    filter_result = 1;
    static char* parsed_args[] = {bang, nullptr};
    parse_result = parsed_args;
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, true);
    expect(name, created.size() == 1 && created[0].arguments == parsed_args, "create uses parse result");
}

void test_trigger_name_parses() {
    const char* name = "trigger name";
    reset();
    unsigned char row[10] = {};
    row[2] = 2;
    fill_row(0, row);
    char entity[] = "TrIgGeR";
    char arg[] = "w=1";
    char* args[] = {arg, nullptr};
    Record record = make_record(entity, args, 0, 0.f, 0.f, 0.f);
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, false);
    expect(name, parsed.size() == 1 && parsed[0].selector == 2, "trigger forces parse");
}

void test_delayed_record() {
    const char* name = "delayed";
    reset();
    unsigned char row[10] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
    fill_row(0, row);
    char entity[] = "crate";
    char bang[] = "!!!closestdynamiclevel";
    char* args[] = {bang, nullptr};
    Record record = make_record(entity, args, 8, 0.f, 0.f, 40.f);
    parse_result = nullptr;
    unsigned char delayed[56];
    std::memset(delayed, 0xab, sizeof delayed);
    int count = 1;
    norrath_megatile_do_entity_dispatch(record.bytes, 0, delayed, &count, true);
    expect(name, created.empty() && spawns.empty() && cap_calls == 0, "no create");
    expect(name, count == 2, "count incremented");
    expect(name, read_float(delayed + 28) == 0.f, "x");
    expect(name, read_float(delayed + 32) == 0.f, "y");
    expect(name, read_float(delayed + 36) == 100.f, "z includes byte9*60");
    expect(name, delayed[28 + 16] == 1 && delayed[28 + 17] == 0, "final type is the region kind");
    expect(name, parsed.size() == 1, "parse ran before the null return");
}

void test_negative_origin() {
    const char* name = "negative origin";
    reset();
    unsigned char row[10] = {0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    fill_row(0, row);
    char entity[] = "crate";
    char* args[] = {entity, nullptr};
    Record record = make_record(entity, args, 0, 0.f, 5.f, 6.f);
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, false);
    expect(name, created.size() == 1, "created");
    if (created.size() == 1) expect_point(name, created[0].point, 720.f, 5.f, 6.f);
}

void test_kitten() {
    const char* name = "kitten";
    reset();
    unsigned char row[10] = {};
    fill_row(0, row);
    char entity[] = "KiTtEnBob";
    char arg[] = "tail";
    char* args[] = {arg, nullptr};
    Record record = make_record(entity, args, 0, 1.f, 2.f, 3.f);
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, false);
    expect(name, parsed.empty(), "kitten is not specialparse");
    expect(name, pooled.size() == 1 && pooled[0] == "name=kitten0", pooled.empty() ? "missing" : pooled[0]);
    expect(name, read_word(norrath_megatile_counters_view + 16) == 1u, "kitten count");
    expect(name, read_word(norrath_megatile_counters_view + 4) == 2u, "name plus argument");
    expect(name, cap_calls == 1 && created.size() == 1 && created[0].arguments == &cap_sentinel,
           "create uses the capped list");

    reset();
    fill_row(0, row);
    char shorter[] = "kitte";
    record = make_record(shorter, args, 0, 0.f, 0.f, 0.f);
    norrath_megatile_do_entity_dispatch(record.bytes, 0, nullptr, nullptr, false);
    expect(name, pooled.empty() && cap_calls == 0 && created.size() == 1, "shorter name is not a prefix");
}
}

int main() {
    test_region_stride_and_kind0();
    test_kind_rotations();
    test_mspawn_prefix();
    test_push_trigger_zeroes_type();
    test_specialparse_and_filter();
    test_trigger_name_parses();
    test_delayed_record();
    test_negative_origin();
    test_kitten();
    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::printf("do_entity_dispatch host checks passed\n");
    return 0;
}

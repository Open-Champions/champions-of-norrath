#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void norrath_construct_Line(unsigned char* line, const void* point, short type, char** arguments);
extern "C" unsigned long long norrath_line_host_base = 0;
extern "C" unsigned char norrath_route_world_word_bytes[4] = {};

static int failures = 0;
static std::vector<std::string> calls;
static int alloc_bytes = -1;
static const char* alloc_label = nullptr;
static unsigned char* alloc_result = nullptr;
static short seen_angle = 1;
static int seen_axis = -1;
static void* init_object = nullptr;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static int read_int(const unsigned char* p) {
    unsigned value = read_word(p);
    return value < 0x80000000u ? static_cast<int>(value) : static_cast<int>(value - 0x80000000u) - 2147483647 - 1;
}
static float read_float(const unsigned char* p) {
    union {
        unsigned bits;
        float number;
    } converted;
    converted.bits = read_word(p);
    return converted.number;
}
static unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
static void set_world(unsigned char* world) {
    write_word(norrath_route_world_word_bytes, low_pointer(world));
}

extern "C" char* norrath_factory_strstr(const char* text, const char* needle) {
    return const_cast<char*>(std::strstr(text, needle));
}
extern "C" char* norrath_factory_strchr(const char* text, int character) {
    return const_cast<char*>(std::strchr(text, character));
}
extern "C" int norrath_factory_strcasecmp(const char* left, const char* right) {
    while (*left != 0 || *right != 0) {
        unsigned a = static_cast<unsigned char>(*left);
        unsigned b = static_cast<unsigned char>(*right);
        if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
        if (b >= 'A' && b <= 'Z') b = b - 'A' + 'a';
        if (a != b) return static_cast<int>(a) - static_cast<int>(b);
        if (a == 0) return 0;
        ++left;
        ++right;
    }
    return 0;
}
extern "C" int sscanf(const char* text, const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    int count = std::vsscanf(text, format, args);
    va_end(args);
    return count;
}
extern "C" void* memAlloc__FiPc(int bytes, char* label) {
    calls.push_back("alloc");
    alloc_bytes = bytes;
    alloc_label = label;
    return alloc_result;
}
extern "C" void getMegatileOriginalCenter__FG6Point2(void* center, const void* point) {
    (void)point;
    calls.push_back("original");
    write_word(static_cast<unsigned char*>(center), 0x41200000u);
    write_word(static_cast<unsigned char*>(center) + 4, 0x41a00000u);
}
extern "C" void getMegatileCenter__FG6Point2(void* center, const void* point) {
    (void)point;
    calls.push_back("center");
    write_word(static_cast<unsigned char*>(center), 0x3f800000u);
    write_word(static_cast<unsigned char*>(center) + 4, 0x40000000u);
}
extern "C" void matIdent__FP8Matrix33(void* matrix) {
    (void)matrix;
    calls.push_back("ident");
}
extern "C" void matRot__FP8Matrix33is(void* matrix, int axis, short angle) {
    (void)matrix;
    calls.push_back("rot");
    seen_axis = axis;
    seen_angle = angle;
}
extern "C" void matTransform__FP6Point3P8Matrix33T0(void* destination, void* matrix, const void* source) {
    (void)matrix;
    calls.push_back("transform");
    std::memcpy(destination, source, 12);
    auto* out = static_cast<unsigned char*>(destination);
    union {
        unsigned bits;
        float number;
    } value;
    value.bits = read_word(out);
    value.number += 100.0f;
    write_word(out, value.bits);
}
extern "C" void initializeMe__4Line(void* line) {
    calls.push_back("init");
    init_object = line;
}

static bool saw(const char* name) {
    for (const auto& call : calls)
        if (call == name) return true;
    return false;
}

int main() {
    alignas(256) static unsigned char arena[4096];
    norrath_line_host_base = reinterpret_cast<unsigned long long>(arena) & ~0xffffffffull;
    unsigned char* world = arena;
    unsigned char* line = arena + 256;
    alloc_result = arena + 1024;
    std::memset(arena, 0, sizeof arena);
    unsigned char point[12] = {0, 0, 0x80, 0x3f, 0, 0, 0, 0x40, 0, 0, 0xa0, 0x40};
    char* none[] = {nullptr};
    set_world(world);
    norrath_construct_Line(line, point, 7, none);
    expect(init_object == line && calls.size() == 1u && calls[0] == "init", "a null argument list only initializes");
    expect(read_int(line + 8) == -1 && read_word(line) == 0u && read_word(line + 4) == 0u &&
               read_word(line + 12) == 0u && read_word(line + 20) == 0u,
           "empty construction clears the header and sets word 8 to -1");

    calls.clear();
    std::memset(line, 0x11, 32);
    char barrier_text[] = "name=Barrier";
    char other_text[] = "name=door";
    char continuous_text[] = "continous=6";
    char spelled_right[] = "continuous=4";
    char* names[] = {barrier_text, other_text, continuous_text, spelled_right, nullptr};
    norrath_construct_Line(line, point, 7, names);
    expect(read_word(line) == low_pointer(other_text + 5), "the last name= value replaces the stored pointer");
    expect((read_word(line + 12) & 1u) == 1u, "Barrier sets the flag without matching case");
    expect(read_int(line + 8) == 6, "the observed continous spelling stores the integer");
    expect(!saw("alloc"), "name and continous arguments do not allocate");

    calls.clear();
    world[112] = 1;
    char norotate_text[] = "norotate=3";
    char* rotated[] = {norotate_text, nullptr};
    norrath_construct_Line(line, point, 9, rotated);
    expect(!saw("original") && !saw("rot") && saw("init"), "a nonzero norotate skips the megatile adjustment");

    calls.clear();
    world[112] = 0;
    char count_text[] = "pcount=2";
    char first_point[] = "p=1,2,3";
    char second_point[] = "p=4,5,6";
    char* forward[] = {count_text, first_point, second_point, nullptr};
    norrath_construct_Line(line, point, -15, forward);
    expect(alloc_bytes == 128 && alloc_label && std::strcmp(alloc_label, "point list") == 0, "two points allocate 128 bytes");
    expect(read_float(alloc_result) == 1.0f && read_float(alloc_result + 4) == 2.0f && read_float(alloc_result + 8) == 3.0f,
           "the first argument fills slot 0");
    expect(read_float(alloc_result + 64) == 4.0f && read_int(alloc_result + 60) == 0 && read_int(alloc_result + 124) == 1,
           "slot indices follow slot order");
    expect(!saw("original"), "a zero world mode does not query megatiles");

    calls.clear();
    char reverse_count[] = "pcount=-2";
    char* backward[] = {reverse_count, first_point, second_point, nullptr};
    norrath_construct_Line(line, point, 7, backward);
    expect(read_float(alloc_result) == 4.0f && read_float(alloc_result + 64) == 1.0f,
           "a negative count reads the point arguments in reverse");
    expect(read_int(alloc_result + 60) == 0 && read_int(alloc_result + 124) == 1, "reverse slots still store 0 then 1");

    calls.clear();
    world[112] = 1;
    char one_count[] = "pcount=1";
    char one_point[] = "p=3,4,5";
    char* adjusted[] = {one_count, one_point, nullptr};
    norrath_construct_Line(line, point, 30, adjusted);
    expect(saw("original") && saw("center") && saw("ident") && saw("rot") && saw("transform") && saw("init"),
           "a live world mode adjusts and then initializes");
    expect(seen_axis == 2 && seen_angle == 30, "rotation uses axis 2 and the type halfword");
    expect(read_float(alloc_result) == 94.0f && read_float(alloc_result + 4) == -14.0f && read_float(alloc_result + 8) == 5.0f,
           "original center, transform, and current center combine, and z is kept");
    expect(calls.back() == "init", "initializeMe is the last call");
    return failures == 0 ? 0 : 1;
}

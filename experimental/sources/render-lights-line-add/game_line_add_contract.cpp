// Host contract for norrath_game_line_add. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_game_line_count;
extern "C" unsigned char* norrath_game_lines[100];
extern "C" void norrath_game_line_add(const void* point, short type, char** arguments);

static int failures = 0;
static int allocs = 0;
static int requested = -1;
static unsigned count_at_alloc = 0xffffffffu;
static unsigned char* alloc_result = nullptr;
static int constructs = 0;
static unsigned char* seen_line = nullptr;
static const unsigned char* seen_point = nullptr;
static unsigned char seen_bytes[12];
static short seen_type = 0;
static char** seen_args = nullptr;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" unsigned char* norrath_block_alloc(int size) {
    ++allocs;
    requested = size;
    count_at_alloc = norrath_game_line_count;
    return alloc_result;
}

extern "C" void norrath_construct_Line(unsigned char* line, const void* point, short type, char** arguments) {
    ++constructs;
    seen_line = line;
    seen_point = static_cast<const unsigned char*>(point);
    std::memcpy(seen_bytes, point, 12);
    seen_type = type;
    seen_args = arguments;
}

static void reset_calls() {
    allocs = 0;
    requested = -1;
    count_at_alloc = 0xffffffffu;
    constructs = 0;
    seen_line = nullptr;
    seen_point = nullptr;
    seen_type = 0;
    seen_args = nullptr;
    std::memset(seen_bytes, 0, sizeof seen_bytes);
}

int main() {
    expect(norrath_game_line_count == 0u, "count starts at zero");
    for (unsigned char* slot : norrath_game_lines) expect(slot == nullptr, "line slots start clear");

    alignas(4) unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char* args[] = {const_cast<char*>("a"), nullptr};
    unsigned char storage[24];
    alloc_result = storage;
    reset_calls();
    norrath_game_line_add(point, 7, args);
    expect(allocs == 1, "one allocation");
    expect(requested == 24, "line is 24 bytes");
    expect(count_at_alloc == 1u, "count is incremented before allocation");
    expect(norrath_game_line_count == 1u, "count remains incremented");
    expect(constructs == 1, "constructor runs for a live allocation");
    expect(seen_line == storage, "constructor receives the allocation");
    expect(seen_point != point, "constructor receives the copied point");
    expect(std::memcmp(seen_bytes, point, 12) == 0, "copied point bytes");
    expect(seen_type == 7, "positive type");
    expect(seen_args == args, "argument list");
    expect(norrath_game_lines[0] == storage, "first slot");
    expect(norrath_game_lines[1] == nullptr, "second slot stays clear");

    unsigned char second[24];
    alignas(4) unsigned char other[12];
    for (int i = 0; i < 12; ++i) other[i] = static_cast<unsigned char>(20 + i);
    alloc_result = second;
    reset_calls();
    norrath_game_line_add(other, static_cast<short>(-2), args + 1);
    expect(count_at_alloc == 2u, "second increment is visible");
    expect(norrath_game_line_count == 2u, "count is two");
    expect(seen_type == static_cast<short>(-2), "negative type stays signed");
    expect(std::memcmp(seen_bytes, other, 12) == 0, "second point");
    expect(seen_args == args + 1, "second arguments");
    expect(norrath_game_lines[1] == second, "second slot");
    expect(norrath_game_lines[0] == storage, "first slot is retained");

    alignas(4) unsigned char doomed[12] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 11, 12};
    alloc_result = nullptr;
    reset_calls();
    norrath_game_line_add(doomed, 1, args);
    expect(allocs == 1, "null allocation is still requested");
    expect(count_at_alloc == 3u, "count advances when allocation fails");
    expect(constructs == 0, "null allocation skips the constructor");
    expect(norrath_game_lines[2] == nullptr, "null pointer is stored");

    alignas(4) unsigned char mutable_point[12] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41};
    unsigned char saved[12];
    std::memcpy(saved, mutable_point, 12);
    alloc_result = storage;
    reset_calls();
    norrath_game_line_count = 99u;
    norrath_game_line_add(mutable_point, 4, args);
    std::memset(mutable_point, 0x5a, sizeof mutable_point);
    expect(norrath_game_line_count == 100u, "last counted slot advances to 100");
    expect(norrath_game_lines[99] == storage, "last slot");
    expect(std::memcmp(seen_bytes, saved, 12) == 0, "constructor saw the pre-call point");
    expect(seen_point != mutable_point, "copy is distinct storage");

    if (failures != 0) return 1;
    std::printf("game-line-add contract passed\n");
    return 0;
}

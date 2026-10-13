#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16 | 1 << 15];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned norrath_host_address(const unsigned char* p) { return arena_base + static_cast<unsigned>(p - arena); }
#include "parse.cpp"
#include <string.h>

static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static int file_type, objects, parse_calls, fail_at;
static void* type_arg; static void* num_arg;
static unsigned snapshot[10][12];
extern "C" int FileType__9VIObjFile(void* f) { type_arg = f; return file_type; }
extern "C" int NumObjects__9VIObjFile(void* f) { num_arg = f; return objects; }
extern "C" int ParseObject__10VIESFParse(void* p) {
    if (parse_calls < 10) for (int i = 0; i < 12; ++i) snapshot[parse_calls][i] = reinterpret_cast<unsigned*>(p)[i];
    return ++parse_calls == fail_at ? -5 : 0;
}

int main() {
    const unsigned parser = 0, file = 1024, scene = 2048;     // scene is 19368 bytes long
    memset(arena, 0, sizeof arena);
    W(scene + 19352) = 0x1111; W(scene + 19356) = 0x2222; W(scene + 19360) = 0x3333; W(scene + 19364) = 0x4444;
    file_type = 0xab4f; objects = 3; parse_calls = 0; fail_at = 0;
    if (Parse__10VIESFParseR9VIObjFileR7VIScene(arena + parser, arena + file, arena + scene) != 0) return 1;
    if (parse_calls != 3 || type_arg != arena + file || num_arg != arena + file) return 2;
    for (int i = 0; i < 3; ++i) {
        if (snapshot[i][6] != arena_base + scene || snapshot[i][9] != arena_base + file) return 3;
        if (snapshot[i][3] != 0x2222 || snapshot[i][4] != 0x3333 || snapshot[i][5] != 0x4444 || snapshot[i][8] != 0x1111 || snapshot[i][7] != 0) return 4;
    }
    for (int i = 0; i < 12; ++i) if (W(parser + 4 * i) != 0) return 5;     // everything cleared again
    // wrong file type: -1 and the fields stay set
    memset(arena, 0, 64); file_type = 0x1234; parse_calls = 0;
    if (Parse__10VIESFParseR9VIObjFileR7VIScene(arena + parser, arena + file, arena + scene) != -1 || parse_calls != 0) return 6;
    if (W(24) != arena_base + scene || W(36) != arena_base + file || W(20) != 0x4444 || W(32) != 0x1111 || W(12) != 0x2222 || W(16) != 0x3333 || W(28) != 0) return 7;
    // an object failure on the second object
    memset(arena, 0, 64); file_type = 0xab4f; parse_calls = 0; fail_at = 2;
    if (Parse__10VIESFParseR9VIObjFileR7VIScene(arena + parser, arena + file, arena + scene) != -1 || parse_calls != 2) return 8;
    if (W(36) != arena_base + file) return 9;
    // zero objects: no parse calls, cleared
    memset(arena, 0, 64); parse_calls = 0; fail_at = 0; objects = 0;
    if (Parse__10VIESFParseR9VIObjFileR7VIScene(arena + parser, arena + file, arena + scene) != 0 || parse_calls != 0 || W(24) != 0 || W(36) != 0) return 10;
    return 0;
}

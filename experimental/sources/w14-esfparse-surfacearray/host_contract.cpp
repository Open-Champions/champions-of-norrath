#include <stdint.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
#define NORRATH_WORD_ADDRESS(word) host_address(word)
#include "parse_surface_array.cpp"

static std::string tr; static int count, end_ret; static int surface_ret[8]; static int surface_calls;
static void* file_seen;
extern "C" int NumSubObjects__9VIObjFile(void* f) { file_seen = f; tr += "N;"; return count; }
extern "C" int ReadEnd__9VIObjFile(void* f) { if (f != file_seen && file_seen) tr += "BADFILE"; tr += "E;"; return end_ret; }
extern "C" int ParseSurface__10VIESFParse(void* p) { (void)p; tr += "S;"; return surface_ret[surface_calls++]; }

int main() {
    alignas(8) unsigned char parse[64] = {0};
    unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[9] = 0x55;
    // flag clear: only ReadEnd runs
    w[3] = 0; count = 3; end_ret = 0; tr.clear();
    if (ParseSurfaceArrayObj__10VIESFParse(parse) != 0 || tr != "E;") return 1;
    end_ret = -2; tr.clear();
    if (ParseSurfaceArrayObj__10VIESFParse(parse) != -1) return 2;
    // flag set: count is re-read each pass, surfaces parsed in order
    w[3] = 1; count = 2; end_ret = 5; tr.clear(); surface_calls = 0;
    if (ParseSurfaceArrayObj__10VIESFParse(parse) != 0 || tr != "N;S;N;S;N;E;" || surface_calls != 2) return 3;
    count = 0; tr.clear();
    if (ParseSurfaceArrayObj__10VIESFParse(parse) != 0 || tr != "N;E;") return 4;
    // a negative surface result stops at once, without ReadEnd
    count = 3; surface_calls = 0; surface_ret[0] = 0; surface_ret[1] = -1; tr.clear();
    if (ParseSurfaceArrayObj__10VIESFParse(parse) != -1 || tr != "N;S;N;S;") return 5;
    return 0;
}

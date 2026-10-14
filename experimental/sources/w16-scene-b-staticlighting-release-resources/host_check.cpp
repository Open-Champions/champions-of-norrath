// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "release_resources.cpp"
#include <cstdio>
#include <vector>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
struct Call { void* raster; int buffer; void* dictionary; };
static std::vector<Call> calls;
extern "C" int ReleaseColorBuffer__8VIRasteriR12VIDictionary(void* raster, int buffer, void* dictionary) {
    calls.push_back({raster, buffer, dictionary});
    return 0;
}

int main() {
    bool ok = true;
    int raster, dictionary;
    for (int n : {0, 1, 3, -2}) {
        calls.clear();
        unsigned* self = reinterpret_cast<unsigned*>(arena);
        self[2] = static_cast<unsigned>(n);
        self[3] = 0x400;
        int* data = reinterpret_cast<int*>(arena + 0x400);
        for (int i = 0; i < 4; ++i) data[i] = 50 + i;
        if (ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary(arena, &raster, &dictionary) != 0) ok = false;
        int expect = n > 0 ? n : 0;
        if (static_cast<int>(calls.size()) != expect) ok = false;
        for (int i = 0; i < static_cast<int>(calls.size()); ++i)
            if (calls[i].raster != &raster || calls[i].dictionary != &dictionary || calls[i].buffer != 50 + i) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

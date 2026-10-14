// Host contract for inflateEnd. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// On the 64-bit host the stream record is built by field offset with the real pointer width, so only the
// logic is checked: -2 for a null stream, null state or null release function; the block free called with
// (blocks, stream) only when blocks is set; the release function called with (opaque, state) after it;
// the state pointer cleared; 0 returned. The 32-bit layout (offsets 28, 36, 40, state offset 20) is covered
// by the private paired run in the notes.
#include <cstdio>
#include <string>
static std::string trace;
static void *free_blocks, *free_stream, *rel_opaque, *rel_addr;
#include "inflate_end.cpp"
extern "C" int inflate_blocks_free(void* b, NorrathZStream* s) { trace += "B;"; free_blocks = b; free_stream = s; return 0; }
static void release(void* o, void* a) { trace += "R;"; rel_opaque = o; rel_addr = a; }
int main() {
    int bad = 0;
    if (inflateEnd(nullptr) != -2) { std::puts("null stream"); ++bad; }
    NorrathZStream z{}; NorrathZState st{}; int blocks = 0, opaque = 0;
    if (inflateEnd(&z) != -2) { std::puts("null state"); ++bad; }
    z.state = &st;
    if (inflateEnd(&z) != -2 || z.state != &st) { std::puts("null release"); ++bad; }
    z.release = release; z.opaque = &opaque;
    trace.clear();
    if (inflateEnd(&z) != 0 || trace != "R;" || rel_opaque != &opaque || rel_addr != &st || z.state != nullptr) { std::puts("no blocks"); ++bad; }
    z.state = &st; st.blocks = &blocks; trace.clear();
    if (inflateEnd(&z) != 0 || trace != "B;R;" || free_blocks != &blocks || free_stream != &z || rel_addr != &st || z.state != nullptr) { std::puts("with blocks"); ++bad; }
    return bad;
}

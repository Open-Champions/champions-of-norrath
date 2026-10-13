// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "destroypool.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace; static int end_value;
static unsigned* M() { return reinterpret_cast<unsigned*>(arena + 0x100); }
static unsigned* N(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 112 * i); }
extern "C" int End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void*) { return end_value; }
extern "C" void _$_10VIMatrix44(void* p, int f) { trace += std::to_string((static_cast<unsigned char*>(p) - arena - 0x400 - 48) / 112) + (f == 2 ? ";" : "?;"); }
static void link(int i, int parent, int l, int r) { N(i)[1] = parent; N(i)[2] = l; N(i)[3] = r; }
int main() {
    bool ok = true;
    std::memset(arena, 0x5a, sizeof arena);
    M()[2] = 0x400; end_value = 0;
    // empty tree
    M()[3] = 0; trace.clear();
    if (DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(M()) != 0 || !trace.empty()) ok = false;
    // keys by in-order position: root 4, 4.left = 2 (1,3), 4.right = 6 (5,7); 7 has right child 8
    M()[3] = 4; trace.clear();
    link(4, 0, 2, 6); link(2, 4, 1, 3); link(1, 2, 0, 0); link(3, 2, 0, 0);
    link(6, 4, 5, 7); link(5, 6, 0, 0); link(7, 6, 0, 8); link(8, 7, 0, 0);
    if (DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(M()) != 0 || trace != "1;2;3;4;5;6;7;8;") ok = false;
    // single node
    M()[3] = 3; trace.clear(); link(3, 0, 0, 0);
    DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(M());
    if (trace != "3;") ok = false;
    // a chain of right children
    M()[3] = 1; trace.clear(); link(1, 0, 0, 2); link(2, 1, 0, 3); link(3, 2, 0, 0);
    DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(M());
    if (trace != "1;2;3;") ok = false;
    // a chain of left children
    M()[3] = 3; trace.clear(); link(3, 0, 2, 0); link(2, 3, 1, 0); link(1, 2, 0, 0);
    DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(M());
    if (trace != "1;2;3;") ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "erase.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x4000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
unsigned norrath_host_address(const void* pointer) { return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - arena); }
static std::string trace;
extern "C" void _$_7VIActor(void* a, int flags) { trace += "dtor@" + std::to_string(norrath_host_address(a)) + "," + std::to_string(flags) + ";"; }
extern "C" void __builtin_delete(void* p) { trace += "delete@" + std::to_string(norrath_host_address(p)) + ";"; }
extern "C" int End__Ct6VIList1Z7VIActor(const void* l) { trace += "end@" + std::to_string(norrath_host_address(l)) + ";"; return 0x7fffffff; }
static const unsigned A = 0x1000, B = 0x2000, C = 0x3000;
static unsigned* actor(unsigned a) { return reinterpret_cast<unsigned*>(arena + a); }
static unsigned* L() { return reinterpret_cast<unsigned*>(arena + 0x100); }
static void setup() {
    std::memset(arena, 0x5a, sizeof arena); trace.clear();
    L()[0] = 3; L()[1] = A; L()[2] = C;
    actor(A)[40] = B; actor(A)[41] = 0;
    actor(B)[40] = C; actor(B)[41] = A;
    actor(C)[40] = 0; actor(C)[41] = B;
}
int main() {
    bool ok = true;
    setup();
    if (Erase__t6VIList1Z7VIActori(L(), B) != static_cast<int>(C)) ok = false;
    if (trace != "dtor@8192,2;delete@8192;") ok = false;
    if (L()[0] != 2 || L()[1] != A || L()[2] != C || actor(A)[40] != C || actor(C)[41] != A) ok = false;
    setup();
    if (Erase__t6VIList1Z7VIActori(L(), A) != static_cast<int>(B)) ok = false;
    if (L()[0] != 2 || L()[1] != B || L()[2] != C || actor(B)[41] != 0) ok = false;
    setup();
    if (Erase__t6VIList1Z7VIActori(L(), C) != 0x7fffffff) ok = false;
    if (trace != "dtor@12288,2;delete@12288;end@256;") ok = false;
    if (L()[0] != 2 || L()[1] != A || L()[2] != B || actor(B)[40] != 0) ok = false;
    // single element list
    setup();
    L()[0] = 1; L()[1] = B; L()[2] = B; actor(B)[40] = 0; actor(B)[41] = 0;
    if (Erase__t6VIList1Z7VIActori(L(), B) != 0x7fffffff) ok = false;
    if (L()[0] != 0 || L()[1] != 0 || L()[2] != 0) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

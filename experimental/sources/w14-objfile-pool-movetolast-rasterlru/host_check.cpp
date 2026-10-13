// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "movetolast.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x1000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned* P() { return reinterpret_cast<unsigned*>(arena + 0x100); }
static unsigned* E(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 24 * i); }
static const unsigned N = 0xffffffffu;
// chain through B: front(pool+20) = 3, 3->1->2 (B), A links back
static void setup() {
    std::memset(arena, 0x5a, sizeof arena);
    P()[2] = 0x400; P()[5] = 3; P()[4] = 2;
    E(3)[4] = N; E(3)[5] = 1;
    E(1)[4] = 3; E(1)[5] = 2;
    E(2)[4] = 1; E(2)[5] = N;
}
int main() {
    bool ok = true;
    // move the middle entry to the front: 1, 3, 2
    setup(); MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(P(), 1);
    if (P()[5] != 1 || P()[4] != 2 || E(1)[4] != N || E(1)[5] != 3 || E(3)[4] != 1 || E(3)[5] != 2 || E(2)[4] != 3 || E(2)[5] != N) ok = false;
    // move the front entry: unchanged order
    setup(); MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(P(), 3);
    if (P()[5] != 3 || P()[4] != 2 || E(3)[4] != N || E(3)[5] != 1 || E(1)[4] != 3 || E(1)[5] != 2 || E(2)[4] != 1) ok = false;
    // move the back entry: 2, 3, 1
    setup(); MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(P(), 2);
    if (P()[5] != 2 || P()[4] != 1 || E(2)[4] != N || E(2)[5] != 3 || E(3)[4] != 2 || E(3)[5] != 1 || E(1)[4] != 3 || E(1)[5] != N) ok = false;
    // a single entry list
    std::memset(arena, 0x5a, sizeof arena);
    P()[2] = 0x400; P()[5] = 4; P()[4] = 4; E(4)[4] = N; E(4)[5] = N;
    MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(P(), 4);
    if (P()[5] != 4 || P()[4] != 4 || E(4)[4] != N || E(4)[5] != N) ok = false;
    // an entry outside the list while the list is empty
    std::memset(arena, 0x5a, sizeof arena);
    P()[2] = 0x400; P()[5] = N; P()[4] = N; E(5)[4] = N; E(5)[5] = N;
    MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(P(), 5);
    if (P()[5] != 5 || P()[4] != 5 || E(5)[4] != N || E(5)[5] != N) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

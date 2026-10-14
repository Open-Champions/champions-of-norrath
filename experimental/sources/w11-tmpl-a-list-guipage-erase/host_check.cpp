#include "erase.cpp"
#include <cstring>
static void* deleted;
static int dels;
extern "C" void __builtin_delete(void* p) { deleted = p; ++dels; }
int main() {
    // Three nodes a, b, c linked a<->b<->c, list head a, tail c, count 3.
    static unsigned L[4], a[4], b[4], c[4];
    auto set = [&]() {
        std::memset(L, 0, sizeof L); std::memset(a, 0, sizeof a); std::memset(b, 0, sizeof b); std::memset(c, 0, sizeof c);
        auto u = [](unsigned* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); };
        L[0] = 3; L[1] = u(a); L[2] = u(c);
        a[1] = u(b); b[1] = u(c); b[2] = u(a); c[2] = u(b);
        dels = 0;
    };
    auto u = [](unsigned* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); };
    set();
    if (Erase__t6VIList1ZP9VIGUIPagei(L, u(b)) != static_cast<int>(u(c))) return 1;
    if (dels != 1 || deleted != b || L[0] != 2 || L[1] != u(a) || L[2] != u(c) || a[1] != u(c) || c[2] != u(a)) return 2;
    set();
    if (Erase__t6VIList1ZP9VIGUIPagei(L, u(a)) != static_cast<int>(u(b))) return 3;
    if (deleted != a || L[0] != 2 || L[1] != u(b) || L[2] != u(c) || b[2] != 0) return 4;
    set();
    if (Erase__t6VIList1ZP9VIGUIPagei(L, u(c)) != 0x7fffffff) return 5;
    if (deleted != c || L[0] != 2 || L[1] != u(a) || L[2] != u(b) || b[1] != 0) return 6;
    // sole node
    set();
    L[0] = 1; L[1] = u(a); L[2] = u(a); a[1] = 0; a[2] = 0;
    if (Erase__t6VIList1ZP9VIGUIPagei(L, u(a)) != 0x7fffffff) return 7;
    if (L[0] != 0 || L[1] != 0 || L[2] != 0) return 8;
    return 0;
}

// Host check only; not a build input.
#include "next.cpp"
#include <cstdio>

int main() {
    Node nodes[3] = {{1, 0x1234}, {2, 0}, {3, -5}};
    bool ok = Next__Ct6VIList1ZP9VIGUIPagei(nullptr, &nodes[0]) == 0x1234;
    ok = ok && Next__Ct6VIList1ZP9VIGUIPagei(nullptr, &nodes[1]) == 0x7fffffff;
    ok = ok && Next__Ct6VIList1ZP9VIGUIPagei(nullptr, &nodes[2]) == -5;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

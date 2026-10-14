// Host check only; not a build input.
#include "destroypool.cpp"
#include <cstdio>
#include <string>

static std::string trace;
static int begin_value, end_value;
extern "C" int Begin__Ct5VISet1ZP5VIWnd(const void*) { trace += "B;"; return begin_value; }
extern "C" int End__Ct5VISet1ZP5VIWnd(const void*) { trace += "E;"; return end_value; }
extern "C" int Next__Ct5VISet1ZP5VIWndi(const void*, int i) { trace += "N" + std::to_string(i) + ";"; return i + 1; }

int main() {
    bool ok = true;
    int object;
    begin_value = 0; end_value = 0; trace.clear();
    ok = ok && DestroyPool__t5VISet1ZP5VIWnd(&object) == 0 && trace == "B;E;";
    begin_value = 1; end_value = 3; trace.clear();
    ok = ok && DestroyPool__t5VISet1ZP5VIWnd(&object) == 0 && trace == "B;E;N1;E;N2;E;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

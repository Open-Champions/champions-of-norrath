// Host check only; not a build input. Iteration helpers are scripted: Begin returns the start,
// Next follows a table, End returns a fixed sentinel. The walk must visit the table in order and
// must call End once per test (including the failing one).
#include "destroy.cpp"
#include <cstdio>
#include <string>
static std::string trace;
static const int end_value = 99;
static const int chain[] = {3, 7, 5, 99};  // Begin gives 1; Next(1)=3 -> 7 -> 5 -> 99
static int begin_value;
extern "C" int Begin__Ct6VIPool1ZP12VICollBuffer(const void*) { trace += "B;"; return begin_value; }
extern "C" int End__Ct6VIPool1ZP12VICollBuffer(const void*) { trace += "E;"; return end_value; }
extern "C" int Next__Ct6VIPool1ZP12VICollBufferi(const void*, int c) { trace += "N" + std::to_string(c) + ";"; return c == 1 ? 3 : c == 3 ? 7 : c == 7 ? 5 : 99; }
int main() {
    (void)chain;
    bool ok = true;
    unsigned char pool[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    begin_value = 99; trace.clear();
    if (DestroyPool__t6VIPool1ZP12VICollBuffer(pool) != 0 || trace != "B;E;") { std::puts(trace.c_str()); ok = false; }
    begin_value = 1; trace.clear();
    if (DestroyPool__t6VIPool1ZP12VICollBuffer(pool) != 0 || trace != "B;E;N1;E;N3;E;N7;E;N5;E;") { std::puts(trace.c_str()); ok = false; }
    for (int i = 0; i < 8; ++i) if (pool[i] != i + 1) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

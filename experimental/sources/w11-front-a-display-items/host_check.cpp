// Host check for displayItems__FiiP12screenStructi. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
#include "display_items.cpp"
#include <cstdio>
#include <string>
static std::string ev;
static int local;
static int token;
extern "C" {
void shopInventoryGridRun__FiiiP12screenStruct(int p, int t, int f, void* s) {
    ev += "R" + std::to_string(p) + "," + std::to_string(t) + "," + std::to_string(f) + (s == &token ? ",ok;" : ",?;");
}
int netPlayerIsLocal__Fi(int p) { ev += "L" + std::to_string(p) + ";"; return local; }
void shopInventoryGridDraw__FiiiP12screenStruct(int p, int t, int f, void* s) {
    ev += "D" + std::to_string(p) + "," + std::to_string(t) + "," + std::to_string(f) + (s == &token ? ",ok;" : ",?;");
}
}
int main() {
    local = 1;
    displayItems__FiiP12screenStructi(11, 22, &token, 33);
    if (ev != "R22,33,11,ok;L22;D22,33,11,ok;") { std::puts(ev.c_str()); return 1; }
    ev.clear(); local = 0;
    displayItems__FiiP12screenStructi(-5, 2, &token, 3);
    if (ev != "R2,3,-5,ok;L2;") { std::puts(ev.c_str()); return 2; }
    ev.clear(); local = 7;
    displayItems__FiiP12screenStructi(0, 0, &token, 0);
    if (ev != "R0,0,0,ok;L0;D0,0,0,ok;") return 3;
    std::puts("display items passed");
    return 0;
}

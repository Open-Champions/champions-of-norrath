// Host contract for gameMiscTaskSync__Fv. Not an EE plan input.
#include <cstring>
static char trace[16];
static int n;
extern "C" void playerRecallSpell__Fv() { trace[n++] = 'R'; }
extern "C" void playerCheckRecallHotKey__Fv() { trace[n++] = 'K'; }
#include "misc_task_sync.cpp"
int main() {
    gameMiscTaskSync__Fv();
    if (n != 2 || std::strncmp(trace, "RK", 2) != 0) return 1;
    return 0;
}

// Host contract for P_Pop__Fv. Not an EE plan input. The body must leave
// every byte of a poisoned neighbourhood unchanged.
#include <cstdio>
#include <cstring>

extern "C" void P_Pop__Fv();
extern "C" unsigned pushLevel;
unsigned pushLevel = 7u;

int main() {
    const unsigned before = pushLevel;
    P_Pop__Fv();
    if (pushLevel != before) {
        std::fprintf(stderr, "fail: P_Pop changed state\n");
        return 1;
    }
    return 0;
}

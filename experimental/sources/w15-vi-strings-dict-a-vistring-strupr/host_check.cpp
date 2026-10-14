// strupr is stubbed: it records the pointer it received and returns a distinct sentinel.
static char* seen; static char sentinel[1];
#define strupr strupr_unused_host
#include <string.h>
#undef strupr
#include "strupr.cpp"
extern "C" char* strupr(char* t) { seen = t; return sentinel; }
int main() {
    char buf[8] = "abc";
    char* r = strupr__8VIStringPc(buf);
    if (seen != buf) return 1;
    if (r != sentinel) return 2;
    return 0;
}

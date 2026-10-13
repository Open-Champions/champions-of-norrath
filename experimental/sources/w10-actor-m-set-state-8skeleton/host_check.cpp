// Host contract for setState__8Skeletoni; not an EE input. The authored body is a stub that records its arguments.
#include <initializer_list>
static int ncalls, got_state; static void* got_self;
extern "C" void norrath_skeleton_set_state(void* s, int r) { ++ncalls; got_self = s; got_state = r; }
#include "set_state.cpp"
int main() {
    int bad = 0;
    char obj[8];
    for (int st : {0, 1, -1, 15, 0x10005, 2147483647}) {
        ncalls = 0; got_self = nullptr; got_state = 12345;
        setState__8Skeletoni(obj, st);
        if (ncalls != 1 || got_self != obj || got_state != st) bad = 1;
    }
    return bad;
}

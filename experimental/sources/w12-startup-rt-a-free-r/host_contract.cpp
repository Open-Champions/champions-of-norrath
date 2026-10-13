// Host contract for the _free_r name provider. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The authored body is scripted; the wrapper must pass the record and the allocation through, once, in order.
#include <cstdio>
static unsigned char *seen_reent, *seen_alloc;
static int calls;
extern "C" void norrath_player_free_reentrant(unsigned char* reent, unsigned char* allocation) { ++calls; seen_reent = reent; seen_alloc = allocation; }
#include "free_r.cpp"
int main() {
    unsigned char a[4], b[4];
    _free_r(a, b);
    if (calls != 1 || seen_reent != a || seen_alloc != b) return 1;
    _free_r(b, nullptr);
    if (calls != 2 || seen_reent != b || seen_alloc != nullptr) return 2;
    return 0;
}

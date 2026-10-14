// Host contract for the _malloc_r name provider. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The authored body is scripted; the wrapper must pass the record and the size through, once, in order,
// and return the body's result unchanged.
static unsigned char* seen_reent;
static unsigned seen_size;
static int calls;
static unsigned char block[8];
extern "C" unsigned char* norrath_player_malloc_reentrant(unsigned char* reent, unsigned requested) {
    ++calls; seen_reent = reent; seen_size = requested;
    return requested == 0xffffffffu ? nullptr : block;
}
#include "malloc_r.cpp"
int main() {
    unsigned char r[4];
    if (_malloc_r(r, 100) != block || calls != 1 || seen_reent != r || seen_size != 100) return 1;
    if (_malloc_r(nullptr, 0xffffffffu) != nullptr || calls != 2 || seen_reent != nullptr || seen_size != 0xffffffffu) return 2;
    return 0;
}

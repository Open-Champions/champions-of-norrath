// Host check only; not a build input.
#include "trailfx_end.cpp"
#include <cstdio>
unsigned norrath_trail_inactive[4] __asm__("VITrailFx$sInactiveTrailFx");
static int calls; static void* a[4]; static const char* names[4];
extern "C" void Remove__7VILNode(void* n) { names[calls] = "R"; a[calls++] = n; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) { names[calls] = "A"; a[calls++] = l; a[calls++] = n; }
int main() {
    int node = 0;
    VITrailFxEnd__FP9VITrailFx(&node);
    if (calls != 3 || names[0][0] != 'R' || a[0] != &node || a[1] != norrath_trail_inactive || a[2] != &node) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}

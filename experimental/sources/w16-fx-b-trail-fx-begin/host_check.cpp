// Host check only; not a build input.
#include "trail_fx_begin.cpp"
static int rem, inits, adds; static void *node_to_give, *i_node, *a_list, *a_node; static long il; static int ii; static float ic;
extern "C" {
unsigned norrath_trail_free[4], norrath_trail_active[4];
void* RemHead__7VILList(void* list) { rem += (list == norrath_trail_free); return node_to_give; }
void Init__9VITrailFxlif(void* t, long a, int b, float c) { inits++; i_node = t; il = a; ii = b; ic = c; }
void AddTail__7VILListP7VILNode(void* l, void* n) { adds++; a_list = l; a_node = n; }
}
int main() {
    static char node;
    node_to_give = &node;
    void* r = VITrailFxBegin__Flif(9, -4, 2.5f);
    if (r != &node || rem != 1 || inits != 1 || adds != 1 || i_node != &node || il != 9 || ii != -4 || ic != 2.5f) return 1;
    if (a_list != norrath_trail_active || a_node != &node) return 2;
    node_to_give = nullptr; inits = adds = 0;
    if (VITrailFxBegin__Flif(1, 2, 3.0f) != nullptr || inits || adds) return 3;
    return 0;
}

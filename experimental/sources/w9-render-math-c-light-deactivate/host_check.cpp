// Host contract for norrath_light_deactivate. Not an EE plan input.
#include <cstdio>
struct LightLinks { LightLinks* next; LightLinks* previous; };
extern "C" void norrath_light_deactivate(LightLinks*);
int main() {
    LightLinks a{}, b{}, c{};
    a.next = &b; b.previous = &a; b.next = &c; c.previous = &b;
    norrath_light_deactivate(&b);
    if (a.next != &c || c.previous != &a || b.next || b.previous) return 1;
    norrath_light_deactivate(&a);   // head: no previous
    if (c.previous != nullptr || a.next || a.previous) return 2;
    LightLinks d{}; norrath_light_deactivate(&d);
    if (d.next || d.previous) return 3;
    std::printf("light deactivate contract passed\n");
    return 0;
}

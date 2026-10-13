// Host check for debugReport: callable, changes nothing.
#include <cstdio>
extern "C" void debugReport__11AddParticle();
int main() {
    debugReport__11AddParticle();
    puts("debugReport ok");
    return 0;
}

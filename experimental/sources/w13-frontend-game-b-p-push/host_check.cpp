// Host contract for P_Push__Fv. Not an EE plan input.
#include <cstdio>
extern "C" void P_Push__Fv();
int main() {
    P_Push__Fv();
    std::printf("P_Push contract passed\n");
    return 0;
}

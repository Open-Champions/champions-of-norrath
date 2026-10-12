// Host contract for _GLOBAL_$I$addpmlist__FPc. Not an EE plan input.
// The entry only forwards to a local no-op, so the check is that it returns
// normally and can be called repeatedly. The EE object was inspected for the
// call with arguments 1 and 0xffff.
#include <cstdio>
extern "C" void _GLOBAL_$I$addpmlist__FPc();
int main() {
    _GLOBAL_$I$addpmlist__FPc();
    _GLOBAL_$I$addpmlist__FPc();
    std::printf("_GLOBAL_$I$addpmlist contract passed\n");
    return 0;
}

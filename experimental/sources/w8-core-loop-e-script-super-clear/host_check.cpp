// Host check for scriptSuperClear__Fv. Not an EE plan input.
extern "C" {
int nmDeferedCalls = 3;
int nextNmDeferedCalls = 4;
int scriptFrame = 5;
unsigned char norrath_script_post_action_view[200];
}
#include "script_super_clear.cpp"
int main() {
    for (int i = 0; i < 200; ++i) norrath_script_post_action_view[i] = 0x41;
    scriptSuperClear__Fv();
    if (nmDeferedCalls != 0) return 1;
    if (nextNmDeferedCalls != 0) return 2;
    if (scriptFrame != 0) return 3;
    if (norrath_script_post_action_view[0] != 0) return 4;
    for (int i = 1; i < 200; ++i)
        if (norrath_script_post_action_view[i] != 0x41) return 5;
    return 0;
}

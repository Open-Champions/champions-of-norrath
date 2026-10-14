// Host check for actionCoolOff. Not an EE plan input.
#include "action_cool_off.cpp"

int main() {
    if (sizeof(actionCoolOff) != 4 || actionCoolOff != 0) return 1;
    return 0;
}

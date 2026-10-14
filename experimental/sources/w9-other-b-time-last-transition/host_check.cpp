// Host check for time_last_transition.cpp; not an EE plan input.
#include "time_last_transition.cpp"

int main() {
    if (timeLastTransition != -9999999) return 1;
    timeLastTransition = 40;
    if (timeLastTransition != 40) return 2;
    return 0;
}

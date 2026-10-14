// Host check for recall_enter.cpp; not an EE plan input.
#include "recall_enter.cpp"

int main() {
    if (recallEnter != 0) return 1;
    recallEnter = -1;
    if (recallEnter != -1) return 2;
    return 0;
}

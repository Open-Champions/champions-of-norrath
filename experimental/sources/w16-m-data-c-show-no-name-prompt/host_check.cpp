// Host check for showNoNamePrompt. Not an EE plan input; it includes the definition file.
#include "showNoNamePrompt.cpp"
int main() {
    if (sizeof showNoNamePrompt != 1u) return 1;
    if (showNoNamePrompt != 0) return 2;
    return 0;
}

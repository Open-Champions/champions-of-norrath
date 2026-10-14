// Host check for ambientOnly. Not an EE plan input.
#include "ambient_only.cpp"

int main() {
    if (sizeof(ambientOnly) != 4 || ambientOnly != 0) return 1;
    return 0;
}

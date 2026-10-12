// Host check for t.cpp; not an EE plan input.
#include "t.cpp"

int main() {
    if (t != 0.0f) return 1;
    t = 0.25f;
    if (t != 0.25f) return 2;
    return 0;
}

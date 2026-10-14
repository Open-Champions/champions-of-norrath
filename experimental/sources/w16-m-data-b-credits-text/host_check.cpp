// Host check for creditsText. Not an EE plan input; it includes the definition file.
#include "creditsText.cpp"
int main() {
    if (sizeof creditsText != 2408u) return 1;
    for (unsigned char byte : creditsText) if (byte != 0) return 2;
    return 0;
}

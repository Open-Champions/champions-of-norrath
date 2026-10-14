// Host check for blueSpark. Not an EE plan input; it includes the definition file.
#include "blueSpark.cpp"
int main() {
    if (sizeof blueSpark != 80u) return 1;
    for (unsigned char byte : blueSpark) if (byte != 0) return 2;
    return 0;
}

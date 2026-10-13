// Host check for num_environments.cpp; not an EE plan input.
#include "num_environments.cpp"

int main() {
    if (numEnvironments != 1) return 1;
    numEnvironments = -2;
    if (numEnvironments != -2) return 2;
    return 0;
}

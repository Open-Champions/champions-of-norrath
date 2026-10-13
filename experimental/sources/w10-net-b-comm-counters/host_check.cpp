// Host check for the comm counter storage. Not an EE plan input.
#include "comm_counters.cpp"
int main() {
    if (timeout || readcount || writecount) return 1;
    if (sizeof timeout != 4 || sizeof readcount != 4 || sizeof writecount != 4) return 2;
    return 0;
}

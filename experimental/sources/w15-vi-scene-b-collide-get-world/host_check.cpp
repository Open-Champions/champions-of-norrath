// Host check only; not a build input.
#include "get_world.cpp"
int main() {
    unsigned char record[64];
    return GetWorld__C9VICollide(record) == record + 16 ? 0 : 1;
}

// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp s_pad.cpp
// The recorded extent is 4 bytes (0x011b1e30 to 0x011b1e34); a pointer is 4 bytes on the EE target.
extern "C" void* sPad;
static_assert(sizeof(void*) != 4 || sizeof(sPad) == 4, "word-sized on the target");
int main() {
    if (sPad != nullptr) return 1;
    int object = 0;
    sPad = &object;                       // the window setup stores a pointer here
    if (sPad != &object) return 2;
    return 0;
}

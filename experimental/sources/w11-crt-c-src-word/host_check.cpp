// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp   (the source is included)
#include "src_word.cpp"
static_assert(sizeof(src) == 0x486d04 - 0x486d00, "span up to dst");
int main() { return src == 0 ? 0 : 1; }

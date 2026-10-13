#include <cstddef>
extern "C" unsigned char nameTags[128];
int main() {
    if (sizeof(nameTags) != 128) return 1;
    for (std::size_t i = 0; i < sizeof(nameTags); ++i) if (nameTags[i] != 0) return 2;
    return 0;
}

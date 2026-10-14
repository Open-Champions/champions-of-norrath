// Host contract for norrath_list_head_copy_construct. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void norrath_list_head_copy_construct(void* destination, const void* source);

int main() {
    unsigned char destination[12];
    unsigned char source[12];
    std::memset(destination, 0xa5, sizeof destination);
    std::memset(source, 0x77, sizeof source);
    norrath_list_head_copy_construct(destination, source);
    int failures = 0;
    for (int i = 0; i < 4; ++i) if (destination[i] != 0) ++failures;
    for (int i = 4; i < 12; ++i) if (destination[i] != 0xa5) ++failures;
    for (int i = 0; i < 12; ++i) if (source[i] != 0x77) ++failures;
    if (failures != 0) {
        std::fprintf(stderr, "list head copy contract failed\n");
        return 1;
    }
    std::printf("list-head-copy contract passed\n");
    return 0;
}

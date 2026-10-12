// Host contract for lineToY__Fi. Not an EE plan input.
#include <cstdio>
extern "C" int lineToY__Fi(int);
int main() {
    int failures = 0;
    const struct { int in, out; } cases[] = {{0, 295}, {1, 313}, {2, 331}, {-1, 277}, {10, 475}, {-17, -11},
                                             {0x7fffffff, static_cast<int>(0x7fffffffu * 18u + 295u)}};
    for (const auto& c : cases) {
        if (lineToY__Fi(c.in) != c.out) { std::fprintf(stderr, "fail: lineToY(%d) = %d\n", c.in, lineToY__Fi(c.in)); ++failures; }
    }
    if (failures) return 1;
    std::printf("lineToY contract passed\n");
    return 0;
}

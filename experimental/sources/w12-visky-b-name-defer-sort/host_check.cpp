#include "sort.cpp"

int main() {
    const float values[] = {-3.5f, -0.0f, 0.0f, 1.0f, 2.5f, 1e30f, -1e30f};
    for (float x : values)
        for (float y : values) {
            float pair[2] = {x, y};
            int expect = x < y ? -1 : (x == y ? 0 : 1);
            if (VINameDeferSort__FPCvT0(&pair[0], &pair[1]) != expect) return 1;
        }
    return 0;
}

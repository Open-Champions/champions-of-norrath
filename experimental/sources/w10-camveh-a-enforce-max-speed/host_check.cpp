// Host contract for enforceMaxSpeed__6CameraP6Point3. Not an EE plan input.
#include "enforce_max_speed.cpp"

#include <cstdio>

int main() {
    int failures = 0;
    struct Case { float in[3]; float want[3]; } cases[] = {
        {{0, 0, 0}, {0, 0, 0}},
        {{3.5f, -3.5f, 1.5f}, {3, -3, 1}},
        {{-9, 9, -1.5f}, {-3, 3, -1}},
        {{3, -3, 1}, {3, -3, 1}},
        {{2.9f, -2.9f, 0.99f}, {2.9f, -2.9f, 0.99f}},
        {{1.5f, 2.5f, 1.01f}, {1.5f, 2.5f, 1}},
        {{1.5f, -2.5f, -1.01f}, {1.5f, -2.5f, -1}},
        {{3.01f, 1.5f, 0.5f}, {3, 1.5f, 0.5f}},
    };
    for (const Case& c : cases) {
        float p[3] = {c.in[0], c.in[1], c.in[2]};
        enforceMaxSpeed__6CameraP6Point3(nullptr, p);
        for (int i = 0; i < 3; ++i)
            if (p[i] != c.want[i]) { std::fprintf(stderr, "fail: %g %g %g component %d\n", c.in[0], c.in[1], c.in[2], i); ++failures; }
    }
    float n[3] = {__builtin_nanf(""), 1, 1};
    enforceMaxSpeed__6CameraP6Point3(nullptr, n);
    if (n[0] == n[0]) { std::fprintf(stderr, "fail: NaN replaced\n"); ++failures; }
    return failures ? 1 : 0;
}

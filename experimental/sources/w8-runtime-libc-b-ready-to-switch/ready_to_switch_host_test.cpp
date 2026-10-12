// Host check only; not an EE build input.
#include <cstdio>
extern "C" int readyToSwitch__3Cat(const void* self);

int main() {
    signed char object[300] = {};
    const int cases[][2] = {{0, 0}, {1, 1}, {2, 0}, {-1, 0}, {127, 0}, {-128, 0}, {0x11, 0}};
    for (const auto& c : cases) {
        object[292] = static_cast<signed char>(c[0]);
        if (readyToSwitch__3Cat(object) != c[1]) return 1;
    }
    object[292] = 0;
    object[291] = 1; object[293] = 1;  // neighbours do not matter
    if (readyToSwitch__3Cat(object) != 0) return 2;
    std::puts("ready_to_switch ok");
    return 0;
}

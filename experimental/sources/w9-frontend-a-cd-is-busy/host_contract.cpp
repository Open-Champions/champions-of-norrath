#include "cd_is_busy.cpp"

#include <cstdio>

signed char cdBusy;
signed char streaming;

int main() {
    const int cases[][3] = {{0, 0, 0}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}, {-1, 0, 1}, {0, -128, 1}};
    for (const auto& c : cases) {
        cdBusy = static_cast<signed char>(c[0]);
        streaming = static_cast<signed char>(c[1]);
        if (cdIsBusy__Fv() != c[2]) return 1;
    }
    std::puts("cd is busy passed");
    return 0;
}

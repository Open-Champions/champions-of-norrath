// Host check only; not a build input.
#include "filter_actor.cpp"
#include <cstdio>
int main() {
    int a = 0, b = 0;
    bool ok = FilterActor__13WeatherFilterR7VIScenei(&a, &b, 5) == 0 && FilterActor__13WeatherFilterR7VIScenei(&a, &b, -1) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

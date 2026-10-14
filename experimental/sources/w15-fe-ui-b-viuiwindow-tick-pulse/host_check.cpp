#include "tick_pulse.cpp"
#include <cmath>

static long long clock_value;
extern "C" long long GetTime__7VITimer() { return clock_value; }
extern "C" float sinf(float x) { return static_cast<float>(std::sin(static_cast<double>(x))); }

static float expected(float p, long long start, long long now) {
    double ease = 1.0 - p;
    double shaped = 1.0 - ease * ease;
    double t = static_cast<double>(now - start) * 0.01;
    if (t > 6.2831855) t -= 6.2831855;
    return static_cast<float>(shaped * (std::sin(t + 1.5707964) * 0.5 + 0.5));
}

int main() {
    alignas(16) unsigned char window[400] = {};
    struct Case { float p; long long start, now; } cases[] = {
        {0.0f, 0, 0}, {0.5f, 0, 100}, {1.0f, 0, 100}, {0.25f, 10, 700}, {0.75f, 0, 1500}, {1.0f, 5, 1000000},
    };
    for (const Case& c : cases) {
        *reinterpret_cast<float*>(window + 40) = c.p;
        *reinterpret_cast<long long*>(window + 352) = c.start;
        clock_value = c.now;
        const float got = TickPulse__C10VIUIWindow(window);
        if (std::fabs(got - expected(c.p, c.start, c.now)) > 1e-4f) return 1;
    }
    // At full progress (p = 1) the weight a is 1; at p = 0 it is 0, so the pulse vanishes.
    *reinterpret_cast<float*>(window + 40) = 0.0f;
    *reinterpret_cast<long long*>(window + 352) = 0;
    clock_value = 100;
    if (TickPulse__C10VIUIWindow(window) != 0.0f) return 2;
    return 0;
}

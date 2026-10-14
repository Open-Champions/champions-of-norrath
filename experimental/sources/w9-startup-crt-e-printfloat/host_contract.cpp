#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include "printfloat.cpp"

static std::string output;
static int compare_calls, multiply_calls, divide_calls;
static double as_double(unsigned long long bits) { double d; std::memcpy(&d, &bits, 8); return d; }
static unsigned long long bits_of(double d) { unsigned long long b; std::memcpy(&b, &d, 8); return b; }
static int put(int c) { output += static_cast<char>(c); return c; }
extern "C" int (*_putchar)(int) = put;
extern "C" int norrath_softdouble_compare_2fba48(unsigned long long l, unsigned long long r) {
    ++compare_calls; const double a = as_double(l), b = as_double(r); return a < b ? -1 : (a > b ? 1 : 0);
}
extern "C" unsigned long long norrath_softdouble_multiply_2fb680(unsigned long long l, unsigned long long r) { ++multiply_calls; return bits_of(as_double(l) * as_double(r)); }
extern "C" unsigned long long norrath_factory_double_subtract(unsigned long long l, unsigned long long r) { return bits_of(as_double(l) - as_double(r)); }
extern "C" unsigned long long norrath_player_double_divide(unsigned long long l, unsigned long long r) { ++divide_calls; return bits_of(as_double(l) / as_double(r)); }
extern "C" unsigned long long __fixunsdfdi(unsigned long long v) { return bits_of(std::floor(as_double(v))); }
extern "C" int ftoi(unsigned long long v) { return static_cast<int>(std::floor(as_double(v) + 0.5)); }
extern "C" void kprintf(const char* format, ...) {
    char buffer[64]; va_list ap; va_start(ap, format); std::vsnprintf(buffer, sizeof buffer, format, ap); va_end(ap); output += buffer;
}
static std::string run(double v) { output.clear(); compare_calls = multiply_calls = divide_calls = 0; printfloat(bits_of(v)); return output; }
int main() {
    if (run(125.0) != "0.125000e+3") return 1;                    // divided down three times
    if (divide_calls != 3) return 2;
    if (run(0.5) != "0.500000e+0" || divide_calls != 0 || multiply_calls != 1) return 3;
    if (run(0.05) != "0.500000e-1" || multiply_calls != 2) return 4;  // one scale-up plus the final x1e6
    if (run(-250.0) != "-0.250000e+3") return 5;
    if (run(-0.0625) != "-0.625000e-1") return 6;
    if (run(1.0) != "0.100000e+1") return 7;                      // 1.0 is divided once, not left alone
    if (run(0.1) != "0.100000e+0") return 8;                      // 0.1 is not below 0.1
    if (run(0.015625) != "0.156250e-1") return 9;
    if (run(1000000.0) != "0.100000e+7") return 10;
    // compare calls on entry: sign, 0.1, 1.0 for a mid value plus the loop tests
    run(0.5);
    if (compare_calls != 3) return 11;
    run(0.05);
    if (compare_calls != 1 + 1 + 2 + 0) return 12;                // sign, entry 0.1, two loop tests
    std::puts("printfloat contract passed");
    return 0;
}

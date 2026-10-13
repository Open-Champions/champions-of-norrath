// Host check only; not an EE build input. Build with -ffp-contract=off:
//   c++ -std=c++17 -ffp-contract=off -o check host_check.cpp && ./check
// The soft-double helpers are replaced by host IEEE double operations and fabs/litodp/dptoli/dpcmp by their plain
// meanings (dpcmp returns 0 for equal, as the routine's loop needs). __kernel_rem_pio2 is a recording stub.
// Checks: the quadrant count n and the two-part remainder y[0] + y[1] against exact values (computed once in
// rational arithmetic and listed below), over small, medium and negative arguments; the exact special results (zero, infinity, NaN); and for the large path
// that the stub receives e0, nx and pieces which reassemble to x * 2^-e0 and that the sign is applied afterwards.
#include <cmath>
#include <initializer_list>
#include <cstdio>
#include <cstring>
typedef unsigned long long Dp;
static double d(Dp b) { double v; std::memcpy(&v, &b, 8); return v; }
static Dp b(double v) { Dp r; std::memcpy(&r, &v, 8); return r; }
extern "C" Dp dpmul(Dp l, Dp r) { return b(d(l) * d(r)); }
extern "C" Dp dpadd(Dp l, Dp r) { return b(d(l) + d(r)); }
extern "C" Dp dpsub(Dp l, Dp r) { return b(d(l) - d(r)); }
extern "C" int dpcmp(Dp l, Dp r) { return d(l) == d(r) ? 0 : (d(l) < d(r) ? -1 : 1); }
extern "C" int dptoli(Dp v) { return static_cast<int>(d(v)); }
extern "C" Dp litodp(int v) { return b(static_cast<double>(v)); }
#define fabs norrath_fabs  // the routine imports fabs on 64-bit values; keep the host libm declaration apart
extern "C" Dp norrath_fabs(Dp v) { return b(__builtin_fabs(d(v))); }
static int stubE0, stubNx, stubPrec;
static double stubPieces[3];
extern "C" int __kernel_rem_pio2(Dp* x, Dp* y, int e0, int nx, int prec, const int* table) {
    stubE0 = e0; stubNx = nx; stubPrec = prec;
    for (int i = 0; i < 3; ++i) stubPieces[i] = d(x[i]);
    if (table[0] != 0xa2f983 || table[65] != 0xc08c6b) std::puts("table words wrong");
    y[0] = b(1.0); y[1] = b(2.0);
    return 6;
}
#include "ieee754_rem_pio2.cpp"
static int bad;
static void expect(bool ok, const char* what, double x) { if (!ok) { std::printf("%s for x=%.17g\n", what, x); ++bad; } }
int main() {
    // x, quadrant count and the exact remainder (high and low double) from a 75-digit pi in rational arithmetic
    struct Row { double x; int n; double hi, lo; };
    static const Row rows[] = {
    {0.8, 1, -0.7707963267948966, 4.9789962505147994e-17},
    {-0.8, -1, 0.7707963267948966, -4.9789962505147994e-17},
    {1.0, 1, -0.5707963267948967, 4.9789962505147994e-17},
    {-1.0, -1, 0.5707963267948967, -4.9789962505147994e-17},
    {1.5707963267948966, 1, -6.123233995736766e-17, 1.4973849048591698e-33},
    {-1.5707963267948966, -1, 6.123233995736766e-17, -1.4973849048591698e-33},
    {2.0, 1, 0.4292036732051034, -5.721188726109832e-18},
    {-2.0, -1, -0.4292036732051034, 5.721188726109832e-18},
    {2.4, 2, -0.7415926535897933, -1.1442377452219664e-17},
    {-2.4, -2, 0.7415926535897933, 1.1442377452219664e-17},
    {3.0, 2, -0.14159265358979323, -1.1442377452219664e-17},
    {-3.0, -2, 0.14159265358979323, 1.1442377452219664e-17},
    {3.14159265358979, 2, -3.2310891488651735e-15, -1.69568553207378e-31},
    {-3.14159265358979, -2, 3.2310891488651735e-15, 1.69568553207378e-31},
    {4.0, 3, -0.7123889803846899, 3.8347585052928333e-17},
    {-4.0, -3, 0.7123889803846899, -3.8347585052928333e-17},
    {10.0, 6, 0.5752220392306203, -3.4327132356658994e-17},
    {-10.0, -6, -0.5752220392306203, 3.4327132356658994e-17},
    {100.0, 64, -0.5309649148733836, -3.308917108348227e-17},
    {-100.0, -64, 0.5309649148733836, 3.308917108348227e-17},
    {12345.678, 7860, -0.7811286078875436, -4.510889904438376e-18},
    {-12345.678, -7860, 0.7811286078875436, 4.510889904438376e-18},
    {100000.0, 63662, -0.035756416708573505, 2.243344863029369e-19},
    {-100000.0, -63662, 0.035756416708573505, -2.243344863029369e-19},
    {1000000.0, 636620, -0.357564167085735, -2.5512230752599545e-17},
    {-1000000.0, -636620, 0.357564167085735, 2.5512230752599545e-17},
    {1600000.0, 1018592, -0.5721026673371761, 3.58935178084699e-18},
    {-1600000.0, -1018592, 0.5721026673371761, -3.58935178084699e-18},
    {5.0, 3, 0.28761101961531016, -1.7163566178329497e-17},
    {-5.0, -3, -0.28761101961531016, 1.7163566178329497e-17},
    {7.0, 4, 0.7168146928204135, -2.2884754904439327e-17},
    {-7.0, -4, -0.7168146928204135, 2.2884754904439327e-17},
    {48.69468613064179, 31, -3.674559378078648e-15, -2.6172985905132346e-31},
    {-48.69468613064179, -31, 3.674559378078648e-15, 2.6172985905132346e-31},
    {50.26548245743669, 32, -1.959434878635765e-15, 4.7916316955493435e-32},
    {-50.26548245743669, -32, 1.959434878635765e-15, -4.7916316955493435e-32},
    {51.83627878423159, 33, -2.443103791928823e-16, 1.2435846928117674e-32},
    {-51.83627878423159, -33, 2.443103791928823e-16, -1.2435846928117674e-32},
    {157079.63267948964, 100000, -1.6980326998151097e-11, 3.404792567419288e-30},
    {-157079.63267948964, -100000, 1.6980326998151097e-11, -3.404792567419288e-30},
    };
    for (const Row& row : rows) {
        Dp y[2] = {0, 0};
        const int n = __ieee754_rem_pio2(b(row.x), y);
        expect(n == row.n, "quadrant count", row.x);
        const double error = (d(y[0]) - row.hi) + (d(y[1]) - row.lo);
        expect(__builtin_fabs(error) <= 1e-16 * (__builtin_fabs(row.hi) + 1e-20), "remainder", row.x);
    }
    {
        Dp y[2] = {7, 7};
        expect(__ieee754_rem_pio2(b(0.25), y) == 0 && d(y[0]) == 0.25 && y[1] == 0, "small argument passes through", 0.25);
    }
    {
        Dp y[2] = {7, 7};
        const double inf = HUGE_VAL;
        const int n = __ieee754_rem_pio2(b(inf), y);
        expect(n == 0 && std::isnan(d(y[0])) && std::isnan(d(y[1])), "infinity gives NaN", inf);
        expect(__ieee754_rem_pio2(b(std::nan("")), y) == 0 && std::isnan(d(y[0])), "NaN gives NaN", 0.0);
    }
    for (double x : {3.0e7, 1.0e20, -1.0e20, 1.0e100, -2.5e250}) {
        Dp y[2] = {0, 0};
        const int n = __ieee754_rem_pio2(b(x), y);
        expect(stubPrec == 2 && stubNx >= 1 && stubNx <= 3, "stub arguments", x);
        long double sum = 0;
        long double scale = 1.0L;
        for (int i = 0; i < stubNx; ++i) { sum += stubPieces[i] * scale; scale /= 16777216.0L; }
        const long double want = std::ldexp(__builtin_fabs(static_cast<long double>(x)), -stubE0);
        expect(__builtin_fabs(static_cast<double>((sum - want) / want)) < 1e-15, "pieces reassemble to |x| * 2^-e0", x);
        expect(stubPieces[0] >= 0 && stubPieces[0] < 16777216.0, "first piece is a 24-bit integer", x);
        if (x > 0) expect(n == 6 && d(y[0]) == 1.0 && d(y[1]) == 2.0, "positive result unchanged", x);
        else expect(n == -6 && d(y[0]) == -1.0 && d(y[1]) == -2.0, "negative result negated", x);
    }
    return bad;
}

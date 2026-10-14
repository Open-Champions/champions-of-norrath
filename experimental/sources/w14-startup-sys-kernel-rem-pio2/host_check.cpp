// Host check for __kernel_rem_pio2. Not a build-plan input.
//   c++ -std=c++17 -ffp-contract=off -o check host_check.cpp && ./check
// dpadd, dpsub, dpmul, dpcmp, dptoli, litodp, scalbn and floor are replaced by host IEEE double operations on
// 64-bit patterns (scalbn and floor are renamed with macros so they do not clash with the C library). The
// vectors are input and output bit patterns produced by the original routine in a private interpreter run
// whose soft-double helpers were also exact IEEE operations (numbers only; the image stays private), so the
// results must match bit for bit. The inputs are arbitrary scaled pieces and exponents, not recorded game
// arguments. A second group checks the contract independent of the vectors: the remainder of a moderate
// argument reduced here must agree with a reference computed from the three-part pi/2, and the quotient
// bits must be right.
#include <cmath>
#include <cstdio>
#include <cstring>
typedef unsigned long long Dp;
static double d(Dp b) { double v; std::memcpy(&v, &b, 8); return v; }
static Dp b(double v) { Dp r; std::memcpy(&r, &v, 8); return r; }
extern "C" {
Dp dpmul(Dp l, Dp r) { return b(d(l) * d(r)); }
Dp dpadd(Dp l, Dp r) { return b(d(l) + d(r)); }
Dp dpsub(Dp l, Dp r) { return b(d(l) - d(r)); }
int dpcmp(Dp l, Dp r) { double x = d(l), y = d(r); return (x > y) - (x < y); }
int dptoli(Dp v) { return static_cast<int>(d(v)); }
Dp litodp(int v) { return b(static_cast<double>(v)); }
Dp kre_scalbn(Dp v, int e) { return b(std::ldexp(d(v), e)); }
Dp kre_floor(Dp v) { return b(std::floor(d(v))); }
}
#define scalbn kre_scalbn
#define floor kre_floor
#include "kernel_rem_pio2.cpp"
#undef scalbn
#undef floor

struct Vector { int e0, nx, prec; Dp x[3]; int ret; Dp y[3]; };
static const Vector vectors[] = {
    {29, 1, 2, {0x41600de220000000ull, 0x0000000000000000ull, 0x0000000000000000ull}, 2, {0xbfe0f7a0b0373346ull, 0xbc84b5102cea6cd5ull, 0x0000000000000000ull}},
    {52, 3, 2, {0x41619ce560000000ull, 0x413e963b00000000ull, 0x412a296c00000000ull}, 2, {0x3fb33c1f2eb6191dull, 0x3c446d2b5111a160ull, 0x0000000000000000ull}},
    {3, 3, 0, {0x4166f59f20000000ull, 0x41584311c0000000ull, 0x414cc6e100000000ull}, 3, {0xbfd0dee335ddc803ull, 0x0000000000000000ull, 0x0000000000000000ull}},
    {26, 3, 2, {0x415825f440000000ull, 0x41626a2940000000ull, 0x40c7a262e0000000ull}, 5, {0x3fc67db760faca5cull, 0x3c627e0cfb42998dull, 0x0000000000000000ull}},
    {27, 3, 0, {0x4162ada5e0000000ull, 0x4140e72480000000ull, 0x414f49a7c0000000ull}, 4, {0x3fe56dfc0154a3e9ull, 0x0000000000000000ull, 0x0000000000000000ull}},
    {24, 3, 0, {0x415f0f14c0000000ull, 0x4155b2aac0000000ull, 0x41516e7e00000000ull}, 0, {0x3fcef26afdf2b2a2ull, 0x0000000000000000ull, 0x0000000000000000ull}},
    {4, 3, 2, {0x416e002c20000000ull, 0x4145628700000000ull, 0x41370eb480000000ull}, 1, {0xbfd4c3a309964c39ull, 0x3c3b14be8bfa72ceull, 0x0000000000000000ull}},
    {131, 3, 3, {0x4157536240000000ull, 0x41589d8a80000000ull, 0x409f6a8c00000000ull}, 6, {0xbfe5d5d287afd31dull, 0xbc8d2830620d2a21ull, 0xb92b6c10f3ef8a96ull}},
    {52, 3, 0, {0x414eafbc80000000ull, 0x416842c1a0000000ull, 0x415c0feee0000000ull}, 6, {0xbfd915b6deccf368ull, 0x0000000000000000ull, 0x0000000000000000ull}},
    {52, 3, 2, {0x41611ddae0000000ull, 0x4156df3000000000ull, 0x40a9550200000000ull}, 6, {0xbfe6bc0b16313fcbull, 0x3c8eb70ff545e01eull, 0x0000000000000000ull}},
    {29, 2, 3, {0x4164b375e0000000ull, 0x416d959d60000000ull, 0x0000000000000000ull}, 3, {0xbfe0504195cc7a8eull, 0x3c6583b6e521db81ull, 0xb90c745b4c9c2ea2ull}},
    {5, 3, 0, {0x41332a4d00000000ull, 0x413b464d00000000ull, 0x415f51b300000000ull}, 1, {0xbfc50eba7c1087c5ull, 0x0000000000000000ull, 0x0000000000000000ull}},
    {27, 3, 2, {0x41632011a0000000ull, 0x416f79bc80000000ull, 0x40c919bdc0000000ull}, 2, {0x3fc601f7f951fb2full, 0xbc3ae9abe0863cf2ull, 0x0000000000000000ull}},
    {50, 3, 2, {0x415ea18e40000000ull, 0x416124bd40000000ull, 0x41471b7500000000ull}, 3, {0xbfe4e322c2659b46ull, 0xbc8909f5fff988f1ull, 0x0000000000000000ull}},
    {52, 3, 1, {0x416ae18fa0000000ull, 0x4160c230e0000000ull, 0x40ceea6a40000000ull}, 0, {0x3fe128b811b50483ull, 0xbc6e6dd83df570c8ull, 0x0000000000000000ull}},
    {28, 1, 2, {0x410b291800000000ull, 0x0000000000000000ull, 0x0000000000000000ull}, 4, {0x3fc6bc1d7c6a618aull, 0x3c5f436593608b74ull, 0x0000000000000000ull}},
    {48, 3, 2, {0x4155731bc0000000ull, 0x416edc6720000000ull, 0x40ce3d69e0000000ull}, 4, {0x3fe4c8c67173239eull, 0x3c8a3be1d6e5a396ull, 0x0000000000000000ull}},
    {24, 1, 2, {0x415b441fc0000000ull, 0x0000000000000000ull, 0x0000000000000000ull}, 4, {0xbfe4025f2f78480dull, 0x3c69c0c210e3aefcull, 0x0000000000000000ull}},
    {26, 3, 0, {0x41258d7600000000ull, 0x416c7c7ea0000000ull, 0x41660ad0c0000000ull}, 0, {0xbfc8f1bbd3f74d4dull, 0x0000000000000000ull, 0x0000000000000000ull}},
    {29, 3, 2, {0x4163ad5360000000ull, 0x41654c1360000000ull, 0x40c8b17cc0000000ull}, 7, {0x3fad184e458c0579ull, 0x3c16250cd1dc845cull, 0x0000000000000000ull}},
    {50, 3, 0, {0x416dbb47e0000000ull, 0x41625dd740000000ull, 0x40ca898920000000ull}, 4, {0xbfda9cd2af1d4ec5ull, 0x0000000000000000ull, 0x0000000000000000ull}},
    {27, 1, 2, {0x416fe7c9a0000000ull, 0x0000000000000000ull, 0x0000000000000000ull}, 0, {0xbfe43ef5fcb2f38dull, 0xbc874da2e3d0ea0eull, 0x0000000000000000ull}},
    {3, 3, 1, {0x4116cd5400000000ull, 0x4166b75aa0000000ull, 0x415188be60000000ull}, 5, {0xbfd7458e52e8f735ull, 0xbc525b5d92b819c1ull, 0x0000000000000000ull}},
    {27, 3, 2, {0x416c37bda0000000ull, 0x415a4b8b40000000ull, 0x40c6985220000000ull}, 1, {0x3facfeefcfcdda49ull, 0x3c2c2f61ccb49809ull, 0x0000000000000000ull}},
};

static const int two_over_pi[66] = {
    0xa2f983, 0x6e4e44, 0x1529fc, 0x2757d1, 0xf534dd, 0xc0db62, 0x95993c, 0x439041, 0xfe5163, 0xabdebb, 0xc561b7,
    0x246e3a, 0x424dd2, 0xe00649, 0x2eea09, 0xd1921c, 0xfe1deb, 0x1cb129, 0xa73ee8, 0x8235f5, 0x2ebb44, 0x84e99c,
    0x7026b4, 0x5f7e41, 0x3991d6, 0x398353, 0x39f49c, 0x845f8b, 0xbdf928, 0x3b1ff8, 0x97ffde, 0x05980f, 0xef2f11,
    0x8b5a0a, 0x6d1f6d, 0x367ecf, 0x27cb09, 0xb74f46, 0x3f669e, 0x5fea2d, 0x7527ba, 0xc7ebe5, 0xf17b3d, 0x0739f7,
    0x8a5292, 0xea6bfb, 0x5fb11f, 0x8d5d08, 0x560330, 0x46fc7b, 0x6babf0, 0xcfbc20, 0x9af436, 0x1da9e3, 0x91615e,
    0xe61b08, 0x659985, 0x5f14a0, 0x68408d, 0xffd880, 0x4d7327, 0x310606, 0x1556ca, 0x73a8c9, 0x60e27b, 0xc08c6b,
};

int main() {
    int bad = 0;
    int index = 0;
    for (const Vector& v : vectors) {
        Dp x[3] = {v.x[0], v.x[1], v.x[2]};
        Dp y[3] = {0x5a5a5a5a5a5a5a5aull, 0x5a5a5a5a5a5a5a5aull, 0x5a5a5a5a5a5a5a5aull};
        const int r = __kernel_rem_pio2(x, y, v.e0, v.nx, v.prec, two_over_pi);
        const int words = v.prec == 0 ? 1 : (v.prec == 3 ? 3 : 2);
        bool ok = r == v.ret;
        for (int i = 0; i < words; ++i) ok = ok && y[i] == v.y[i];
        for (int i = words; i < 3; ++i) ok = ok && y[i] == 0x5a5a5a5a5a5a5a5aull;
        if (!ok) { std::printf("vector %d: ret %d (want %d) y0 %016llx (want %016llx)\n", index, r, v.ret, y[0], v.y[0]); ++bad; }
        ++index;
    }
    // Independent check on moderate arguments: x = N*pi/2 + r, |r| <= pi/4, N from the three-part pi/2.
    const double pio2_1 = 1.57079632673412561417e+00, pio2_1t = 6.07710050650619224932e-11;
    const double invpio2 = 6.36619772367581382433e-01;
    for (int k = 0; k < 40; ++k) {
        double x = 1.0e3 * (k + 1) + 12.345678 * k;
        int e0 = static_cast<int>((reinterpret_cast<const unsigned long long&>(x) >> 52) & 0x7ff) - 1046;
        double z = std::ldexp(x, -e0);
        Dp tx[3];
        for (int i = 0; i < 2; ++i) { double piece = static_cast<double>(static_cast<int>(z)); tx[i] = b(piece); z = (z - piece) * 16777216.0; }
        tx[2] = b(z);
        int nx = 3;
        while (nx > 1 && d(tx[nx - 1]) == 0.0) --nx;
        Dp y[2];
        const int n = __kernel_rem_pio2(tx, y, e0, nx, 2, two_over_pi);
        const long long N = static_cast<long long>(x * invpio2 + 0.5);
        const double fn = static_cast<double>(N);
        const double ref = (x - fn * pio2_1) - fn * pio2_1t;
        const double got = d(y[0]) + d(y[1]);
        if (n != static_cast<int>(N & 7) || std::fabs(got - ref) > 1e-9) { std::printf("moderate %d: n %d want %lld, got %g ref %g\n", k, n, N & 7, got, ref); ++bad; }
        if (std::fabs(got) > 0.7854) { std::printf("moderate %d: remainder out of range %g\n", k, got); ++bad; }
    }
    return bad;
}

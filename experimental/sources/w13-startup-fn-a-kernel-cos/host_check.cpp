// Host check only; not an EE build input. Build with -ffp-contract=off:
//   c++ -std=c++17 -ffp-contract=off -o check host_check.cpp && ./check
// dpmul, dpadd, dpsub and dptoli are replaced by host IEEE double operations. The vectors are input and output
// bit patterns produced by the original routine (with its own soft-double helpers) in a private interpreter run
// (numbers only; the image stays private). The image's soft-double helpers round differently from host IEEE
// doubles, so the vectors are compared within 16 units in the last place; the exact comparison is the private
// paired run with the image's own helpers. NaN results are compared as "both NaN".
#include <cmath>
#include <cstdio>
#include <cstring>
typedef unsigned long long Dp;
static double d(Dp b) { double v; std::memcpy(&v, &b, 8); return v; }
static Dp b(double v) { Dp r; std::memcpy(&r, &v, 8); return r; }
extern "C" Dp dpmul(Dp l, Dp r) { return b(d(l) * d(r)); }
extern "C" Dp dpadd(Dp l, Dp r) { return b(d(l) + d(r)); }
extern "C" Dp dpsub(Dp l, Dp r) { return b(d(l) - d(r)); }
extern "C" int dptoli(Dp v) { return static_cast<int>(d(v)); }
#include "kernel_cos.cpp"
struct Vector { Dp x, y; int unused; Dp result; };
static const Vector vectors[] = {
    {0x3e72a717c9a42bb6ull, 0xbc8c3d161e1756b5ull, 0, 0x3fefffffffffffeaull},
    {0xbddb7cdfd9d7bdbbull, 0xbfdac26f6d9cfba0ull, 0, 0x3ff0000000000000ull},
    {0xbfdaad3ef0ec61a4ull, 0x3c9804a37652d05eull, 0, 0x3fed4298788c4d33ull},
    {0x3e723489135b7fcaull, 0xbc9c1eaf77a543f9ull, 0, 0x3fefffffffffffebull},
    {0xbe7094db32fd46a6ull, 0x3c83faf397c08498ull, 0, 0x3fefffffffffffeeull},
    {0xbfdf1283241abc7dull, 0xbc91568ad251f859ull, 0, 0x3fec4d53f51b98aaull},
    {0xbfb5bf6fec7620c0ull, 0xbfcf916b5cac3740ull, 0, 0x3fef36d2cdc53fefull},
    {0x3fe49ab5212e1e76ull, 0x3c98ec8c31e93edaull, 0, 0x3fe997b62e4e0e85ull},
    {0x8000000000000000ull, 0xbc76e9b65b882858ull, 0, 0x3ff0000000000000ull},
    {0xbe25b621909ca260ull, 0x3fe57c595f1fe4aeull, 0, 0x3ff0000000000000ull},
    {0xbfda4b96698264eaull, 0xbc9aba9a5e01cf2bull, 0, 0x3fed563b36fb4433ull},
    {0x3f76ba8c5cc9e500ull, 0x3fba1bd146de0930ull, 0, 0x3feffb3ce17ec753ull},
    {0x3e787d26f7d0f088ull, 0x3c7cb36cb806d008ull, 0, 0x3fefffffffffffdaull},
    {0x3f63cab52a9a6700ull, 0xbc958dfc648afff3ull, 0, 0x3feffff9e1229709ull},
    {0xbfc65711ae5ab834ull, 0xbc9a46deb91133e6ull, 0, 0x3fef838bd5fb2edbull},
    {0x3fca140ba9251b3aull, 0x3fd815b9f4a3ba1cull, 0, 0x3fece279ae9b23b9ull},
    {0x7ff0000000000000ull, 0xbc99abf38aaa50c4ull, 0, 0x7ff8000000000000ull},
    {0x3e6329b85bd42428ull, 0xbfe770319d1a0a74ull, 0, 0x3ff000000704961bull},
    {0xbe60da49aab86d82ull, 0x3c43d6b95f429dc0ull, 0, 0x3feffffffffffffbull},
    {0xbe66deb847d6bcf9ull, 0x3c770a7ff790fe70ull, 0, 0x3feffffffffffff7ull},
    {0x3fa990babb1d0d50ull, 0x3c99053ce0d15ce2ull, 0, 0x3feff5ca31de72c5ull},
    {0xbfdaf849323aff4aull, 0xbc98d96d209f778dull, 0, 0x3fed3353a770c69full},
    {0xbfe44d744fd2b84full, 0xbfde489b7c77a67aull, 0, 0x3fe02a749c7260cdull},
    {0x3fe626da02edbbc0ull, 0x3c9a043203bea7c0ull, 0, 0x3fe8a256570ae571ull},
    {0xb9b4484bfeebc2a0ull, 0x3c98dab67be23b76ull, 0, 0x3ff0000000000000ull},
    {0xbfcce3410fc07b2cull, 0x3c70e66cbc101008ull, 0, 0x3fef30425017bc44ull},
    {0xbfc704c77eb31188ull, 0xbc77560816ff5860ull, 0, 0x3fef7be44ec86782ull},
    {0x3e40000000000000ull, 0xbc9123229e4ef6a3ull, 0, 0x3fefffffffffffffull},
    {0x3fe9341e5db46a28ull, 0xbc8de2bbdb58d24eull, 0, 0x3fe693c7a5557801ull},
    {0x3fbc82af6777d8c0ull, 0x3c8b4d7422fa7d24ull, 0, 0x3fefcd3fde82580cull},
    {0xbfd0a892a1e1e7ccull, 0xbc761d442a671b70ull, 0, 0x3feeec0eda3a822dull},
    {0xbfce3ddf93336534ull, 0x3c99dba356d2dafaull, 0, 0x3fef1c6bed65b674ull},
    {0x8000000000000000ull, 0x3e5e5d120ba297b8ull, 0, 0x3ff0000000000000ull},
    {0xbfc093a2d3dc3f28ull, 0x3c996d5deb509dd6ull, 0, 0x3fefbb6631174e1dull},
    {0xb9b4484bfeebc2a0ull, 0x3c971a2477770a34ull, 0, 0x3ff0000000000000ull},
    {0x0000000000000001ull, 0x3c947e21c0552568ull, 0, 0x3ff0000000000000ull},
    {0xbfcff94466909db4ull, 0xbc973eeb6812e051ull, 0, 0x3fef01bf2fbb19b2ull},
    {0xbfe6c6a37f1e0b14ull, 0x3c7e5ead2dbe5ec8ull, 0, 0x3fe83b2859a519d8ull},
    {0x3fc95692e08adc96ull, 0x3c96b13cc56a2ca0ull, 0, 0x3fef600482f0a2e8ull},
    {0xbf7291ff49ad6100ull, 0xbc8bf3b9791fa447ull, 0, 0x3fefffea724fd2a0ull},
    {0x3fe043a36fc0e62eull, 0xbc61200ee487b7d0ull, 0, 0x3febf47bec970ef9ull},
    {0x3fdac2934eda924cull, 0xbfe8c04aa1739b0aull, 0, 0x3ff3cbd1d70a27b2ull},
    {0x3fd7eefbd5c39fc8ull, 0xbc2540591a25a780ull, 0, 0x3fedc9d4a3b50237ull},
    {0x7e37e43c8800759cull, 0x3c9137cc06e9b112ull, 0, 0xfff0000000000000ull},
    {0x3fe692fd20fedb52ull, 0x3c88a3397bcf73a8ull, 0, 0x3fe85cc5076fec43ull},
    {0x3fac6b36478969d0ull, 0xbc76da393e8e0150ull, 0, 0x3feff36251356615ull},
    {0x3fc2c661ee870eb4ull, 0x3c8e7e7604fd2e9cull, 0, 0x3fefa8088adb3e6cull},
    {0xbfd144da22892594ull, 0x3fd2b6ed82265104ull, 0, 0x3ff0aef9998d68e7ull},
    {0x3dfc13e3968d7200ull, 0xbc46133bdd4a7ba0ull, 0, 0x3ff0000000000000ull},
    {0xbfe49eb226270937ull, 0xbc9804f0abbbad37ull, 0, 0x3fe995510e896b7full},
    {0xbe54c9cae7d10c20ull, 0xbc967c1f7768e040ull, 0, 0x3feffffffffffffeull},
    {0xbfae819e64402854ull, 0xbc95b140d02226cbull, 0, 0x3feff1768ee30099ull},
    {0x3fcc425da6c7b626ull, 0x3c938b1852b7b7c8ull, 0, 0x3fef3929d64cf437ull},
    {0xbfd5cb99e0e395baull, 0x3fdd28bd4a57c3d0ull, 0, 0x3ff1904d2900823bull},
    {0x3e2843eddcbd57a0ull, 0xbc801dbad4685bccull, 0, 0x3ff0000000000000ull},
    {0x01a56e1fc2f8f359ull, 0x3c93bbb0de9a8780ull, 0, 0x3ff0000000000000ull},
    {0x3fda5979bd72d18cull, 0x3c8abc1099d2e54cull, 0, 0x3fed537485ef33aaull},
    {0xbfe93f97f1a4f4faull, 0x3c98e367350f2720ull, 0, 0x3fe68ba46b82de67ull},
    {0xbfe36678e368d42cull, 0xbc76a63d28482d78ull, 0, 0x3fea4c110fca0586ull},
    {0x3e30000000000000ull, 0x3c6a075a7ce80db0ull, 0, 0x3ff0000000000000ull},
    {0x3fda544410472e20ull, 0xbc78b64e8e54e844ull, 0, 0x3fed547f47b1c90full},
    {0x3f93dd5150d0fa10ull, 0xbc9b3a2727ec3eb7ull, 0, 0x3feffe7569c37a6bull},
    {0xbfd1d2a4b9e4472full, 0x3fda57168e235c8cull, 0, 0x3ff137a760884225ull},
    {0xbfbaddb23144cd98ull, 0x3c9197a304e38456ull, 0, 0x3fefd2ee157036d7ull},
    {0x3fde8c611e1edb70ull, 0x3fc6b7778291b5f6ull, 0, 0x3fe9b66cc21e5ea6ull},
    {0xbfd01da9c0154b20ull, 0x3c6b158d1fd58180ull, 0, 0x3feefda5ee6b86bcull},
    {0xbfd96c2537d1e910ull, 0x3c75eaa74dac3e98ull, 0, 0x3fed82272dec7081ull},
    {0x3faf9bb2d8090b00ull, 0xbc933e258c226682ull, 0, 0x3feff064f4d7afbbull},
    {0x3fd1dc8ea2ea4a28ull, 0x3c87a4a57d44088cull, 0, 0x3feec307c829b013ull},
    {0x3e69fa91d06ba548ull, 0x3c8ef61b85d17830ull, 0, 0x3feffffffffffff5ull},
    {0x3e6db4cb94598d74ull, 0xbfcc82eb0b62dcc0ull, 0, 0x3ff00000034ef752ull},
    {0x3fca56b0b2eae9deull, 0xbc8c07f6d32585f0ull, 0, 0x3fef532e1fed8783ull},
    {0x3fcff3744564ceb0ull, 0xbfca8a529f868b7cull, 0, 0x3ff0550ce69be6cdull},
    {0x3fdf42a5c846a5d0ull, 0x3c96adb5d0bbb806ull, 0, 0x3fec4210beeba79full},
    {0x3fe76c998ba6743eull, 0x3c9b3215ff6c43d0ull, 0, 0x3fe7cd7e43f299ecull},
    {0xbddb7cdfd9d7bdbbull, 0xbfe6edd140ebefd2ull, 0, 0x3ff0000000000000ull},
    {0x3fb412c1f933e270ull, 0x3c9c974763bedd44ull, 0, 0x3fefe6d45283d8c3ull},
    {0x3fe3dbfbebb18566ull, 0x3e40000000000000ull, 0, 0x3fea086b1b905240ull},
    {0x3fb4a69eb7cef2d0ull, 0xbfc75866ba99108aull, 0, 0x3ff02ef169dae05bull},
    {0xbe752c13f94e8c69ull, 0xbc850fdc419dbd7aull, 0, 0x3fefffffffffffe3ull},
    {0x3fe25bc846b2b72eull, 0xbc73f1cc5ca7b768ull, 0, 0x3feae06e8c0d2f51ull},
    {0xbfd1c0728a83d406ull, 0xbc84920d80b43c10ull, 0, 0x3feec6e3ff261592ull},
    {0x3fdbdfc775054cbcull, 0x3c95c12b3282be5aull, 0, 0x3fed033ca8bda4a6ull},
    {0xbe68ccf8c9fdcd98ull, 0xbfc8855431a65678ull, 0, 0x3feffffffb3fbc05ull},
    {0x3e1975ff1e68d840ull, 0xb9b4484bfeebc2a0ull, 0, 0x3ff0000000000000ull},
    {0x3fb4315eac2fc068ull, 0xbc8863041df61786ull, 0, 0x3fefe6875cbeebc9ull},
    {0xbe6190021c8f68d8ull, 0x3c7515b7f1fa2340ull, 0, 0x3feffffffffffffbull},
    {0xbfdfdd053eb24addull, 0x3c8dab4bc7df799cull, 0, 0x3fec1d8665dc31bcull},
    {0x3fbb234f10214e10ull, 0xbc981e95805af635ull, 0, 0x3fefd2038f19730aull},
    {0x3fb26a031154cde8ull, 0xbc866e80056c3a04ull, 0, 0x3fefead11169b97cull},
    {0x3e634b4652936c68ull, 0x3c9a236f177b5ed0ull, 0, 0x3feffffffffffffaull},
    {0xb9b4484bfeebc2a0ull, 0x3c888039c85bd728ull, 0, 0x3ff0000000000000ull},
    {0x3fb2a7e5e0e40868ull, 0xbc93d2baaefe6ebeull, 0, 0x3fefea41cfb3a676ull},
    {0x3fd4a036443895c8ull, 0xbfe3c6555cbe2e7cull, 0, 0x3ff25cdcf4ad5a3full},
    {0xbfde65b4294ccba4ull, 0x3c72e3c4180f6580ull, 0, 0x3fec753ec40856afull},
    {0xbfdc7c540fd8c380ull, 0xbc8b43d823d5e796ull, 0, 0x3fece1dff3a6ddebull},
    {0xbfa58f2fbe4da960ull, 0xbc92b784944b2447ull, 0, 0x3feff8bd0ea4263full},
    {0xbfb98e9f5103c9b0ull, 0x3c617f9665a00510ull, 0, 0x3fefd7360416c8a7ull},
    {0x3fd2bf392d7df470ull, 0x3c8d304d33ad1b2cull, 0, 0x3feea30e993879d4ull},
    {0x3e70cc6c04e69ba8ull, 0xbc6411a7bf3c7b58ull, 0, 0x3fefffffffffffeeull},
    {0xbdf06e4b8dab9000ull, 0x3c9463363c480e6eull, 0, 0x3ff0000000000000ull},
    {0x3fcebde5b0d7920aull, 0xbe733e990ff4dcccull, 0, 0x3fef14de897a6645ull},
    {0xbfd69d880b7461baull, 0x3c9161441ff311c2ull, 0, 0x3fee05d8722f0c53ull},
    {0x3fc5d25340e17218ull, 0x3c9c3a9d614b2c7aull, 0, 0x3fef893e201a2eebull},
    {0x3fe68251e997754cull, 0xbc942d3584dd0694ull, 0, 0x3fe86790763e1fe2ull},
    {0xbfc54b539072f188ull, 0x3fca5adaf86b0bf6ull, 0, 0x3ff053c07f04d4d4ull},
    {0x3f62f13111ace100ull, 0x3c71d0712608d5d0ull, 0, 0x3feffffa64c79a44ull},
    {0x3fd53d2459fed2e8ull, 0xbc579764d272c870ull, 0, 0x3fee4109583f9a98ull},
    {0x3fdac6a768a4c710ull, 0xbc7d82698ab3f4c0ull, 0, 0x3fed3d71789536ddull},
    {0xbfd3b3d689573a3dull, 0xb9b4484bfeebc2a0ull, 0, 0x3fee7ede305a7762ull},
    {0x3fd8e2a85ca659acull, 0xbc8dc69df58f91daull, 0, 0x3fed9c7b2a1a4845ull},
    {0xbfe476e77330997cull, 0x3c92986d6b5e9c34ull, 0, 0x3fe9ad245e3fb70cull},
    {0x3fcd7b271bc204b8ull, 0xbc7eb382d65b8fc4ull, 0, 0x3fef27acef73910eull},
    {0xbe5608ce328e0100ull, 0x3c7a0c1eea430ce8ull, 0, 0x3feffffffffffffeull},
    {0xbfa745e181699c30ull, 0x3c6f048d088d7100ull, 0, 0x3feff789d91afbdeull},
    {0xbe4311e0c00cbef8ull, 0x3e40000000000000ull, 0, 0x3ff0000000000000ull},
    {0x0000000000000000ull, 0xbc6759c279d91da0ull, 0, 0x3ff0000000000000ull},
    {0x3fe376a7345c06b2ull, 0xbfe2e37d35ac216cull, 0, 0x3ff2dff8a9db025cull},
    {0xbe66c03f04f8e743ull, 0x3c96d0e9de018cc0ull, 0, 0x3feffffffffffff7ull},
    {0xbfd09a4cda06083eull, 0xbbf0e21547996000ull, 0, 0x3feeede4405d126cull},
};
int main() {
    int bad = 0;
    for (const Vector& v : vectors) {
        const Dp got = __kernel_cos(v.x, v.y);
        const bool both_nan = std::isnan(d(got)) && std::isnan(d(v.result));
        const long long distance = static_cast<long long>(got) - static_cast<long long>(v.result);
        if ((distance > 16 || distance < -16) && !both_nan) { std::printf("x=%016llx y=%016llx: got %016llx want %016llx\n", v.x, v.y, got, v.result); ++bad; }
    }
    // against the host cosine for a reduced argument (the kernel is within an ulp or two)
    for (double x = -0.78; x < 0.78; x += 0.013) {
        const double got = d(__kernel_cos(b(x), 0));
        if (std::fabs(got - std::cos(x)) > 4e-16) { std::printf("cos(%g): %g vs %g\n", x, got, std::cos(x)); ++bad; }
    }
    return bad;
}

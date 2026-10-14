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
#include "kernel_sin.cpp"
struct Vector { Dp x, y; int iy; Dp result; };
static const Vector vectors[] = {
    {0x3e72a717c9a42bb6ull, 0xbc8c3d161e1756b5ull, -1, 0x3e72a717c96bb185ull},
    {0xbddb7cdfd9d7bdbbull, 0xbfdac26f6d9cfba0ull, 0, 0xbddb7cdfd9d7bdbbull},
    {0xbfdaad3ef0ec61a4ull, 0x3c9804a37652d05eull, 1, 0xbfd9e933acd6ae6bull},
    {0x3e723489135b7fcaull, 0xbc9c1eaf77a543f9ull, 2, 0x3e72348912eb0508ull},
    {0xbe7094db32fd46a6ull, 0x3c83faf397c08498ull, 2, 0xbe7094db32d550bbull},
    {0xbfdf1283241abc7dull, 0xbc91568ad251f859ull, 1, 0xbfddddacd089c19eull},
    {0xbfb5bf6fec7620c0ull, 0xbfcf916b5cac3740ull, 1, 0xbfd5285092e5fa03ull},
    {0x3fe49ab5212e1e76ull, 0x3c98ec8c31e93edaull, 0, 0x3fe335b780424b08ull},
    {0x8000000000000000ull, 0xbc76e9b65b882858ull, -1, 0x8000000000000000ull},
    {0xbe25b621909ca260ull, 0x3fe57c595f1fe4aeull, 1, 0xbe25b621909ca260ull},
    {0xbfda4b96698264eaull, 0xbc9aba9a5e01cf2bull, 2, 0xbfd98fc9ad80204aull},
    {0x3f76ba8c5cc9e500ull, 0x3fba1bd146de0930ull, -1, 0x3fbb875f3a9201a7ull},
    {0x3e787d26f7d0f088ull, 0x3c7cb36cb806d008ull, 1, 0x3e787d26f7eda3ebull},
    {0x3f63cab52a9a6700ull, 0xbc958dfc648afff3ull, -1, 0x3f63cab3e792114eull},
    {0xbfc65711ae5ab834ull, 0xbc9a46deb91133e6ull, 1, 0xbfc63a13fb358f9cull},
    {0x3fca140ba9251b3aull, 0x3fd815b9f4a3ba1cull, 1, 0x3fe2445e8e3cb358ull},
    {0x7ff0000000000000ull, 0xbc99abf38aaa50c4ull, 0, 0x7ff0000000000000ull},
    {0x3e6329b85bd42428ull, 0xbfe770319d1a0a74ull, 2, 0xbfe7703189f05213ull},
    {0xbe60da49aab86d82ull, 0x3c43d6b95f429dc0ull, 0, 0xbe60da49aab86d81ull},
    {0xbe66deb847d6bcf9ull, 0x3c770a7ff790fe70ull, 1, 0xbe66deb847a8a7f7ull},
    {0x3fa990babb1d0d50ull, 0x3c99053ce0d15ce2ull, 1, 0x3fa98e029a0e1b0full},
    {0xbfdaf849323aff4aull, 0xbc98d96d209f778dull, 0, 0xbfda2dbf437af289ull},
    {0xbfe44d744fd2b84full, 0xbfde489b7c77a67aull, 1, 0xbfef0fd911fbdae3ull},
    {0x3fe626da02edbbc0ull, 0x3c9a043203bea7c0ull, 2, 0x3fe46cab22df0535ull},
    {0xb9b4484bfeebc2a0ull, 0x3c98dab67be23b76ull, 2, 0xb9b4484bfeebc2a0ull},
    {0xbfcce3410fc07b2cull, 0x3c70e66cbc101008ull, 1, 0xbfcca4a2bef023c9ull},
    {0xbfc704c77eb31188ull, 0xbc77560816ff5860ull, 2, 0xbfc6e511881590f8ull},
    {0x3e40000000000000ull, 0xbc9123229e4ef6a3ull, -1, 0x3e3ffffffbb73758ull},
    {0x3fe9341e5db46a28ull, 0xbc8de2bbdb58d24eull, 1, 0x3fe6ad6de2dccdb5ull},
    {0x3fbc82af6777d8c0ull, 0x3c8b4d7422fa7d24ull, 0, 0x3fbc739b5991e8c9ull},
    {0xbfd0a892a1e1e7ccull, 0xbc761d442a671b70ull, 2, 0xbfd07894d015759dull},
    {0xbfce3ddf93336534ull, 0x3c99dba356d2dafaull, 1, 0xbfcdf60c686006d0ull},
    {0x8000000000000000ull, 0x3e5e5d120ba297b8ull, 1, 0x8000000000000000ull},
    {0xbfc093a2d3dc3f28ull, 0x3c996d5deb509dd6ull, -1, 0xbfc087c8a9e80b2aull},
    {0xb9b4484bfeebc2a0ull, 0x3c971a2477770a34ull, 1, 0xb9b4484bfeebc2a0ull},
    {0x0000000000000001ull, 0x3c947e21c0552568ull, 1, 0x0000000000000001ull},
    {0xbfcff94466909db4ull, 0xbc973eeb6812e051ull, -1, 0xbfcfa468c56e1c23ull},
    {0xbfe6c6a37f1e0b14ull, 0x3c7e5ead2dbe5ec8ull, 2, 0xbfe4e6ac3d922fa5ull},
    {0x3fc95692e08adc96ull, 0x3c96b13cc56a2ca0ull, 2, 0x3fc92c4af8de9a1dull},
    {0xbf7291ff49ad6100ull, 0xbc8bf3b9791fa447ull, 2, 0xbf7291fb1e54d5c8ull},
    {0x3fe043a36fc0e62eull, 0xbc61200ee487b7d0ull, 1, 0x3fdf255b04777bf4ull},
    {0x3fdac2934eda924cull, 0xbfe8c04aa1739b0aull, 2, 0xbfd3301d34b40998ull},
    {0x3fd7eefbd5c39fc8ull, 0xbc2540591a25a780ull, 1, 0x3fd7612c0fb078c9ull},
    {0x7e37e43c8800759cull, 0x3c9137cc06e9b112ull, 1, 0xfff8000000000000ull},
    {0x3fe692fd20fedb52ull, 0x3c88a3397bcf73a8ull, 0, 0x3fe4bf74d25ec09full},
    {0x3fac6b36478969d0ull, 0xbc76da393e8e0150ull, 2, 0x3fac677a19de0d8cull},
    {0x3fc2c661ee870eb4ull, 0x3c8e7e7604fd2e9cull, 1, 0x3fc2b52a973f76d1ull},
    {0xbfd144da22892594ull, 0x3fd2b6ed82265104ull, 0, 0xbfd10f66ff7a04cfull},
    {0x3dfc13e3968d7200ull, 0xbc46133bdd4a7ba0ull, 1, 0x3dfc13e3968d7200ull},
    {0xbfe49eb226270937ull, 0xbc9804f0abbbad37ull, 0, 0xbfe338e7ee94aadcull},
    {0xbe54c9cae7d10c20ull, 0xbc967c1f7768e040ull, 1, 0xbe54c9cae938ce17ull},
    {0xbfae819e64402854ull, 0xbc95b140d02226cbull, -1, 0xbfae7cffac66a31dull},
    {0x3fcc425da6c7b626ull, 0x3c938b1852b7b7c8ull, 0, 0x3fcc07bd579e8c56ull},
    {0xbfd5cb99e0e395baull, 0x3fdd28bd4a57c3d0ull, 2, 0x3fb85e009b1f6088ull},
    {0x3e2843eddcbd57a0ull, 0xbc801dbad4685bccull, -1, 0x3e2843eddcbd57a0ull},
    {0x01a56e1fc2f8f359ull, 0x3c93bbb0de9a8780ull, 2, 0x01a56e1fc2f8f359ull},
    {0x3fda5979bd72d18cull, 0x3c8abc1099d2e54cull, 1, 0x3fd99c84840b0b7full},
    {0xbfe93f97f1a4f4faull, 0x3c98e367350f2720ull, 1, 0xbfe6b585013bf827ull},
    {0xbfe36678e368d42cull, 0xbc76a63d28482d78ull, -1, 0xbfe23bc6ed3c9cd3ull},
    {0x3e30000000000000ull, 0x3c6a075a7ce80db0ull, 1, 0x3e30000000000000ull},
    {0x3fda544410472e20ull, 0xbc78b64e8e54e844ull, 0, 0x3fd997be33326a32ull},
    {0x3f93dd5150d0fa10ull, 0xbc9b3a2727ec3eb7ull, 1, 0x3f93dcffaa79c9ceull},
    {0xbfd1d2a4b9e4472full, 0x3fda57168e235c8cull, 1, 0x3fbee6e14983d570ull},
    {0xbfbaddb23144cd98ull, 0x3c9197a304e38456ull, 2, 0xbfbad11412976afaull},
    {0x3fde8c611e1edb70ull, 0x3fc6b7778291b5f6ull, 1, 0x3fe3bba439262d70ull},
    {0xbfd01da9c0154b20ull, 0x3c6b158d1fd58180ull, 2, 0xbfcfe466c7c96cb7ull},
    {0xbfd96c2537d1e910ull, 0x3c75eaa74dac3e98ull, 2, 0xbfd8c256d23b3405ull},
    {0x3faf9bb2d8090b00ull, 0xbc933e258c226682ull, 1, 0x3faf968f4ce939f0ull},
    {0x3fd1dc8ea2ea4a28ull, 0x3c87a4a57d44088cull, 1, 0x3fd1a16dcfedd8a6ull},
    {0x3e69fa91d06ba548ull, 0x3c8ef61b85d17830ull, -1, 0x3e69fa91d0e77db3ull},
    {0x3e6db4cb94598d74ull, 0xbfcc82eb0b62dcc0ull, 1, 0xbfcc82ea948fae61ull},
    {0x3fca56b0b2eae9deull, 0xbc8c07f6d32585f0ull, -1, 0x3fca27354e6e1196ull},
    {0x3fcff3744564ceb0ull, 0xbfca8a529f868b7cull, 0, 0x3fcf9ec6cbb5bceaull},
    {0x3fdf42a5c846a5d0ull, 0x3c96adb5d0bbb806ull, -1, 0x3fde0836fb55e32cull},
    {0x3fe76c998ba6743eull, 0x3c9b3215ff6c43d0ull, 0, 0x3fe5633c4cfd4db8ull},
    {0xbddb7cdfd9d7bdbbull, 0xbfe6edd140ebefd2ull, 1, 0xbddb7cdfd9d7bdbbull},
    {0x3fb412c1f933e270ull, 0x3c9c974763bedd44ull, 1, 0x3fb40d7e58a3d06aull},
    {0x3fe3dbfbebb18566ull, 0x3e40000000000000ull, 0, 0x3fe29bdcefef7ac6ull},
    {0x3fb4a69eb7cef2d0ull, 0xbfc75866ba99108aull, 1, 0xbfb9e906409e6400ull},
    {0xbe752c13f94e8c69ull, 0xbc850fdc419dbd7aull, 1, 0xbe752c13f978ac1bull},
    {0x3fe25bc846b2b72eull, 0xbc73f1cc5ca7b768ull, -1, 0x3fe15e2e3ba82aadull},
    {0xbfd1c0728a83d406ull, 0xbc84920d80b43c10ull, 0, 0xbfd18666748a04c4ull},
    {0x3fdbdfc775054cbcull, 0x3c95c12b3282be5aull, 0, 0x3fdb004ff21bc505ull},
    {0xbe68ccf8c9fdcd98ull, 0xbfc8855431a65678ull, 1, 0xbfc8855494da3997ull},
    {0x3e1975ff1e68d840ull, 0xb9b4484bfeebc2a0ull, -1, 0x3e1975ff1e68d840ull},
    {0x3fb4315eac2fc068ull, 0xbc8863041df61786ull, 1, 0x3fb42c02d2a4ffb4ull},
    {0xbe6190021c8f68d8ull, 0x3c7515b7f1fa2340ull, -1, 0xbe6190021c653d67ull},
    {0xbfdfdd053eb24addull, 0x3c8dab4bc7df799cull, 0, 0xbfde903157276203ull},
    {0x3fbb234f10214e10ull, 0xbc981e95805af635ull, 1, 0x3fbb164de73670c5ull},
    {0x3fb26a031154cde8ull, 0xbc866e80056c3a04ull, 0, 0x3fb265f2b22a2163ull},
    {0x3e634b4652936c68ull, 0x3c9a236f177b5ed0ull, -1, 0x3e634b46536487dfull},
    {0xb9b4484bfeebc2a0ull, 0x3c888039c85bd728ull, 1, 0xb9b4484bfeebc2a0ull},
    {0x3fb2a7e5e0e40868ull, 0xbc93d2baaefe6ebeull, 0, 0x3fb2a3abffe4535eull},
    {0x3fd4a036443895c8ull, 0xbfe3c6555cbe2e7cull, -1, 0xbfd13997e990c0c8ull},
    {0xbfde65b4294ccba4ull, 0x3c72e3c4180f6580ull, 2, 0xbfdd4469f863ab81ull},
    {0xbfdc7c540fd8c380ull, 0xbc8b43d823d5e796ull, 2, 0xbfdb8deddfb2be2cull},
    {0xbfa58f2fbe4da960ull, 0xbc92b784944b2447ull, 1, 0xbfa58d8e3e87131eull},
    {0xbfb98e9f5103c9b0ull, 0x3c617f9665a00510ull, 0, 0xbfb983c2871d75b6ull},
    {0x3fd2bf392d7df470ull, 0x3c8d304d33ad1b2cull, 0, 0x3fd27ae2ce60c9a3ull},
    {0x3e70cc6c04e69ba8ull, 0xbc6411a7bf3c7b58ull, 2, 0x3e70cc6c04dc92d1ull},
    {0xbdf06e4b8dab9000ull, 0x3c9463363c480e6eull, 2, 0xbdf06e4b8dab9000ull},
    {0x3fcebde5b0d7920aull, 0xbe733e990ff4dcccull, 1, 0x3fce727463831847ull},
    {0xbfd69d880b7461baull, 0x3c9161441ff311c2ull, 0, 0xbfd625cb7362e19bull},
    {0x3fc5d25340e17218ull, 0x3c9c3a9d614b2c7aull, 2, 0x3fc5b74df520ba68ull},
    {0x3fe68251e997754cull, 0xbc942d3584dd0694ull, 1, 0x3fe4b2c145e9e071ull},
    {0xbfc54b539072f188ull, 0x3fca5adaf86b0bf6ull, -1, 0x3fa32d1ac4009d5cull},
    {0x3f62f13111ace100ull, 0x3c71d0712608d5d0ull, 0, 0x3f62f12ff67c6cc6ull},
    {0x3fd53d2459fed2e8ull, 0xbc579764d272c870ull, 2, 0x3fd4d9e46e39f200ull},
    {0x3fdac6a768a4c710ull, 0xbc7d82698ab3f4c0ull, -1, 0x3fda006d2e85bd81ull},
    {0xbfd3b3d689573a3dull, 0xb9b4484bfeebc2a0ull, 1, 0xbfd3648b90898a60ull},
    {0x3fd8e2a85ca659acull, 0xbc8dc69df58f91daull, 1, 0x3fd84355b80f43c2ull},
    {0xbfe476e77330997cull, 0x3c92986d6b5e9c34ull, 1, 0xbfe3190911d23713ull},
    {0x3fcd7b271bc204b8ull, 0xbc7eb382d65b8fc4ull, 0, 0x3fcd389a607d436full},
    {0xbe5608ce328e0100ull, 0x3c7a0c1eea430ce8ull, -1, 0xbe5608ce3225d083ull},
    {0xbfa745e181699c30ull, 0x3c6f048d088d7100ull, 2, 0xbfa743d45609619aull},
    {0xbe4311e0c00cbef8ull, 0x3e40000000000000ull, -1, 0xbe188f060065f7c4ull},
    {0x0000000000000000ull, 0xbc6759c279d91da0ull, 0, 0x0000000000000000ull},
    {0x3fe376a7345c06b2ull, 0xbfe2e37d35ac216cull, -1, 0x3fb7202395f66650ull},
    {0xbe66c03f04f8e743ull, 0x3c96d0e9de018cc0ull, 2, 0xbe66c03f04425ff2ull},
    {0xbfd09a4cda06083eull, 0xbbf0e21547996000ull, 1, 0xbfd06ac9b2b8bba0ull},
};
int main() {
    int bad = 0;
    for (const Vector& v : vectors) {
        const Dp got = __kernel_sin(v.x, v.y, v.iy);
        const bool both_nan = std::isnan(d(got)) && std::isnan(d(v.result));
        const long long distance = static_cast<long long>(got) - static_cast<long long>(v.result);
        if ((distance > 16 || distance < -16) && !both_nan) { std::printf("x=%016llx y=%016llx iy=%d: got %016llx want %016llx\n", v.x, v.y, v.iy, got, v.result); ++bad; }
    }
    // against the host sine for a reduced argument (the kernel is within an ulp or two)
    for (double x = -0.78; x < 0.78; x += 0.013) {
        const double got = d(__kernel_sin(b(x), 0, 0));
        if (std::fabs(got - std::sin(x)) > 4e-16) { std::printf("sin(%g): %g vs %g\n", x, got, std::sin(x)); ++bad; }
    }
    return bad;
}

// Host contract for initNoiseTable__Fv. Not an EE plan input.
#include <cstdio>

extern "C" void initNoiseTable__Fv();
extern "C" int eRandState = 0;
extern "C" unsigned char noiseTable[512] = {};

extern "C" float powf(float base, float exponent) {
    if (base == 8.f && exponent == -1.f) return 0.125f;
    if (base == 4.f && exponent == -1.f) return 0.25f;
    if (base == 2.f && exponent == -1.f) return 0.5f;
    if (base == 1.f && exponent == -1.f) return 1.f;
    std::fprintf(stderr, "fail: powf arguments\n");
    return 0.f;
}

namespace {
short half_at(int index) {
    const unsigned char* slot = noiseTable + index * 2;
    const unsigned bits = static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8);
    return static_cast<short>(bits);
}

const short kExpected[256] = {10,2080,6031,8885,10139,13204,14827,14034,11842,7760,2487,-2811,-7873,-8488,-9792,-11632,-12248,-10671,-9072,-7909,-4875,-10318,-7278,-9354,-11925,-13140,-14096,-9343,-11702,-11956,-11132,-10606,-9705,-5652,-10976,-12697,-12369,-5894,-3872,1679,6944,9924,7187,2369,3931,4256,10868,15404,15129,18465,10414,6425,7794,7852,4172,2380,4459,10301,9828,7089,-1921,-7683,-13034,-22259,-15132,-11989,-9182,-9418,-8377,-7135,773,8333,5756,8228,4982,2826,-1690,-1667,1339,274,1734,5361,10217,10817,8563,5650,9463,11595,6121,8187,878,-4717,-5437,-4838,-11476,-13137,-4669,-1004,2493,5566,465,-5319,462,-427,-4433,-7087,-8836,-11052,-14919,-21171,-21928,-23946,-23691,-20570,-19563,-22641,-15670,-8137,-2124,2439,3604,194,-740,4266,2860,5096,7842,15362,10127,11556,6448,9289,10569,9113,10217,13071,10050,13673,8635,5669,-320,-1300,-5895,-14002,-11474,-13468,-9573,-8804,-10564,-11306,-4156,-1563,2072,4193,2566,4942,7004,3082,11115,16164,15660,15652,12532,11223,6611,-247,-4747,-4718,-3075,-2599,131,423,2052,5119,7017,8048,9613,6951,6659,1380,1817,4610,5327,5488,-3839,-7802,-11337,-15169,-20655,-22773,-18706,-16147,-15867,-17829,-14541,-15688,-8544,-4936,118,-170,174,1664,-4846,-10007,-8825,-9640,-11166,-9597,-11690,-10547,-8408,-6269,-5130,-2681,-566,2718,3965,4265,1141,-4665,-4063,637,1649,3745,4591,5378,7496,11167,13561,17506,18860,22083,20665,16776,10744,722,-631,-3356,-6418,-9571,-8714,-6689,-7108,-7527,-9523,-11519,-13515,-15510,-13571,-11633,-9694,-7755,-5816,-3878,-1939,0};
}

int main() {
    for (int byte = 0; byte < 512; ++byte) noiseTable[byte] = 0x5a;
    eRandState = 0;
    initNoiseTable__Fv();
    if (eRandState != static_cast<int>(0x1adddd60u)) {
        std::fprintf(stderr, "fail: seed %08x\n", static_cast<unsigned>(eRandState));
        return 1;
    }
    for (int index = 0; index < 256; ++index) {
        if (half_at(index) != kExpected[index]) {
            std::fprintf(stderr, "fail: half %d got %d expect %d\n", index, half_at(index), kExpected[index]);
            return 1;
        }
    }
    return 0;
}

// Host comparison only. Not an EE build input.
#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" float norrath_integer_sine(int angle);
extern "C" float norrath_integer_cosine(int angle);
extern "C" float norrath_icos_scale, norrath_icos_cubic, norrath_icos_quintic, norrath_icos_septic;
extern "C" float norrath_isin_scale, norrath_isin_cubic, norrath_isin_quintic, norrath_isin_septic;

static unsigned bits_of(float value) {
    unsigned bits;
    std::memcpy(&bits, &value, 4);
    return bits;
}

int main() {
    const unsigned expected[] = {
        0x38490fdau, 0x3e2aaa57u, 0x3c086b24u, 0x39458017u,
        0x38490fdau, 0x3e2aaa57u, 0x3c086b24u, 0x39458017u,
    };
    const float* values[] = {
        &norrath_icos_scale, &norrath_icos_cubic, &norrath_icos_quintic, &norrath_icos_septic,
        &norrath_isin_scale, &norrath_isin_cubic, &norrath_isin_quintic, &norrath_isin_septic,
    };
    for (int i = 0; i < 8; ++i) {
        if (bits_of(*values[i]) != expected[i]) {
            std::printf("coefficient %d bits %08x\n", i, bits_of(*values[i]));
            return 1;
        }
    }
    float max_sine = 0.f;
    float max_cosine = 0.f;
    for (int angle = 0; angle < 65536; ++angle) {
        const double turn = static_cast<double>(angle) * 6.283185307179586 / 65536.0;
        const float sine_error = std::fabs(norrath_integer_sine(angle) - static_cast<float>(std::sin(turn)));
        const float cosine_error = std::fabs(norrath_integer_cosine(angle) - static_cast<float>(std::cos(turn)));
        if (sine_error > max_sine) max_sine = sine_error;
        if (cosine_error > max_cosine) max_cosine = cosine_error;
    }
    if (!(max_sine < 5.0e-7f) || !(max_cosine < 5.0e-7f)) {
        std::printf("trig error sine %g cosine %g\n", max_sine, max_cosine);
        return 1;
    }
    return 0;
}

// Observed: GetOvercastInterp__12VIAtmosphere at 0x0106b578 is 104 bytes. It reads two signed
// words, A at byte offset 1516 and B at 1520, and the float F at 1544, and returns a float:
//   A == 0, B == 0: 0.0      A == 0, B == 1: F      A == 1, B == 0: 1.0 - F
//   every other pair: 1.0.
// Nothing is stored. Inferred: A and B are the previous and current overcast states, F is the
// blend between them, and the result is the overcast weight. Unknown: the state encoding beyond
// the values 0 and 1 and the member names.
extern "C" {
float GetOvercastInterp__12VIAtmosphere(const unsigned char* self) {
    const int a = *reinterpret_cast<const int*>(self + 1516);
    const int b = *reinterpret_cast<const int*>(self + 1520);
    const float f = *reinterpret_cast<const float*>(self + 1544);
    if (a == 0) {
        if (b == 0) return 0.0f;
        if (b == 1) return f;
    } else if (a == 1 && b == 0) {
        return 1.0f - f;
    }
    return 1.0f;
}
}

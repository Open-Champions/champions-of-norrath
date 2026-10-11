#pragma once
// Explicit single-scalar EE operations for genuine operation-sensitive paths.
// Fixed R5900 EABI/single-float profile; float operands/results remain real values.
// No raw words, copied body, memory response, fallback or widened flags.
static_assert(sizeof(float) == 4 && __FLT_MANT_DIG__ == 24 && __FLT_MAX_EXP__ == 129,
              "actual EE single-float representation");
namespace norrath_ee_scalar_operations {
inline float subtract(float left, float right) {
    float result;
    __asm__ volatile ("sub.s %0, %1, %2" : "=f"(result) : "f"(left), "f"(right));
    return result;
}
inline float divide(float numerator, float denominator) {
    float result;
    __asm__ volatile ("div.s %0, %1, %2" : "=f"(result) : "f"(numerator), "f"(denominator));
    return result;
}
}

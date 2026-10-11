// Independently authored target CPU boundaries for collision reconstruction.
// These are architectural operations, not matches for original game functions.
static_assert(__CHAR_BIT__ == 8 && sizeof(float) == 4 && sizeof(int) == 4,
              "requires the established target word model");
extern "C" float ee_collision_sqrt(float value) {
    float result;
    __asm__ volatile ("sqrt.s %0, %1" : "=f" (result) : "f" (value));
    return result;
}
extern "C" float ee_collision_min(float left, float right) {
    float result;
    __asm__ volatile ("min.s %0, %1, %2" : "=f" (result) : "f" (left), "f" (right));
    return result;
}
extern "C" float ee_collision_max(float left, float right) {
    float result;
    __asm__ volatile ("max.s %0, %1, %2" : "=f" (result) : "f" (left), "f" (right));
    return result;
}
extern "C" float ee_collision_int_to_float(int value) {
    float result;
    __asm__ volatile ("mtc1 %1, %0\n\tcvt.s.w %0, %0" : "=f" (result) : "r" (value));
    return result;
}

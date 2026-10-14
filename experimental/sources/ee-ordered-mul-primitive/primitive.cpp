// Independently authored EE CPU boundary; no retail-function match claimed.
static_assert(__CHAR_BIT__ == 8 && sizeof(float) == 4,
              "requires the reviewed target float representation");
extern "C" float ee_mul_ordered(float fs_value, float ft_value) {
    float result;
    __asm__ volatile ("mul.s %0, %1, %2"
                      : "=f" (result)
                      : "f" (fs_value), "f" (ft_value));
    return result;
}

// Conversion to unsigned char retains the low eight bits for every int value.
// This adapter describes one live ordinary byte; original signedness is unknown.
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned char) == 1);
static_assert(sizeof(int) == 4 && __INT_MAX__ == 2147483647);
extern "C" unsigned char norrath_half_frustum_mode;
extern "C" void norrath_set_half_frustum_mode(int value)
{
    norrath_half_frustum_mode = static_cast<unsigned char>(value);
}

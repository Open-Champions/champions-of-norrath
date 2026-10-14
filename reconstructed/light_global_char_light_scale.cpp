// Finite representable float values are the portable source-level contract.
// Exceptional object representations have separate target-instruction evidence.
typedef char float_width[sizeof(float) == 4 ? 1 : -1];
extern "C" float norrath_global_char_light_scale;
extern "C" void norrath_light_global_char_light_scale(float value)
{
    norrath_global_char_light_scale = value;
}

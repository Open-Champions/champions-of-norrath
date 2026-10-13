// Observed clampZoomMag__6Camera at 0x0026fda0, 64 bytes. Reads the float at camera+144.
// When it is greater than 1.0f the field is set to 1.0f and 1 is returned. Otherwise,
// when it is less than 0.0f the field is set to 0.0f and 1 is returned. Otherwise the
// field is untouched and 0 is returned. (NaN passes through unchanged and returns 0.)
// The field's role is inferred from the function name only. Nothing here was
// executed on the EE.
extern "C" int clampZoomMag__6Camera(unsigned char* camera) {
    float* mag = reinterpret_cast<float*>(camera + 144);
    if (*mag > 1.0f) {
        *mag = 1.0f;
        return 1;
    }
    if (*mag < 0.0f) {
        *mag = 0.0f;
        return 1;
    }
    return 0;
}

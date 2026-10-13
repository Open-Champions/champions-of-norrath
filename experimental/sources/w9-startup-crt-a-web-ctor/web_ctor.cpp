// Observed __3Webii at 0x0020fac8, 24 bytes: the Web constructor taking two ints.
// The object address is the first argument. The two ints are stored at +48 and +52
// in argument order, and words +8, +28 and +44 are cleared. No other byte is
// written and nothing is returned. BlackWidow constructs one at its +976 with 35
// and 180; what the two numbers and the cleared words mean is unknown. Fresh
// compilation is not an original-byte match.
extern "C" void __3Webii(void* object, int first, int second) {
    int* words = static_cast<int*>(object);
    words[12] = first;
    words[13] = second;
    words[2] = 0;
    words[7] = 0;
    words[11] = 0;
}

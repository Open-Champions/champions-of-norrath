// Observed: CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44 at 0x010631e0, 160 bytes. Arguments (this, matrix). It forms the squared length
// of the three floats at matrix+48, +52 and +56, summed in the order x*x + y*y + z*z (the vector is first copied into a stack
// temporary). It reads the signed count at this+168 and a float array that starts at this+172. Starting with the result -1, it walks the
// array from element 0 while the index is below the count: when the squared length is less than the square of the element it stops and
// returns the current result, otherwise it adds one to the result. It returns the result (so the count when no element stops it, minus
// one when the first element already does). With a count of zero or less it returns -1. It calls nothing and writes no memory outside its
// stack.
// Inferred: matrix+48..56 is the translation row, so this is a distance test against an ascending list of level-of-detail switch
// distances; the returned value is the index of the last distance that was passed.
// Unknown: the unit of the distances and how the caller handles -1.
extern "C" {
int CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44(const void* object, const void* matrix) {
    const float* translation = reinterpret_cast<const float*>(static_cast<const unsigned char*>(matrix) + 48);
    float vector[3] = {translation[0], translation[1], translation[2]};
    float squared = vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
    const unsigned char* bytes = static_cast<const unsigned char*>(object);
    int count = *reinterpret_cast<const int*>(bytes + 168);
    const float* distances = reinterpret_cast<const float*>(bytes + 172);
    int level = -1;
    for (int i = 0; i < count; ++i) {
        if (squared < distances[i] * distances[i]) return level;
        ++level;
    }
    return level;
}
}

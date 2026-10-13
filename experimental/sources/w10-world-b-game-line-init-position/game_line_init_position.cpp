// Observed initPosition__FP17gameLinePathState at 0x00278b48, 240 bytes in SLUS_205.65.
// Argument: a path state. Its word at +0 points at the position being followed (three
// floats) and its word at +28 is a line number into gameLines (0x008d57a8). From the line
// record (point count at +4, points at +20, 64 bytes each) it finds the point nearest the
// position by Euclidean distance (each distance is sqrt((dx*dx + dy*dy) + dz*dz), measured
// from the position to the point, x then y then z). The first point is the starting
// candidate; a later point replaces it only when strictly nearer. The nearest index is
// stored as a word at +32 of the state and the byte at +17 is set to 1.
// Inferred: +32 is the current point index and +17 an "initialised" flag from its use here;
// the readers are not examined. Unknown: whether the function returns a value;
// the image leaves 1 in the result register only as the value stored to +17.
extern unsigned char* gameLines[100];

extern "C" void initPosition__FP17gameLinePathState(unsigned char* state) {
    unsigned char* record = gameLines[*reinterpret_cast<int*>(state + 28)];
    const float* position = *reinterpret_cast<const float**>(state);
    const unsigned char* points = *reinterpret_cast<unsigned char**>(record + 20);
    const int count = *reinterpret_cast<int*>(record + 4);
    float best = 0;
    int nearest = 0;
    for (int i = 0; i == 0 || i < count; ++i) {
        const float* at = reinterpret_cast<const float*>(points + 64 * i);
        const float dx = position[0] - at[0];
        const float dy = position[1] - at[1];
        const float dz = position[2] - at[2];
        const float distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        if (i == 0 || distance < best) {
            best = distance;
            nearest = i;
        }
    }
    *reinterpret_cast<int*>(state + 32) = nearest;
    state[17] = 1;
}

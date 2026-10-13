// Observed ChangeNetCam__6Camerai at 0x0026fcb0, 240 bytes. Member of Camera; the signed byte at
// camera + 353 is the player index the camera follows. Let list be the int array playersAlive with
// nmPlayersAlive entries and cur the byte at camera + 353.
//  1. idx is the first position in list whose value equals cur; zero when there is no match (also
//     when the list is empty).
//  2. n = idx + step (the second argument). With step above zero a result at or past the count
//     wraps to zero. With step below zero a negative result wraps to count - 1. A step of zero
//     leaves idx unchanged, and so does any other in-range result.
//  3. The byte at camera + 353 becomes the low byte of list[n]. When that byte, as a signed value,
//     differs from cur, Reset__6Cameraii(camera, 0, 0) and then coreStartSwitchScene__Fi(10) run.
// The list is read at n even when the count is not positive, as in the original (no bounds check).
// Unknown: what scene 10 is, and how the list is filled.
extern "C" {
extern int nmPlayersAlive;
extern int playersAlive[];
void Reset__6Cameraii(void* camera, int a, int b);
void coreStartSwitchScene__Fi(int scene);
}

extern "C" void ChangeNetCam__6Camerai(void* camera, int step) {
    signed char* followed = reinterpret_cast<signed char*>(camera) + 353;
    const int count = nmPlayersAlive;
    const signed char current = *followed;
    int index = 0;
    while (index < count && playersAlive[index] != current) ++index;
    if (index == count) index = 0;
    index += step;
    if (step > 0) {
        if (index >= count) index = 0;
    } else if (step < 0) {
        if (index < 0) index = count - 1;
    }
    *followed = static_cast<signed char>(playersAlive[index]);
    if (*followed != current) {
        Reset__6Cameraii(camera, 0, 0);
        coreStartSwitchScene__Fi(10);
    }
}

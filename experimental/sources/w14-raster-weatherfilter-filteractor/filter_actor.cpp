// Observed: FilterActor__13WeatherFilterR7VIScenei at 0x0106b308 is 8 bytes in the pinned SLUS-20565 image. It returns zero for any
// arguments and touches no memory.
// Inferred: a virtual-style hook whose weather-class implementation accepts every actor as "not filtered" (zero); the sense of the
// result (keep or reject) is not established.
// Unknown: how callers interpret the result, and the argument meanings (scene reference, int). Nothing here ran on the EE.
extern "C" {
int FilterActor__13WeatherFilterR7VIScenei(void* self, void* scene, int actor) {
    (void)self;
    (void)scene;
    (void)actor;
    return 0;
}
}

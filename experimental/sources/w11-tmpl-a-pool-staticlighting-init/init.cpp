// Observed: Init__t6VIPool1ZP16VIStaticLightingi at 0x012249b0, 92 bytes. Calls the pool's Clear; a
// negative result returns -1 straight away. Otherwise stores the integer argument at pool+4
// and calls Allocate for the same pool type; the result is -1 when that is negative, else 0.
// Inferred: the argument is the entry capacity.
extern "C" {
int Clear__t6VIPool1ZP16VIStaticLighting(void* pool);
int Allocate__t6VIPool1ZP16VIStaticLighting(void* pool);

int Init__t6VIPool1ZP16VIStaticLightingi(void* pool, int capacity) {
    if (Clear__t6VIPool1ZP16VIStaticLighting(pool) < 0) return -1;
    *(reinterpret_cast<int*>(pool) + 1) = capacity;
    return Allocate__t6VIPool1ZP16VIStaticLighting(pool) < 0 ? -1 : 0;
}
}

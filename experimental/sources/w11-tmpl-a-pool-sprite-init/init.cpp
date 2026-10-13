// Observed: Init__t6VIPool1ZP8VISpritei at 0x01224890, 92 bytes. Calls the pool's Clear; a
// negative result returns -1 straight away. Otherwise stores the integer argument at pool+4
// and calls Allocate for the same pool type; the result is -1 when that is negative, else 0.
// Inferred: the argument is the entry capacity.
extern "C" {
int Clear__t6VIPool1ZP8VISprite(void* pool);
int Allocate__t6VIPool1ZP8VISprite(void* pool);

int Init__t6VIPool1ZP8VISpritei(void* pool, int capacity) {
    if (Clear__t6VIPool1ZP8VISprite(pool) < 0) return -1;
    *(reinterpret_cast<int*>(pool) + 1) = capacity;
    return Allocate__t6VIPool1ZP8VISprite(pool) < 0 ? -1 : 0;
}
}

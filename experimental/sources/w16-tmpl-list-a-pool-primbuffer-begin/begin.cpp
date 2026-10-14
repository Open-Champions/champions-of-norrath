// Observed: Begin__Ct6VIPool1ZP12VIPrimBuffer at 0x01211660 is 44 bytes. It reads the word at
// pool+16. If that word is not -1 it returns it unchanged. If it is -1 it tail-calls
// End__Ct6VIPool1ZP12VIPrimBuffer with the same pool and returns what that call returns.
// Inferred: pool+16 is the index of the first used element and -1 means none, so this is the
// begin iterator of a pool and End supplies the past-the-end value.
// Unknown: the template parameters beyond the mangled name, and what End returns.
extern "C" {
int End__Ct6VIPool1ZP12VIPrimBuffer(const void* pool);

int Begin__Ct6VIPool1ZP12VIPrimBuffer(const void* pool) {
    const int first = *reinterpret_cast<const int*>(static_cast<const unsigned char*>(pool) + 16);
    if (first == -1) return End__Ct6VIPool1ZP12VIPrimBuffer(pool);
    return first;
}
}

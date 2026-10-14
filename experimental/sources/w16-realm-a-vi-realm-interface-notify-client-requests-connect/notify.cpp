// Observed notifyClientRequestsConnect__16VIRealmInterfacePCci at 0x011090c8, 304 bytes, 64-byte frame. With (this,
// name, port), n the word at this + 3476 and max the word at this + 3472, a table of 1144-byte entries starting at
// this + 5088:
//  1. printf("notifyClientRequestsConnect(%s, %i)\n", name, port).
//  2. For i from 0 while i < n - 1 (n re-read after every call): strcmp(name, entry i). The result is not used.
//  3. With n read again: when n < max, entry = this + 5088 + (n - 1) * 1144 and:
//     memset(entry, 0, 1144); snprintf(entry, 1023, name) -- the name itself is the format string;
//     the word at entry + 1024 = port; the word at entry + 1048 = 7; the 64-bit value at entry + 1056 =
//     GetTime__7VITimer(); memset(entry + 1092, 0, 52); then UpdateNumPlayers__16VIRealmInterfacei(this, n' + 1)
//     with n' the word at this + 3476 read once more.
//  When n >= max nothing more happens. The entry index is n - 1, so with n == 0 the entry address is 1144 bytes
//  below the table start; the source reproduces that arithmetic.
// Inferred: a request from a client to connect adds a record to a table of candidate clients; the table seems to
// have a header entry, since the count includes it. Not inferred: that the strcmp loop is a duplicate check (its
// result is dropped and the loop never exits early).
// Unknown: what the count and limit words mean, the meaning of 7, and whether the use of the name as a format
// string is intended (it is what the image does).
extern "C" {
int printf(const char* format, ...);
int strcmp(const char* a, const char* b);
void* memset(void* dest, int value, unsigned long size);
int snprintf(char* dest, unsigned long size, const char* format, ...);
unsigned long long GetTime__7VITimer();
void UpdateNumPlayers__16VIRealmInterfacei(void* self, int count);

void notifyClientRequestsConnect__16VIRealmInterfacePCci(void* self, const char* name, int port) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const volatile int* count = reinterpret_cast<const volatile int*>(bytes + 3476);
    const int* limit = reinterpret_cast<const int*>(bytes + 3472);
    printf("notifyClientRequestsConnect(%s, %i)\n", name, port);
    for (int i = 0; i < *count - 1; ++i) {
        strcmp(name, reinterpret_cast<const char*>(bytes + 5088 + i * 1144));
    }
    int n = *count;
    if (n < *limit) {
        unsigned char* entry = bytes + 5088 + (n - 1) * 1144;
        memset(entry, 0, 1144);
        snprintf(reinterpret_cast<char*>(entry), 1023, name);
        *reinterpret_cast<int*>(entry + 1024) = port;
        *reinterpret_cast<int*>(entry + 1048) = 7;
        *reinterpret_cast<unsigned long long*>(entry + 1056) = GetTime__7VITimer();
        memset(entry + 1092, 0, 52);
        UpdateNumPlayers__16VIRealmInterfacei(self, *count + 1);
    }
}
}

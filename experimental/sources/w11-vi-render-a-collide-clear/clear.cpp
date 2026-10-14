// Observed: Clear__9VICollide at 0x01096eb0 is 144 bytes. With the pool at offset
// 528 it repeats: first = Begin__Ct6VIPool1ZP12VICollBuffer(pool); stop when
// End__Ct6VIPool1ZP12VICollBuffer(pool) equals first; otherwise call
// ReleaseCollBuffer__9VICollidei(this, first) and return -1 if that is negative.
// Begin is read again after every release. When the pool is empty it returns -1
// if Clear__t6VIPool1ZP12VICollBuffer(pool) is negative, else 0.
// Inferred: Begin/End yield an index/handle (it is passed on as an int).
extern "C" {
int Begin__Ct6VIPool1ZP12VICollBuffer(void* pool);
int End__Ct6VIPool1ZP12VICollBuffer(void* pool);
int ReleaseCollBuffer__9VICollidei(void* object, int buffer);
int Clear__t6VIPool1ZP12VICollBuffer(void* pool);

int Clear__9VICollide(void* object) {
    unsigned char* pool = static_cast<unsigned char*>(object) + 528;
    for (;;) {
        int first = Begin__Ct6VIPool1ZP12VICollBuffer(pool);
        if (first == End__Ct6VIPool1ZP12VICollBuffer(pool)) break;
        if (ReleaseCollBuffer__9VICollidei(object, first) < 0) return -1;
    }
    return Clear__t6VIPool1ZP12VICollBuffer(pool) < 0 ? -1 : 0;
}
}

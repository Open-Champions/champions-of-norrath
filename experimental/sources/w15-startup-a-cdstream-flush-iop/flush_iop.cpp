// Observed: FlushIop__Fi at 0x01000b68 is 120 bytes and returns an int; it is a local function
// of the CD-stream file (the image keeps the name local). The argument is a flag. It calls
// sceSifCheckStatRpc on the RPC client record at 0x011e2580; a nonzero answer returns -1 and
// nothing else happens. Otherwise it calls sceSifCallRpc(client, 0x80f0, flag != 0, send block
// (0x011e25c0), 64, reply block (0x011e2600), 64, 0, 0) and returns 0 whatever that call
// returned.
// Inferred: it sends the request block the CD-stream functions filled to the IOP-side CD server
// as RPC function 0x80f0 and lets it complete into the reply block; the flag selects the SDK's
// non-blocking mode value 1 (async) when nonzero, and 0 (wait) when zero. The callers in this
// package all pass zero.
// Unknown: what the server does with each command word, the SDK end-function semantics of the
// last two zero arguments, and why the return ignores the call's result.
extern "C" {
extern int norrath_cdstream_client[];
extern int norrath_cdstream_send[];
extern int norrath_cdstream_recv[];
int sceSifCheckStatRpc(void* client);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_argument);

int FlushIop__Fi(int async) {
    if (sceSifCheckStatRpc(norrath_cdstream_client) != 0) return -1;
    sceSifCallRpc(norrath_cdstream_client, 0x80f0, async != 0, norrath_cdstream_send, 64,
                  norrath_cdstream_recv, 64, nullptr, nullptr);
    return 0;
}
}

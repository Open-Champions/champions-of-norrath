// Observed sceSifSearchModuleByName at 0x002dcc68, 160 bytes.
// A negative _lf_bind result returns 0xffff0000. Any other _lf_version
// result than 0 returns 0xfffefffc. Otherwise the name is copied by strncpy
// to offset 8 of the local _senddata object at 0x008e1e80, with a count of
// 252, and offset 259 is then set to 0. sceSifCallRpc follows with the local
// client at 0x008e2080, operation 9, mode 0, that 512-byte object as both
// send and receive, send size 512, receive size 4, and no completion
// function. A negative call returns 0xfffeffff. A nonnegative call returns
// the first word of the object. _senddata and the client are .bss, st_size
// 0, and local. 512 bytes is the span from _senddata to the client. 40 bytes
// is the span from that client to _lfversion. IOP module search is unknown.
extern "C" int _lf_bind();
extern "C" int _lf_version();
extern "C" char* strncpy(char* destination, const char* source, unsigned int count);
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument);

static unsigned char cd[40];
static union {
    int word;
    unsigned char bytes[512];
} _senddata;

extern "C" int sceSifSearchModuleByName(char* name) {
    if (_lf_bind() < 0)
        return static_cast<int>(0xffff0000u);
    if (_lf_version() != 0)
        return static_cast<int>(0xfffefffcu);
    strncpy(reinterpret_cast<char*>(_senddata.bytes + 8), name, 252);
    _senddata.bytes[259] = 0;
    const int call = sceSifCallRpc(cd, 9, 0, &_senddata, 512, &_senddata, 4, 0, 0);
    if (call < 0)
        return static_cast<int>(0xfffeffffu);
    return _senddata.word;
}

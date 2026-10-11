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
extern "C" void* memcpy(void* destination, const void* source, int count);
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion,
                             void* completion_argument);

static unsigned char cd[40];
static union {
    struct {
        int first;
        int second;
        unsigned char rest[504];
    } header;
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
    return _senddata.header.first;
}

// Observed sceSifUnloadModule at 0x002dcbd8, 144 bytes. The bind and version
// checks match the name search, including the two failure results. The
// identifier is then stored in the first word of _senddata. sceSifCallRpc
// uses the same client, operation 8, mode 0, that object as send and
// receive, and size 4 for both directions. A negative call returns
// 0xfffeffff. A nonnegative call returns the first word after the call.
extern "C" int sceSifUnloadModule(int identifier) {
    if (_lf_bind() < 0)
        return static_cast<int>(0xffff0000u);
    if (_lf_version() != 0)
        return static_cast<int>(0xfffefffcu);
    _senddata.header.first = identifier;
    const int call = sceSifCallRpc(cd, 8, 0, &_senddata, 4, &_senddata, 4, 0, 0);
    if (call < 0)
        return static_cast<int>(0xfffeffffu);
    return _senddata.header.first;
}

// Observed sceSifStopModule at 0x002dc9d0, 520 bytes. The bind and version
// checks match the name search. The identifier is stored in the first word
// of _senddata. A null argument pointer stores 0 in the second word and
// copies nothing. A length below 253 copies that many bytes to offset 260
// and stores the length. A length of 253 or more copies 252 bytes to the
// same offset and stores 252. The original long copy is inline; these bytes
// are copied through memcpy. sceSifCallRpc then uses operation 7, mode 0,
// send size 512, and receive size 8 on this object. A negative call returns
// 0xfffeffff and does not write the result pointer. A nonnegative call
// reads both words, stores the second through the result pointer, and
// returns the first.
extern "C" int sceSifStopModule(int identifier, int argument_length, const char* argument, int* result) {
    if (_lf_bind() < 0)
        return static_cast<int>(0xffff0000u);
    if (_lf_version() != 0)
        return static_cast<int>(0xfffefffcu);
    _senddata.header.first = identifier;
    if (argument == 0) {
        _senddata.header.second = 0;
    } else if (argument_length < 253) {
        memcpy(_senddata.bytes + 260, argument, argument_length);
        _senddata.header.second = argument_length;
    } else {
        memcpy(_senddata.bytes + 260, argument, 252);
        _senddata.header.second = 252;
    }
    const int call = sceSifCallRpc(cd, 7, 0, &_senddata, 512, &_senddata, 8, 0, 0);
    if (call < 0)
        return static_cast<int>(0xfffeffffu);
    const int returned = _senddata.header.first;
    const int stored = _senddata.header.second;
    *result = stored;
    return returned;
}

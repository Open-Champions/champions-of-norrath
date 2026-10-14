// Observed _sceSifLoadModule at 0x002dcdd8, 548 bytes, local in the original and
// published global because sceSifLoadModule calls it. Arguments are the module
// path, the argument length, the argument bytes, a pointer for the module's
// second reply word, and an operation number.
//
// A negative _lf_bind result is returned as is. A nonzero _lf_version result
// returns 0xfffefffc. Otherwise the request object _senddata (512 bytes, shared
// with the other load-file calls) is filled: strncpy of the path into offset 8
// with a count of 252, offset 259 set to 0, and the argument bytes at offset 260.
// With no argument pointer, offset 260 is set to 0 and word 0 to 0. With an
// argument length of 253 or more, 252 bytes are copied and word 0 is 252.
// Otherwise the given length is copied and stored in word 0. sceSifCallRpc then
// runs on the load-file client with the operation, mode 0, _senddata as the
// 512-byte send and 8-byte receive, no completion function. A negative call
// returns 0xfffeffff. Otherwise word 1 of _senddata is stored through the result
// pointer and word 0 is returned.
extern "C" {
extern unsigned char norrath_loadfile_client[40];
extern unsigned _senddata[128];

int _lf_bind();
int _lf_version();
char* strncpy(char* destination, const char* source, __SIZE_TYPE__ count);
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);
int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                  void* receive, int receive_size, void* completion,
                  void* completion_argument);

int _sceSifLoadModule(const char* path, int argument_length, const void* argument,
                      int* result_word, int operation) {
    const int bound = _lf_bind();
    if (bound < 0) return bound;
    if (_lf_version() != 0) return static_cast<int>(0xfffefffcu);

    unsigned char* request = reinterpret_cast<unsigned char*>(_senddata);
    strncpy(reinterpret_cast<char*>(request + 8), path, 252);
    request[259] = 0;
    if (argument == 0) {
        request[260] = 0;
        _senddata[0] = 0;
    } else if (argument_length >= 253) {
        memcpy(request + 260, argument, 252);
        _senddata[0] = 252;
    } else {
        memcpy(request + 260, argument, static_cast<__SIZE_TYPE__>(argument_length));
        _senddata[0] = static_cast<unsigned>(argument_length);
    }

    if (sceSifCallRpc(norrath_loadfile_client, operation, 0, _senddata, 512, _senddata, 8, 0, 0) < 0)
        return static_cast<int>(0xfffeffffu);
    *result_word = static_cast<int>(_senddata[1]);
    return static_cast<int>(_senddata[0]);
}
}

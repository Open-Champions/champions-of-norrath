// Observed hudNetLevelWarp__FPc at 0x001c75b0, 76 bytes. Builds a message on the
// stack whose first byte is 6 and whose remaining bytes are the NUL-terminated
// copy of the level name, then calls netSendMessage(hudMsgHandle, message,
// strlen(name) + 2). The callee's return is not used. The stack buffer holds at
// most 79 name bytes plus the terminator in the observed frame.
extern "C" {
extern int hudMsgHandle;
char* strcpy(char* destination, const char* source);
unsigned int strlen(const char* text);
int netSendMessage__FiPUci(int handle, unsigned char* data, int size);

void hudNetLevelWarp__FPc(char* name) {
    unsigned char message[80];
    message[0] = 6;
    strcpy(reinterpret_cast<char*>(message + 1), name);
    netSendMessage__FiPUci(hudMsgHandle, message, static_cast<int>(strlen(name)) + 2);
}
}

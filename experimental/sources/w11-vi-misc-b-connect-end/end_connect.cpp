// Observed: EndConnect__12VIWndConnect at 0x010a2bc0 is 48 bytes. With the object as this it
// calls ezNetCtlDownInterface(0), then stores 0 at this+1920 and this+1924. Nothing is returned
// and the result of the call is not used.
// Unknown: what the two cleared words mean.
extern "C" {
int ezNetCtlDownInterface(int value);

void EndConnect__12VIWndConnect(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    ezNetCtlDownInterface(0);
    *reinterpret_cast<unsigned*>(bytes + 1920) = 0;
    *reinterpret_cast<unsigned*>(bytes + 1924) = 0;
}
}

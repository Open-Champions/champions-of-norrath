// Observed lightDisableLocalLights__Fv at 0x0013e808, 16 bytes. It adds ten to the
// signed word interfaceMode (0x00483aac) and stores the sum. There is no clamp, no
// test and no call. Inferred only from the name: that a value above zero is what the
// light push routines treat as "already pushed". Fresh compilation is not an
// original-byte match.
extern "C" {
extern int interfaceMode;

void lightDisableLocalLights__Fv() {
    interfaceMode = interfaceMode + 10;
}
}

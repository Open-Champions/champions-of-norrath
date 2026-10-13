// Observed netIsModem__Fv at 0x00135798, 8 bytes. It returns the signed byte
// isModem (0x00483850) widened to int and touches nothing else.
// Inferred: callers use it as a boolean; netInit sets the byte to 1 when the
// INET_SPEED_UART_driver module is found, otherwise 0.
// Unknown: whether any value other than 0 and 1 is ever stored.
extern "C" {
extern signed char isModem;
}

extern "C" int netIsModem__Fv() {
    return isModem;
}

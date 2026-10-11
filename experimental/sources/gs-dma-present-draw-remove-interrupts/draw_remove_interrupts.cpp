/* Observed drawRemoveInterrupts__Fv at 0x0015fb78, 100 bytes. It removes
 * the handler ids in vif0HID, vif1HID, and gifHID from DMAC channels 0, 1,
 * and 2, and disables those channels. It then removes gsHID (0x00486908)
 * from interrupt cause 0 and disables that cause. No instruction in the
 * image stores gsHID; the file word is zero. This unit defines that word.
 * Fresh compilation is not an original-byte match.
 */
extern "C" int vif0HID;
extern "C" int vif1HID;
extern "C" int gifHID;
extern "C" void RemoveDmacHandler(int channel, int identifier);
extern "C" void DisableDmac(int channel);
extern "C" void RemoveIntcHandler(int cause, int identifier);
extern "C" void DisableIntc(int cause);

extern "C" {
int gsHID;
}

extern "C" void drawRemoveInterrupts__Fv() {
    RemoveDmacHandler(0, vif0HID);
    DisableDmac(0);
    RemoveDmacHandler(1, vif1HID);
    DisableDmac(1);
    RemoveDmacHandler(2, gifHID);
    DisableDmac(2);
    RemoveIntcHandler(0, gsHID);
    DisableIntc(0);
}

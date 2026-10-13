// Observed msg_poke__9Savepointi at 0x0021c328, 108 bytes. Does nothing when MC_Busy__Fv returns
// nonzero or, after it returns zero, DLG_CheckMutex__Fv returns nonzero. Otherwise, in order:
// stores 1 in the word at object+132, calls MC_SaveGameBrowser__Fi with the incoming integer,
// activates the member at +136 with lightActivate__FP5Light, stores 1 in the byte at +196 and
// calls objectDoMyActions__FP10GameObjecti with the object and 1. Nothing is returned. The
// store to +132 precedes the browser call. Unknown: what the word and byte mean. Nothing here
// ran on the EE.
extern "C" {
int MC_Busy__Fv();
int DLG_CheckMutex__Fv();
void MC_SaveGameBrowser__Fi(int argument);
void lightActivate__FP5Light(void* light);
void objectDoMyActions__FP10GameObjecti(void* object, int value);

void msg_poke__9Savepointi(void* self, int value) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (MC_Busy__Fv() != 0) return;
    if (DLG_CheckMutex__Fv() != 0) return;
    *reinterpret_cast<int*>(o + 132) = 1;
    MC_SaveGameBrowser__Fi(value);
    lightActivate__FP5Light(o + 136);
    o[196] = 1;
    objectDoMyActions__FP10GameObjecti(self, 1);
}
}

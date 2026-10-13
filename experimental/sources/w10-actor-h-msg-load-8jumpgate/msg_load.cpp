// Observed msg_load__8JumpGatePc at 0x00284560, 140 bytes.
// Reads two bytes from the buffer into object+77 and object+78 (in that order). When the second
// byte is nonzero (tested as a signed byte), it calls lightActivate__FP5Light on the member at
// object+256, looks up a resource with lumpFindResource__FPcT0 (name pointer object+144 and the
// literal "jumpgate_idle.anm"), and passes the result to animAdd__FP14AnimationStateP15AnimationHeaderi
// together with the state at object+216 and the value 3. In every case it then calls
// objectAddToRunList__FP10GameObject on the object and returns the number of bytes consumed, 2.
// Unknown: what the two bytes mean and what kind of resource the lookup returns.
// Nothing here ran on the EE.
extern "C" {
void lightActivate__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* name, char* second);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int mode);
void objectAddToRunList__FP10GameObject(void* object);
}

extern "C" int msg_load__8JumpGatePc(void* self, char* in) {
    unsigned char* object = static_cast<unsigned char*>(self);
    char* cursor = in;
    object[77] = static_cast<unsigned char>(*cursor++);
    char second = *cursor++;
    object[78] = static_cast<unsigned char>(second);
    if (second != 0) {
        lightActivate__FP5Light(object + 256);
        void* header = lumpFindResource__FPcT0(reinterpret_cast<char*>(object + 144), const_cast<char*>("jumpgate_idle.anm"));
        animAdd__FP14AnimationStateP15AnimationHeaderi(object + 216, header, 3);
    }
    objectAddToRunList__FP10GameObject(self);
    return static_cast<int>(cursor - in);
}

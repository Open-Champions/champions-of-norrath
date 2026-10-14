// Observed: __10VIZoneRoom at 0x0111ef38, 116 bytes. Argument: the room (R). It sets the six floats at R+4..R+12 and R+16..R+24 to 0.0 (the
// image stores each of the six twice), calls the constructor __t7VIArray1Zi on R+60 and then on R+68, stores zero in the word at R+116,
// and ends by tail-calling Clear__10VIZoneRoom(R), whose result it returns. The remaining fields are left as they were.
// Inferred: the two vectors are a minimum and a maximum corner (the repeated stores come from an inlined vector constructor followed by explicit
// assignments); the two arrays are zone data lists; R+116 is the data block that Clear frees (see Allocate__10VIZoneRoom).
// Unknown: field names, the element types of the two arrays and the sizes of R.
extern "C" {
void __t7VIArray1Zi(unsigned char* array);
int Clear__10VIZoneRoom(unsigned char* room);

int __10VIZoneRoom(unsigned char* self) {
    float* first = reinterpret_cast<float*>(self + 4);
    float* second = reinterpret_cast<float*>(self + 16);
    first[0] = 0.0f;
    first[1] = 0.0f;
    first[2] = 0.0f;
    second[0] = 0.0f;
    second[1] = 0.0f;
    second[2] = 0.0f;
    __t7VIArray1Zi(self + 60);
    __t7VIArray1Zi(self + 68);
    *reinterpret_cast<int*>(self + 116) = 0;
    return Clear__10VIZoneRoom(self);
}
}

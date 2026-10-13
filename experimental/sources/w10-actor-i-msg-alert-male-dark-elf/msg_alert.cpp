// Observed msg_alert__11MaleDarkElfifG6Point3P10GameObject at 0x00289788, 180 bytes.
// Arguments: the object, an integer kind, a float (not used), the address of a Point3 (the image
// copies its 12 bytes to its own frame and never reads the copy) and another object. Nothing
// happens unless kind is 2, the word at object+1112 is nonzero, the halfword at object+572 is 3 or
// 5 and the other object is not null. Then iatan2(other.y - object.y, other.x - object.x) is taken
// with the singles at +20 and +16 of both objects; when the result differs from the halfword at
// object+56 by less than 10000 in either direction (an unsigned compare of the difference plus 9999
// below 19999), virtual slot 184 (entry at table+184: adjustment halfword, function at +4) runs on
// the object, adjusted, with the argument 16.
// Inferred: this is a reaction to being alerted while facing the other object. Unknown: the meaning
// of the word at +1112 and of the argument 16. Nothing here ran on the EE.
extern "C" {
int iatan2__Fff(float y, float x);
}

extern "C" void msg_alert__11MaleDarkElfifG6Point3P10GameObject(void* self, int kind, float, const void*, void* other_object) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* other = static_cast<unsigned char*>(other_object);
    if (kind != 2) return;
    if (*reinterpret_cast<unsigned*>(object + 1112) == 0u) return;
    const int state = *reinterpret_cast<short*>(object + 572);
    if (state != 3 && state != 5) return;
    if (other == nullptr) return;
    const int angle = iatan2__Fff(*reinterpret_cast<float*>(other + 20) - *reinterpret_cast<float*>(object + 20),
                                  *reinterpret_cast<float*>(other + 16) - *reinterpret_cast<float*>(object + 16));
    if (static_cast<unsigned>(angle - *reinterpret_cast<short*>(object + 56) + 9999) >= 19999u) return;
    unsigned char* entry = *reinterpret_cast<unsigned char**>(object + 72) + 184;
    reinterpret_cast<void (*)(void*, int)>(*reinterpret_cast<unsigned*>(entry + 4))(object + *reinterpret_cast<short*>(entry), 16);
}

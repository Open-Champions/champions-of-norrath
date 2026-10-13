// Observed __3Webii at 0x0020fac8, 24 bytes, the two-argument constructor of the class named Web.
// Authored callers import it under the name norrath_web_construct. The only other source of this
// constructor, w9-camera-map-web-construct, defines both names and clashes with
// w9-startup-crt-a, which already defines __3Webii. This unit supplies the placeholder name alone,
// with the same body: the first int goes to self+48, the second to self+52, and zero words go to
// self+8, self+28 and self+44. Nothing is returned. What the fields mean is not established.
// Nothing here was executed on the EE.
typedef int __attribute__((may_alias)) AliasedInt;

extern "C" void norrath_web_construct(unsigned char* self, int first, int second) {
    *reinterpret_cast<AliasedInt*>(self + 48) = first;
    *reinterpret_cast<AliasedInt*>(self + 52) = second;
    *reinterpret_cast<AliasedInt*>(self + 8) = 0;
    *reinterpret_cast<AliasedInt*>(self + 28) = 0;
    *reinterpret_cast<AliasedInt*>(self + 44) = 0;
}

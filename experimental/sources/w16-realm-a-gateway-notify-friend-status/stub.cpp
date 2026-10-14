// Observed notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb at 0x01214b48, 8 bytes in the pinned image: a return and its delay slot. The image symbol is weak
// (the shape of an inline default of a RealmGatewayAPI virtual); it does not read its arguments or the object.
// Inferred: the base class gives every "notifyFriendStatus" request/notification a do-nothing default that
// derived classes override. Unknown: which derived classes keep this default.
extern "C" void notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb(void* self, const unsigned short* a, bool b) {
    (void)self;
    (void)a;
    (void)b;
}

// Observed notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21 at 0x01214b40, 8 bytes in the pinned image: a return and its delay slot. The image symbol is weak
// (the shape of an inline default of a RealmGatewayAPI virtual); it does not read its arguments or the object.
// Inferred: the base class gives every "notifyRoomMessage" request/notification a do-nothing default that
// derived classes override. Unknown: which derived classes keep this default.
extern "C" void notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21(void* self, const unsigned short* a, const unsigned short* b, const unsigned short* c) {
    (void)self;
    (void)a;
    (void)b;
    (void)c;
}

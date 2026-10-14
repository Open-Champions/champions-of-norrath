// Observed notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1 at 0x01214bd0, 8 bytes in the pinned image: a return and its delay slot. The image symbol is weak
// (the shape of an inline default of a RealmGatewayAPI virtual); it does not read its arguments or the object.
// Inferred: the base class gives every "notifyRoomBanner" request/notification a do-nothing default that
// derived classes override. Unknown: which derived classes keep this default.
extern "C" void notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1(void* self, const unsigned short* a, const unsigned short* b) {
    (void)self;
    (void)a;
    (void)b;
}

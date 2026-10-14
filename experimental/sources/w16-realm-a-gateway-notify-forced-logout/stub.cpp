// Observed notifyForcedLogout__Q212RealmService15RealmGatewayAPIs at 0x01214bf0, 8 bytes in the pinned image: a return and its delay slot. The image symbol is weak
// (the shape of an inline default of a RealmGatewayAPI virtual); it does not read its arguments or the object.
// Inferred: the base class gives every "notifyForcedLogout" request/notification a do-nothing default that
// derived classes override. Unknown: which derived classes keep this default.
extern "C" void notifyForcedLogout__Q212RealmService15RealmGatewayAPIs(void* self, short a) {
    (void)self;
    (void)a;
}

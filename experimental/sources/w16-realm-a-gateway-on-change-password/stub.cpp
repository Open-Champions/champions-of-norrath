// Observed onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv at 0x01214b18, 8 bytes in the pinned image: a return and its delay slot. The image symbol is weak
// (the shape of an inline default of a RealmGatewayAPI virtual); it does not read its arguments or the object.
// Inferred: the base class gives every "onChangePassword" request/notification a do-nothing default that
// derived classes override. Unknown: which derived classes keep this default.
extern "C" void onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv(void* self, unsigned a, int b, void* c) {
    (void)self;
    (void)a;
    (void)b;
    (void)c;
}

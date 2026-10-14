// Observed onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv at 0x012149f0, 8 bytes in the pinned image: a return and its delay slot. The image symbol is weak
// (the shape of an inline default of a RealmGatewayAPI virtual); it does not read its arguments or the object.
// Inferred: the base class gives every "onConsumeKey" request/notification a do-nothing default that
// derived classes override. Unknown: which derived classes keep this default.
extern "C" void onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv(void* self, unsigned a, int b, const unsigned short* c, void* d) {
    (void)self;
    (void)a;
    (void)b;
    (void)c;
    (void)d;
}

// vt_RealmGatewayAPI: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: _vt$Q212RealmService15RealmGatewayAPI at 0x01214760, size 0x240, weak object (V).
//   - Callers: w15-vi-containers-a-realm-gateway-api-dtor/dtor.cpp and w15-gameloop-a-realm-gateway-api-ctor/ctor.cpp store the table address at object +8.
// Inferred:
//   - 576 bytes, 72 slots of 8 bytes. The slot width is inferred from the sibling tables, not checked.
// Unknown:
//   - Every slot value. The slot names belong to the RealmService owner, not to this unit.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
alignas(4) unsigned char vt_RealmGatewayAPI[0x240] __asm__("_vt$Q212RealmService15RealmGatewayAPI");
}
static_assert(sizeof vt_RealmGatewayAPI == 0x240u, "vt_RealmGatewayAPI extent");

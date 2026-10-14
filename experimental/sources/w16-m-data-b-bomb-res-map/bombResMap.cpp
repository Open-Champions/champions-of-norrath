// bombResMap: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: bombResMap at 0x00438e98, size 0x20, type d (initialised data).
//   - Caller: w12-actor-spell-a-ctor-firebomblet-jr/ctor.cpp passes bombResMap to game_object_construct.
//   - actor-view-tables-views/norrath_actors_a_view_438e98.cpp names the same address as a separate view.
// Inferred:
//   - 32 bytes, the resource-map size the caller implies for its entries.
// Unknown:
//   - The initial bytes. They are zero here because the image bytes are not reproduced.
//   - Which storage survives integration: this unit and the actor view name one address.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
unsigned char bombResMap[32];
}
static_assert(sizeof bombResMap == 32u, "bombResMap extent");

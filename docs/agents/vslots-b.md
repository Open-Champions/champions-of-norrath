# Virtual-slot methods, second half

Package `agents/vslots-b`. The 95 names opened by the data-views merge were sorted in plain byte order. This package is numbers 49 through 95. The partial link of the shared tip (`999b562`, 803 authored inputs) still lists all 95, so none of this half was already defined. Direct `jal` counts are from the pinned `SLUS_205.65` `.text` and `.srs` sections. Pointer counts are absolute words in allocated data. Nothing here has run on the EE or as gameplay.

Work is ordered by how often other code names the symbol, not by the byte-order split. Creature methods sit in dozens of vtables. The single-slot actor methods follow them.

`sources/vslots-b-bytes/bytes.hpp` (`0dc420c3cf8c5ad79a96f08fa379e0eb0357fbcebd7f789e89f1172ce9ece5c5`) is the field helper. It is a header closure, not its own plan input. Host checks define `NORRATH_VSLOTS_B_HOST` and are not plan inputs.

## msg_route__8Creaturei

Observed at `0x113488`, 44 bytes. 53 data pointers, no direct `jal`. The reviewed compile emits 140 bytes. A nonzero argument clears the words at object+708 and object+716. A zero argument clears object+708, stores -1 at object+580 and object+584, and adds one to object+716, wrapping in 32 bits.

## stun__8Creaturei

Observed at `0x112be0`, 80 bytes. 52 data pointers, no direct `jal`. The reviewed compile emits 132 bytes. Signed halfword 10 at object+572 returns without a write. Any other halfword stores object+808 plus the argument at object+804, then calls virtual slot byte 184 with integer 9. That creature slot is `setState__8Creaturei`, loaded through the object's vtable, so this unit does not import it.

## Imports closed

- `msg_route__8Creaturei`
- `stun__8Creaturei`

## Imports opened

None.

## Build-plan entries needed

Profile `authored_cpp`. Do not add `host_check.cpp`. The integrator assigns `input_order`.

```json
{"source":"sources/vslots-b-msg-route/msg_route.cpp","object_name":"msg_route.o","sha256":"3de93b207493a65b0f548bdc3e960c3efceee0af8e80f72e08b0d4de49d2519b","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_route__8Creaturei","compiled_body_bytes":140}]}
{"source":"sources/vslots-b-stun/stun.cpp","object_name":"stun.o","sha256":"15d2d41f26653e5343a7ea235d8b2e2446bd750435f7878dde69fca87853d612","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"stun__8Creaturei","compiled_body_bytes":132}]}
```

## Authored but never executed

`msg_route__8Creaturei` and `stun__8Creaturei`. Host checks ran on this machine. The reviewed EE compiler produced relocatable objects. No EE, original-process, or gameplay run.

## Unknown

The words at object+580, +584, +708, +716, +804, and +808 are not named, and state numbers 9 and 10 are not named. The other 45 symbols in this half are not reconstructed yet. `setState__8Creaturei` is a weak 8-byte store in the image and is still open. Callees that later bodies need stay unresolved until those bodies land.

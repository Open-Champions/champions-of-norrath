# Virtual-slot methods, second half

Package `agents/vslots-b`. The 95 names opened by the data-views merge were sorted in plain byte order. This package is numbers 49 through 95. The partial link of the shared tip (`999b562`, 803 authored inputs) still lists all 95, so none of this half was already defined. Direct `jal` counts are from the pinned `SLUS_205.65` `.text` and `.srs` sections. Pointer counts are absolute words in allocated data. Nothing here has run on the EE or as gameplay.

Work is ordered by how often other code names the symbol, not by the byte-order split. Creature methods sit in dozens of vtables. The single-slot actor methods follow them.

`sources/vslots-b-bytes/bytes.hpp` (`0dc420c3cf8c5ad79a96f08fa379e0eb0357fbcebd7f789e89f1172ce9ece5c5`) is the field helper. It is a header closure, not its own plan input. Host checks define `NORRATH_VSLOTS_B_HOST` and are not plan inputs.

## msg_route__8Creaturei

Observed at `0x113488`, 44 bytes. 53 data pointers, no direct `jal`. The reviewed compile emits 140 bytes. A nonzero argument clears the words at object+708 and object+716. A zero argument clears object+708, stores -1 at object+580 and object+584, and adds one to object+716, wrapping in 32 bits.

## Imports closed

- `msg_route__8Creaturei`

## Imports opened

None.

## Build-plan entries needed

Profile `authored_cpp`. Do not add `host_check.cpp`. The integrator assigns `input_order`.

```json
{"source":"sources/vslots-b-msg-route/msg_route.cpp","object_name":"msg_route.o","sha256":"3de93b207493a65b0f548bdc3e960c3efceee0af8e80f72e08b0d4de49d2519b","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_route__8Creaturei","compiled_body_bytes":140}]}
```

## Authored but never executed

`msg_route__8Creaturei`. The host check ran on this machine. The reviewed EE compiler produced a relocatable object. No EE, original-process, or gameplay run.

## Unknown

The words at object+580, +584, +708, and +716 are not named. The other 46 symbols in this half are not reconstructed yet. `setState__8Creaturei` is a weak 8-byte store in the image and is still open. Callees that later bodies need stay unresolved until those bodies land.

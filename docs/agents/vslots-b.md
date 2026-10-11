# Virtual-slot methods, second half

Package `agents/vslots-b`. The 95 names opened by the data-views merge were sorted in plain byte order. This package is numbers 49 through 95. The partial link of the shared tip (`999b562`, 803 authored inputs) still lists all 95, so none of this half was already defined. Direct `jal` counts are from the pinned `SLUS_205.65` `.text` and `.srs` sections. Pointer counts are absolute words in allocated data. Nothing here has run on the EE or as gameplay.

Work is ordered by how often other code names the symbol, not by the byte-order split. Creature methods sit in dozens of vtables. The single-slot actor methods follow them.

`sources/vslots-b-bytes/bytes.hpp` (`c5fea9b60c5c91b0e84169c526dbf41d4cb6dd4b2206f0daa586a117fc2e5578`) is the field helper. It is a header closure, not its own plan input. Host checks define `NORRATH_VSLOTS_B_HOST` and are not plan inputs.

## msg_route__8Creaturei

Observed at `0x113488`, 44 bytes. 53 data pointers, no direct `jal`. The reviewed compile emits 140 bytes. A nonzero argument clears the words at object+708 and object+716. A zero argument clears object+708, stores -1 at object+580 and object+584, and adds one to object+716, wrapping in 32 bits.

## stun__8Creaturei

Observed at `0x112be0`, 80 bytes. 52 data pointers, no direct `jal`. The reviewed compile emits 132 bytes. Signed halfword 10 at object+572 returns without a write. Any other halfword stores object+808 plus the argument at object+804, then calls virtual slot byte 184 with integer 9. That creature slot is `setState__8Creaturei`, loaded through the object's vtable, so this unit does not import it.

## setActiveChunks__8Creature

Observed at `0x113828`, 104 bytes. 51 data pointers, no direct `jal` of this symbol. The reviewed compile emits 164 bytes. Virtual slot byte 128 (`getModel__8Creature` on the creature vtable) is called once. A null return stops and leaves object+552 unchanged. A non-null return calls that slot again. `modelGetActiveChunks__FP12_modelHeaderi` receives the second return and the word at object+244. The eight-byte return register is stored at object+552. The callee is not defined in this package.

## msg_hurt__8CreaturefP10GameObjecti

Observed at `0x113618`, 240 bytes. 50 data pointers and 14 direct calls. The reviewed compile emits 868 bytes. The reviewed EE body keeps the amount in `f12`, the other object in `a1`, and the integer in `a2`. A nonzero byte at object+570 returns immediately. Otherwise `frameNm` is copied to object+812, the integer is ORed into object+744, and the singles at object+560 and object+192 move by the amount after any list work. A non-null other object and a positive single at object+560 also call `iatan2__Fff`, `norrath_list_destroy`, and maybe `norrath_list_oneway_link`. Those two list names are already defined. `frameNm` is already defined.

## Imports closed

- `msg_route__8Creaturei`
- `stun__8Creaturei`
- `setActiveChunks__8Creature`
- `msg_hurt__8CreaturefP10GameObjecti`

## Imports opened

- `modelGetActiveChunks__FP12_modelHeaderi` — called by `setActiveChunks__8Creature`. Not defined here.
- `iatan2__Fff` — called by `msg_hurt__8CreaturefP10GameObjecti`. Not defined here.

## Build-plan entries needed

Profile `authored_cpp`. Do not add `host_check.cpp`. The integrator assigns `input_order`.

```json
{"source":"sources/vslots-b-msg-route/msg_route.cpp","object_name":"msg_route.o","sha256":"3de93b207493a65b0f548bdc3e960c3efceee0af8e80f72e08b0d4de49d2519b","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_route__8Creaturei","compiled_body_bytes":140}]}
{"source":"sources/vslots-b-stun/stun.cpp","object_name":"stun.o","sha256":"15d2d41f26653e5343a7ea235d8b2e2446bd750435f7878dde69fca87853d612","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"stun__8Creaturei","compiled_body_bytes":132}]}
{"source":"sources/vslots-b-set-active-chunks/set_active_chunks.cpp","object_name":"set_active_chunks.o","sha256":"864b2a370c54e0b2a715bc4f0822f6b96d27f0fa2886480e8c63a519e23f40c4","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setActiveChunks__8Creature","compiled_body_bytes":164}]}
{"source":"sources/vslots-b-msg-hurt/msg_hurt.cpp","object_name":"msg_hurt.o","sha256":"27d1e7b429b1b74e7448ec6fc87716d101a81511de91f0fad440ff1a7a52e2f8","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_hurt__8CreaturefP10GameObjecti","compiled_body_bytes":868}]}
```

## Authored but never executed

`msg_route__8Creaturei`, `stun__8Creaturei`, `setActiveChunks__8Creature`, and `msg_hurt__8CreaturefP10GameObjecti`. Host checks ran on this machine. The reviewed EE compiler produced relocatable objects. No EE, original-process, or gameplay run.

## Unknown

The words at object+244, +552, +580, +584, +708, +716, +744, +804, +808, and +812 are not named, and state numbers 9 and 10 are not named. The upper half of the `modelGetActiveChunks` return register is stored because the caller uses an eight-byte store; what the callee writes there is not established. R5900 NaN and denormal results of the hurt compare and the two single updates are not established. The other 43 symbols in this half are not reconstructed yet. `setState__8Creaturei` is a weak 8-byte store in the image and is still open.

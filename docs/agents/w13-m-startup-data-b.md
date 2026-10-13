# w13-m-startup-data-b: startup, runtime-support and Sony library data words, sprite and spell vtables (part 2 of 2)

Scope: the 30 unresolved imports listed for this package in the wave file. Source
units go under `experimental/sources/w13-m-startup-data-b-*`. Evidence is the
private reference image at the documented location, read only on this machine.
No raw bytes, disassembly or decompiler output is published here; addresses and
symbol names are the observed facts.

## Baseline

Default authored partial link on branch `agents/w13-m-startup-data-b` at the
starting commit `0f1a1ca6`, using the reviewed EE toolchain and a private copy of
the plan (the in-tree plan was not modified):

- `selected_inputs` 5528, `function_count` 4240, `unresolved_symbol_count` 1168,
  `common_symbol_count` 0, `original_files_used` false, `runtime_verified` false.
- All 30 package names are present in `unresolved-imports.json` at that point.

The build output lives under `work/w13-m-startup-data-b-build-0` (ignored by git).

## Observed evidence (private image, not published)

- The image is a little-endian MIPS ELF, not stripped. Storage symbols below
  are LOCAL objects in the image.
- `sSend` (0x011edb00, 512 bytes), `sRecv` (0x011edd00, 512 bytes),
  `sFunctionIndex` (0x011edf00, 8-byte slot), `sIopFunctionData` (0x011edf08, 512
  bytes), `sSifClient` (0x011ee108, 56 bytes to the next symbol at 0x011ee140).
  All five spans are zero in the image bytes.
- A second, different set of LOCAL names (`sSend` at 0x011e25c0, `sRecv` at
  0x011e2600, `sSifClient` at 0x011e2580) is not the object used by these units.
- Other data observed: `rollover_counter` (0x00447f50, 8 bytes), `timer_handle`
  (0x00447f58, 4 bytes), `osdsrc` (0x00444a38, 1960 bytes, content not
  published), `tlbsrc` (0x004459d0, 816 bytes, content not published),
  `sceCbPoff_Gp` (0x008e1c80, zero-size LOCAL label in .bss).
- Vtable spans (size in bytes): `_vt$8Skeleton` 240, `_vt$13FireBombletJr` 144,
  `_vt$5Spell` 48, `_vt$13WeatherFilter` 24, and the VI sprite tables at 96 bytes
  each (`_vt$11VILODSprite`, `_vt$12VINameSprite`, `_vt$12VIPointLight`,
  `_vt$12VISkinSprite`, `_vt$13VIFloraSprite`, `_vt$13VIGroupSprite`,
  `_vt$13VIPointSprite`, `_vt$13VISoundSprite`, `_vt$14VISimpleSprite`,
  `_vt$16VIParticleSprite`, `_vt$19VIStreamAudioSprite`,
  `_vt$20VIEffectVolumeSprite`, `_vt$9VICSprite`, `_vt$9VIHSprite`).
- Table slots are 8-byte pairs: a zero adjustment word and a function pointer.

## Closed

### Commit 1: overlay SIF/IOP command block (`sSend`, `sRecv`, `sFunctionIndex`, `sSifClient`, `sIopFunctionData`)

Unit: `experimental/sources/w13-m-startup-data-b-sif-overlay-buffers/`.

- Defines `sSend`, `sRecv`, `sFunctionIndex`, `sIopFunctionData`, `sSifClient`
  with the observed sizes (512, 512, 8, 512, 56 bytes).
- Defines `norrath_overlay_sSend_11edb00` and `norrath_overlay_sIopFunctionData_11edf08`
  as source-level `alias` attributes of the same storage. These are compiler
  aliases inside one object, not link-time aliases; the build driver does not
  apply `symbol_aliases` for them.
- Checks: EE compile with the reviewed flags; `readelf -sW` shows the five sizes
  and both alias pairs at equal values. Host check
  (`host_check.cpp`, clang++ with `-Werror`): sizes by `static_assert`, zero start
  state, and alias identity on non-Darwin hosts only (Darwin rejects the attribute).
- Import names closed: `sSend`, `sRecv`, `sFunctionIndex`, `sSifClient`,
  `norrath_overlay_sSend_11edb00`, `norrath_overlay_sIopFunctionData_11edf08`.
  The `sFunctionIndex` name is closed by an 8-byte `int[2]`; the consumer declares
  it as `int`, which matches only the first word.
- Inferred: the first word of the slot is the command index used by
  `SetIopFunction__FUii` and `FlushIopCommand__Fb`.
- Unknown: the second word of the slot; the fields of `sSifClient` (its type is an
  external SDK type and is held as bytes); which SIF call consumes `sSend`.


- Timer words `rollover_counter` (8 bytes) and `timer_handle` (4 bytes): unit `w13-m-startup-data-b-ee-timer-words`. Host check passes; the EE object has both as 8- and 4-byte global objects. Signedness of `timer_handle` and the split of the 8-byte counter are not established; they are held as bytes and an int.
- Pointer word `sceCbPoff_Gp`: unit `w13-m-startup-data-b-cb-poff-gp-span`, a 64-byte, 64-aligned span to the next label at 0x008e1cc0. Only the first word is used by known code; the rest are zero and unread.

- Vtable `_vt$13VISoundSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-visound`. Host check passes; the EE object's relocations in the table match the observed slot order (checked with a private script). Its function slots are imports opened by this unit.

- Vtable `_vt$13VIFloraSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-viflora`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$13VIGroupSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-vigroup`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$9VIHSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-vihsprite`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$9VICSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-vicsprite`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$11VILODSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-vilod`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$13VIPointSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-vipoint`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$12VISkinSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-viskin`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$16VIParticleSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-viparticle`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$19VIStreamAudioSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-vistreamaudio`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

- Vtable `_vt$14VISimpleSprite` (96 bytes, 12 slots, 11 named functions): unit `w13-m-startup-data-b-vt-visimple`. Host check passes; the EE object's relocations in the table match the observed slot order. Its function slots are imports opened by this unit.

## Imports opened by this package

None so far.

## Build-plan entries needed

Not yet. The sources above need an entry once the lead merges plans:

- `sources/w13-m-startup-data-b-sif-overlay-buffers/sif_overlay_buffers.cpp`, profile `authored_cpp`.

## Authored but never executed

- The overlay SIF buffer unit: compiled and host-checked; not linked into the image,
  not executed on the EE, no SIF call reached.

## Unknown

- Whether the `sSend`/`sRecv` set and the other LOCAL `sSend` at 0x011e25c0 are
  the same object in other contexts. Only the 0x011edb00 set is defined here.

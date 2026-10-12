# simloop-services

Package work is on `agents/simloop-services`. The branch was merged with
eleven units and no summary note, so this file records the integrator's
selection. All eleven units were selected. Each unit has a host check
beside its source that returns failure on wrong behavior. Eight passed
on this machine. The remaining three (`cdProcess`, `soundMapSound`,
`soundResetStream`) assume 32-bit pointers in their harnesses: the
cd-process test round-trips a pointer through a 32-bit word, the
sound-map test stores 4-byte pointers in record slots, and the
sound-reset-stream test asserts a `memset` size that only holds with
32-bit struct layout. They fail on this 64-bit machine for that reason,
the same documented limitation as the ctors-ant-lantern contracts; the
reviewed EE compilations passed. No game binary or disassembly listing
was committed.

## Observed

`cdDirectoryLookup__FPcPiT1i` at `0x00164f00`, 540 bytes. The fourth
argument is never read. File-local helper `find_row` is recorded
alongside it.

`cdProcess__Fv` at `0x00165238`, 436 bytes. A finished read stores
through `cdResult`, and a completed lump fills the next 40-byte cache
record.

`padInitPadIndexMap__Fv` at `0x00168228`, 44 bytes. Stores bytes 3, 2,
1, 0 at `padIndexMap+3` through `padIndexMap+0`.

`padProcess__Fv` at `0x00167e68`, 708 bytes.

`soundMakeSequenceUnpausable__Fi` at `0x0024f9e0`, 64 bytes. An argument
of -1 returns. Bit `0x8000` must be set.

`soundMapSound__FPci` at `0x0024f608`, 124 bytes. The second argument is
unused. `nmSounds` at `0x0048594c` is the signed count. A
case-insensitive name match returns the word stored beside that name.

`soundResetStream__Fi` at `0x0024af80`, 272 bytes. The selected 452-byte
stream record clears byte 0 and the words at its offsets, then a reset
command is sent to the CD RPC.

`soundSetStreamVolume__Fif` at `0x0024f5b8`, 80 bytes. The float is
clamped to the closed range 0 through 1.

`soundStartDialogStream__FiPci` at `0x0024ef40`, 136 bytes.
`soundFindDialog` receives the name; a null result returns without
starting.

`soundStartMusic__FPci` at `0x0024eed0`, 108 bytes. `soundMusicVolume`
is compared with the word `0x3d4ccccc`.

`soundGetStreamPlayStatus__Fi` at `0x0024adb8`, 248 bytes. Always calls
`sceSifCallRpc` on `gCd`, command 80, mode 0.

## Inferred

The defined names are reconstructions of the observed originals. Call
shapes and hardware behavior beyond the host cases remain unknown.

## Imports closed

The eleven defined names, to the extent each was an unresolved import
on the integration tip.

## Imports opened

Whatever the units reference that the tip does not define; the measured
link delta decides the list.

## Build-plan entries needed

Eleven inputs with fresh `input_order` values, profile `authored_cpp`,
empty `header_closure`, and the measured emitted-function lists. Host
files stay out of the plan. Snapshot `files` records every new source
and host file.

## Authored but never executed

All eleven units above. Eight host checks passed on this machine; the
three 32-bit-assuming contracts are recorded above. The reviewed EE
compiler produced relocatable objects and the partial link succeeded. No
routine has run on the EE, under the original process, or as gameplay.

## Unknown

CD, pad, and sound hardware behavior remain unverified.

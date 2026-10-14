# w14-sound: sound device, wave/XM sound objects and battle music

Branch `agents/w14-sound`. One commit per function; source is under `experimental/sources/w14-sound-*`, each directory holding the
authored source and a `host_check.cpp` (host headers, not a plan input). Addresses and sizes are from the pinned private
`SLUS_205.65`. Nothing here ran on the EE or inside the game. Compiled sizes are those of the reviewed EE `authored_cpp` object; they
are not claims of byte identity (several differ from the image).

## How each function was checked

* Each unit compiles with the reviewed EE g++ and the `authored_cpp` flags, and links in a private partial build (the shared tip plus
  these 20 units; `--check-plan` passed on the private plan).
* Each unit has a native host check (`c++ -std=c++17 -Wall -Wextra -Werror`, exit 0) with stubbed callees; pointer words live in an
  address arena (`NORRATH_HOST_ARENA`) because a 64-bit host cannot hold 32-bit pointer words. That branch exists only for the host
  check; the plan build compiles the plain branch.
* Each unit was also compared with the image in a private paired run (an R5900 interpreter outside git): the original function and
  the freshly compiled authored function run from identical memory snapshots with identical stubbed callees; call logs (arguments,
  including float bits and, where it matters, memory the callee sees), return values where they are defined, and every changed byte
  outside the stack must be equal. Every instruction of the original was reached in every
  paired run except alignment nops and annulled delay slots (the exact counts are in the commit messages). Deliberately wrong variants (changed constants, offsets, branch senses, call order)
  were run per function and fail, with the equivalent mutants named in the commit message.
* Return registers of `void` functions were ignored (`Stop__13VIBattleMusicb`, `StopNow__13VIBattleMusic`); where a function
  returns a value on only one path the paired run caught it (`Play__13VIBattleMusici` and `Process__13VIBattleMusic` return `-1`
  on one path each; this was not visible in the image's other paths).
* Commit-message accuracy: the `FirstChannel`/`NextChannel` messages say "exhaustive single-bit masks"; the run used single bits
  with a stride of three plus random and edge masks, not all 64. The `Process__13VIBattleMusic` message gives 424 bytes; the correct
  compiled size is 432.
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.

## Units

| Function | Image address | Image bytes | Compiled bytes |
|---|---|---|---|
| `Type__C7VISound` | 0x0117b7c0 | 8 | 8 |
| `AsWave__7VISound` | 0x0117b7c8 | 16 | 16 |
| `AsXm__7VISound` | 0x0117b7d8 | 20 | 20 |
| `ChannelCount__C6VIWave` | 0x01138168 | 8 | 8 |
| `SampleRate__C6VIWave` | 0x011381f0 | 8 | 8 |
| `ChannelCount__C4VIXm` | 0x0113a878 | 24 | 24 |
| `FirstChannel__13VISoundDeviceUl` | 0x01126870 | 68 | 64 |
| `NextChannel__13VISoundDeviceUli` | 0x011268b8 | 76 | 72 |
| `NewPlayback__13VISoundDevice` | 0x01126908 | 76 | 68 |
| `BeginAdpcmTransfer__13VISoundDevicePPvUiiPi` | 0x01126f30 | 48 | 48 |
| `BeginXmTransfer__13VISoundDevicePPvUiUiiPi` | 0x01126f60 | 52 | 52 |
| `SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf` | 0x01126690 | 136 | 152 |
| `ReleaseSound__13VISoundDeviceiR12VIDictionary` | 0x011263b8 | 260 | 260 |
| `AllocateChannels__13VISoundDeviceii` | 0x01125bc0 | 412 | 456 |
| `Reset__13VISoundDevice` | 0x011248a8 | 656 | 612 |
| `IdleTransfer__13VISoundDevice` | 0x01125f70 | 480 | 588 |
| `Stop__13VIBattleMusicb` | 0x0106ded8 | 160 | 148 |
| `StopNow__13VIBattleMusic` | 0x0106df78 | 140 | 156 |
| `Play__13VIBattleMusici` | 0x0106d920 | 368 | 456 |
| `Process__13VIBattleMusic` | 0x0106da90 | 436 | 432 |

## Observed, inferred, unknown

The leading comment of each source file is the record. Short form:

* Observed: the sound device keeps a 64-bit in-use channel mask at +0 (bits 0 to 45, so 46 channels), 46 playback records of 56
  bytes at +8, 128 sound pointers at +2584, a live playback list at +3284 and a free list at +3296, a sample-transfer state block at
  +3368..+3420, a two-entry class volume table at +3424, four 256-byte BGM name strips from +3436 and a BGM buffer pointer at
  +4472. A sound object keeps its kind at +8 (0 behind `AsWave`, 1 behind `AsXm`), a reference count at +4, a dictionary link at
  +0 and a virtual table pointer at +12; the table's entry at byte offset 8 is a destructor-shaped member called with flag 3 and the
  entry at offset 48 is `IsCached__6VIWave` / `IsCached__4VIXm` in the two derived tables.
* Observed: the battle music object keeps its scene at +0 (the scene's sound device is at scene+19360), the current track at +4, a
  state at +8 (0, 1, 2 and 3 are used), the fade volume and step at +12 and +16, a "stop immediately" bool at +20, a sound index
  at +24 and a playback id at +28 (-1 for none). A feature-flag bit 0x100 selects between a file-streamed path (`PlayBgm`/`StopBgm`
  with the format `bgm\battle%02d.bgm`) and the sound-object path; `Process` does nothing with that bit set.
* Observed: `IdleTransfer` pumps a sample upload in chunks of at most 64 KiB (a multiple of 64 bytes) through `sceSifSetDma`,
  then calls IOP functions 1 and 7 through the `sIopFunctionData` staging array when a chunk or a whole transfer finishes.
  `Reset` prints ten trace lines (`qaw!`, `qaw!1` to `qaw!9`) and rebuilds the playback free list.
* Inferred: field names and meanings, the unit of the sizes, that the 46 channels map to hardware voices, and that a lower
  priority number is evicted first by `AllocateChannels` (the code evicts records whose number is not greater than the request's).
* Unknown: the object sizes (only the used ranges), what IOP functions 1 and 7 are, `sRecv` word 1, the class enum values, the
  meaning of +3096/+3368/+3432/+4460/+4464/+4468, why track 9 is special in `Play`, and the feature-flag bit.

## Imports closed (20)

`AllocateChannels__13VISoundDeviceii`, `AsWave__7VISound`, `AsXm__7VISound`, `BeginAdpcmTransfer__13VISoundDevicePPvUiiPi`,
`BeginXmTransfer__13VISoundDevicePPvUiUiiPi`, `ChannelCount__C4VIXm`, `ChannelCount__C6VIWave`, `FirstChannel__13VISoundDeviceUl`,
`IdleTransfer__13VISoundDevice`, `NewPlayback__13VISoundDevice`, `NextChannel__13VISoundDeviceUli`, `Play__13VIBattleMusici`,
`Process__13VIBattleMusic`, `ReleaseSound__13VISoundDeviceiR12VIDictionary`, `Reset__13VISoundDevice`, `SampleRate__C6VIWave`,
`SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf`, `StopNow__13VIBattleMusic`, `Stop__13VIBattleMusicb`, `Type__C7VISound`.

All 20 are absent from `unresolved-imports.json` of the private build (shared tip plus these 20 units; `--check-plan` passed;
946 imports remained unresolved in that build).

## Imports opened or still needed by these units

Still unresolved in the private build and referenced by these units: `ChannelCount__13VISoundDeviceUl`,
`FirstPlayback__13VISoundDevice`, `NextPlayback__13VISoundDeviceP10VIPlayback`, `Sound__13VISoundDevicei`,
`Preload__13VIBattleMusici`, `StopAllBgm__13VISoundDevice`, `BgmBufferSize__13VISoundDevice`, `FreeSpuMem__13VISoundDeviceUii`,
`norrath_overlay_sRecv_11edd00` (new data import, word 1 read by `IdleTransfer`, address 0x011edd00; follows the existing
`norrath_overlay_*` naming). Also unresolved but owned elsewhere: `VIGetFeatureFlags__Fv` (w14-startup-sys),
`Init__7VILList`, `AddTail__7VILListP7VILNode`. I did not check which of these other units already reference.
Referenced and already closed: `StopBgm`, `Stop`, `PlayBgm`, `SetVolume`, `Remove__12VIDictionaryR10VIResource`, `rand`, `sprintf`,
`printf`, `__builtin_delete`, `sceSifDmaStat`, `sceSifSetDma`, `FlushCache`, `SetIopFunction__FUii`, `norrath_overlay_sIopFunctionData_11edf08`.

## Build-plan entries needed

`python3 docs/agents/w14-sound-plan-entries.py --toolchain /path/to/ee` prints the 20 JSON plan entries (one per directory,
profile `authored_cpp`, no header closure, object names `w14snd_*.o`); `--private-root DIR` writes a private plan and snapshot
copy that contain them (nothing is written inside the repository). Shared `build-plan.json` and `snapshot.json` are not edited.
(`docs/agents/*.py` is gitignored; the script is committed with `git add -f`.)

## Authored but never executed

Nothing ran on the EE. Everything ran only in host checks and the private R5900 interpreter with stubbed callees. Not covered: real
callee behaviour, hardware DMA/IOP effects (`IdleTransfer`, `Reset`), the real virtual tables (the destructor and `IsCached`
calls went through fake entries), `rand` sequences beyond the tested values, and object lifetimes. `Type__C7VISound` is
dereferenced unguarded in `Reset` as in the image, so a null sound crashes there. The `NORRATH_HOST_ARENA` branches exist only
for host checks.

## Unknown / not done

See the unknown list above. Not attempted: other `VISoundDevice`, `VIWave`, `VIXm` members outside the 20 names (for example
`ChannelCount__13VISoundDeviceUl`, `FirstPlayback`, `NextPlayback`, `Sound`, `StopAllBgm`, `Preload__13VIBattleMusici`).

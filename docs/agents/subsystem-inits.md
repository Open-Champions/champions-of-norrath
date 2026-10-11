# subsystem-inits

Package work is on `agents/subsystem-inits`. New source is under `experimental/sources/subsystem-inits-*`. Host checks sit beside that source and are not EE plan inputs: they include host headers. The reviewed profile is `authored_cpp`. Each unit was compiled with those flags using the reviewed EE g++. The official driver rejects a plan whose inputs differ from `snapshot.json`, so that check was a direct compile kept outside git, not an edit of the shared plan. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

`engineInit__Ff` calls these functions in this order: `padInit__Fv`, `emathInit__Fv`, `listInit__Fv`, `animInit__Fv`, `objectInit__Fv`, `worldInit__Fv`, `P_Init__Fv`, `lightInit__Fv`. It also stores COP0 Count in `eRandState` and clears the first byte of `engineCurrentLevelName`. Addresses are from the pinned SLUS-20565 symbol table. Fresh EE objects are not original-byte matches. No EE startup stage has run.

The default plan on this branch does not reference these names yet, because `engineInit__Ff` is still on `agents/engine-init`. They are the imports that function opens.

## padInit

`padInit__Fv` is 292 bytes at `0x00167538`. It calls `scePadInit(0)` and `sceMtapInit`, then for ports 0 and 1 `scePadPortOpen(port, 0, pad_dma_buf + port * 1280)` and `sceMtapPortOpen(port)`. It then opens slots 1 through 4 at `pad_dma_buf + port * 1280 + slot * 256`. Four `memset` pairs clear `pad + i * 88` (88 bytes) and `stateData + i * 8` (8 bytes). `padInitPadIndexMap__Fv` is last.

`pad_dma_buf` at `0x006e1240` is 2560 bytes. `pad` at `0x0034beb8` is 352 bytes. `stateData` at `0x006e1c40` is 32 bytes. Those objects stay imports.

## emathInit

`emathInit__Fv` is 28 bytes at `0x0015a8e0`. It calls `initNoiseTable__Fv`.

## listInit

`listInit__Fv` is 92 bytes at `0x00194420`. `nodes` at `0x007ad000` runs 121920 bytes, to `oniSuckParticle.1175`: 6096 records of 20. The image counter is 6094 and the walk writes each record's first word as the next record, then stores zero in the last record's first word. `freeNodes` receives `nodes`. Bytes after the first word are not written.

## animInit

`animInit__Fv` is 508 bytes at `0x001629f0`.

`frameStates` at `0x006162c0` is 150 records of 3868. `memset` clears `150 * 3868` bytes. The symbol continues 24 bytes past that, and those bytes are not written. Word +12 of records 0 through 148 stores the next record. Word +16 of records 1 through 148 stores the previous record. Word +16 of record 149 stores record 148 plus `0x40`. `fsList` receives `frameStates`. Record 0's previous word and record 149's next word stay zero.

`animResults` at `0x006a3d40` is 100 records of 1568, fully cleared. Word +1556 of records 0 through 98 stores the next record. Word +1560 of records 1 through 99 stores the previous record. `arList` receives `animResults`. `arListBack` receives the last record.

`identityResult + 4` receives `0x6dc18000`. The halfword at +8 receives 64. From `identityResult + 16`, 64 groups of 12 halfwords store 4096 at indexes 0, 5, and 10. `powf` then runs for exponents -64 through 63 with the `.lit4` bits `0x3f879c7c` from `0x0047ff80`. Results land at `table + 8408`. The image also loads `glistp` and spills it to the stack. That spill has no later read and is not represented.

## objectInit

`objectInit__Fv` is 228 bytes at `0x00140a80`. It calls `engineAddTask__FPFv_viPci` with `runObjectsTask__Fv`, 10, the bytes `runObjects` at `0x00461e40`, and 0, then with `drawObjectsTask__Fv`, 21, the bytes `drawObjects` at `0x00461e30`, and 1. `listDestroy__FP8ListHead` receives a zero word. `objectGrid` and `ghostObjectGrid` are 120 by 120 words and are stored as zero, then `offTheGrid` and `objectStrings`, then the 512 words of `stringHash` and `objectHash`.

## worldInit

`worldInit__Fv` is 64 bytes at `0x0017dc78`. `farClip` is stored with bits `0x44e10000` before `worldSetFrustum__Fffff`. The four arguments are bits `0x42480000`, the `.lit4` word `0x3faa3d70` at `0x004800c4`, `0x42a00000`, and `0x3f800000`.

## P_Init

`P_Init__Fv` is 492 bytes at `0x00180038`. Sixty-four pairs are stored at `wigglyTab` (`0x006ffe60`, 544 bytes). Entry `i` uses angle `i << 10`. The stored words are `isin__Fi` and `icos__Fi` multiplied by bits `0x40a00000`, then converted with `cvt.w.s`. A float-to-int cast emits `cvt.w.s` on the reviewed EE g++. The host cast truncates, so the host fixtures are exact integers. Only the first 512 bytes of `wigglyTab` are written.

`nmSingle`, `nmLines`, and the `P_FrameNum` halfword are stored as zero. `descListMark` (`0x00780280`, 128 bytes) is filled with `0xffffffff`. `allocDescriptor__FP11ParticleDef` receives `invisible`.

`block` at `0x00700080` is 128 pages of 4096 bytes. A record whose index modulo 32 is 0 stores two zero words, then `value | 0x04000000` and `value | 0x6d3c0000`. `value` alternates 576 and 128, starting at 576. A record whose index modulo 32 is 31 stores `0x14000018` and three zero words. Every other record stores a zero halfword at offset 14 and leaves the other bytes alone.

Then `P_Clear__Fv`, `initTrails__Fv`, `initParticleSystems__Fv`, `engineAddTask(P_Update__Fv, 15, "updateParticles", 2)`, and `engineAddTask(drawParticlesTask__Fv, 65, "drawParticles", 1)`. Those names are the bytes at `0x00463e60` and `0x00463e70`.

## lightInit

`lightInit__Fv` is 312 bytes at `0x0013d0f8`. The direction bits `0xbf800000`, `0xbf800000`, and `0x3f800000` are scaled by `rsqrt` of the sum of squares. The reviewed compiler emits `rsqrt.s` for `1.0f / __builtin_sqrtf`. The 12-byte result is stored at `scene` (`0x004f4060`, 56 bytes). Bytes 12 through 35 are not written. The following bytes are 64, 70, 80, 0, 32, 32, 32, 0, 64, 70, 80, 0, 32, 32, 32, 0, and the word 128. Then `transformSceneLightTask__Fv`, `lightResetOptionalParams__Fv`, and `engineAddTask(transformSceneLightTask__Fv, 12, "transformSceneLight", 0)`. The name is the bytes at `0x00461dd0`.

## eRandState and engineCurrentLevelName

`eRandState` at `0x00483e58` is four zero file bytes in `.sdata`. `syncRandState` is the next symbol. `engineCurrentLevelName` at `0x0037f988` is 80 zero file bytes in `.data`. `packetBuffer` begins 88 bytes later; the eight intervening bytes are not this object. The reviewed `-G0` profile places both zero objects in BSS. `engineInit` writes both later; these definitions are the initial bytes.

## Imports closed

`padInit__Fv`, `emathInit__Fv`, `listInit__Fv`, `animInit__Fv`, `objectInit__Fv`, `worldInit__Fv`, `P_Init__Fv`, `lightInit__Fv`, `eRandState`, `engineCurrentLevelName`.

## Imports opened

`allocDescriptor__FP11ParticleDef`, `animResults`, `arList`, `arListBack`, `block`, `descListMark`, `drawObjectsTask__Fv`, `drawParticlesTask__Fv`, `engineAddTask__FPFv_viPci`, `farClip`, `frameStates`, `freeNodes`, `fsList`, `ghostObjectGrid`, `icos__Fi`, `identityResult`, `initNoiseTable__Fv`, `initParticleSystems__Fv`, `initTrails__Fv`, `invisible`, `isin__Fi`, `lightResetOptionalParams__Fv`, `listDestroy__FP8ListHead`, `memset`, `nmLines`, `nmSingle`, `nodes`, `objectGrid`, `objectHash`, `objectStrings`, `offTheGrid`, `P_Clear__Fv`, `P_FrameNum`, `P_Update__Fv`, `pad`, `pad_dma_buf`, `padInitPadIndexMap__Fv`, `powf`, `runObjectsTask__Fv`, `sceMtapInit`, `sceMtapPortOpen`, `scePadInit`, `scePadPortOpen`, `scene`, `stateData`, `stringHash`, `table`, `transformSceneLightTask__Fv`, `wigglyTab`, `worldSetFrustum__Fffff`.

`memset` is already defined by the runtime-memory unit. `engineAddTask__FPFv_viPci` stays with the scheduler package. `objectGrid`, `ghostObjectGrid`, `stringHash`, `objectHash`, `offTheGrid`, `objectStrings`, and `freeNodes` are the original names of objects the object-infra package published under different view names. This package does not define a second copy.

## Build-plan entries needed

Append these authored inputs. Empty `header_closure`. Profile `authored_cpp`. Kind `independently_authored_source`. The integrator assigns `input_order`. Host checks are not inputs.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "pad_init.o", "profile": "authored_cpp", "sha256": "48b164d7641bad0f594e966e442332ae3f174034ff7e05934c7ee78cc658b129", "source": "sources/subsystem-inits-pad/pad_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "emath_init.o", "profile": "authored_cpp", "sha256": "8cb8b45e08401a6e0c307993377d6918832d137c06c6d25066910b53326e2350", "source": "sources/subsystem-inits-emath/emath_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "list_init.o", "profile": "authored_cpp", "sha256": "88587f80976a97c6abfc31ab337774f02ac5bfa72c20dab972b687c06ef0b258", "source": "sources/subsystem-inits-list/list_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "anim_init.o", "profile": "authored_cpp", "sha256": "584dc88ca8accfdec1887ce7df6ca06ceb86e7a0c8c35bd7da9758ce348e040b", "source": "sources/subsystem-inits-anim/anim_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "object_init.o", "profile": "authored_cpp", "sha256": "34d41d4cbd262ddec506e962ee14d143ec9026cc6383b1fcbed7112f6a483bea", "source": "sources/subsystem-inits-object/object_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "world_init.o", "profile": "authored_cpp", "sha256": "a19a9741c07e4debe9cd680e8e35bb5e06db3bdfa79a8c7b3d56ecc752265d23", "source": "sources/subsystem-inits-world/world_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "p_init.o", "profile": "authored_cpp", "sha256": "a326f797a2191806d5fea476655f1276730922332dd925aed0d904b90ac230ee", "source": "sources/subsystem-inits-particles/p_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "light_init.o", "profile": "authored_cpp", "sha256": "2a2f4075ace17ed394323cc0646eab73122c6ea17fbf1d7e7a714bc248e090b7", "source": "sources/subsystem-inits-light/light_init.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "e_rand_state.o", "profile": "authored_cpp", "sha256": "4566ec0f6220273ec17eee51791cb3bb411b566d64703c50c69426b42c91c87c", "source": "sources/subsystem-inits-rand-state/e_rand_state.cpp"}
```

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "level_name.o", "profile": "authored_cpp", "sha256": "0e6d2ee336b7246087cd8bd29ee2c1d87804cb69d653cca199c1550ce4678e5c", "source": "sources/subsystem-inits-level-name/level_name.cpp"}
```

## Authored but never executed

All ten units were compiled with the reviewed EE toolchain and checked on the host. None of the functions, and no pad, GS, DMA, particle, or frustum call they make, has run on the EE. No startup stage has run.

## Unknown

The unread stack spill of `glistp` has no effect after `animInit` returns. Why record 149's frame back-pointer is record 148 plus `0x40`, rather than the record base used by the earlier records, is unknown. The constants `0x6dc18000`, `0x14000018`, `0x04000000`, and `0x6d3c0000` are the stored bits; what the GS or GIF does with them is unknown. Whether `worldSetFrustum__Fffff` reads `farClip` is unknown. The host float-to-int cast truncates, while the EE cast is `cvt.w.s`; halfway values were not sampled. Bytes in a particle record other than the written halfword stay at their previous value. In the image those records are BSS, so the previous value is zero. `vblank` timing, descriptor allocation, and the saved task-name pointers are not executed here.

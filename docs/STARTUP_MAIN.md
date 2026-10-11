# main

`main` at 0x002d0818 is 1140 bytes and does not return. Fresh compilation of this reconstruction is 1284 bytes and is not an original-byte match. The host contract covers the call order, the two argument scans, the intro branch, the pad-word branch, and both loop exits. The default partial link is 993 unresolved imports, zero COMMON, and 421 function bodies. Defining `main` removes that one import and adds 83. No EE startup stage has run.

`engineAddTask__FPFv_viPci` is called and left undefined. It is not stubbed.

The first float argument of `engineInit__Ff` is the `.lit4` word `0x3f7fffff`. It is not a field of `__EXCEPTION_TABLE__`.

`_engineCheckStack__FPCci` is called with 2648 and later 2950. Both calls pass the address 0x004866a0. That address is a 7-byte NUL-terminated text sequence in `.sdata`. It is not inside `coreLoadingStart`, whose recorded size is 4 bytes at 0x00486698. The next symbol is `skipIntro` at 0x004866c0.

`levelName` is 322 bytes. The function stores zero at offsets 0, 81, and 242, then copies 9 bytes from 0x0047b058 over the start, then stores `0xFF` at offset 80. Those three early stores are visible to `cutInitNet__Fv`. The 9-byte source is an 8-byte NUL-terminated sequence plus the following byte.

The word at `pad + 8` is compared with 16398. `engineDisplayDisable` is set to 5 on both sides of that comparison. `worldWaterBumpSpeed` is set to 1 only when the word matches. What 16398 selects is unknown.

`skipIntro` gates the second `engineSetScreenColor__Fiiii(-128, -128, -128, 0)` and `simpleDrawLoop__FPFv_i(displaySBLogoScreen__Fv)`. The first draw is `displaySOELogo__Fv`. `mcProcess__Fv` is always passed afterward. `simpleDrawLoop__FPFv_i` itself calls the pointer with `jalr` and a nop delay slot, then tests the return register. What integer, if any, reaches that callback is unknown. These three symbols are `Fv`.

The two `sceSif` results are shifted right with `srl` by 10 and passed to `scePrintf`. The host contract checks a negative result so the shift stays logical.

Argument scans start at `argv[1]` and stop at the first `strcmp` hit. The first needle is 0x004867d8. A non-null hit stores 1 in `netNetworkMode`; a null hit does not. The second needle is 0x0047b138, inside the loop. A non-null hit calls `frontEndInit__Fi(0)` before the `netNetworkMode` test. When that byte is zero, `frontEndInit__Fi(0)` runs again. `fromSRSButEnteringGame` is then cleared, or, if it was already zero and `fromSRS` is nonzero, `frontEndStartInNetMenu__Fv` runs and `fromSRS` is cleared.

Each loop calls `gameInit__Fv`, `lumpClear__Fi(5)`, and `gameLoop__Fv`. A nonzero `netInitState` calls `netFree__Fb(0)`. A zero `coreLaunchSRS` repeats the loop. Otherwise `launchSRS__Fv` runs and the loop repeats. `main` does not reach a return.

These addresses have no ELF symbol. `USEnglishKbdMapTable` ends at 0x0047aa34 and does not contain them. Each one is a separate unresolved object. The original bytes are not in this source.

| Address | Use | Text length before the NUL |
| --- | --- | --- |
| 0x0047afe8 | second halo resource | 9 |
| 0x0047aff8 | third halo resource | 9 |
| 0x0047b008 | fourth halo resource | 9 |
| 0x0047b018 | fifth halo resource | 12 |
| 0x0047b028 | sixth halo resource | 9 |
| 0x0047b038 | seventh halo resource | 9 |
| 0x0047b048 | first `scePrintf` text | 15 |
| 0x0047b058 | 9-byte `levelName` source | 8 |
| 0x0047b068 | `engineAddTask` text | 8 |
| 0x0047b078 | `lumpLoad` | 9 |
| 0x0047b088 | `lumpLoad` and font lump | 8 |
| 0x0047b098 | `defaultFont` resource | 13 |
| 0x0047b0a8 | `mediumFont` resource | 15 |
| 0x0047b0b8 | `smallFont` resource | 17 |
| 0x0047b0d0 | `lumpLoad` | 9 |
| 0x0047b0e0 | `lumpLoad` | 12 |
| 0x0047b0f0 | first halo lump | 10 |
| 0x0047b100 | first halo resource | 9 |
| 0x0047b110 | eighth halo resource | 9 |
| 0x0047b120 | second `scePrintf` text | 20 |
| 0x0047b138 | second `strcmp` needle | 10 |
| 0x004866a0 | stack-check site | 7 |
| 0x004867d0 | `lumpLoad` and later halo lump | 6 |
| 0x004867d8 | first `strcmp` needle | 5 |

`coreBeenInSRS` is one byte at 0x004867af. 0x004867d0 and 0x004867d8 are past that byte. Font and halo results are stored in `defaultFont`, `mediumFont`, `smallFont`, and `haloTexture1` through `haloTexture8`. `lumpLock__Fi` is called with 100 and then 50.

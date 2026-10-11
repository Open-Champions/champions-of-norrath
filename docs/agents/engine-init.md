# engine-init

`engineInit__Ff` at 0x001902b0 is 1944 bytes. `_engineCheckStack__FPCci` at 0x00191310 is 144 bytes. Both were read from the pinned SLUS-20565 image. Fresh EE compilation is not an original-byte match. The host contracts below failed when the call order, the 0xA5 ranges, or the GS and DMA values were wrong. No EE startup stage has run.

`main` calls `engineInit__Ff` with the `.lit4` word `0x3f7fffff`, then calls the stack check with the separate `core.cc` text. `engineInit` itself calls the stack check with `engine.cc` and 1075. The stack check is also called from many other sites; those sites are not in this package. `floorInit__Fv` and `vblankHandler__Fi` stay imports. `vblankHandler__Fi` is the 512-byte function at 0x0018e4a0 passed to `AddIntcHandler`.

## Stack canary

The check reads 5120 bytes at 0x01fc0000, the address of `_stack`. Each byte is compared with 0xA5. A mismatch calls `scePrintf` with `stack error!  %s:%d\n` (two spaces) and the two arguments, then stores zero at address 0. That text has no ELF symbol. The store is not a branch. The scan continues if the store returns. On the EE that store faults. A C store of address 0 is removed under the reviewed flags, so the EE build emits the observed `sw` from assembly and falls through.

`engineInit` prints `stack size=%d\n`, then fills 0xA5 from the stack base up through `top - 5120`. For this image the base is below 0x01ffffff, so `top` is 0x02000000 and the size is 0x40000. The fill runs downward from 0x01ffec00 through 0x01fc0000. The other arm in the image uses top 0x08000000. The same test selects the heap top: 0x02000000 on the taken arm and 0x05a00000 on the other arm. Naming those tops as the 32MB and a larger EE memory limit is inferred from the public memory map.

The heap size is `(heap_top - _end) + 0xfffb1000`, with `_end` at 0x008ec91c. `__wrap_malloc` receives that size minus 4096. The following 1024 bytes, at `size - 5120` from the returned pointer, are filled with 0xA5 through `memset`. `memAllocPtr` and `memAllocSize` are stored before `memset`. `memInit__FPvi` receives the block and `size - 5120`.

## GS, DMA, and the rest of engineInit

COP0 Count is stored in `eRandState`. The first semaphore word at offset 0 is not written. Offset 4 is `0x7ffffc17` and offset 8 is 0, for two `CreateSema` calls. The results go to `vblankSemaphore` and `vblankSemaphore2`. The thread from `GetThreadId` is passed to `ChangeThreadPriority` with 10.

These stores happen before `sceGsResetGraph(0, 1, 2, 1)`: `displayBuffer2StartAddress = 0x002bc000`, `engineEnvTexAddress = 0x00348000`, `engineShadowTexAddress = 0x00358000`, `displayBuffer1StartAddress = 0x00230000`, `textureBufferStartAddress = 0x00359000`, and `engineDrawBufferStartAddress = 0`. `AddIntcHandler(2, vblankHandler__Fi, 0)` is saved in `vblankHID` before `EnableIntc(2)`.

Each vblank wait reloads `vblankCount` twice, calls `WaitSema(vblankSemaphore2)` while the count is unchanged, then loads `vblankField` and discards it. There are two waits around the second store of 0 to 0x12000000, three after `sceGsPutDispEnv`, and one after the first byte of `engineCurrentLevelName` is cleared.

`textureInitTexture__FP8_texture` runs on `ntsc_legallineTex` and `ntsc_legallinebottomTex`. The halfword at `(packet | 0x20000000) + 20` is the reloaded `displayBuffer1StartAddress`, or that word plus 255 when it is negative, shifted right by 8. The second texture adds `0x0008c000`, or `0x0008c0ff` when that sum is negative, then shifts by 8. `sceDmaSendN` gets the DMA channel, the packet address, and the halfword at texture offset 6. `sync` runs before each send. `sceGsSyncPath(0, 0)` follows the first two sends.

0x12000090 is set to 5120, then `sceGsSetDefDispEnv(dispEnv, 0, 640, 224, 0, 0)` and `sceGsPutDispEnv(dispEnv)`. After the three waits the 64-bit stores are: 0x12000000 = 2, 0x12000020 = 1, 0x12000070 = `(displayBuffer1StartAddress >> 13) | 0x1400 | (0x8000 << 28)`, 0x12000090 = that frame word without the high bit, and 0x12000080 and 0x120000a0 = `0x1be9ff0183227c`. Identifying these as GS PMODE, SMODE2, DISPFB, and DISPLAY is inferred from the public GS map.

`engineDisplayDisable` is cleared, then `sprintf(buf, "a%d%f\n", 1, bits 0x3fb9999980000000)`, `sscanf(".1", "%lf", ...)`, and `strtod("1.0004", 0)` run. Their results are not read again in this function. Those format strings have no ELF symbols. They are not the `madePacket.769` word.

Later, `sceDmaReset(1)` is followed by `sceDmaGetChan(2)`, `sceDmaGetChan(0)`, and `sceDmaGetChan(1)`. Bit 0x40 is cleared on the first channel word and set on the other two. The pointers are `dmaGif`, `dmaVif0`, and `dmaVif1`. `FlushCache(0)` runs after the VIF0 store and again after the VIF1 store. The same names also exist at earlier BSS addresses; the GP offsets in this function are 0x00486a3c, 0x00486a40, 0x00486a44, and `textureBufferStartAddress` at 0x00486a34.

256 is stored at 0x1000e020 and that word is loaded again. `initVIF__Fv` takes no arguments and overwrites `a0`, so this function does not pass the loaded word in. 130, 0, and 159 are stored at 0x10000010, 0x10000000, and 0x10000810 before `emathInit__Fv`. Naming 0x1000e020 as D_PCR and the 0x10000xxx words as timer registers is inferred from the public EE map.

`textureInit__Fii` receives `textureBufferStartAddress` and `0x00400000 - textureBufferStartAddress`. `soundInit__Ff` receives the float argument. The lump names are `engine.lmp`, `font.lmp`, `gamefonta.fnt`, `shadowblob.tex`, and `halo1.tex`. They are separate copies from the startup-text objects that hold the same bytes at other addresses. `lumpLock__Fi` is called with 100000 after `defaultFont` is stored. The shadow resource is sent on `dmaGif` with `engineShadowTexAddress >> 8` in the same uncached halfword slot. `haloTexture1` is stored before `P_Init__Fv`. `sceDmaSync(dmaGif, 0, 0)` runs after the last vblank wait. The IOP lines are `preCD`, `postCD`, `preSound`, `postSound`, and `postInit`, each `IOP %dk %dk\n`, with both sizes shifted right by 10 using a logical shift.

```sh
c++ -std=c++17 -O2 -fsanitize=address,undefined -Wall -Wextra -Werror \
  tools/checks/engine_check_stack_contract.cpp -o /tmp/engine-check-stack
/tmp/engine-check-stack
c++ -std=c++17 -O2 -fsanitize=address,undefined -Wall -Wextra -Werror \
  tools/checks/engine_init_contract.cpp -o /tmp/engine-init-check
/tmp/engine-init-check
```

The second contract also runs the arm where the stack base is above 0x01ffffff, with a smaller heap so the 0x08000000 and 0x05a00000 arithmetic is executed. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

## Imports closed

`_engineCheckStack__FPCci`, `engineInit__Ff`.

## Imports opened

`__wrap_malloc`, `_end`, `AddIntcHandler`, `animInit__Fv`, `blockAllocInit__Fv`, `cdInit__Fv`, `ChangeThreadPriority`, `CreateSema`, `defaultFont`, `dispEnv`, `displayBuffer1StartAddress`, `displayBuffer2StartAddress`, `dmaGif`, `dmaVif0`, `dmaVif1`, `drawInit__Fv`, `emathInit__Fv`, `EnableIntc`, `engineCurrentLevelName`, `engineDisplayDisable`, `engineDrawBufferStartAddress`, `engineEnvTexAddress`, `engineShadowTexAddress`, `eRandState`, `floorInit__Fv`, `FlushCache`, `GetThreadId`, `haloTexture1`, `initSif__Fv`, `initSupersample__Fv`, `initVIF__Fv`, `lightInit__Fv`, `listInit__Fv`, `lumpFindResource__FPcT0`, `lumpLoad__FPc`, `lumpLock__Fi`, `memAllocPtr`, `memAllocSize`, `memInit__FPvi`, `memset`, `ntsc_legallinebottomTex`, `ntsc_legallineTex`, `objectInit__Fv`, `P_Init__Fv`, `padInit__Fv`, `sceDmaGetChan`, `sceDmaReset`, `sceDmaSendN`, `sceDmaSync`, `sceGsPutDispEnv`, `sceGsResetGraph`, `sceGsResetPath`, `sceGsSetDefDispEnv`, `sceGsSyncPath`, `scePrintf`, `sceSifQueryMaxFreeMemSize`, `sceSifQueryTotalFreeMemSize`, `scriptInit__Fv`, `soundInit__Ff`, `sprintf`, `sscanf`, `strtod`, `textureAllocateCacheBuffer__Fv`, `textureBufferStartAddress`, `textureInit__Fii`, `textureInitTexture__FP8_texture`, `vblankCount`, `vblankField`, `vblankHandler__Fi`, `vblankHID`, `vblankSemaphore`, `vblankSemaphore2`, `WaitSema`, `worldInit__Fv`.

`_engineCheckStack__FPCci` is an import of `engineInit` and is defined by the other unit in this package. `scePrintf` is the checker's only import.

## Build-plan entries needed

```json
{
  "header_closure": [
    "sources/engine-init-startup/ee_mmio.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "check_stack.o",
  "profile": "authored_cpp",
  "sha256": "36f031fd80a9272f72f84b5e926d4aee09b244111b8798d70d652a582bee7952",
  "source": "sources/engine-init-startup/check_stack.cpp"
}
```

```json
{
  "header_closure": [
    "sources/engine-init-startup/ee_mmio.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "engine_init.o",
  "profile": "authored_cpp",
  "sha256": "bcf13dfb628b041a65090342be9aef9db4e0c4a9ea2d11dc599b56394da66188",
  "source": "sources/engine-init-startup/engine_init.cpp"
}
```

`ee_mmio.hpp` sha256 is `beee209ffaeef7a3b5f8f71379a817e9eb5657c72792b27e3a53aad0ad21d4ae`. The integrator assigns `input_order`. No shared plan file was edited.

## Authored but never executed

Both functions were compiled with the reviewed EE toolchain and checked on the host. Neither function, and no GS, DMA, semaphore, interrupt, or IOP call they make, has run on the EE. No startup stage has run.

## Unknown

The unread semaphore word at offset 0, and any words past offset 8, are not written. What `0x7ffffc17` selects is unknown. The negative halfword arms are in the image and run only when the reloaded buffer word or the second sum is negative; this image stores a positive word, and whether a callee changes it is unknown. The discarded `vblankField` loads have no use in this function. `sprintf`, `sscanf`, and `strtod` run and their results are not read here; why they are present is unknown. The reload of 0x1000e020 is not an argument of `initVIF__Fv`. Hardware effects of the GS, DMA, and timer stores are unknown. Which of the duplicate `dmaGif`, `dmaVif0`, `dmaVif1`, and `textureBufferStartAddress` symbols a final link binds is not settled by using the names. `vblankHandler__Fi` was not reconstructed.

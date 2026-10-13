# VI front-end framework classes, KbdMapTable to VIPad

Package `w10-vi-a`, branch `agents/w10-vi-a`. Addresses and bytes were read from
the private pinned image `work/private/SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`) with the
reviewed EE binutils. No game bytes, listings or decompiler output are
committed. Each source file opens with an `Observed` comment (address, size,
behavior); `Inferred` and `Unknown` lines are kept separate there.

Every unit is its own commit. Each compiles with the plan's `authored_cpp`
profile (reviewed EE g++ 15.2.0) and has a `host_contract.cpp` that returns
nonzero when a checked call, argument, stored value or return value is wrong.
Host contracts are not plan inputs. Run one with

```sh
c++ -arch x86_64 -std=c++17 -Wl,-pagezero_size,0x10000 -Wall -Wextra -Werror \
  -o /tmp/check experimental/sources/w10-vi-a-<unit>/host_contract.cpp && /tmp/check
```

The `-arch x86_64 -pagezero_size` flags matter only to `fader-update`, whose
fake raster is mapped below 4 GiB because the object stores a 32-bit pointer
word; the others run on any host. All 33 objects link together with
`ld -r` (elf32lr5900) without duplicate definitions.

Compiled bodies are not the original sizes and are not byte matches. These are
behavior reconstructions. Nothing here has run on the EE or as gameplay. The
three vector-store pairs in the `VICollide` constructor and `VICamera::Init`
zeroing are written as plain float stores.

## Functions

| source | symbol | image | compiled bytes | sha256 |
|---|---|---|---|---|
| `w10-vi-a-camera-call/call.cpp` | `__cl__8VICameraR7VIVect3T1` | `0x010871b0`, 172 bytes | 132 | `1a50524e0b4a1d9b98c91116a27182d0f04ef263a5b1cb8a7825ddea346bbf68` |
| `w10-vi-a-camera-init/init.cpp` | `Init__8VICameraR8VIRasterRC7VIVect3T2ffff` | `0x010858d8`, 444 bytes | 360 | `d860d1b104fcff8bf660aecae6e434dfb29da13b74cec5c391767bca6ba90dc7` |
| `w10-vi-a-collide-close/close.cpp` | `Close__9VICollide` | `0x01096e30`, 44 bytes | 32 | `0663aedc570951d6691a7a395024fbf95d6c01d873f43581e83c90f9f3559aa8` |
| `w10-vi-a-collide-ctor/ctor.cpp` | `__9VICollide` | `0x01092608`, 212 bytes | 80 | `1db43c304783e5dcfa906ad31caf86f528e8292c720e04554799cbbd7eec6a7d` |
| `w10-vi-a-collide-dtor/dtor.cpp` | `_$_9VICollide` | `0x01096d80`, 116 bytes | 120 | `9f70ea6c886b75176877b18c6ede9eaf63ad130df85a1d8d1b143b9ff3cb53ce` |
| `w10-vi-a-collide-init/init.cpp` | `Init__9VICollide` | `0x01096df8`, 52 bytes | 52 | `04803d174ce252f4d4847d8fd02ff1f23fbd0279692d2615fbf5d4dc31ee641b` |
| `w10-vi-a-csffile-ctor/ctor.cpp` | `__9VICSFFile` | `0x010a4be0`, 12 bytes | 12 | `a251c3239f7881b96dae1e7bffa7702c17301173ecf111891392b2dd04e8856a` |
| `w10-vi-a-csffile-parse/parse.cpp` | `Parse__9VICSFFilePCcR10VIESFParseR7VIScene` | `0x010a4bf0`, 192 bytes | 184 | `47f0bf24558ce6d1338a618f10e02de4efcc196077ae794b78c4bfcba3c8dfe7` |
| `w10-vi-a-dict-clear/clear.cpp` | `Clear__12VIDictionary` | `0x010bf288`, 20 bytes | 8 | `c6576c27711c2f1d0556c5696c8345f960187a0075abdd234091557e8c518d65` |
| `w10-vi-a-dict-ctor/ctor.cpp` | `__12VIDictionary` | `0x010bf1c8`, 20 bytes | 8 | `384a3cc1c1169d81117b8d3a014244ba1c194ae258852e50e265a1eeba88a906` |
| `w10-vi-a-dict-dtor/dtor.cpp` | `_$_12VIDictionary` | `0x010bf1e0`, 92 bytes | 96 | `5de5831bb0b0a3c244718d6d56996d3156216d44d7ee0e39d014290cf71d2b18` |
| `w10-vi-a-dict-init/init.cpp` | `Init__12VIDictionaryi` | `0x010bf240`, 44 bytes | 32 | `00362e3b852a17fcb6ca38b580788cfe8b983fc9aaff41b0c5c1ebb0eb57fed8` |
| `w10-vi-a-esfparse-ctor/ctor.cpp` | `__10VIESFParse` | `0x0117aa28`, 88 bytes | 88 | `9db047d44a8ea2185cdb8508c3b770875602317882345c04f2759158d0d39dc9` |
| `w10-vi-a-esfparse-dtor/dtor.cpp` | `_$_10VIESFParse` | `0x0117aa80`, 44 bytes | 32 | `e44d87e1aeb29fcfe60536893525b144f2a6cc565e084d5bff14e6c6daa4ece4` |
| `w10-vi-a-fader-begin/begin.cpp` | `Begin__7VIFaderff` | `0x0117cd68`, 60 bytes | 64 | `cf86875cee06e24c49a480d956fe13b71498616c524881e7f962f2ac7cd85aef` |
| `w10-vi-a-fader-ctor/ctor.cpp` | `__7VIFader` | `0x0117cc20`, 12 bytes | 12 | `4167ecc1c8187ce959f2c6063e59c8d13f651424ea6e01b71af248aa8a38dfac` |
| `w10-vi-a-fader-dtor/dtor.cpp` | `_$_7VIFader` | `0x0117cc30`, 84 bytes | 76 | `8996e87f8b66c04afc8eb2f16b54e4b7070aeabcb58caefbc21dbb098014f02b` |
| `w10-vi-a-fader-free/free.cpp` | `Free__7VIFader` | `0x0117cd18`, 76 bytes | 96 | `1cd6fb6ee3c7de6ed7cfdab2aa7def8d6b80a0459ff39cb81cda2f490eac14e6` |
| `w10-vi-a-fader-init/init.cpp` | `Init__7VIFaderP8VIRasterP12VIDictionaryf` | `0x0117cc88`, 140 bytes | 136 | `be557bc62ee188ebc116a0bf9b8f59e2c243b3fc1ea51c63a62573255373a408` |
| `w10-vi-a-fader-update/update.cpp` | `Update__7VIFader` | `0x0117caa8`, 372 bytes | 360 | `e58b7a40d89482f801a8699d8d9fb7d18e37d3610ae66549f9d87fb1deeff726` |
| `w10-vi-a-font-ctor/ctor.cpp` | `__6VIFont` | `0x01012130`, 28 bytes | 28 | `fb5239d8fe6796b6687acee140307760be735802d86d4260c6398406657b7e61` |
| `w10-vi-a-gameoptions-load/load.cpp` | `Load__13VIGameOptions` | `0x01019990`, 168 bytes | 176 | `95b74efff5edc584ea28049e829537d459f3619e37ec59b96d59ac715b9fe4a1` |
| `w10-vi-a-guiobject-cleanup/cleanup.cpp` | `CleanUp__11VIGUIObject` | `0x01031918`, 16 bytes | 16 | `1b9a82a61f56308535d55521c94904bccf9d08c39f7879aebe4213b7af293fb0` |
| `w10-vi-a-kbd-map-table/kbd_map_table.cpp` | `KbdMapTable` | `0x00447c60`, 750 bytes | 750 | `9a5b0f01af9f0eb8582e3ba15cf39abb504b88751abf775ad8a2fb6bf450682b` |
| `w10-vi-a-keyboard-ctor/ctor.cpp` | `__10VIKeyboard` | `0x002cbef0`, 12 bytes | 12 | `a987fa0319e5b144a062cc2152236c8f36b586317350149502f898e2e2568db5` |
| `w10-vi-a-keyboard-dtor/dtor.cpp` | `_$_10VIKeyboard` | `0x002cbf00`, 44 bytes | 32 | `fe54c83d7f5c5cd28d07d5af1ded829ca9245c1d56d67bd485bad447734fb5a7` |
| `w10-vi-a-keyboard-read/read.cpp` | `Read__10VIKeyboardR12VIConsoleKey` | `0x002cb4a0`, 1352 bytes | 1092 | `ec5a8e76da0fbeaa563dd09ff84e52c7642e30ef3cc1dcc664a26d27867a88ce` |
| `w10-vi-a-matrix44-dtor/dtor.cpp` | `_$_10VIMatrix44` | `0x0109ac20`, 44 bytes | 32 | `86e224177f37b185575420ef0719230409d9bd3754121b765d852601c4ec3b07` |
| `w10-vi-a-pad-close/close.cpp` | `Close__5VIPad` | `0x010ff1a8`, 48 bytes | 40 | `1fcd13ab7b5b7ca1c5a0104ef39aec4ecc5c6198249c1ed9185f1ad8733ed74e` |
| `w10-vi-a-pad-dtor/dtor.cpp` | `_$_5VIPad` | `0x010ff0b0`, 84 bytes | 76 | `cfee29cbd5ca1ae459ad34c5e1f14056a41feea2154080ff38d3472426618ce5` |
| `w10-vi-a-pad-init/init.cpp` | `Init__5VIPadii` | `0x010ff108`, 156 bytes | 168 | `a2304cfbc9b8b1b55c0f86682421153f274953df1e9af624d213fb3e4923df22` |
| `w10-vi-a-pad-read/read.cpp` | `Read__5VIPadR12VIConsolePad` | `0x010fec38`, 1128 bytes | 1348 | `31dcd488adf2950680da9abc02cfbf8e8ae390e971600f5f41f26158559e9247` |
| `w10-vi-a-usocket-free/free.cpp` | `VIFreeUSocketExApi__Fv` | `0x01048df8`, 36 bytes | 12 | `0b20aa57407f1c61534f1d4085e5315b0a59c7d1d251cefb660dae0a550fef8e` |

`KbdMapTable` is initialized data: 75 ten-byte records (750 bytes). The object
built from the source has a `.data` section byte-identical to the image bytes at
0x00447c60 (checked privately; the host contract carries an FNV-1a and byte-sum
of the table plus three spot records).

## Imports closed (33)

`KbdMapTable`, `Parse__9VICSFFilePCcR10VIESFParseR7VIScene`, `__9VICSFFile`,
`Init__8VICameraR8VIRasterRC7VIVect3T2ffff`, `__cl__8VICameraR7VIVect3T1`,
`Close__9VICollide`, `Init__9VICollide`, `_$_9VICollide`, `__9VICollide`,
`Clear__12VIDictionary`, `Init__12VIDictionaryi`, `_$_12VIDictionary`,
`__12VIDictionary`, `_$_10VIESFParse`, `__10VIESFParse`, `Begin__7VIFaderff`,
`Free__7VIFader`, `Init__7VIFaderP8VIRasterP12VIDictionaryf`, `Update__7VIFader`,
`_$_7VIFader`, `__7VIFader`, `__6VIFont`, `VIFreeUSocketExApi__Fv`,
`CleanUp__11VIGUIObject`, `Load__13VIGameOptions`,
`Read__10VIKeyboardR12VIConsoleKey`, `_$_10VIKeyboard`, `__10VIKeyboard`,
`_$_10VIMatrix44`, `Close__5VIPad`, `Init__5VIPadii`,
`Read__5VIPadR12VIConsolePad`, `_$_5VIPad`.

## Imports opened

These are the undefined symbols of the 33 objects linked together. Most are SDK
or other-package symbols and may already be defined in the merged link; compare
against `unresolved-imports.json` after integration. A full partial build was not
run here (one at a time, kept off the machine). Nothing below is stubbed.

VI-family: `Begin2D__8VIRaster`, `BeginTriStrip__8VIRaster`,
`CalcAxes__8VICameraRC7VIVect3R7VIVect3N22`, `Clear__9VICollide`,
`Color__8VIRasterRC10VIColor32F`, `CreateMaterial__8VIRasteri`,
`DequeueKeyEvent__10VIKeyboardP12VIConsoleKey`, `End2D__8VIRaster`,
`EndTriStrip__8VIRaster`, `Frustum__8VIRasterR10VIMatrix44ffffff`,
`GetTime__7VITimer`, `Identity__10VIMatrix44`, `InitSolidFill__10VIMaterial`,
`Material__8VIRasteri`, `Open__6VIFilePCci`, `__6VIFile`, `_$_6VIFile`,
`Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene` (the overload taking a
`VIFile`), `PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey`,
`ReleaseMaterial__8VIRasteriR12VIDictionary`, `ScaleStick__5VIPadUc`,
`SetDefaults__13VIGameOptions`,
`SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode`, `SetMaterial__8VIRasteri`,
`TranslateEvent__10VIKeyboardP11_lgKbdEvent`,
`Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44`,
`VIMemCardReadFile__FPCcPvi`, `Vertex__8VIRasterRC8VIVect2I`, and the
`VIMap<unsigned, VIDictionary::VIDictEntry>` members
`__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`,
`_$_t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`,
`Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`,
`Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`, and the pool constructor and
destructor `__t6VIPool1ZP12VICollBuffer`, `_$_t6VIPool1ZP12VICollBuffer`.

Data words left to their owners: `_11VIGUIObject$m_alphaFillMaterial`
(0x0118e818; `CleanUp__11VIGUIObject` stores -1 into it), `sMessagePool`
(0x011e5c18, local in the image; read by `VIFreeUSocketExApi__Fv`), `PadData`
(0x011e9480, local; the buffer passed to `scePadPortOpen`), and
`gKeyboardAttached` (0x004865f0), which `iop-module-boot-keyboard-init` already
defines.

SDK and runtime: `scePadPortOpen`, `scePadPortClose`, `scePadRead`,
`scePadGetState`, `scePadGetReqState`, `scePadInfoMode`, `scePadInfoAct`,
`scePadSetActAlign`, `scePadSetActDirect`, `scePadSetMainMode`,
`lgKbdEnumerate`, `lgKbdOpen`, `lgKbdRead`, `lgKbdClose`, `lgKbdEnumHint`,
`lgKbdSetRepeatRate`, `memset`, `printf`, `tanf`, `__wrap_free`,
`__builtin_delete`, `__floatdisf`.

## Build-plan entries needed

Profile `authored_cpp`, empty header closure, kind `independently_authored_source`.
`input_order` and the snapshot record (sources and hashes, not
`host_contract.cpp`) are for the integrator. Object names carry a package prefix
because several units share file names. Neither shared file is edited here.

```json
[
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_camera_call.o",
      "profile": "authored_cpp",
      "sha256": "1a50524e0b4a1d9b98c91116a27182d0f04ef263a5b1cb8a7825ddea346bbf68",
      "source": "sources/w10-vi-a-camera-call/call.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_camera_init.o",
      "profile": "authored_cpp",
      "sha256": "d860d1b104fcff8bf660aecae6e434dfb29da13b74cec5c391767bca6ba90dc7",
      "source": "sources/w10-vi-a-camera-init/init.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_collide_close.o",
      "profile": "authored_cpp",
      "sha256": "0663aedc570951d6691a7a395024fbf95d6c01d873f43581e83c90f9f3559aa8",
      "source": "sources/w10-vi-a-collide-close/close.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_collide_ctor.o",
      "profile": "authored_cpp",
      "sha256": "1db43c304783e5dcfa906ad31caf86f528e8292c720e04554799cbbd7eec6a7d",
      "source": "sources/w10-vi-a-collide-ctor/ctor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_collide_dtor.o",
      "profile": "authored_cpp",
      "sha256": "9f70ea6c886b75176877b18c6ede9eaf63ad130df85a1d8d1b143b9ff3cb53ce",
      "source": "sources/w10-vi-a-collide-dtor/dtor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_collide_init.o",
      "profile": "authored_cpp",
      "sha256": "04803d174ce252f4d4847d8fd02ff1f23fbd0279692d2615fbf5d4dc31ee641b",
      "source": "sources/w10-vi-a-collide-init/init.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_csffile_ctor.o",
      "profile": "authored_cpp",
      "sha256": "a251c3239f7881b96dae1e7bffa7702c17301173ecf111891392b2dd04e8856a",
      "source": "sources/w10-vi-a-csffile-ctor/ctor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_csffile_parse.o",
      "profile": "authored_cpp",
      "sha256": "47f0bf24558ce6d1338a618f10e02de4efcc196077ae794b78c4bfcba3c8dfe7",
      "source": "sources/w10-vi-a-csffile-parse/parse.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_dict_clear.o",
      "profile": "authored_cpp",
      "sha256": "c6576c27711c2f1d0556c5696c8345f960187a0075abdd234091557e8c518d65",
      "source": "sources/w10-vi-a-dict-clear/clear.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_dict_ctor.o",
      "profile": "authored_cpp",
      "sha256": "384a3cc1c1169d81117b8d3a014244ba1c194ae258852e50e265a1eeba88a906",
      "source": "sources/w10-vi-a-dict-ctor/ctor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_dict_dtor.o",
      "profile": "authored_cpp",
      "sha256": "5de5831bb0b0a3c244718d6d56996d3156216d44d7ee0e39d014290cf71d2b18",
      "source": "sources/w10-vi-a-dict-dtor/dtor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_dict_init.o",
      "profile": "authored_cpp",
      "sha256": "00362e3b852a17fcb6ca38b580788cfe8b983fc9aaff41b0c5c1ebb0eb57fed8",
      "source": "sources/w10-vi-a-dict-init/init.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_esfparse_ctor.o",
      "profile": "authored_cpp",
      "sha256": "9db047d44a8ea2185cdb8508c3b770875602317882345c04f2759158d0d39dc9",
      "source": "sources/w10-vi-a-esfparse-ctor/ctor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_esfparse_dtor.o",
      "profile": "authored_cpp",
      "sha256": "e44d87e1aeb29fcfe60536893525b144f2a6cc565e084d5bff14e6c6daa4ece4",
      "source": "sources/w10-vi-a-esfparse-dtor/dtor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_fader_begin.o",
      "profile": "authored_cpp",
      "sha256": "cf86875cee06e24c49a480d956fe13b71498616c524881e7f962f2ac7cd85aef",
      "source": "sources/w10-vi-a-fader-begin/begin.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_fader_ctor.o",
      "profile": "authored_cpp",
      "sha256": "4167ecc1c8187ce959f2c6063e59c8d13f651424ea6e01b71af248aa8a38dfac",
      "source": "sources/w10-vi-a-fader-ctor/ctor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_fader_dtor.o",
      "profile": "authored_cpp",
      "sha256": "8996e87f8b66c04afc8eb2f16b54e4b7070aeabcb58caefbc21dbb098014f02b",
      "source": "sources/w10-vi-a-fader-dtor/dtor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_fader_free.o",
      "profile": "authored_cpp",
      "sha256": "1cd6fb6ee3c7de6ed7cfdab2aa7def8d6b80a0459ff39cb81cda2f490eac14e6",
      "source": "sources/w10-vi-a-fader-free/free.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_fader_init.o",
      "profile": "authored_cpp",
      "sha256": "be557bc62ee188ebc116a0bf9b8f59e2c243b3fc1ea51c63a62573255373a408",
      "source": "sources/w10-vi-a-fader-init/init.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_fader_update.o",
      "profile": "authored_cpp",
      "sha256": "e58b7a40d89482f801a8699d8d9fb7d18e37d3610ae66549f9d87fb1deeff726",
      "source": "sources/w10-vi-a-fader-update/update.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_font_ctor.o",
      "profile": "authored_cpp",
      "sha256": "fb5239d8fe6796b6687acee140307760be735802d86d4260c6398406657b7e61",
      "source": "sources/w10-vi-a-font-ctor/ctor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_gameoptions_load.o",
      "profile": "authored_cpp",
      "sha256": "95b74efff5edc584ea28049e829537d459f3619e37ec59b96d59ac715b9fe4a1",
      "source": "sources/w10-vi-a-gameoptions-load/load.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_guiobject_cleanup.o",
      "profile": "authored_cpp",
      "sha256": "1b9a82a61f56308535d55521c94904bccf9d08c39f7879aebe4213b7af293fb0",
      "source": "sources/w10-vi-a-guiobject-cleanup/cleanup.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_kbd_map_table.o",
      "profile": "authored_cpp",
      "sha256": "9a5b0f01af9f0eb8582e3ba15cf39abb504b88751abf775ad8a2fb6bf450682b",
      "source": "sources/w10-vi-a-kbd-map-table/kbd_map_table.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_keyboard_ctor.o",
      "profile": "authored_cpp",
      "sha256": "a987fa0319e5b144a062cc2152236c8f36b586317350149502f898e2e2568db5",
      "source": "sources/w10-vi-a-keyboard-ctor/ctor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_keyboard_dtor.o",
      "profile": "authored_cpp",
      "sha256": "fe54c83d7f5c5cd28d07d5af1ded829ca9245c1d56d67bd485bad447734fb5a7",
      "source": "sources/w10-vi-a-keyboard-dtor/dtor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_keyboard_read.o",
      "profile": "authored_cpp",
      "sha256": "ec5a8e76da0fbeaa563dd09ff84e52c7642e30ef3cc1dcc664a26d27867a88ce",
      "source": "sources/w10-vi-a-keyboard-read/read.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_matrix44_dtor.o",
      "profile": "authored_cpp",
      "sha256": "86e224177f37b185575420ef0719230409d9bd3754121b765d852601c4ec3b07",
      "source": "sources/w10-vi-a-matrix44-dtor/dtor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_pad_close.o",
      "profile": "authored_cpp",
      "sha256": "1fcd13ab7b5b7ca1c5a0104ef39aec4ecc5c6198249c1ed9185f1ad8733ed74e",
      "source": "sources/w10-vi-a-pad-close/close.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_pad_dtor.o",
      "profile": "authored_cpp",
      "sha256": "cfee29cbd5ca1ae459ad34c5e1f14056a41feea2154080ff38d3472426618ce5",
      "source": "sources/w10-vi-a-pad-dtor/dtor.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_pad_init.o",
      "profile": "authored_cpp",
      "sha256": "a2304cfbc9b8b1b55c0f86682421153f274953df1e9af624d213fb3e4923df22",
      "source": "sources/w10-vi-a-pad-init/init.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_pad_read.o",
      "profile": "authored_cpp",
      "sha256": "31dcd488adf2950680da9abc02cfbf8e8ae390e971600f5f41f26158559e9247",
      "source": "sources/w10-vi-a-pad-read/read.cpp"
    },
    {
      "header_closure": [],
      "kind": "independently_authored_source",
      "object_name": "w10_vi_a_usocket_free.o",
      "profile": "authored_cpp",
      "sha256": "0b20aa57407f1c61534f1d4085e5315b0a59c7d1d251cefb660dae0a550fef8e",
      "source": "sources/w10-vi-a-usocket-free/free.cpp"
    }
]
```

## Authored but never executed

All of it on the EE. Host contracts execute the C++ on the build host against
mocked callees, which checks call order, arguments, stored words and return
values, not EE behavior. Specifically never exercised against real data:
`Read__5VIPadR12VIConsolePad` and `Read__10VIKeyboardR12VIConsoleKey` (scripted SDK
returns only), `Update__7VIFader` (float rounding is the host's, and `min`/`max`
are compares rather than the EE `min.s`/`max.s`), `Init__8VICameraR8VIRasterRC7VIVect3T2ffff`
(host `tanf`), and the draw sequence of the fader. `VICollide`'s constructor drops
the redundant vector stores.

## Unknown

- Sizes of VIFader, VICollide, VIESFParse, VICSFFile, VIFont, VIKeyboard,
  VIPad and VICamera; only the touched offsets are established.
- The `VIFader` dictionary word at offset 4: `Free` reads it but `Init` never
  stores its dictionary argument, so some other code must.
- Meaning of the fader floats' units, VIFont's four 1.0 words, the VIPad words
  at 8 to 47 and 36, and of the VIESFParse scalars and 256-entry table.
- What `VIKeyboard::Read` events' halfwords are called, the key numbers it
  emits, the enumerate buffer size, and the error values 0x80000000/0x80000004.
- Which byte `KbdMapTable` is sorted on and what its first byte means.
- `VIPad::Read` out-structure (`VIConsolePad`) field names; the float index to
  button mapping is recorded by position only.
- Whether `PadData`, `sMessagePool` and `m_alphaFillMaterial` have their own
  definitions in the original sources or are locals of the functions here.

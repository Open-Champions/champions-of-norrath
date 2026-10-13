# w11-vi-misc-a: VI windows, files, sound and input (1 of 2)

All 34 scoped symbols are authored, one per commit, in
`experimental/sources/w11-vi-misc-a-*/`. Original names, sizes and layouts were
read from the pinned SLUS-20565 image with local tools; nothing from the image is
committed. Each commit message records its check: a paired run of the original
function and the authored one on a private R5900 interpreter (outside git),
comparing return value, call log, and every changed byte outside the stack
(branching functions also report instruction-word coverage; the few uncovered words
are alignment nops, and for `Process__9VIStation` an unreachable aligned block-copy
branch). Mutated copies were shown to fail where the commit says so.
`RunPatcher__7VISetup` records a draft that the run caught (the original also tests
`MainLoopBegin`'s result).

Observed, inferred and unknown details for each function are in the header
comment of its source file. Most are in `RunPatcher__7VISetup`, which looks like
a DNAS authentication wait screen (inferred). Codes -105 and -101 are unknown.

## Imports closed

- Type__C7VISound
- Find__12VIDictionaryUiR14VIResourceTypeRi
- Open__6VIFilePCci
- _$_6VIFile
- __6VIFile
- _$_19VIFlatFileStructure
- __19VIFlatFileStructure
- SetDefaults__13VIGameOptions
- VIHashResourceID
- DequeueKeyEvent__10VIKeyboardP12VIConsoleKey
- PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey
- TranslateEvent__10VIKeyboardP11_lgKbdEvent
- AddTail__7VILListP7VILNode
- Init__7VILList
- _$_7VILList
- __7VILList
- __7VILNode
- VIMemCardReadFile__FPCcPvi
- ScaleStick__5VIPadUc
- _$_16VIRealmInterface
- __16VIRealmInterfaceP20VIWndGenericRendererPCc
- RunPatcher__7VISetup
- AsWave__7VISound
- AsXm__7VISound
- BeginAdpcmTransfer__13VISoundDevicePPvUiiPi
- BeginXmTransfer__13VISoundDevicePPvUiUiiPi
- IdleTransfer__13VISoundDevice
- Reset__13VISoundDevice
- Init__9VIStationiPCcPcT3T2T2i
- Process__9VIStation
- Load__13VIStringTablePCc
- Unload__13VIStringTable
- _$_13VIStringTable
- __13VIStringTable

## Imports opened

Each source declares the callees it needs as extern; none is defined here. Those
listed below are other packages' or not yet reconstructed, and stay unresolved
until their owners land them: the `Init__*`, `Update__*`, `Render__*` and
`Set*__*` methods of VIUI, VIRaster, VIScene, VICamera, VIFader, VIWindowManager,
VIWndEula, VIWndGenericRenderer, VIIconDefinition, VIPad and VIKeyboard (callees
of `Init__9VIStation...`, `Process__9VIStation` and the realm interface
constructor); the DNAS calls `VIDnasInit__Fv`, `VIDnasGetStatus__FP14sceDNAS2Status`,
`VIDnasAuthInstall__Fv`, `VIDnasShutdown__Fv`; `MainLoopBegin__7VISetup`,
`MainLoopEnd__7VISetup`, `sAlarmCallback__FiUsPv`, `GetTime__7VITimer`;
`CDSearchFile__6VIFileP10sceCdlFILEPCc`, `CDDiskReady__6VIFilei`,
`BufferClear__6VIFile`, `Close__6VIFile`; `VIMemCardReadFileAsync__FPCcPvi`,
`sceMcSync`, `sceMcClose`; `SetIopFunction__FUii`, `sceSifSetDma`, `sceSifDmaStat`,
`FreeSpuMem__13VISoundDevice...`, `Sound__13VISoundDevicei` and the playback
iterators; `CombineDeadkey__10VIKeyboardUc`, `QueueKeyEvent__10VIKeyboard...`;
`LoadRawFile__13VIStringTable...`, `ProcessRawFile__13VIStringTable...`,
`FixupTextBuffer__13VIStringTable`; VIString `snprintf` variants; the VIMap
`Find`/`End` template instances; string-class `clone`; the `RealmGatewayAPI`,
`VIUDPSocket`, `VIESFParse`, `VIArray1Zc` and `VIFlatFileDirectory`
constructors and destructors. The exact list is the delta in
`unresolved-imports.json` after the plan is merged. Data externs: `sLastTickTime`,
`sAsyncFd`, `sMemCardInitialized`, `sIopFunctionData`, `sRecv`, `sUsDeadkey`,
`sUsDeadkeyCount`, `USEnglishKbdMapTable`, `sIconDefinition`, `sListIcon`,
`sProductTitle`, the VIFile IOP stream word, the station active-tries word, the
realm and flat-file-directory vtables, the empty-string rep and the allocator
free list.

## Build-plan entries needed

Each file below is an `independently_authored_source` with profile `authored_cpp`,
no header closure. Paths are relative to `experimental/`; the integrator fills
`input_order`, `object_name` and `emitted_functions` (the `compiled_body_bytes`
come from the build). The sha256 is of the committed file.

| source | sha256 |
|---|---|
| `sources/w11-vi-misc-a-dictionary-find/find.cpp` | `43244d007135f53e4b35f47ead3efa63da490a1dbb7075657dd8e9bb2c628e1d` |
| `sources/w11-vi-misc-a-file-ctor/ctor.cpp` | `701a49f00a8edaaa9818a07127f9495fcd042d4f02886cf09f7a588afc017cb4` |
| `sources/w11-vi-misc-a-file-dtor/dtor.cpp` | `dbd565932371f8d2c65c17f459d9fd476d12e095e38fe9e16087102551cd6279` |
| `sources/w11-vi-misc-a-file-open/open.cpp` | `eee9e5c250677839fd4bce7c48ff93fb1e7e714076890c5f7e96197fd49aed75` |
| `sources/w11-vi-misc-a-flat-file-structure-ctor/ctor.cpp` | `56cbe8dc508157d17bb51a7244f50c8add189dfd01cd88c97316e228d37400d5` |
| `sources/w11-vi-misc-a-flat-file-structure-dtor/dtor.cpp` | `55ea1cd3f5b1e28b9c3b889c5b9804f906d3d1c85e727136aacd5d623059360f` |
| `sources/w11-vi-misc-a-game-options-defaults/defaults.cpp` | `0e653c3646ab8099d77dd0d86063a47c4973dd84c06e4b3175da777bb05a70a0` |
| `sources/w11-vi-misc-a-hash-resource-id/hash.cpp` | `57b6f94ad02c9f72ce83d0116a1ba4d5ad17952240ed49c00a3c3210d34801e3` |
| `sources/w11-vi-misc-a-keyboard-dequeue/dequeue.cpp` | `2a74594211f71822cd09cb01d422996047557e11032784dc88f396a99af881a0` |
| `sources/w11-vi-misc-a-keyboard-prepare-queue/prepare.cpp` | `98679b01c343cfb4d306f2257ecfcd1cb6b58cfde3768031bf2dff0e267d7bc6` |
| `sources/w11-vi-misc-a-keyboard-translate/translate.cpp` | `2e31f7f207f072ede903f178b0943cc3b6358e811c3adaef1642a4b6e2171232` |
| `sources/w11-vi-misc-a-list-add-tail/add_tail.cpp` | `4217d4a139e4e67f09a018c38624c28c015a3230ae10e7d13884c6bbc2b35d68` |
| `sources/w11-vi-misc-a-list-ctor/ctor.cpp` | `5680019ec99faed96da303307eb47137605a6df1702987694196bc3b7b00250e` |
| `sources/w11-vi-misc-a-list-dtor/dtor.cpp` | `4594bf5920ede711efb27c004376e5bb3e7046402c760df5c0e3fc9d365a2b46` |
| `sources/w11-vi-misc-a-list-init/init.cpp` | `e66ce08d8b082e92623c057e41cf09f581c58de43c5cd6f94aed28cc27ea6fc5` |
| `sources/w11-vi-misc-a-list-node-ctor/node_ctor.cpp` | `e4a9ecfd260d7b8f7b0397c4be455f4f747cbf16f46f9800925b07f312c7d5ef` |
| `sources/w11-vi-misc-a-memcard-read-file/read_file.cpp` | `b29721bbe386d8e76fb418a5687c946cd2fdca650b47573ab398d337e41693f1` |
| `sources/w11-vi-misc-a-pad-scale-stick/scale.cpp` | `886b53a5d4c6f0631c7a2be17617eb76770290317450e5b16cd0a3e7e5ca5021` |
| `sources/w11-vi-misc-a-realm-interface-ctor/ctor.cpp` | `990d5e6b02317a1ae37c19af6bd1b89ce80c724c3002cd1b0e3bbc9ba3bb719f` |
| `sources/w11-vi-misc-a-realm-interface-dtor/dtor.cpp` | `d1444b284d8f858227e3f0f1b6c5d715f373bf3eec180c1def24607a2fd5fdd9` |
| `sources/w11-vi-misc-a-setup-run-patcher/run_patcher.cpp` | `4f5a684e065db36c17128ff424138892b3ad496eeb59c6318cf649924e9aa33f` |
| `sources/w11-vi-misc-a-sound-as-wave/as_wave.cpp` | `79a3156e0aca5c5723a119129846df19b6de66398e8fad0be2ec2b4cedda0b53` |
| `sources/w11-vi-misc-a-sound-as-xm/as_xm.cpp` | `31a9ada7e4c79fd0c0b83d6c6045301aa65806392233fe49cde4631ee8318921` |
| `sources/w11-vi-misc-a-sound-device-begin-adpcm/begin_adpcm.cpp` | `0adfdcaaae6d3acb23b7bf9e16b1b83e27f835433825e87590794ff167a24192` |
| `sources/w11-vi-misc-a-sound-device-begin-xm/begin_xm.cpp` | `2af4dd48613c38237b13ead01877e0bcd88aef11a432f37a69faf69d50544ee2` |
| `sources/w11-vi-misc-a-sound-device-idle-transfer/idle_transfer.cpp` | `30e46ba3c418b6d9916b6111f409a83e084f99738ff97c8cd941b2ed2d9d4f69` |
| `sources/w11-vi-misc-a-sound-device-reset/reset.cpp` | `22ef532236dd3a876897ef8d149aa5bf5267e2e4b4a965065d070fa451789127` |
| `sources/w11-vi-misc-a-sound-type/type.cpp` | `cf5355087b77e248e28b3dd8763f325f782e20570bdbefbc3a198020181ebd7f` |
| `sources/w11-vi-misc-a-station-init/station_init.cpp` | `80a4bf4bbe284531b0746e67bd10d7c4182ccda7b828fc57780869911b8d71b7` |
| `sources/w11-vi-misc-a-station-process/station_process.cpp` | `fbfac7138949eb6aa58a82ced2731dc45da36e02445d6c57113a30b24ac99e48` |
| `sources/w11-vi-misc-a-string-table-ctor/ctor.cpp` | `f7da503699061424de9bc653c3172b3c8cbaf9145f39310b38fe2515f708caf9` |
| `sources/w11-vi-misc-a-string-table-dtor/dtor.cpp` | `8cfc9163a3296f5a8321025388c5f5409302259a8644dffddeb96fb30d59f85d` |
| `sources/w11-vi-misc-a-string-table-load/load.cpp` | `6a5fe11318b4ebee8c2eb414971bdeeeadd2265773da22ad58d153058b49d20a` |
| `sources/w11-vi-misc-a-string-table-unload/unload.cpp` | `64df153c1ef0393beffe58b6a12473e6cb870a280b11e54659d55387eed9622c` |

## Authored but never executed

All 34 functions. The paired runs executed the original and the authored code on
a host interpreter against scripted hooks. No function has run on the EE, with
real devices (DMA, SPU, memory card, CD, DNAS network) or in gameplay. Fresh
compiled bodies are not claimed to be original-byte matches unless a commit says
so; the commits claim behavioural equivalence only.

## Unknown

The meaning of DNAS codes -105 and -101 and the fields cleared at the start of
`RunPatcher__7VISetup`. The layouts of the larger objects (VIStation, the realm
interface, VISetup) are known only at the offsets the functions touch.
`Process__9VIStation`'s aligned block-copy branch was not reached by the harness.
Whole-object types and the names of most touched fields are not established.

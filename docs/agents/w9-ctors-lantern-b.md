# Ant-lantern constructor views (b)

Package `w9-ctors-lantern-b`, branch `agents/w9-ctors-lantern-b`. Closes 40 data imports the
`ctors-ant-lantern-constructors` unit refers to: twelve virtual tables and twenty-eight string
literals. No functions were needed for this package.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0` (checked before use).
Addresses are that file's. No game bytes, listings or decompiler output are committed.

## Virtual tables (`w9-ctors-lantern-b-vtables/`)

Convention as in `w8-core-loop-d` and `vehicle-actors-views`: a table of {adjustment word, function
pointer} pairs, one pair per slot, whose targets are the sized ELF function symbols at the slot
addresses, ended by a null pair. The table is the whole sized symbol, so the extent equals the
image's. Each source was compiled with the reviewed EE g++ `authored_cpp` flags and `readelf` shows
one `.data` OBJECT of the listed size. Each `host/*_check.cpp` (not an EE input) defines every slot
function under its image name and fails if the table lists a different name, adjustment or null slot.

| view | image symbol | size |
|---|---|---|
| `norrath_ctors_ant_lantern_view_44cc28` | `_vt$3Ice` `0x44cc28` | 144 |
| `norrath_ctors_ant_lantern_view_44dbe8` | `_vt$10CloudGiant` `0x44dbe8` | 240 |
| `norrath_ctors_ant_lantern_view_44dd68` | `_vt$5Demon` `0x44dd68` | 240 |
| `norrath_ctors_ant_lantern_view_44e550` | `_vt$8Innoruuk` `0x44e550` | 240 |
| `norrath_ctors_ant_lantern_view_44e9d0` | `_vt$10AutoPropLP` `0x44e9d0` | 168 |
| `norrath_ctors_ant_lantern_view_44ebb0` | `_vt$13AutoContainer` `0x44ebb0` | 144 |
| `norrath_ctors_ant_lantern_view_44ec40` | `_vt$8AutoProp` `0x44ec40` | 168 |
| `norrath_ctors_ant_lantern_view_44ece8` | `_vt$9AutoChest` `0x44ece8` | 144 |
| `norrath_ctors_ant_lantern_view_44f6a8` | `_vt$10Arenabeast` `0x44f6a8` | 240 |
| `norrath_ctors_ant_lantern_view_450160` | `_vt$6Cthulu` `0x450160` | 240 |
| `norrath_ctors_ant_lantern_view_450530` | `_vt$13FemaleDarkElf` `0x450530` | 240 |
| `norrath_ctors_ant_lantern_view_4506b0` | `_vt$8Catapult` `0x4506b0` | 144 |

## Strings (`w9-ctors-lantern-b-strings/`)

Each is an unnamed NUL-terminated literal in `.rodata`, defined as an `extern "C" const char[]`
(precedent: `vehicle-actors-views`). The image pads literals to eight bytes; the padding is not
reproduced. The host check compares size, bytes and length. Contents are short tag, lump and
animation-name templates, not dialogue.

| address | text |
|---|---|
| `0x46d070` | `iceSheetBroken` |
| `0x46d300` | `cloudgiantking` |
| `0x46dbe0` | `faydark1` |
| `0x46dc90` | `lumpname` |
| `0x46dca0` | `idlename` |
| `0x46dcb0` | `%s_idle.anm` |
| `0x46dcc0` | `movename` |
| `0x46dcd0` | `%s_move.anm` |
| `0x46dce0` | `followname` |
| `0x46dcf0` | `%s_follow.anm` |
| `0x46dd00` | `fleename` |
| `0x46dd10` | `%s_flee.anm` |
| `0x46dd20` | `osidlename` |
| `0x46dd30` | `%s_osidle.anm` |
| `0x46dd40` | `osfleename` |
| `0x46dd50` | `%s_osflee.anm` |
| `0x46dd60` | `turnSpeed` |
| `0x46dd70` | `change%d` |
| `0x46ded0` | `generator` |
| `0x46dee0` | `triggerable` |
| `0x46def0` | `radiustrigger` |
| `0x46edb0` | `invulnerable` |
| `0x46f120` | `froglocktrident` |
| `0x46f130` | `froglockblowgun` |
| `0x46f310` | `gnomesoldier` |
| `0x46f320` | `gnomeworker` |
| `0x46f330` | `gnomepeasant` |
| `0x46fa28` | `antblack` |

## Observed / inferred / unknown

Observed: all table contents and string bytes above, read from the sized symbols and `.rodata`
of the pinned file. Table slot 15 of the AutoProp, AutoPropLP, AutoContainer, AutoChest and
Catapult tables points at an address that carries the zero-sized markers `gcc2_compiled.` and
`__gnu_compiled_cplusplus` as well as the sized function `clone__10GameObject`; the sized function
is used, as in the earlier tables.

Inferred (from names only): the strings are tag/lump keys and `%s`/`%d` templates read by the
constructors that reference them (Critter's ten animation pairs, Generator's three tags, and so on).
Which game-data field each key reads is not established here.

Unknown: dispatch order and override semantics of any slot; whether the image pads or merges the
literals the way a rebuilt `.rodata` would (placement is not claimed); the owners of the new slot
imports below; and anything about execution.

## Closing summary

Imports closed (40): norrath_ctors_ant_lantern_view_44cc28, norrath_ctors_ant_lantern_view_44dbe8, norrath_ctors_ant_lantern_view_44dd68, norrath_ctors_ant_lantern_view_44e550, norrath_ctors_ant_lantern_view_44e9d0, norrath_ctors_ant_lantern_view_44ebb0, norrath_ctors_ant_lantern_view_44ec40, norrath_ctors_ant_lantern_view_44ece8, norrath_ctors_ant_lantern_view_44f6a8, norrath_ctors_ant_lantern_view_450160, norrath_ctors_ant_lantern_view_450530, norrath_ctors_ant_lantern_view_4506b0, norrath_ctors_ant_lantern_view_46d070, norrath_ctors_ant_lantern_view_46d300, norrath_ctors_ant_lantern_view_46dbe0, norrath_ctors_ant_lantern_view_46dc90, norrath_ctors_ant_lantern_view_46dca0, norrath_ctors_ant_lantern_view_46dcb0, norrath_ctors_ant_lantern_view_46dcc0, norrath_ctors_ant_lantern_view_46dcd0, norrath_ctors_ant_lantern_view_46dce0, norrath_ctors_ant_lantern_view_46dcf0, norrath_ctors_ant_lantern_view_46dd00, norrath_ctors_ant_lantern_view_46dd10, norrath_ctors_ant_lantern_view_46dd20, norrath_ctors_ant_lantern_view_46dd30, norrath_ctors_ant_lantern_view_46dd40, norrath_ctors_ant_lantern_view_46dd50, norrath_ctors_ant_lantern_view_46dd60, norrath_ctors_ant_lantern_view_46dd70, norrath_ctors_ant_lantern_view_46ded0, norrath_ctors_ant_lantern_view_46dee0, norrath_ctors_ant_lantern_view_46def0, norrath_ctors_ant_lantern_view_46edb0, norrath_ctors_ant_lantern_view_46f120, norrath_ctors_ant_lantern_view_46f130, norrath_ctors_ant_lantern_view_46f310, norrath_ctors_ant_lantern_view_46f320, norrath_ctors_ant_lantern_view_46f330, norrath_ctors_ant_lantern_view_46fa28.

Imports opened (70), all virtual-slot function symbols named by the tables above that no
authored unit defines yet (measured by linking these objects into the partial object of the current
branch tip): _$_10Arenabeast, _$_10AutoPropLP, _$_10CloudGiant, _$_13AutoContainer, _$_13FemaleDarkElf, _$_3Ice, _$_5Demon, _$_6Cthulu, _$_8Catapult, _$_8Innoruuk, _$_9AutoChest, __tf10Arenabeast, __tf10AutoPropLP, __tf10CloudGiant, __tf13AutoContainer, __tf13FemaleDarkElf, __tf3Ice, __tf5Demon, __tf6Cthulu, __tf8Catapult, __tf8Innoruuk, __tf9AutoChest, clone__13FemaleDarkElf, clone__3Ice, isEnemy__5DemonP10GameObject, msg_alert__13FemaleDarkElfifG6Point3P10GameObject, msg_draw__10Arenabeast, msg_draw__10CloudGiant, msg_draw__13FemaleDarkElf, msg_draw__3Ice, msg_draw__5Demon, msg_draw__6Cthulu, msg_draw__8Catapult, msg_draw__8Innoruuk, msg_hurt__10ArenabeastfP10GameObjecti, msg_hurt__10CloudGiantfP10GameObjecti, msg_hurt__8CatapultfP10GameObjecti, msg_hurt__8InnoruukfP10GameObjecti, msg_load__10CloudGiantPc, msg_load__13FemaleDarkElfPc, msg_load__5DemonPc, msg_load__8CatapultPc, msg_moveCollision__10ArenabeastPP10GameObject, msg_moveCollision__6CthuluPP10GameObject, msg_poke__9AutoChesti, msg_run__10Arenabeast, msg_run__10CloudGiant, msg_run__13AutoContainer, msg_run__13FemaleDarkElf, msg_run__3Ice, msg_run__5Demon, msg_run__6Cthulu, msg_run__8Catapult, msg_run__8Innoruuk, msg_run__9AutoChest, msg_save__10CloudGiantPc, msg_save__13FemaleDarkElfPc, msg_save__5DemonPc, msg_save__8CatapultPc, msg_trigger__10Arenabeasti, msg_trigger__10CloudGianti, msg_trigger__8Catapulti, msg_trigger__8Innoruuki, setState__10Arenabeasti, setState__10CloudGianti, setState__13FemaleDarkElfi, setState__5Demoni, setState__6Cthului, setState__8Innoruuki, updateEnemy__8Innoruuk.

Build-plan entries needed (40; `emitted_functions` is empty, `input_order` and the snapshot are for
the integrator to assign; the hashes are of the committed sources at the last commit of this branch):

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44cc28.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44cc28.cpp",
  "sha256": "39773fc5f5fa1530b9cfe722d8e9a9b23d0d0d4e2c0a6c6d819d53bdd6f332b5",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44dbe8.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44dbe8.cpp",
  "sha256": "e41712730ca8704169bc27fa1ef0a36a320f1de4fa9332e12e0ccf1866bfb196",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44dd68.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44dd68.cpp",
  "sha256": "2de09c89452c26de147dfc2d934708063a7a576fb958361a25b93717b3778a22",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44e550.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44e550.cpp",
  "sha256": "6894150737ba4e4666191b8fe338b54cb5a42c6a58a35a77861e24ab54bc64ce",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44e9d0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44e9d0.cpp",
  "sha256": "d78aa1a428421c1af09c5d5c42a2a9b4b04dcb5656d467441684eee5e4d895f6",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44ebb0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44ebb0.cpp",
  "sha256": "ede9550c72795f55de6f50fad84a9c3dacf5bf27b549bd9d299b0b691005e1a8",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44ec40.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44ec40.cpp",
  "sha256": "ad925820a97c756d16bb6c5c20965a0c2b95df53f58095aef14c960da02c57d9",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44ece8.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44ece8.cpp",
  "sha256": "95684353a33329173dad30842b1c4dcf0f2883d9917973e2f061bc976e82da6d",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_44f6a8.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_44f6a8.cpp",
  "sha256": "57136911fd6d4525ec1d2d1b7870366ac72fd129cee06d9b98785d7d617d8400",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_450160.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_450160.cpp",
  "sha256": "eca7eaeb9d4e9d7c5dc06ba4e2fb5f4f5d9a61cc1e3d9150af091f963f360bf9",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_450530.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_450530.cpp",
  "sha256": "37d37adf0c79afd9f20bc193a9618eb4941a847e958e7239bdcfd242ffe98433",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_4506b0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-vtables/view_4506b0.cpp",
  "sha256": "ebbdc78c19008b0f30fd7f87a461cde885249e35f885f3250cf858c984acd797",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46d070.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46d070.cpp",
  "sha256": "40c8c0c46fc74f5744c62558aaff3381404adbaf3ff188aec36e15046d545c62",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46d300.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46d300.cpp",
  "sha256": "d6b06bea5b87abbdc13fd66cbd3a0360b6096054c4a9c9121abb5018a842fd71",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dbe0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dbe0.cpp",
  "sha256": "38ba3bf57d0ebdfb31d7e40f69eab23dcc02459daef16d134674077686a7bec5",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dc90.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dc90.cpp",
  "sha256": "28a6414520c38960957eee93d3dd8b775d261e08c9568140aaec4e4b4d038f36",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dca0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dca0.cpp",
  "sha256": "d105f6dbef2735a325f971a20612cb9f1ceca9ee9d397bb5d2631f0ca5912235",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dcb0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dcb0.cpp",
  "sha256": "e3c1d4f0058bf3e2fd800c082b824191d2cea483bf0319bb8eb6e94bb5a1e05c",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dcc0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dcc0.cpp",
  "sha256": "454d3122d3842d2bd8b1252260c88a1ca256e95d25d14ffa1aa5c96d7fad89bc",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dcd0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dcd0.cpp",
  "sha256": "a1f9db534a1a48dde8c2005e59b5138a10799f47e8ffa81eddf1fb66c7ca750f",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dce0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dce0.cpp",
  "sha256": "47a69f88d1a254bc1ee4bc81d834f612999c219988475bc35667e22d0d8896ba",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dcf0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dcf0.cpp",
  "sha256": "02a0d6242a85e1940ca9a0d7a3a935830f1fc9d3ef1adc9bc0bba97de5fcfd22",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd00.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd00.cpp",
  "sha256": "c8d3be40f5a9f94ffd4add17ff542742556f01f9eeef6105b10f032a986c62d7",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd10.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd10.cpp",
  "sha256": "1a6fe5d3e0570113e4d2f922f1d8786d29bab3e2db550e997bb7f264eca707fb",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd20.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd20.cpp",
  "sha256": "228511871ac88b9b369271c15d28410d5783b9788944c64d50da6d43d761d1c4",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd30.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd30.cpp",
  "sha256": "bb9ed9e072d499d19652602537b59cbd4e1e56c8566cc414ebc4cbd21fe813a7",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd40.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd40.cpp",
  "sha256": "96c9709efb536b2ec6c0e7b39c98fde1419cce643303eb83ce82d11d2e415ba2",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd50.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd50.cpp",
  "sha256": "a310ec99098557efbf5ac541b863e1cc8cf16082960c7d5f199f308c5f089484",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd60.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd60.cpp",
  "sha256": "5389230f9b469f061329a1c32c619d6512e124b5a9f1bf5e0c5334d2519804e0",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dd70.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dd70.cpp",
  "sha256": "cbbc33eaf670b9ba20260bfb2905b798d9690de2d156ab2b62ea5173e99ca5f0",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46ded0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46ded0.cpp",
  "sha256": "b21515a42266f2ac2974251d0852a84190fba792811f990feb64bc7f1554cce6",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46dee0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46dee0.cpp",
  "sha256": "bfe5c5058cff31c3ecbf27120518dc8a36fced05eef85539e90e1663120424cc",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46def0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46def0.cpp",
  "sha256": "d2dee346d956265da2c249fbdf6c957237783baa0be650ee76812983424a2731",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46edb0.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46edb0.cpp",
  "sha256": "4a7b31af919ed9e237b72d35020757b25aaa963deaa51373a8e7578ecea7fb30",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46f120.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46f120.cpp",
  "sha256": "e3a0a439082b2b38ee33dd07558306aff4deaa85a0f7fe906044817a0577bf39",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46f130.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46f130.cpp",
  "sha256": "b98f27d7df458071fa7967a286693681fb466e6291720cac8e6baa9e3d8a54f6",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46f310.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46f310.cpp",
  "sha256": "896ba09cd2c9c950f93b996dd25f39254600ece172a2b7bc72433a9abb16ac28",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46f320.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46f320.cpp",
  "sha256": "2d6238f768bfbe5bf9e5062ba09af44403a7157f1ba14d59629738d6e5aebdee",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46f330.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46f330.cpp",
  "sha256": "46cb4f6643111b91e38474a2250f79aadfc686eff299265830e95efdc82e5d64",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w9lb_view_46fa28.o",
  "profile": "authored_cpp",
  "source": "sources/w9-ctors-lantern-b-strings/view_46fa28.cpp",
  "sha256": "86f11d78ad412269a434294ce1e6bbd447cbec06366f8a2fe1603b18ce7edb99",
  "emitted_functions": []
 }
]
```

Authored but never executed: everything. The host checks run on the build host only. Nothing has
run on the EE, and the link check was a relocatable `ld -r` of these objects with the earlier
partial object, not the full build driver (a private plan needs a private snapshot).

Not done: no function was reconstructed; nothing else in the unresolved list belongs to this package.

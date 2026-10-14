# Lava-wraith constructor and view data (b)

Package `w9-ctors-wraith-b`, branch `agents/w9-ctors-wraith-b`. 38 data views, one commit
each (plus one comment-only correction). Reference: `work/private/SLUS_205.65`; addresses
are that file's. No game bytes or listings are committed; the sources contain only the
tag text and the function symbol names the original's tables point at.

## Checks performed

- Each source compiles with the reviewed EE g++ and the `authored_cpp` flags (single unit).
- Virtual tables (22 of 240 bytes, 2 of 144 bytes): the slot words were read from the
  executable and every non-null function word was matched to exactly one sized function
  symbol (twice: from the symbol dump and from the ELF symbol table; an ambiguous address
  would have stopped the run). The compiled `.data` relocations were compared slot by slot
  with those names and no extra relocation was allowed. A host test calls every slot and
  checks which function it reached, the adjustment word, and the null last slot.
- Strings: the compiled `.rodata` bytes were compared with the original bytes through the
  first NUL. A host test compares the characters.
- Nothing ran on the EE. `python3 -m unittest discover -s tools -p 'test_*.py'` was run.

## Imports closed

`norrath_ctors_lava_wraith_view_44af90`
`norrath_ctors_lava_wraith_view_44b080`
`norrath_ctors_lava_wraith_view_44b260`
`norrath_ctors_lava_wraith_view_44b350`
`norrath_ctors_lava_wraith_view_44b630`
`norrath_ctors_lava_wraith_view_44b720`
`norrath_ctors_lava_wraith_view_44b810`
`norrath_ctors_lava_wraith_view_44c710`
`norrath_ctors_lava_wraith_view_44c7a0`
`norrath_ctors_lava_wraith_view_44def0`
`norrath_ctors_lava_wraith_view_44dfe0`
`norrath_ctors_lava_wraith_view_44e2b0`
`norrath_ctors_lava_wraith_view_44f3d8`
`norrath_ctors_lava_wraith_view_44f5b8`
`norrath_ctors_lava_wraith_view_44f798`
`norrath_ctors_lava_wraith_view_44fa38`
`norrath_ctors_lava_wraith_view_44fb28`
`norrath_ctors_lava_wraith_view_44fc18`
`norrath_ctors_lava_wraith_view_44fd08`
`norrath_ctors_lava_wraith_view_44fdf8`
`norrath_ctors_lava_wraith_view_44fee8`
`norrath_ctors_lava_wraith_view_450440`
`norrath_ctors_lava_wraith_view_46cd20`
`norrath_ctors_lava_wraith_view_46cd30`
`norrath_ctors_lava_wraith_view_46cd40`
`norrath_ctors_lava_wraith_view_46cd50`
`norrath_ctors_lava_wraith_view_46cd60`
`norrath_ctors_lava_wraith_view_46cd70`
`norrath_ctors_lava_wraith_view_46d7b8`
`norrath_ctors_lava_wraith_view_470358`
`norrath_ctors_lava_wraith_view_4718e8`
`norrath_ctors_lava_wraith_view_4718f8`
`norrath_ctors_lava_wraith_view_471908`
`norrath_ctors_lava_wraith_view_471918`
`norrath_ctors_lava_wraith_view_471928`
`norrath_ctors_lava_wraith_view_471938`
`norrath_ctors_lava_wraith_view_471948`
`norrath_ctors_lava_wraith_view_471958`

## Build-plan entries needed

One input per row: kind `independently_authored_source`, profile `authored_cpp`,
`header_closure` `[]`, object name `<unit>.o`, source `sources/w9-ctors-wraith-b-views/<unit>.cpp`.
The same hashes belong in `files`. Order is for the integrator. `host/` files are not
plan inputs.

| Unit | sha256 |
|---|---|
| `norrath_ctors_lava_wraith_view_44af90` | `05eba48aaad694ac805c16b63f8cdacc43fe30bc82827515bf94ccbc0d901474` |
| `norrath_ctors_lava_wraith_view_44b080` | `5c8ddd16a06d2a1f5c813a7eb3932fd9dc71f204ea070f99852e2992a55dda90` |
| `norrath_ctors_lava_wraith_view_44b260` | `fdfc5e4265fde740df7076c88d29c500ad64bd125579b4c86ac59a0673b97385` |
| `norrath_ctors_lava_wraith_view_44b350` | `48bd9d8ddebbeb722469a984976567283e98b922d5fe3f70afb6b4d968d2dec6` |
| `norrath_ctors_lava_wraith_view_44b630` | `acd63534a327a62b08b6b05f0f1c77492bb62338817aa5c44a5a643d90a97fcb` |
| `norrath_ctors_lava_wraith_view_44b720` | `8c0866467695e7266ae0eecbcf760c6a202b14c85ab94e6571c9c649699c9ffb` |
| `norrath_ctors_lava_wraith_view_44b810` | `344dbc89fb9828adf335e99fabd937910dd1821031406336a841dd1d61902b2b` |
| `norrath_ctors_lava_wraith_view_44c710` | `38817378a7fa5429afaa7184bec0fe03a65be6d7498370b896e2a35b5ce298c8` |
| `norrath_ctors_lava_wraith_view_44c7a0` | `3b952b9535ead8185229a75703288e0062ed663b00a9cfd7e90aea2f8ecfc45c` |
| `norrath_ctors_lava_wraith_view_44def0` | `56ab4c7afcfe53923511743b2c0db36197fdb952e5fffd17b4a57a0eecb030dc` |
| `norrath_ctors_lava_wraith_view_44dfe0` | `e36b46f35f1c40038091ccef4074292b91072527823ef51a9d357380405c3af7` |
| `norrath_ctors_lava_wraith_view_44e2b0` | `f6a8de461d69ca4bc2edda123d6a737b9ff3b90e92fe818d727966a41deb6e76` |
| `norrath_ctors_lava_wraith_view_44f3d8` | `668c50eeb005106a79f20ea8dcfd54c181253f66ae544cdfd1289b38edcb23e4` |
| `norrath_ctors_lava_wraith_view_44f5b8` | `e9c5b19aa98185723ac229705c000c9ff9b3dc0fc5ca42a39a6e19341da61c65` |
| `norrath_ctors_lava_wraith_view_44f798` | `8193dd3c278d88fda50c9560450889ea4669b3ee578ba4854012c544796f4fb9` |
| `norrath_ctors_lava_wraith_view_44fa38` | `1b7aecc5d7f5a23d65426d116d48f269b011eb5ac55574804133e7204ea48796` |
| `norrath_ctors_lava_wraith_view_44fb28` | `373ee9dfd138ee592c89fe766587091fd5006b0a903950f14a608e01c7626499` |
| `norrath_ctors_lava_wraith_view_44fc18` | `fba79136fa41c63cadff77659ef8d50116d6b31be2e4b72e2d0764eb949e2896` |
| `norrath_ctors_lava_wraith_view_44fd08` | `e055a8ef14ac253b088d19d10681ff312fd37dafc98dd24d20346f9f4b0f9ccd` |
| `norrath_ctors_lava_wraith_view_44fdf8` | `47f2ca98cb87ca47d5f96aa543e74c9e2efb4f60e004adbeaeeca0de5a9b4c52` |
| `norrath_ctors_lava_wraith_view_44fee8` | `b51406b8ab94acf78bea7d350021f2459b31390cff4a4bb571461eddddcfae92` |
| `norrath_ctors_lava_wraith_view_450440` | `d5ba0a735d36a062e22d9dabab5dfb465fdb097d034368b0fa2bd67d4132ae65` |
| `norrath_ctors_lava_wraith_view_46cd20` | `bb8af1b8a4df81ae3755655b8f2d5f68e9a70d765be1103ccafaeb23a74aa43d` |
| `norrath_ctors_lava_wraith_view_46cd30` | `a7b5a2040a47acd876d2309034959bab9f8bd5a529a1ce9698b80e693ebdc566` |
| `norrath_ctors_lava_wraith_view_46cd40` | `b0c45c839efdad72584e58e9df673dd6dcd40bb565a09cd4bcbf2c02626620d2` |
| `norrath_ctors_lava_wraith_view_46cd50` | `d927560f1b4b106e68a0ead8e76e4d03e478d9dc947829ebbb1a046209822734` |
| `norrath_ctors_lava_wraith_view_46cd60` | `f2cd1f5ea237b98dafbb2b064cd9051af8a835028de47e3a0a512e2c791bc60b` |
| `norrath_ctors_lava_wraith_view_46cd70` | `97a7426c0916240a6229b73cd4ca5491515b36264376cc23b5d08a885e6aed27` |
| `norrath_ctors_lava_wraith_view_46d7b8` | `83c957d8e341548074e8f758bd81261853824b6bb56c53b9bac79aa0bb02b9d4` |
| `norrath_ctors_lava_wraith_view_470358` | `960bf36f2312053af10f11015a3c71055083ebb95dfe6598e89e57d083cf859a` |
| `norrath_ctors_lava_wraith_view_4718e8` | `ad14e7b2313c809ed12ddceeff9601903d1739d1a9b0480180805e2e41369cf3` |
| `norrath_ctors_lava_wraith_view_4718f8` | `1324e74aaccaf7e2d76a4bc58ae335d975473597cd8e03b32da32fd9ee646d88` |
| `norrath_ctors_lava_wraith_view_471908` | `b6fd185b959a2d43442c19c4dea858003494181fcc81f35c54a184d5190a0a40` |
| `norrath_ctors_lava_wraith_view_471918` | `01f26038f8a6e692288a192ec9688c158dc6057166c9d7f9868e0146c5bb5ee8` |
| `norrath_ctors_lava_wraith_view_471928` | `4230c464bee679f75bc71d9c63d28fdfaa2cf1856a94b68b1f87f648f2e60d27` |
| `norrath_ctors_lava_wraith_view_471938` | `2c843a3d338a031ffcbedfcb3f58f41520d3418370684d11f2de9cf8c4a5314c` |
| `norrath_ctors_lava_wraith_view_471948` | `89fc7df7e4067cd3ad8ac4c63286171bd9e23c25397107c8ec060e837ffba117` |
| `norrath_ctors_lava_wraith_view_471958` | `54967c9648e7f3fbe7ebfd97cd5ba463a3f3ca0fb13e5551b5886c25ae8d9f9d` |

## Imports opened

Each virtual table names function symbols; those no other package defines become new
function imports (for example the `msg_*`, `clone__`, `setState__`, `setActiveChunks__`
methods of the 22 classes and the shared `Creature` methods). I did not run a merged
partial build, so I have not counted them.

## Authored but never executed

All 38. Host tests only; nothing ran on the EE or in the game, and no full partial link
was done on this branch.

## Observed, inferred, unknown

Observed: table sizes, adjustment words (all zero), the function each slot targets, and
the string texts. Strings `ladddown` and the Orc names are spelled as in the original.

Inferred: the use notes in the PushTrigger, LavaMonster and WebTrap strings come from the
authored constructors that reference them.

Unknown: what dispatch does with the tables; the use of the eight Orc strings
(`0x4718e8`..`0x471958`: no authored unit references them; the Orc constructor reads a
resource table whose payload I did not open); the original placement of any view.

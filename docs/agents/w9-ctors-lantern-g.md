# ant-lantern constructor view data (g)

Package `w9-ctors-lantern-g`, branch `agents/w9-ctors-lantern-g`. It defines 40 small data
views that the authored constructors in `ctors-ant-lantern-constructors` import. One commit
each. Sources are in `experimental/sources/w9-ctors-lantern-g-views/`; host contracts are in
its `host/` directory and are not EE inputs.

Private reference: `work/private/SLUS_205.65`. Addresses are that file's. No game bytes are
committed except the values of these few words and short tag strings.

## Checks performed per view

- Compiled with the reviewed EE g++ and the `authored_cpp` flags from `build-plan.json`.
- Host contract compiled with clang++ `-std=c++17 -Wall -Werror` and run; it fails on a wrong
  bit pattern, string, size or initial value.
- A private script outside git compared the emitted object bytes (or zero fill for `.bss`)
  against the bytes at the same address in the private file. All 40 matched.
- Nothing ran on the EE or in a full partial link; I ran no partial build.

## Evidence

- Observed: 23 `.lit4` words, each a single 4-byte float literal (exact bits written as hex
  floats, so values such as `0x3f199999` that differ from a rounded decimal are kept).
  Callers read some of them as a word and some as a float; three (`48198c`, `481990`,
  `48239c`) hold the same bits `0x3791a2b3`.
- Observed: 14 `.sdata` strings, 8-byte spaced, defined as characters plus NUL only.
  `%s.lmp`, `%s.vif`, `%s.tex`, `%s.anm`, `%f` and tag names such as `wakname`, `rawname`,
  `path`, `oneshot`, `Color`, `Type`, `ant`, `hatecut`, `cellar1`.
- Observed: `483e5c`, `483f88`, `4844c0` are 4-byte `.sdata` words that carry the ELF symbols
  `syncRandState`, `resourceFrame`, `levelFlags`. The first and third are zero, `resourceFrame`
  starts at 1. I defined only the view names; no alias to the ELF names or to
  `norrath_actors_a_view_483e5c` / `norrath_constructor_random_state_view`.
- Inferred: JumpGate treats `483e5c` as a linear-congruential state; Goblin treats `483f88` as
  a frame stamp; Chest/AutoChest treat `4844c0` as level flags. From the callers only.
- Unknown: whether these view names and the ELF/other-package names must be one object. Until
  the integrator decides, the link has two separate state words and the constructors would
  not share state with the rest of the game.

## Imports closed

- `norrath_ctors_ant_lantern_view_4818a8`
- `norrath_ctors_ant_lantern_view_48198c`
- `norrath_ctors_ant_lantern_view_481990`
- `norrath_ctors_ant_lantern_view_481ab4`
- `norrath_ctors_ant_lantern_view_481ab8`
- `norrath_ctors_ant_lantern_view_481abc`
- `norrath_ctors_ant_lantern_view_481ac0`
- `norrath_ctors_ant_lantern_view_481ac4`
- `norrath_ctors_ant_lantern_view_481ac8`
- `norrath_ctors_ant_lantern_view_482328`
- `norrath_ctors_ant_lantern_view_48232c`
- `norrath_ctors_ant_lantern_view_482330`
- `norrath_ctors_ant_lantern_view_482334`
- `norrath_ctors_ant_lantern_view_482338`
- `norrath_ctors_ant_lantern_view_48233c`
- `norrath_ctors_ant_lantern_view_482340`
- `norrath_ctors_ant_lantern_view_48239c`
- `norrath_ctors_ant_lantern_view_482414`
- `norrath_ctors_ant_lantern_view_482418`
- `norrath_ctors_ant_lantern_view_48241c`
- `norrath_ctors_ant_lantern_view_482420`
- `norrath_ctors_ant_lantern_view_4825bc`
- `norrath_ctors_ant_lantern_view_4825c0`
- `norrath_ctors_ant_lantern_view_483e5c`
- `norrath_ctors_ant_lantern_view_483f88`
- `norrath_ctors_ant_lantern_view_4844c0`
- `norrath_ctors_ant_lantern_view_485158`
- `norrath_ctors_ant_lantern_view_485160`
- `norrath_ctors_ant_lantern_view_485168`
- `norrath_ctors_ant_lantern_view_485170`
- `norrath_ctors_ant_lantern_view_485178`
- `norrath_ctors_ant_lantern_view_485180`
- `norrath_ctors_ant_lantern_view_485188`
- `norrath_ctors_ant_lantern_view_485190`
- `norrath_ctors_ant_lantern_view_485198`
- `norrath_ctors_ant_lantern_view_485230`
- `norrath_ctors_ant_lantern_view_485270`
- `norrath_ctors_ant_lantern_view_4852d8`
- `norrath_ctors_ant_lantern_view_4852e0`
- `norrath_ctors_ant_lantern_view_485398`

## Imports opened

None. Every unit is self-contained data with no external reference.

## Build-plan entries needed

Not added to the shared plan. Each needs an `inputs` entry (kind `independently_authored_source`,
profile `authored_cpp`, empty `header_closure`, `object_name` `<name>.o`, `source`
`sources/w9-ctors-lantern-g-views/<name>.cpp`) with an `input_order` assigned by the
integrator, and the same sha256 in `files`. Do not add `host/`.

| Unit | sha256 |
|---|---|
| `norrath_ctors_ant_lantern_view_4818a8` | `8e0a94acb2b4f33fcce2a5b7a12a21a250e73deec0ff19d3f9b7fdea28c5d7ed` |
| `norrath_ctors_ant_lantern_view_48198c` | `61b28a728a91cabe4ccd3514f77b12e9846be0c045788341a950a29ca6ff66ec` |
| `norrath_ctors_ant_lantern_view_481990` | `38d127b5d2f5f3c3383cf7ed58fb4d1b1d8cdd53fcbe4b6f34af9dd08bb7f454` |
| `norrath_ctors_ant_lantern_view_481ab4` | `a899485d430d0fd123fa2a08aeebc83f7bb3d4b7482e182289f62286fc782f4c` |
| `norrath_ctors_ant_lantern_view_481ab8` | `8bd5034651b2fe6c0ad91a5b66e7f9c3bac13aa99be6b06772d62bc45b300ed3` |
| `norrath_ctors_ant_lantern_view_481abc` | `0e7bdb35b7a69a55508269d3009685f4b9db43385f2a9ef869bbaef6fc37aa86` |
| `norrath_ctors_ant_lantern_view_481ac0` | `341ac873316e145f1a710582c0f8efd6fab6012446b9c91d1f4bd5de85b95aac` |
| `norrath_ctors_ant_lantern_view_481ac4` | `483ba2199ae342f4949b1db18e614a18e37f6c1565483b832329ccfa6ef376a7` |
| `norrath_ctors_ant_lantern_view_481ac8` | `234b4ae1a0ad0e6065019696624cdefe90d5763595805518aeba5e713a3b7bc0` |
| `norrath_ctors_ant_lantern_view_482328` | `7ec2c3c4d83d97308143254ad435ca356d501807be57c25c8b85b3bba41c3e09` |
| `norrath_ctors_ant_lantern_view_48232c` | `d297ecb336fd21b9459127da5aab52502c103d8fc8380e7da5997dcd35a94ef1` |
| `norrath_ctors_ant_lantern_view_482330` | `3ad250b835cc9ee230cdc4bc74f21af9670586e310c9f5a456bb65056f0962e8` |
| `norrath_ctors_ant_lantern_view_482334` | `9e682730da4a5ce507be4470c1bd5e8809f4215227765a90b5a97e93e25dd0d5` |
| `norrath_ctors_ant_lantern_view_482338` | `a63ae8869edca4c0f5d4c75a72d3827603e0f5b5cfaba528649119c0de22a3ce` |
| `norrath_ctors_ant_lantern_view_48233c` | `1e8053002a5470cc648070ab1e5a1367d857fca015a64f2364be61fdb2af42b4` |
| `norrath_ctors_ant_lantern_view_482340` | `bbf4bb34359f02510968850236e06efab27aa4fa05ae6a0c6e134e225dd7dbdf` |
| `norrath_ctors_ant_lantern_view_48239c` | `dae299f8c5f3a16ba123a22656295a6e88d6e8ec812937639fdba13fbf6e9db4` |
| `norrath_ctors_ant_lantern_view_482414` | `811afa6c27821554d1ab1fcbbaa1b8c2f132f34a0523f04a0d66d1855a990308` |
| `norrath_ctors_ant_lantern_view_482418` | `da73f3386425f828834160e859fdd763b8f1e57f2a923d629464be481420c5f9` |
| `norrath_ctors_ant_lantern_view_48241c` | `5aeeb39faebd1df8a1b8d19b78f802e046283e20154c734249bb98abbb62490e` |
| `norrath_ctors_ant_lantern_view_482420` | `dc3a0811e1f625cfab933bdbcf4c363f113fb0f83c51b136db13f64ab8f9ef65` |
| `norrath_ctors_ant_lantern_view_4825bc` | `366c35296337d6cb70a63ea3d8af53a5ed2b2f9e18b0157afc16a2b66220e2b9` |
| `norrath_ctors_ant_lantern_view_4825c0` | `5bf3eb49d9f68bea45885848cedc9bdaa0d752789c4302feafd7ee11fa183f20` |
| `norrath_ctors_ant_lantern_view_483e5c` | `b136e964e45fab703eafa3d872e2e8ecae71d410e3b7562a0ea91902aa2f658a` |
| `norrath_ctors_ant_lantern_view_483f88` | `388a95390e4d2d02109e81112f14910b1d93d4f3cb0ce8fcee57943c14af2eae` |
| `norrath_ctors_ant_lantern_view_4844c0` | `baa557483d0e9ed15774269fef85b2c63f090d436f262feaf175a80cea1803f0` |
| `norrath_ctors_ant_lantern_view_485158` | `b65e8670b9edcea37c652514dda86d09a9bf185cbcd29a7f6e02f10744485df2` |
| `norrath_ctors_ant_lantern_view_485160` | `9e9e6e05bb35cef0698db9a552265a24fa286c2445b1c7a63eb07171b1f50a14` |
| `norrath_ctors_ant_lantern_view_485168` | `035e0ff274bb9add67fa5da82212fa46bb57540849af2e88abe4f3e5cd379780` |
| `norrath_ctors_ant_lantern_view_485170` | `c5bf0a9ab942a89397e679b9babd6344e205006c3adec230d51b97874c8b6391` |
| `norrath_ctors_ant_lantern_view_485178` | `0c0a505bd86ff54733e29e3eb355c3ee3c56221b29545cb6ef590f88da0c1fc1` |
| `norrath_ctors_ant_lantern_view_485180` | `8442e2f843a70fca805e5852de1fcaf8307e7b03e122234c11612e6eb8b8f9cb` |
| `norrath_ctors_ant_lantern_view_485188` | `844fecb3c150b21dac0bfc983d36b495840de008fad20f26a9423e98213145c8` |
| `norrath_ctors_ant_lantern_view_485190` | `c2d2ff989f4d58f80bfec8e40728ab38bdd6a436c9dfcde91ef9d982e33d423e` |
| `norrath_ctors_ant_lantern_view_485198` | `4d4a984e8fcb50ba000b008a198155735be0192d12e72d6d3aee3208809e90e1` |
| `norrath_ctors_ant_lantern_view_485230` | `0703cb4814559e97dcb15f3a24ee48b85e09e610d4e7b76960791a3c1fc2bdf2` |
| `norrath_ctors_ant_lantern_view_485270` | `fba06fb8ef5f8f2c7afb79f23e7ccd388c9951d3ad2424790f1a3af867e8f0b8` |
| `norrath_ctors_ant_lantern_view_4852d8` | `6ea7b100de0011a2d421d93812041aaf37955f92648edca57cec21c946411533` |
| `norrath_ctors_ant_lantern_view_4852e0` | `50f4ab59eecb3effca1aefa4c24b508b26bf96ac3e73851fd7dde1dce58b2885` |
| `norrath_ctors_ant_lantern_view_485398` | `e7a0872e8534edfe4a5f858ee01e86fdd35a1b84b5f42dd4d56e6d3342b4fc32` |

Entries as JSON (without `input_order`):

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_4818a8.o",
  "profile": "authored_cpp",
  "sha256": "8e0a94acb2b4f33fcce2a5b7a12a21a250e73deec0ff19d3f9b7fdea28c5d7ed",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_4818a8.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_48198c.o",
  "profile": "authored_cpp",
  "sha256": "61b28a728a91cabe4ccd3514f77b12e9846be0c045788341a950a29ca6ff66ec",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_48198c.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_481990.o",
  "profile": "authored_cpp",
  "sha256": "38d127b5d2f5f3c3383cf7ed58fb4d1b1d8cdd53fcbe4b6f34af9dd08bb7f454",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_481990.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_481ab4.o",
  "profile": "authored_cpp",
  "sha256": "a899485d430d0fd123fa2a08aeebc83f7bb3d4b7482e182289f62286fc782f4c",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_481ab4.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_481ab8.o",
  "profile": "authored_cpp",
  "sha256": "8bd5034651b2fe6c0ad91a5b66e7f9c3bac13aa99be6b06772d62bc45b300ed3",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_481ab8.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_481abc.o",
  "profile": "authored_cpp",
  "sha256": "0e7bdb35b7a69a55508269d3009685f4b9db43385f2a9ef869bbaef6fc37aa86",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_481abc.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_481ac0.o",
  "profile": "authored_cpp",
  "sha256": "341ac873316e145f1a710582c0f8efd6fab6012446b9c91d1f4bd5de85b95aac",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_481ac0.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_481ac4.o",
  "profile": "authored_cpp",
  "sha256": "483ba2199ae342f4949b1db18e614a18e37f6c1565483b832329ccfa6ef376a7",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_481ac4.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_481ac8.o",
  "profile": "authored_cpp",
  "sha256": "234b4ae1a0ad0e6065019696624cdefe90d5763595805518aeba5e713a3b7bc0",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_481ac8.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482328.o",
  "profile": "authored_cpp",
  "sha256": "7ec2c3c4d83d97308143254ad435ca356d501807be57c25c8b85b3bba41c3e09",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482328.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_48232c.o",
  "profile": "authored_cpp",
  "sha256": "d297ecb336fd21b9459127da5aab52502c103d8fc8380e7da5997dcd35a94ef1",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_48232c.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482330.o",
  "profile": "authored_cpp",
  "sha256": "3ad250b835cc9ee230cdc4bc74f21af9670586e310c9f5a456bb65056f0962e8",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482330.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482334.o",
  "profile": "authored_cpp",
  "sha256": "9e682730da4a5ce507be4470c1bd5e8809f4215227765a90b5a97e93e25dd0d5",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482334.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482338.o",
  "profile": "authored_cpp",
  "sha256": "a63ae8869edca4c0f5d4c75a72d3827603e0f5b5cfaba528649119c0de22a3ce",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482338.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_48233c.o",
  "profile": "authored_cpp",
  "sha256": "1e8053002a5470cc648070ab1e5a1367d857fca015a64f2364be61fdb2af42b4",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_48233c.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482340.o",
  "profile": "authored_cpp",
  "sha256": "bbf4bb34359f02510968850236e06efab27aa4fa05ae6a0c6e134e225dd7dbdf",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482340.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_48239c.o",
  "profile": "authored_cpp",
  "sha256": "dae299f8c5f3a16ba123a22656295a6e88d6e8ec812937639fdba13fbf6e9db4",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_48239c.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482414.o",
  "profile": "authored_cpp",
  "sha256": "811afa6c27821554d1ab1fcbbaa1b8c2f132f34a0523f04a0d66d1855a990308",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482414.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482418.o",
  "profile": "authored_cpp",
  "sha256": "da73f3386425f828834160e859fdd763b8f1e57f2a923d629464be481420c5f9",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482418.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_48241c.o",
  "profile": "authored_cpp",
  "sha256": "5aeeb39faebd1df8a1b8d19b78f802e046283e20154c734249bb98abbb62490e",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_48241c.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_482420.o",
  "profile": "authored_cpp",
  "sha256": "dc3a0811e1f625cfab933bdbcf4c363f113fb0f83c51b136db13f64ab8f9ef65",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_482420.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_4825bc.o",
  "profile": "authored_cpp",
  "sha256": "366c35296337d6cb70a63ea3d8af53a5ed2b2f9e18b0157afc16a2b66220e2b9",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_4825bc.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_4825c0.o",
  "profile": "authored_cpp",
  "sha256": "5bf3eb49d9f68bea45885848cedc9bdaa0d752789c4302feafd7ee11fa183f20",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_4825c0.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_483e5c.o",
  "profile": "authored_cpp",
  "sha256": "b136e964e45fab703eafa3d872e2e8ecae71d410e3b7562a0ea91902aa2f658a",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_483e5c.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_483f88.o",
  "profile": "authored_cpp",
  "sha256": "388a95390e4d2d02109e81112f14910b1d93d4f3cb0ce8fcee57943c14af2eae",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_483f88.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_4844c0.o",
  "profile": "authored_cpp",
  "sha256": "baa557483d0e9ed15774269fef85b2c63f090d436f262feaf175a80cea1803f0",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_4844c0.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485158.o",
  "profile": "authored_cpp",
  "sha256": "b65e8670b9edcea37c652514dda86d09a9bf185cbcd29a7f6e02f10744485df2",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485158.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485160.o",
  "profile": "authored_cpp",
  "sha256": "9e9e6e05bb35cef0698db9a552265a24fa286c2445b1c7a63eb07171b1f50a14",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485160.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485168.o",
  "profile": "authored_cpp",
  "sha256": "035e0ff274bb9add67fa5da82212fa46bb57540849af2e88abe4f3e5cd379780",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485168.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485170.o",
  "profile": "authored_cpp",
  "sha256": "c5bf0a9ab942a89397e679b9babd6344e205006c3adec230d51b97874c8b6391",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485170.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485178.o",
  "profile": "authored_cpp",
  "sha256": "0c0a505bd86ff54733e29e3eb355c3ee3c56221b29545cb6ef590f88da0c1fc1",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485178.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485180.o",
  "profile": "authored_cpp",
  "sha256": "8442e2f843a70fca805e5852de1fcaf8307e7b03e122234c11612e6eb8b8f9cb",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485180.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485188.o",
  "profile": "authored_cpp",
  "sha256": "844fecb3c150b21dac0bfc983d36b495840de008fad20f26a9423e98213145c8",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485188.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485190.o",
  "profile": "authored_cpp",
  "sha256": "c2d2ff989f4d58f80bfec8e40728ab38bdd6a436c9dfcde91ef9d982e33d423e",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485190.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485198.o",
  "profile": "authored_cpp",
  "sha256": "4d4a984e8fcb50ba000b008a198155735be0192d12e72d6d3aee3208809e90e1",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485198.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485230.o",
  "profile": "authored_cpp",
  "sha256": "0703cb4814559e97dcb15f3a24ee48b85e09e610d4e7b76960791a3c1fc2bdf2",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485230.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485270.o",
  "profile": "authored_cpp",
  "sha256": "fba06fb8ef5f8f2c7afb79f23e7ccd388c9951d3ad2424790f1a3af867e8f0b8",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485270.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_4852d8.o",
  "profile": "authored_cpp",
  "sha256": "6ea7b100de0011a2d421d93812041aaf37955f92648edca57cec21c946411533",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_4852d8.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_4852e0.o",
  "profile": "authored_cpp",
  "sha256": "50f4ab59eecb3effca1aefa4c24b508b26bf96ac3e73851fd7dde1dce58b2885",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_4852e0.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_485398.o",
  "profile": "authored_cpp",
  "sha256": "e7a0872e8534edfe4a5f858ee01e86fdd35a1b84b5f42dd4d56e6d3342b4fc32",
  "source": "sources/w9-ctors-lantern-g-views/norrath_ctors_ant_lantern_view_485398.cpp"
 }
]
```

## Authored but never executed

The 40 objects were compiled individually and byte-compared to the private image, and the
host contracts ran on the host. None has been linked into the partial build or run on the EE
or in gameplay. Reads through the constructors' `unsigned char[]` extern declarations against
these `float`, `char[]` and `unsigned` definitions rely on C linkage having no type; that
mismatch was not examined by a link.

## Unknown

- Whether the three word views must be unified with their ELF-named objects.
- The source expressions behind each float and which constants are shared literals.
- The exact string sharing the original linker performed.

# w15-fe-mcerr-a: memory-card error window screens and front-end windows

Scope: the unresolved imports listed in the package brief. One function or data object per commit; each
function has a host check (`host_check.cpp`, not a build input) next to its source, and the ProcessScreen
functions were also paired against the original in a private harness kept outside git (see the commit
messages for scenario counts). Nothing here is a byte match unless a commit says so.

## Imports closed (names)
McErrSetMode__Fi, McErr__FP5VIWndRC8VIVect2Ii, ProcessScreen04, 10, 11, 12, 15, 16, 17, 20, 30, 50, 52, 54,
60, 61, 62, 63, 64 (all `__10VIWndMcErriii`), BringUpNetwork__12VIWndConnect, Init__20VIWndGenericRenderer,
Save__13VIGameOptions, SetIcon__16VIIconDefinitionPvi, SetPassword__20VIWndGenericRendererPCc,
SetStationName__20VIWndGenericRendererPCc, SetText__9VIWndEulaPCUsi, SetTitle__16VIIconDefinitionPCc,
Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii, sIconDefinition, sKeyMap, sListIcon,
sMemCardInitialized, sProductTitle.

## Not closed
- `creditsText` (0x00428a20, 2408 bytes): game text. Earlier waves deliberately left it unauthored and the
  integration notes ask for a maintainer decision. Not authored here either.

## Imports opened
- `DrawPrompt__10VIWndMcErriPCUsT2` is authored (commit be51f7b0) and used by several screens; the ProcessScreen
  sources call it, so it needs a plan entry too.
- The screens use `hostlib.hpp` (host-side stubs only) and external calls into the text drawer, string helpers
  and memory-card layer owned by other packages; they stay unresolved imports of this package.

## Build-plan entries needed
One `independently_authored_source`, profile `authored_cpp` entry per source file, each listing the
function it emits; the integrator computes sha256, input order and compiled size. Sources (relative to
`experimental/`):
- sources/w15-fe-mcerr-a-mcerr-open/mcerr.cpp, -setmode/setmode.cpp, -screen04, -screen10, -screen11, -screen12,
  -screen15, -screen16, -screen17, -screen20, -screen30, -screen50, -screen52, -screen54, -screen60, -screen61,
  -screen62, -screen63, -screen64 (each `screenNN.cpp`), -drawprompt/drawprompt.cpp
- sources/w15-fe-mcerr-a-connect-bringupnetwork/bringup.cpp, -eula-settext/settext.cpp,
  -gameoptions-save/save.cpp, -icon-seticon/seticon.cpp, -icon-settitle/settitle.cpp,
  -message-setup/setup.cpp, -renderer-init/init.cpp, -renderer-setpassword/setpassword.cpp,
  -renderer-setstationname/setstationname.cpp
- data: -data-icon-definition/icon_definition.cpp, -data-key-map/key_map.cpp,
  -data-list-icon-reservation/list_icon.cpp, -data-mem-card-initialized/initialized.cpp,
  -data-product-title/product_title.cpp

## Authored but never executed
Everything: no code was run on the EE or in the game. Host checks run on the build machine against stubs;
DrawPrompt has only a host check (no paired run). Data objects were not compared byte-for-byte except where
their commit says so; sListIcon is a 118232-byte reservation whose contents are not published.

## Unknown
- What the "|q" and "|w" markers mean to the text drawer, the TextBox integer arguments beyond register order,
  field names of VIWndMcErr, and the exact type of the highlight flag.
- The contents of creditsText as a publishable table (see above).

add_library (lua-lib STATIC)
add_executable (lua)
add_executable (luac)

target_link_libraries (lua PRIVATE lua-lib)
target_link_libraries (luac PRIVATE lua-lib)

target_sources (lua-lib PRIVATE
    "src/lapi.c"
    "src/lapi.h"
    "src/lauxlib.c"
    "src/lauxlib.h"
    "src/lbaselib.c"
    "src/lcode.c"
    "src/lcode.h"
    "src/lcorolib.c"
    "src/lctype.c"
    "src/lctype.h"
    "src/ldblib.c"
    "src/ldebug.c"
    "src/ldebug.h"
    "src/ldo.c"
    "src/ldo.h"
    "src/ldump.c"
    "src/lfunc.c"
    "src/lfunc.h"
    "src/lgc.c"
    "src/lgc.h"
    "src/linit.c"
    "src/liolib.c"
    "src/ljumptab.h"
    "src/llex.c"
    "src/llex.h"
    "src/llimits.h"
    "src/lmathlib.c"
    "src/lmem.c"
    "src/lmem.h"
    "src/loadlib.c"
    "src/lobject.c"
    "src/lobject.h"
    "src/lopcodes.c"
    "src/lopcodes.h"
    "src/lopnames.h"
    "src/loslib.c"
    "src/lparser.c"
    "src/lparser.h"
    "src/lprefix.h"
    "src/lstate.c"
    "src/lstate.h"
    "src/lstring.c"
    "src/lstring.h"
    "src/lstrlib.c"
    "src/ltable.c"
    "src/ltable.h"
    "src/ltablib.c"
    "src/ltm.c"
    "src/ltm.h"
    "src/lua.h"
    "src/luaconf.h"
    "src/lualib.h"
    "src/lundump.c"
    "src/lundump.h"
    "src/lutf8lib.c"
    "src/lvm.c"
    "src/lvm.h"
    "src/lzio.c"
    "src/lzio.h"
)

target_include_directories(lua-lib PUBLIC
    "include"
)

target_sources (lua PRIVATE
    "src/lua.c"
)

target_sources (luac PRIVATE
    "src/luac.c"
)

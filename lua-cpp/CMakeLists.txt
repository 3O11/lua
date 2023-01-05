add_library (lua-cpp STATIC)

target_sources (lua-cpp PRIVATE
    "Include/LibTools.hpp"
    "Include/LuaRuntime.hpp"
    "Source/LuaRuntime.cpp"
)

target_include_directories (lua-cpp PUBLIC
    "Include"
)

target_link_libraries(lua-cpp PRIVATE
    lua-lib
)

set_property (TARGET lua-cpp PROPERTY CXX_STANDARD 20)

if (PROJECT_IS_TOP_LEVEL)
    if (MSVC)
        target_compile_options (lua-cpp PRIVATE /W4)
    else ()
        target_compile_options (lua-cpp PRIVATE -Wall -Wextra -Wpedantic)
    endif ()
endif ()

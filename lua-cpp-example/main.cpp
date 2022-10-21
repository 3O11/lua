#include <lua-cpp.hpp>
#include <lua-utils.hpp>

int main(int argc, char ** argv)
{
    LuaInstance l;

    l.Register("example", [](lua_State* L) -> int
    {
        auto[a, b] = LuaUtils::GetArgs<double, int64_t>(L);

        return LuaUtils::Return<double, std::string>(L, { a, std::to_string(b) });
    });

    l.SetGlobal("VERSION", std::string("VERSION"));
    l.SetGlobal("V", (int64_t)4500);

    std::string version;
    l.GetGlobal("VERSION", version);
    printf("%s\n", version.c_str());

    int64_t v;
    l.GetGlobal("V", v);
    printf("%lld\n", v);

    if (argc > 1)
    {
        l.DoFile(argv[1]);
    }
    if (argc > 2)
    {
        l.DoFile(argv[2]);
    }
}

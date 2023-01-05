#include "LuaRuntime.hpp"

#include <iostream>
#include "LibTools.hpp"

namespace Lua
{
    Runtime::Runtime()
        : mL(luaL_newstate())
    {}

    Runtime::~Runtime()
    {
        if (mL)
        {
            lua_close(mL);
        }
    }

    void Runtime::Restart()
    {
        if (mL)
        {
            lua_close(mL);
        }
        mL = luaL_newstate();
    }

    void Runtime::OpenLibs()
    {
        luaL_openlibs(mL);
    }

    void Runtime::Register(const Function& func)
    {
        lua_register(mL, func.GetName().c_str(), func.GetFunc());
    }

    void Runtime::Register(const Library& lib)
    {
        lua_newtable(mL);

        for (auto&& func : lib.GetFuncs())
        {
            lua_pushstring(mL, func.GetName().c_str());
            lua_pushcfunction(mL, func.GetFunc());
            lua_settable(mL, -3);
        }

        lua_setglobal(mL, lib.GetName().c_str());
    }

    bool Runtime::DoString(const std::string& script)
    {
        if (luaL_dostring(mL, script.c_str()))
        {
            std::cout << lua_tostring(mL, -1);
            lua_pop(mL, 1);
            return false;
        }
        return true;
    }

    bool Runtime::DoFile(const std::string& path)
    {
        if (luaL_dofile(mL, path.c_str()))
        {
            std::cout << lua_tostring(mL, -1);
            lua_pop(mL, 1);
            return false;
        }
        return true;
    }

    lua_State *Runtime::GetRawState()
    {
        return mL;
    }

    lua_State *Runtime::ReleaseRawState()
    {
        lua_State *L = mL;
        mL = 0;
        return L;
    }
}

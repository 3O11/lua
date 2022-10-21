#include "lua-cpp.hpp"
#include <lua.hpp>

#include <cstdio>

LuaInstance::LuaInstance()
    : m_L(luaL_newstate())
{
    luaL_openlibs(m_L);
}

LuaInstance::~LuaInstance()
{
    if (m_L)
    {
        lua_close(m_L);
    }
}

void LuaInstance::Restart()
{
    if (m_L)
    {
        lua_close(m_L);
    }
    m_L = luaL_newstate();
}

void LuaInstance::Register(const std::string& name, lua_CFunction func)
{
    lua_register(m_L, name.c_str(), func);
}

void LuaInstance::SetGlobal(const std::string& name, const std::string& value)
{
    lua_pushstring(m_L, value.c_str());
    lua_setglobal(m_L, name.c_str());
}

void LuaInstance::SetGlobal(const std::string& name, int64_t value)
{
    lua_pushinteger(m_L, value);
    lua_setglobal(m_L, name.c_str());
}

void LuaInstance::SetGlobal(const std::string& name, double value)
{
    lua_pushnumber(m_L, value);
    lua_setglobal(m_L, name.c_str());
}

void LuaInstance::SetGlobal(const std::string& name, bool value)
{
    lua_pushboolean(m_L, value);
    lua_setglobal(m_L, name.c_str());
}

bool LuaInstance::GetGlobal(const std::string& name, std::string& value)
{
    lua_getglobal(m_L, name.c_str());
    if (lua_isstring(m_L, -1))
    {
        value = lua_tostring(m_L, -1);
        lua_pop(m_L, 1);
        return true;
    }
    else
    {
        return false;
    }
}

bool LuaInstance::GetGlobal(const std::string& name, int64_t& value)
{
    lua_getglobal(m_L, name.c_str());
    if (lua_isinteger(m_L, -1))
    {
        value = lua_tointeger(m_L, -1);
        lua_pop(m_L, 1);
        return true;
    }
    else
    {
        return false;
    }
}

bool LuaInstance::GetGlobal(const std::string& name, double& value)
{
    lua_getglobal(m_L, name.c_str());
    if (lua_isnumber(m_L, -1))
    {
        value = lua_tonumber(m_L, -1);
        lua_pop(m_L, 1);
        return true;
    }
    else
    {
        return false;
    }
}

bool LuaInstance::GetGlobal(const std::string& name, bool& value)
{
    lua_getglobal(m_L, name.c_str());
    if (lua_isboolean(m_L, -1))
    {
        value = lua_toboolean(m_L, -1);
        lua_pop(m_L, 1);
        return true;
    }
    else
    {
        return false;
    }
}


bool LuaInstance::DoString(const std::string& code)
{
    if (luaL_dostring(m_L, code.c_str()))
    {
        fprintf(stderr, "%s\n", lua_tostring(m_L, -1));
        lua_pop(m_L, 1);
        return false;
    }

    return true;
}

bool LuaInstance::DoFile(const std::string& filename)
{
    if (luaL_dofile(m_L, filename.c_str()))
    {
        fprintf(stderr, "%s\n", lua_tostring(m_L, -1));
        lua_pop(m_L, 1);
        return false;
    }

    return true;
}

lua_State* LuaInstance::GetRawState()
{
    return m_L;
}

lua_State* LuaInstance::ReleaseRawState()
{
    lua_State* temp = m_L;
    m_L = nullptr;
    return temp;
}

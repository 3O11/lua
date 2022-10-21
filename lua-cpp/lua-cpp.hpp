#ifndef _LUA_CPP_HPP
#define _LUA_CPP_HPP

#include <cstdint>
#include <string>
#include <optional>

struct lua_State;
typedef int (*lua_CFunction) (lua_State *L);

class LuaInstance
{
public:
    LuaInstance();
    ~LuaInstance();

    void Restart();

    void Register(const std::string& name, lua_CFunction func);

    void SetGlobal(const std::string& name, const std::string& value);
    void SetGlobal(const std::string& name, int64_t value);
    void SetGlobal(const std::string& name, double value);
    void SetGlobal(const std::string& name, bool value);
    bool GetGlobal(const std::string& name, std::string& value);
    bool GetGlobal(const std::string& name, int64_t& value);
    bool GetGlobal(const std::string& name, double& value);
    bool GetGlobal(const std::string& name, bool& value);

    bool DoString(const std::string& code);
    bool DoFile(const std::string& filename);

    lua_State* GetRawState();
    lua_State* ReleaseRawState();
private:
    lua_State * m_L;
};

#endif //_LUA_CPP_HPP

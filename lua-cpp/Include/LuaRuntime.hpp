#ifndef _LUA_RUNTIME_HPP
#define _LUA_RUNTIME_HPP

#include <string>
#include <vector>

struct lua_State;
typedef int (*lua_CFunction) (lua_State *L);

namespace Lua
{
    class Function
    {
    public:
        explicit Function(const std::string& name, lua_CFunction func)
            : mName(name), mLuaFunc(func)
        {}

        const std::string&  GetName() const { return mName; }
        lua_CFunction       GetFunc() const { return mLuaFunc; }
    private:
        std::string   mName;
        lua_CFunction mLuaFunc;
    };

    class Library
    {
    public:
        explicit Library(const std::string& name)
            : mName(name), mFunctions()
        {}

        void AddFunction(const Function& func)
        {
            mFunctions.push_back(func);
        }

        const std::string&           GetName()  const { return mName; }
        const std::vector<Function>& GetFuncs() const { return mFunctions; }
    private:
        std::string           mName;
        std::vector<Function> mFunctions;
    };

    class Runtime
    {
    public:
        explicit Runtime();
        ~Runtime();

        Runtime(Runtime&&) noexcept = default;
        Runtime& operator=(Runtime&&) noexcept = default;
        Runtime(const Runtime&) = delete;
        Runtime& operator=(Runtime&) = delete;

        void Restart();
        void OpenLibs();

        void Register(const Function& func);
        void Register(const Library& lib);

        bool DoString(const std::string& script);
        bool DoFile(const std::string& path);

        lua_State *GetRawState();
        lua_State *ReleaseRawState();
    private:
        lua_State *mL;
    };
}

#endif //_LUA_RUNTIME_HPP

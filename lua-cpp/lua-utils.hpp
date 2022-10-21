#ifndef _LUA_UTILS_HPP
#define _LUA_UTILS_HPP

#include <lua.hpp>

#include <cstdint>
#include <utility>
#include <string>
#include <type_traits>

namespace LuaUtils
{
    namespace detail
    {
        template <size_t I, typename T>
        std::remove_cvref_t<T> ParseArg(lua_State* L)
        {
            if constexpr (std::is_same_v<T, double>)
            {
                return luaL_checknumber(L, I);
            }
            else if constexpr (std::is_same_v<T, int64_t>)
            {
                return luaL_checkinteger(L, I);
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return std::string(luaL_checkstring(L, I));
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
            {
                if (!lua_isboolean(L, I))
                {
                    luaL_error(L, "Not a boolean!");
                }
                return lua_toboolean(L, I);
            }
            else if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>)
            {
                if (!lua_islightuserdata(L, I))
                {
                    luaL_error(L, "Not light userdata!");
                }
                return static_cast<std::remove_cvref_t<T>>(lua_touserdata(L, I));
            }
        }
        
        template <typename ... Pack, size_t ... Is>
        std::tuple<Pack ...> GetArgsImpl(lua_State* L, std::index_sequence<Is ...>)
        {
            return std::make_tuple(ParseArg<Is + 1, Pack>(L)...);
        }

        template <typename T>
        int ReturnValue(lua_State* L, T&& value)
        {
            if constexpr (std::is_same_v<std::remove_cvref_t<T>, double>)
            {
                lua_pushnumber(L, value);
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, int64_t>)
            {
                lua_pushinteger(L, value);
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::string>)
            {
                lua_pushstring(L, value.c_str());
            }
            else if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
            {
                lua_pushboolean(L, value);
            }
            else if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>)
            {
                lua_pushlightuserdata(L, value);
            }

            return 0;
        }

        template <typename ... Pack, size_t ... Is>
        void ReturnAll(lua_State* L, Pack ... values)
        {
            int unused[] = {ReturnValue(L, values)...};
            (void)unused;
        }

        template <typename ... Pack, size_t ... Is>
        void ReturnImpl(lua_State* L, std::tuple<Pack ...> values, std::index_sequence<Is ...>)
        {
            ReturnAll(L, std::get<Is>(values)...);
        }
    }

    template <typename ... Pack>
    std::tuple<Pack ...> GetArgs(lua_State* L)
    {
        if (lua_gettop(L) != sizeof...(Pack))
        {
            luaL_error(L, "Incorrect argument count!\n");
            return std::tuple<Pack ...>();
        }

        return detail::GetArgsImpl<Pack ...>(L, std::make_index_sequence<sizeof...(Pack)>());
    }

    template <typename ... Pack>
    int Return(lua_State* L, std::tuple<Pack ...> values)
    {
        detail::ReturnImpl(L, values, std::make_index_sequence<sizeof...(Pack)>());
        return sizeof...(Pack);
    }
}

#endif //_LUA_UTILS_HPP

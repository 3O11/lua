#ifndef _LUA_LIB_TOOLS_HPP
#define _LUA_LIB_TOOLS_HPP

#include <utility>
#include <type_traits>
#include <vector>
#include <unordered_map>

#include <lua.hpp>

namespace cc::LibUtils
{
    namespace detail
    {
        template<typename Type, template<typename ...> class Template>
        struct is_specialization : std::false_type {};

        template<template<typename ...> class Template, typename ... Args>
        struct is_specialization<Template<Args ...>, Template>: std::true_type {};

        template<typename Type, template<typename ...> class Template>
        constexpr bool is_specialization_v = is_specialization<Type, Template>::value;

        template <typename T>
        struct get_params;

        template <template <typename ...> class Template, typename ... Pack>
        struct get_params<Template<Pack ...>>
        {
            static constexpr std::size_t value = sizeof...(Pack);

            template <std::size_t N>
            using pack = typename std::tuple_element<N, std::tuple<Pack...>>::type;
        };

        template <typename ... Pack>
        void ReturnAll(lua_State *L, Pack ... values)
        {
            int unused[] = {Push(L, values)...};
            (void)unused;
        }

        template <typename ... Pack, size_t ... Is>
        void ReturnImpl(lua_State *L, std::tuple<Pack ...> values, std::index_sequence<Is ...>)
        {
            ReturnAll<Pack ...>(L, std::get<Is>(values)...);
        }
    }

    template <typename T>
    int Push(lua_State *L, T&& value)
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
        else if constexpr (std::is_same_v<std::remove_cvref_t<T>, const char*>)
        {
            lua_pushstring(L, value);
        }
        else if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
        {
            lua_pushboolean(L, value);
        }
        else if constexpr (detail::is_specialization_v<std::remove_cvref_t<T>, std::vector>)
        {
            lua_newtable(L);
            for (int64_t i = 0; static_cast<size_t>(i) < value.size(); ++i)
            {
                lua_pushinteger(L, i + 1);
                Push(L, value[i]);
                lua_settable(L, -3);
            }
        }
        else if constexpr (detail::is_specialization_v<std::remove_cvref_t<T>, std::unordered_map>)
        {
            lua_newtable(L);
            for (auto&& [key, val] : value)
            {
                Push(L, key);
                Push(L, val);
                lua_settable(L, -3);
            }
        }
        else if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>)
        {
            lua_pushlightuserdata(L, reinterpret_cast<void *>(value));
        }

        return 0;
    }

    template <typename T>
    std::remove_cvref_t<T> Top(lua_State *L)
    {
        if constexpr (std::is_same_v<T, double>)
        {
            double value = luaL_checknumber(L, -1);
            return value;
        }
        else if constexpr (std::is_same_v<T, int64_t>)
        {
            double value = luaL_checkinteger(L, -1);
            return value;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            std::string value = luaL_checkstring(L, -1);
            return value;
        }
        else if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
        {
            if (!lua_isboolean(L, -1))
            {
                luaL_error(L, "expected a boolean");
            }
            bool value = lua_toboolean(L, -1);
            return value;
        }
        else if constexpr (detail::is_specialization_v<std::remove_cvref_t<T>, std::vector>)
        {
            using Ta = typename detail::get_params<std::remove_cvref_t<T>>::template pack<0>;
            std::vector<Ta> value;
            size_t index = 0;
            value.resize(lua_rawlen(L, -1));

            lua_pushnil(L);
            while (lua_next(L, -2))
            {
                value[index++] = Pop<Ta>(L);
            }

            return value;
        }
        else if constexpr (detail::is_specialization_v<std::remove_cvref_t<T>, std::unordered_map>)
        {
            using Ta = typename detail::get_params<std::remove_cvref_t<T>>::template pack<0>;
            using Tb = typename detail::get_params<std::remove_cvref_t<T>>::template pack<1>;
            std::unordered_map<Ta, Tb> value;

            lua_pushnil(L);
            while(lua_next(L, -2))
            {
                Tb val = Pop<Tb>(L);
                value[Top<Ta>(L)] = val;
            }

            return value;
        }
        else if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>)
        {
            if (!lua_islightuserdata(L, -1))
            {
                luaL_error(L, "expected light userdata");
            }
            void * value = lua_touserdata(L, -1);
            return reinterpret_cast<std::remove_cvref_t<T>>(value);
        }
    }

    template <typename T>
    bool Top(lua_State *L, T& value)
    {
        int success = 0;

        if constexpr (std::is_same_v<std::remove_cvref_t<T>, double>)
        {
            if (lua_isnumber(L, -1))
            {
                value = lua_tonumber(L, -1);
                success = true;
            }
        }
        else if constexpr (std::is_same_v<std::remove_cvref_t<T>, int64_t>)
        {
            if (lua_isinteger(L, -1))
            {
                value = lua_tointeger(L, -1);
                success = true;
            }
        }
        else if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::string>)
        {
            if (lua_isstring(L, -1))
            {
                value = lua_tostring(L, -1);
                success = true;
            }
        }
        else if constexpr (std::is_same_v<std::remove_cvref_t<T>, bool>)
        {
            if (lua_isboolean(L, -1))
            {
                value = lua_toboolean(L, -1);
                success = true;
            }
        }
        else if constexpr (detail::is_specialization_v<std::remove_cvref_t<T>, std::vector>)
        {
            using Ta = typename detail::get_params<std::remove_cvref_t<T>>::template pack<0>;
            value.resize(lua_rawlen(L, -1));
            size_t index = 0;
            lua_pushnil(L);
            while (lua_next(L, -2))
            {
                if (!Pop<Ta>(L, value[index++]))
                {
                    lua_pop(L, 2);
                    return false;
                }
            }
        }
        else if constexpr (detail::is_specialization_v<std::remove_cvref_t<T>, std::unordered_map>)
        {
            using Ta = typename detail::get_params<std::remove_cvref_t<T>>::template pack<0>;
            using Tb = typename detail::get_params<std::remove_cvref_t<T>>::template pack<1>;
            lua_pushnil(L);
            while(lua_next(L, -2))
            {
                Tb val;
                if (!Pop<Tb>(L, val))
                {
                    return false;
                }

                Ta key;
                if (!Top<Ta>(L, key))
                {
                    return false;
                }

                value[key] = val;
            }
        }
        else if constexpr (std::is_pointer_v<std::remove_cvref_t<T>>)
        {
            if (lua_islightuserdata(L, -1))
            {
                value = reinterpret_cast<T>(lua_touserdata(L, -1));
                success = true;
            }
        }

        return success;
    }

    template <typename T>
    T Pop(lua_State *L)
    {
        std::remove_cvref_t<T> value = Top<T>(L);
        lua_pop(L, 1);
        return value;
    }

    template <typename T>
    bool Pop(lua_State *L, T& value) noexcept
    {
        if (Top(L, value))
        {
            lua_pop(L, 1);
            return true;
        }

        return false;
    }

    template <typename ... Pack>
    std::tuple<Pack ...> PopArgs(lua_State* L)
    {
        if (lua_gettop(L) != sizeof...(Pack))
        {
            luaL_error(L, "Incorrect argument count!\n");
            return std::tuple<Pack ...>();
        }

        return std::make_tuple(Pop<Pack>(L)...);
    }

    template <typename ... Pack>
    int Return(lua_State* L, std::tuple<Pack ...> values)
    {
        detail::ReturnImpl(L, values, std::make_index_sequence<sizeof...(Pack)>());
        return sizeof...(Pack);
    }
}

#endif //_LUA_LIB_TOOLS_HPP

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

extern const char _binary_Assembler_lua_start[];
extern const char _binary_Assembler_lua_end[];

int main(int argc, char **argv) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Calculate length from the pointer addresses
    size_t size = _binary_Assembler_lua_end - _binary_Assembler_lua_start;

	lua_createtable(L, argc, 0);

    for (int i = 0; i < argc; i++) {
        lua_pushstring(L, argv[i]);
        lua_rawseti(L, -2, i);
    }

    lua_setglobal(L, "arg");

    // Load from memory
    if (luaL_loadbuffer(L, _binary_Assembler_lua_start, size, "embedded_script") == LUA_OK) {
        lua_pcall(L, 0, LUA_MULTRET, 0);
    } else {
        printf("Error: %s\n", lua_tostring(L, -1));
    }

    lua_close(L);
    return 0;
}
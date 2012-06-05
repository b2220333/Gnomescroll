#pragma once

int run_lua_test();

int lua_load_block_dat();

namespace LUA
{

void set_options_file(char* path);

void load_options();

void register_int_option(const char* name, int* var);
void register_uint_option(const char* name, unsigned int* var);
void register_bool_option(const char* name, bool* var);
void register_float_option(const char* name, float* var);
void register_string_option(const char* name, char** var);

}

extern "C"
{
    void LUA_set_int_option(int option_id, int value)  GNOMESCROLL_API;
    void LUA_set_uint_option(int option_id, unsigned int value)  GNOMESCROLL_API;
    void LUA_set_bool_option(int option_id, int value)  GNOMESCROLL_API;
    void LUA_set_float_option(int option_id, float value)  GNOMESCROLL_API;
    void LUA_set_string_option(int option_id, char* value)  GNOMESCROLL_API;
}


#if PRODUCTION
//stuff
#endif
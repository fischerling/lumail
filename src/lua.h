/*
 * lua.h - Wrapper for the lua interpreter.
 *
 * This file is part of lumail - http://lumail.org/
 *
 * Copyright (c) 2015 by Steve Kemp.  All rights reserved.
 *
 **
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 dated June, 1991, or (at your
 * option) any later version.
 *
 * On Debian GNU/Linux systems, the complete text of version 2 of the GNU
 * General Public License can be found in `/usr/share/common-licenses/GPL-2'
 */


#pragma once


extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <vector>
#include <string>

#include "observer.h"
#include "singleton.h"

/**
 * A singleton class holding our Lua interpreter state.
 *
 * This class also implements the observer-pattern, responding to
 * changes in the `CConfig` class.
 *
 */
class CLua : public Singleton<CLua>, public Observer
{
public:
    CLua();
    ~CLua();

public:

    /**
     * Populate "args" - the command-line arguments which are exposed to Lua.
     */
    void set_args(char *argv[], int argc);

    /**
     * Load the specified Lua file, and evaluate it.
     *
     * If there is an error loading the file then the program will abort,
     * and the error message will be shown to the user.
     */
    void load_file(std::string filename);

    /**
     * Evaluate the given string.
     *
     * Return true on success.  False on error.
     */
    bool execute(std::string lua);

    /**
     * Lookup a key in a nested table structure - used for keyboard lookups.
     */
    char *get_nested_table(std::string table, const char *key, const char *subkey);

    /**
     * Call `on_complete` to complete a string.
     */
    std::vector<std::string> get_completions(std::string token);

    /**
     * This method is called when a configuration key changes,
     * via our observer implementation.
     */
    void update(std::string key_name);

    /**
     * Call a Lua function which will return a table of text.
     */
    std::vector<std::string> function2table(std::string function);

    /**
     * Call the user "on_error" function with given error message.
     */
    void on_error(std::string msg);

    /**
     * Return the (string) contents of a variable.
     * Used for our test suite only.
     */
    std::string get_variable(std::string name);

private:

    /**
     * The handle to the Lua interpreter.
     */
    lua_State * m_lua;

};

#pragma once
#include <stdint.h>

enum dvar_flags : uint16_t
{
    none = 0x0,
    saved = 0x1,
    user_info = 0x2, // sent to server on connect or change
    server_info = 0x4, // sent in response to front end requests
    replicated = 0x8,
    write_protected = 0x10,
    latched = 0x20,
    read_only = 0x40,
    cheat_protected = 0x80,
    temp = 0x100,
    no_restart = 0x400, // do not clear when a cvar_restart is issued
    user_created = 0x4000, // created by a set command
};

enum class dvar_type : int8_t
{
    boolean = 0,
    decimal = 1,
    vec2 = 2,
    vec3 = 3,
    vec4 = 4,
    integer = 5,
    enumeration = 6,
    string = 7,
    color = 8,
    rgb = 9 // Color without alpha
};
constexpr const char* dvar_types[] = { "boolean", "float", "2d vector", "3d vector", "4d vector", "integer", "enumeration", "string", "color", "rgb"};

union dvar_value
{
    bool enabled;
    int integer;
    unsigned int unsignedInt;
    float decimal;
    float vector[4];
    const char* string;
    char color[4];
};

union dvar_limits
{
    struct
    {
        int stringCount;
        const char** strings;
    } enumeration;

    struct
    {
        int min;
        int max;
    } integer;

    struct
    {
        float min;
        float max;
    } decimal;

    struct
    {
        float min;
        float max;
    } vector;
};

struct dvar_t
{
    const char* name;
    const char* description;
    dvar_flags flags; // short
    dvar_type type; // char
    bool modified;
    dvar_value current;
    dvar_value latched;
    dvar_value reset;
    dvar_limits domain;
    bool(__cdecl* domainFunc)(dvar_t*, dvar_value);
    dvar_t* hashNext;


    bool toggle_flag(const dvar_flags flag)
    {
        if (this->flags & flag)
            (*(uint16_t*)&this->flags) &= ~(uint16_t)flag;
        else
            (*(uint16_t*)&this->flags) |= (uint16_t)flag;
        return this->flags & flag;
    }
};

inline dvar_t* Dvar_Find(const char* name)
{
    constexpr unsigned long addr = 0x56b5d0;
    __asm
    {
        mov edi, name
        call[addr]
    }
}
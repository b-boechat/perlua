/*#include <string>
#include <iostream>
#include <fstream> // open
#include <EXTERN.h>
#include <perl.h>

#include <vector>
#include <math.h> // floor
#include "data.h"

#define SEPARATOR '\003'
*/

// TODO Remove this later.

#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED 1

#include <string>
#include <iostream>

enum LuaDataType {luaString, luaNumber, luaBoolean, luaNilType}; // Represents implemented Lua types.
enum LuaSpecialValue {luaFalse, luaTrue, luaNilValue}; // Represents Lua special values.

// TODO Declarei aqui so pra conseguir colocar nos return values do LuaData.
class LuaBoolean;

class LuaData {
    // Defines the base class for Lua data types, from which all data types inherit.
    public:
        virtual ~LuaData() {};
        virtual LuaDataType get_type() = 0;
        virtual LuaBoolean* is_truthy() = 0;
        virtual LuaSpecialValue eval_boolean() { std::cout << "Should not be here! alsf3f3dsd2" << std::endl; exit(1); return luaFalse; }
        virtual std::string eval_string() { std::cout << "Should not be here! fdsdsksfdk2" << std::endl; exit(1); return ""; }
        virtual double eval_number() { std::cout << "RuntimeError: Expected number!" << std::endl; exit(1); return 0; }
        virtual LuaSpecialValue eval_nil() { std::cout << "Should not be here! lc0wqcsd2" << std::endl; exit(1); return luaNilValue; }
};
class LuaBoolean : public LuaData {
    // Defines the boolean Lua type.
    // TODO: Boolean is defined first here so that "is_truthy" in other types don't result in a compile error. When using headers, move it to after LuaNumber.
    public:
        LuaBoolean(LuaSpecialValue value_);
        LuaSpecialValue eval_boolean() override;
        LuaDataType get_type() override;
        // Truthiness of a boolean type evaluates to a copy of itself, as expected.
        LuaBoolean* is_truthy() override;
    private:
        LuaDataType type;
        LuaSpecialValue value;
};

class LuaString : public LuaData {
    // Defines the string Lua type.
    public:
        LuaString(std::string value_);
        std::string eval_string() override;
        LuaDataType get_type() override;
        // In Lua, all strings are considered "truthy" (including empty string).
        LuaBoolean* is_truthy() override;
    private:
        LuaDataType type;
        std::string value;
};

class LuaNumber : public LuaData {
    // Defines the number Lua type.
    public:
        LuaNumber(double value_);
        double eval_number() override;
        LuaDataType get_type() override;
        // In Lua, all numbers are considered "truthy" (including number 0).
        LuaBoolean* is_truthy() override;
    private:
        LuaDataType type;
        double value;
};

class LuaNil : public LuaData {
    // Defines the nil Lua type.
    public:
        LuaNil();
        LuaSpecialValue eval_nil() override;
        LuaDataType get_type() override;
        // A nil value is considered "falsy" in Lua.
        LuaBoolean* is_truthy() override;
    private:
        LuaDataType type;
        LuaSpecialValue value;
};

#endif

#include <string>
#include "data.h"

// TODO Remove this later.
using namespace std;


// ===== LuaBoolean =====
LuaBoolean::LuaBoolean(LuaSpecialValue value_) {
    type = luaBoolean;
    value = value_;
}
LuaSpecialValue LuaBoolean::eval_boolean() { return value; }
LuaDataType LuaBoolean::get_type() { return type; }
// Truthiness of a boolean type evaluates to a copy of itself, as expected.
LuaBoolean* LuaBoolean::is_truthy() { return new LuaBoolean(value); }
// ==========

// ===== LuaString =====

LuaString::LuaString(string value_) {
    type = luaString;
    value = value_;
}
string LuaString::eval_string() { return value; }
LuaDataType LuaString::get_type() { return type; }
// In Lua, all strings are considered "truthy" (including empty string).
LuaBoolean* LuaString::is_truthy() { return new LuaBoolean(luaTrue); }
// ==========

// ===== LuaNumber =====
LuaNumber::LuaNumber(double value_) {
    type = luaNumber;
    value = value_;
}
double LuaNumber::eval_number() { return value; }
LuaDataType LuaNumber::get_type() { return type; }
// In Lua, all numbers are considered "truthy" (including number 0).
LuaBoolean* LuaNumber::is_truthy() { return new LuaBoolean(luaTrue); }
// ==========

// ===== LuaNil =====
LuaNil::LuaNil() {
    type = luaNilType;
    value = luaNilValue;
}
LuaSpecialValue LuaNil::eval_nil() { return value; }
LuaDataType LuaNil::get_type() { return type; }
// A nil value is considered "falsy" in Lua.
LuaBoolean* LuaNil::is_truthy() { return new LuaBoolean(luaFalse); }
// ==========

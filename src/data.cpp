#include <string>
#include "data.h"

using namespace std;


Data::Data() : type(NIL) {} // Nil doesn't need a explicit value, since it only has one.

Data::Data(double num_) : type(NUMBER) {
    value.lua_num = num_;
}

Data::Data(size_t num_) : type(NUMBER) {
    value.lua_num = static_cast<double>(num_);
}

Data::Data(const char* str_) : type(STRING) {
    value.lua_str = new string(str_);
}

Data::Data(string str_) : type(STRING) {
    value.lua_str = new string(str_);
}

Data::Data(bool bool_) : type(BOOLEAN) {
    value.lua_bool = bool_;
}

Data::Data(const Data& data) : type(data.get_type()) {
    switch (type) {
        case (STRING):
            value.lua_str = new string(*(data.get_value().lua_str));
        break;
        case (NUMBER):
            value.lua_num = data.get_value().lua_num;
        break;
        case (BOOLEAN):
            value.lua_bool = data.get_value().lua_bool;
        break;
        default: ; // No value assignment is done for nil.
    }
}


LuaType Data::get_type() const {
    return type;
}

Value Data::get_value() const {
    return value;
}

Data::~Data() {
    if (type == STRING and value.lua_str) {
        delete (value.lua_str);
        value.lua_str = NULL;
    }
}

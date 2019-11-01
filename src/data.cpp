#include <string>
#include "data.h"

using namespace std;


Data::Data() : type(NIL) {} // Nil doesn't need a explicit value, since it only has one.

Data::Data(double number) : type(NUMBER) {
    value.lua_num = number;
}

Data::Data(const char* str) : type(STRING) {
    value.lua_str = new string(str);
}

Data::Data(bool bool_) : type(BOOLEAN) {
    value.lua_bool = bool_;
}

Data::~Data() {
    if (type == STRING)
        delete value.lua_str;
}

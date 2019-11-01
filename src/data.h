#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED 1
#include <string>


enum LuaType {NIL, NUMBER, STRING, BOOLEAN};

union Value {
    // Lua values can be either numbers, strings, boolean or nil.
    double lua_num;
    bool lua_bool;
    std::string* lua_str;
};

class Data {
    // Represents Lua data as type-value pairs.
    public:
        // Default constructor; constructs nil.
        Data();
        // Specific constructors for each type of data.
        Data(double number);
        Data(bool boolean);
        Data(const char* str);
        ~Data();
    private:
        LuaType type;
        Value value;
};



#endif

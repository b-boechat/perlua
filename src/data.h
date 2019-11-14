#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED 1

// These macros are useful for retreiving values with less verbosity.
#define STR(data) (*(data.get_value().lua_str))
#define NUM(data) (data.get_value().lua_num)
#define BOOL(data) (data.get_value().lua_bool)

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
        Data(double num_);
        Data(size_t num_);
        Data(bool bool_);
        Data(const char* str_);
        Data(std::string str_);
        // Copy constructor.
        Data(const Data& data);

        ~Data();
        LuaType get_type() const;
        Value get_value() const;
    private:
        const LuaType type;
        Value value;
};



#endif

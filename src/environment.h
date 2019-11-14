/* This header defines the environments used to store variable-value bindings, which are used by the evaluator to correctly assign and retrieve values depending on scope. */

#ifndef ENVIRONMENT_H_INCLUDED
#define ENVIRONMENT_H_INCLUDED 1

#include <unordered_map>
#include <string>
#include <memory>
#include "token.h"
#include "data.h"

class Environment {
    friend class Evaluator;
    public:
        // Constructor for global environments.
        Environment();
        // Constructor for local environments, which stores a pointer to the enclosing one.
        Environment(Environment* enclosing_);
        ~Environment();
        void assign (const std::string& var, const Data& val);
        void declare (const std::string& var, const Data& val);
        Data get(const std::string& var) const; 

    private:
        std::unordered_map<std::string, std::unique_ptr<Data> > env_map;
        Environment* enclosing;
};



#endif

#include <string>
#include <memory>
#include <iostream> // debug
#include <unordered_map>
#include "token.h"
#include "data.h"
#include "environment.h"

using namespace std;

Environment::Environment() : env_map(), enclosing(NULL) {}
Environment::Environment(Environment* enclosing_) : env_map(), enclosing(enclosing_) {}
// Destruction of enclosing environments is done by the Evaluator class.
Environment::~Environment() {}

void Environment::assign(const string& var, const Data& val) {
    // First, tries to find var in the innermost scope. This case has to be separated because aux cannot be assigned to "this".
    if (env_map.count(var)) {
        // Reassignment is done by deallocating previous value and allocating again. All of the implemented types are immutable in Lua.
        // unique_ptr does this operation automatically with "reset".
        env_map[var].reset(new Data(val));
        return;
    }
    // If var wasn't found but innermost scope is the global one, assign here.
    if (!enclosing) {
        env_map[var] = make_unique<Data>(val);
    }
    // Otherwise iterates inwards until global scope, looking for var
    Environment* aux = enclosing;
    while (aux) {
        // If var is found in current local scope, assign.
        if ((aux->env_map).count(var)) {
            (aux->env_map[var]).reset(new Data(val));
            return;
        }
        // If iterated to global scope, assign.
        if (!aux->enclosing) {
            aux->env_map[var] = make_unique<Data>(val);
            return;
        }
        aux = aux->enclosing;
    }
}

void Environment::declare(const string& var, const Data& val) {
    // Declarations are always done locally.
    // Lua allows redeclaration of local variables.
    if (env_map.count(var))
        env_map[var].reset(new Data(val));
    else 
        env_map[var] = make_unique<Data>(val);
}

Data Environment::get(const string& var) const {
    // Tries to find var in the innermost scope. If found, returns it by value using the copy constructor of Data.
    if (env_map.count(var)) {
        // Method "at" is used instead of operator[] because of const qualifier.
        return Data(*(env_map.at(var)));
    }
    Environment* aux = enclosing;
    // Iterates outwards from innermost (visible) scope until global scope.
    while (aux) {
        if ((aux->env_map).count(var))
            return Data(*(aux->env_map.at(var)));
        aux = aux->enclosing;
    }
    // If var is not found in any scope, returns nil.
    return Data();
}

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED 1

// TODO Definir um ErrorClass com os supertipos de erro (parse, runtime, lexing etc.)
// TODO Definir uma função global aqui pra tratar erros (em vez de colocar uma em cada classe tipo parser, interpreter etc.)
enum ErrorType {OK, INVALID_SYNTAX, EXPECTED_CLOSING_PAR};

#endif

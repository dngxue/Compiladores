// analizador_lexico.h

#ifndef a_lexico_h
#define a_lexico_h

#include <bits/stdc++.h>
using namespace std;

enum tipoToken {
    VAR,
    IF,
    ELSE,
    PRINT,
    POTENCIA,
    FOR,
    WHILE,
    IDENTIFICADOR,
    NUMERO,
    STRING,
    // signos
    SUMA,
    RESTA,
    MULTIPLICACION,
    DIVISION,
    MENOR,
    MAYOR,
    IGUALDAD,
    MENOR_IGUAL,
    MAYOR_IGUAL,
    CORR_DER,
    CORR_IZQ,
    DIFERENTE,
    OR,
    AND,
    // 
    PARENTESIS_APERTURA,
    PARENTESIS_CIERRE,
    CORCHETE_APERTURA,
    CORCHETE_CIERRE,
    PUNTO_COMA,
    COMA,
    ASIGNACION,
    COMILLAS,
    //
    COMENTARIO,
    //
    TOKEN_NO_RECONOCIDO,
    FIN
};

class TokenProcesado {
public:
    int linea;
    string valor;
    tipoToken tipo;

    TokenProcesado(tipoToken tipo, string valor, int linea);
};

extern vector<TokenProcesado> tokensProcesados;
void AFD(string cadena);

#endif

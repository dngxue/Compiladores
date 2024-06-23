#include<bits/stdc++.h>
#include "a_lexico.h"
#define endl '\n'
using namespace std;

void PROGRAMA();
void DECLARACION();

void STATEMENT();

void VAR_D();
void VAR_INIT();
void TERMINO();
void TERMINO_2();
void TERMINO_3();
void TERMINO_4();
void PRIM();
void PRIMARIO();

void PRINT_S();
void CONTENIDO();
void CONTENIDO_OP();
void CADENA();

void BLOCK();

void POTENCIA_S();
void NUM_P();

void EXPR_S();
void IGUAL_E();
void EXPR_2();

void IF_S();
void ELSE_S();
void EXPRESION();
void COMPARACION();
void LOGIC_OR();
void LOGIC_AND();
void LOGIC_AND2();
void EQUALITY();

unordered_map<tipoToken, string> tipoTokenString = {
    {IF,"IF"},
    {ELSE,"ELSE"},
    {VAR,"VAR"},
    {PRINT,"PRINT"},
    {POTENCIA,"POTENCA"},
    {FOR,"FOR"},
    {WHILE,"WHILE"},
    {IDENTIFICADOR,"IDENTIFICADOR"},
    {NUMERO,"NUMERO"},
    {STRING,"STRING"},
    // signos
    {SUMA,"SUMA"},
    {RESTA,"RESTA"},
    {MULTIPLICACION,"MULTIPLICACION"},
    {DIVISION,"DIVISION"},
    {MENOR,"MENOR"},
    {MAYOR,"MAYOR"},
    {IGUALDAD,"IGUALDAD"},
    {MENOR_IGUAL,"MENOR_IGUAL"},
    {MAYOR_IGUAL,"MAYOR_IGUAL"},
    {CORR_DER,"CORR_DER"},
    {CORR_IZQ,"CORR_IZQ"},
    {DIFERENTE,"DIFERENTE"},
    //
    {PARENTESIS_APERTURA,"PARENTESIS_APERTURA"},
    {PARENTESIS_CIERRE,"PARENTESIS_CIERRE"},
    {CORCHETE_APERTURA,"CORCHETE_APERTURA"},
    {CORCHETE_CIERRE,"CORCHETE_CIERRE"},
    {PUNTO_COMA,"PUNTO_COMA"},
    {COMA,"COMA"},
    {ASIGNACION,"ASIGNACION"},
    {COMILLAS,"COMILLAS"},
    //
    {COMENTARIO,"COMENTARIO"},
    //
    {TOKEN_NO_RECONOCIDO,"TOKEN_NO_RECONOCIDO"},
    {FIN,"FIN"}
};

void parser(vector<TokenProcesado> tokensProcesados);
unordered_set<tipoToken> operador = {SUMA, RESTA, MULTIPLICACION, DIVISION};
unordered_map<string,string> tablaSimbolos;

// Conjuntos primero
unordered_set<tipoToken> primero_ = {};

// CADENA
unordered_set<tipoToken> primero_statement = {PRINT, POTENCIA, IDENTIFICADOR, IF};
unordered_set<tipoToken> primero_cadena = {RESTA, SUMA, NUMERO, PARENTESIS_APERTURA, COMILLAS, IDENTIFICADOR};
unordered_set<tipoToken> primero_termino = {RESTA, SUMA, IDENTIFICADOR, NUMERO, PARENTESIS_APERTURA};

// Conjuntos siguientes
unordered_set<tipoToken> siguiente_statement = {VAR,PRINT,POTENCIA,IDENTIFICADOR,CORCHETE_CIERRE,FIN};
unordered_set<tipoToken> siguiente_cadena = {CORR_IZQ, PUNTO_COMA};
unordered_set<tipoToken> siguiente_primario = {MULTIPLICACION,SUMA,RESTA,DIVISION,PUNTO_COMA,CORR_IZQ,PARENTESIS_CIERRE,MENOR,MAYOR,IGUALDAD,MENOR_IGUAL,MAYOR_IGUAL,DIFERENTE};
unordered_set<tipoToken> siguiente_block = {VAR,PRINT,POTENCIA,IDENTIFICADOR,CORCHETE_CIERRE,FIN,ELSE};


// unordered_set<tipoToken> siguiente_ = {};
string nombreArchivo;
bool match(tipoToken tt);

int main(int argc, char *argv[]) {
    if (argc > 1)
        nombreArchivo = argv[1];
    else
        cout << "nombreArchivo.txt" << endl;

    ifstream archivo(nombreArchivo);
    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    string linea;
    string cadena = "";

    while(getline(archivo, linea)){
        cadena += linea + '\n';
    }

    cadena += (char)0;
    // analizador léxico
    AFD(cadena);
    
    cout << "\033[92mLista de tokens\033[0m" << endl;
    for(TokenProcesado i: tokensProcesados){
        if(i.tipo != TOKEN_NO_RECONOCIDO)
            cout << "<\033[93m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m, \033[95m" << i.linea << "\033[0m >"<< endl;
        else
            cout << "<\033[91m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m, \033[95m" << i.linea << "\033[0m >"<< endl;
    }

    // funcion de análisis sintáctico
    parser(tokensProcesados);

    archivo.close();

    return 0;
}

// declaramos una variables auxiliar llamada
// preanalisis, inicializamos con un valor cualquiera
TokenProcesado preanalisis = {NUMERO, "", 1};
TokenProcesado aux = {NUMERO, "", 1};
// lista de tokens de tipo TokenProcesado
vector<TokenProcesado> listatokens;
int pos = 0;
int lineaactual = 0;

// funcion para nuestro análisis sintáctico
// Recibe: Lista de tokens procesador por el analizador léxico
void parser(vector<TokenProcesado> tokensProcesados){
    listatokens = tokensProcesados;
    preanalisis = listatokens[pos];

    PROGRAMA();

    // si no hemos llegado al final de la cadena después de terminar con la gramática
    if(preanalisis.tipo != FIN){
        cout << "\033[91mCadena no reconocida\033[0m" << endl;
        //exit(0);
    }
}

// PROGRAMA -> DECLARACION
void  PROGRAMA() {
    DECLARACION();
}

// DECLARACION  -> VAR_D DECLARACION
//              -> STATEMENT DECLARACION
//              -> ε
void DECLARACION() {
    cout << "declaracion->";
    if(preanalisis.tipo == VAR) {
        cout << "var->"<<endl;
        VAR_D();
        DECLARACION();
    }else if(primero_statement.count(preanalisis.tipo)) {
        STATEMENT();
        DECLARACION();
    }
    // ε
}

// STATEMENT    -> PRINT_S
//              -> POTENCIA_S
//              -> EXPR_S
void STATEMENT() {
    cout << "statement->"<<endl;
    if(preanalisis.tipo == PRINT) {
        PRINT_S();
    }else if(preanalisis.tipo == POTENCIA) {
        POTENCIA_S();
    }else if(preanalisis.tipo == CORCHETE_APERTURA) {
        BLOCK();
    }else if(preanalisis.tipo == IDENTIFICADOR) {
        EXPR_S();
    }else if(preanalisis.tipo == IF) {
        IF_S();
    }
}

/****************************************************************
 *                      DECLARACION VARIABLE                    *
 ****************************************************************/
// VAR_D -> var id VAR_INIT ;
void VAR_D() {
    cout << "var_d->";
    match(VAR); 
    if(!match(IDENTIFICADOR)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(preanalisis.tipo != PUNTO_COMA && preanalisis.tipo != ASIGNACION && preanalisis.linea == lineaactual) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
    }

    string nombreVar = listatokens[pos - 1].valor;
    if (tablaSimbolos.find(nombreVar) != tablaSimbolos.end()) {
        cout << "\033[91mError semantico en la linea" << preanalisis. linea << ": Variable " << nombreVar << " ya fue declarada.\033[0m" << endl;
    }
    tablaSimbolos[nombreVar] = ""; // Inicializar con un valor vacío 

    VAR_INIT();

    if(!match(PUNTO_COMA)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(preanalisis.tipo != PUNTO_COMA && preanalisis.linea == lineaactual) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        pos++;
        preanalisis = listatokens[pos];
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }  
}

// VAR_INIT -> = TERM | ε
void VAR_INIT() {
    cout << "var_init->";
    if(preanalisis.tipo == ASIGNACION) {
        match(ASIGNACION);
        TERMINO();

        string valor = listatokens[pos - 1].valor;
        tablaSimbolos[listatokens[pos - 2].valor] = valor;
    }
    // ε
}
// TERM -> TERM_2 PRIM
void TERMINO() {
    cout << "termino->" << endl;
    TERMINO_2();
    PRIM();
}
// TERM_2 -> - | + | ε
void TERMINO_2() {
    cout << "termino_2->";
    if(preanalisis.tipo == RESTA)
        match(RESTA);
    else if(preanalisis.tipo == SUMA)
        match(SUMA);
    // ε
}
// PRIM -> PRIMARIO TERM_3
void PRIM() {
    cout << "prim->";
    PRIMARIO();
    TERMINO_3();
}
// PRIMARIO -> id | numero | ( TERMINO )
void PRIMARIO() {
    cout << "primario->";
    if(preanalisis.tipo == IDENTIFICADOR){
        if (tablaSimbolos.find(preanalisis.valor) == tablaSimbolos.end()) {
            cout << "\033[91mError semantico en la linea " << preanalisis.linea << ": Variable " << preanalisis.valor << " no declarada.\033[0m" << endl;
        }
        match(IDENTIFICADOR);
    }
    else if(preanalisis.tipo == NUMERO)
        match(NUMERO);
    else if (preanalisis.tipo == PARENTESIS_APERTURA) {
        match(PARENTESIS_APERTURA);
        TERMINO();
        if(!match(PARENTESIS_CIERRE)) {
            lineaactual = preanalisis.linea;
            aux = listatokens[pos-1];
            if(aux.linea != lineaactual)
                return;

            while(preanalisis.tipo != PARENTESIS_CIERRE) {
                pos++;
                preanalisis = listatokens[pos];
                if(preanalisis.tipo == FIN)
                    break;
                if(preanalisis.linea != lineaactual)
                    break;
            }
            pos++;
            preanalisis = listatokens[pos];
            cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
        }
    }
    else{
        cout << preanalisis.valor << "LOOOOL?";
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba IDENTIFICADOR, NUMERO, PARENTESIS _APERTURA pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(!siguiente_primario.count(preanalisis.tipo)) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}
// TERM_3 -> TERM_4 PRIM | ε
void TERMINO_3() {
    cout << "termino_3->";
    if(operador.count(preanalisis.tipo)) {
        TERMINO_4();
        PRIM();
    }
    // ε
}
// TERM_4 -> * | + | - | /
void TERMINO_4() {
    cout << "termino->4";
    if(preanalisis.tipo == SUMA)
        match(SUMA);
    else if(preanalisis.tipo == RESTA)
        match(RESTA);
    else if(preanalisis.tipo == MULTIPLICACION)
        match(MULTIPLICACION);
    else if(preanalisis.tipo == DIVISION)
        match(DIVISION);
    else{
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba OPERADOR pero se encontro " << tipoTokenString[preanalisis.tipo] << "\033[0m"<< endl;
    }
}

/****************************************************************
 *                      DECLARACION PRINT                       *
 ****************************************************************/
// PRINT_S -> print >> CONTENIDO ;
void PRINT_S() {
    cout << "print_s->";
    if(!match(PRINT)) {
        pos++;
        preanalisis = listatokens[pos];
    }    
    if(!match(CORR_DER)) {
        while(!primero_cadena.count(preanalisis.tipo)) {
            pos++;
            preanalisis = listatokens[pos];
        }
    }
    CONTENIDO();
    if(!match(PUNTO_COMA)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(preanalisis.tipo != PUNTO_COMA && preanalisis.linea == lineaactual) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        pos++;
        preanalisis = listatokens[pos];
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}

// CONTENIDO -> CADENA CONTENIDO_OP
void CONTENIDO() {
    cout << "contenido->";
    CADENA();
    CONTENIDO_OP();
}

// CADENA -> " string " | id | TERMINO
void CADENA() {
    cout << "cadena->";
    if(preanalisis.tipo == COMILLAS) {
        match(COMILLAS);
        match(STRING);
        if(!match(COMILLAS)) {
            lineaactual = preanalisis.linea;
            aux = listatokens[pos-1];
            if(aux.linea != lineaactual)
                return;
            while(!siguiente_cadena.count(preanalisis.tipo)) {
                pos++;
                preanalisis = listatokens[pos];
                if(preanalisis.tipo == FIN)
                    break;
                if(preanalisis.linea != lineaactual)
                    break;
            }
            cout << "Estoy en la funcion cadena con valor " << preanalisis.valor << "linea" << preanalisis.linea << endl;
            return;
        }
    }else if(primero_termino.count(preanalisis.tipo)) {
        TERMINO();
    }else {
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba TERMINO o COMILLAS pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        lineaactual = preanalisis.linea;
        while(!siguiente_cadena.count(preanalisis.tipo)) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        return;
    }
}

// CONTENIDO_OP -> << CADENA CONTENIDO_OP | ε
void CONTENIDO_OP() {
    cout << "contenido_op->";
    if(preanalisis.tipo == CORR_IZQ) {
        match(CORR_IZQ);
        CADENA();
        CONTENIDO_OP();
    }
}

/****************************************************************
 *                      DECLARACION POTENCIA                    *
 ****************************************************************/
// POTENCIA_S -> potencia ( NUM_P , numero ) ;
void POTENCIA_S() {
    cout << "potencia->";
    match(POTENCIA);
    if(!match(PARENTESIS_APERTURA)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;
        while(preanalisis.tipo != NUMERO && preanalisis.tipo != IDENTIFICADOR) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
        }
        cout << "Estoy en la funcion potencia_S con valor " << preanalisis.valor << "linea" << preanalisis.linea << endl;
    }
    NUM_P();
    if(!match(COMA)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;
        while(preanalisis.tipo != NUMERO && preanalisis.tipo != IDENTIFICADOR) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
        }
        cout << "Estoy en la funcion potencia_S con valor " << preanalisis.valor << "linea" << preanalisis.linea << endl;
    }
    if(!match(NUMERO)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;
        while(preanalisis.tipo != PARENTESIS_CIERRE) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
        }
    }
    if(!match(PARENTESIS_CIERRE)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;
        while(preanalisis.tipo != PUNTO_COMA) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
        }
    }
    if(!match(PUNTO_COMA)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(preanalisis.tipo != PUNTO_COMA && preanalisis.linea == lineaactual) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        pos++;
        preanalisis = listatokens[pos];
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}
// NUM_P -> numero | id
void NUM_P() {
    cout << "num_p->";
    if(preanalisis.tipo == NUMERO)
        match(NUMERO);
    else if(preanalisis.tipo == IDENTIFICADOR){
        if (tablaSimbolos.find(preanalisis.valor) == tablaSimbolos.end()) {
            cout << "\033[91mError semantico en la linea " << preanalisis.linea << ": Variable " << preanalisis.valor << " no declarada.\033[0m" << endl;
        }
        match(IDENTIFICADOR);
    }else {
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba NUMERO o IDENTIFICADOR pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;
        while(preanalisis.tipo != COMA) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        cout << "Estoy en la funcion num_p con valor " << preanalisis.valor << " linea " << preanalisis.linea << endl;
    }
}

/****************************************************************
 *                      DECLARACION BLOCK                       *
 ****************************************************************/
// BLOCK -> { DECLARACION }
void BLOCK() {
    match(CORCHETE_APERTURA);
    cout << "..." << preanalisis.valor << "----";
    DECLARACION();

    if(!match(CORCHETE_CIERRE)) {
        cout << preanalisis.valor;
        while(preanalisis.tipo != ELSE) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
        }
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}

/****************************************************************
 *                      DECLARACION EXPRESION                   *
 ****************************************************************/
// EXPR_S -> id = EXPR_2 ;
void EXPR_S() {
    cout << "expr_s->";
    if (tablaSimbolos.find(preanalisis.valor) == tablaSimbolos.end()) {
            cout << "\033[91mError semantico en la linea " << preanalisis.linea << ": Variable " << preanalisis.valor << " no declarada.\033[0m" << endl;
    }
    match(IDENTIFICADOR);

    if(!match(ASIGNACION)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        if(preanalisis.tipo == CORCHETE_APERTURA) {
            // while(!preanalisis.tipo != CORCHETE_CIERRE) {
            // pos++;
            // preanalisis = listatokens[pos];
            // if(preanalisis.tipo == FIN)
            //     return;
            // }
            BLOCK();
            return;
        }
        while(!primero_termino.count(preanalisis.tipo)) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
            if(preanalisis.tipo == CORCHETE_CIERRE) {
                pos++;
                preanalisis = listatokens[pos];
                return;
            }
        }
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }

    EXPR_2();
    if(!match(PUNTO_COMA)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(preanalisis.tipo != PUNTO_COMA && preanalisis.linea == lineaactual) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        pos++;
        preanalisis = listatokens[pos];
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}
// EXPR_2 -> TERMINO IGUAL_E
void EXPR_2() {
    cout << "expr_2->";
    TERMINO();
    IGUAL_E();
}
// IGUAL_E -> = EXPR_2 | ε
void IGUAL_E() {
    cout << "igual_e->";
    if(preanalisis.tipo == ASIGNACION) {
        match(ASIGNACION);
        EXPR_2();
    }
}
/****************************************************************
 *                      DECLARACION VARIABLE                    *
 ****************************************************************/
// IF_S -> if ( EXPRESION ) BLOCK ELSE_S
void IF_S() {
    cout << "if->";
    match(IF);
    if(!match(PARENTESIS_APERTURA)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(!primero_termino.count(preanalisis.tipo)) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
        }
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
    EXPRESION();

    if(!match(PARENTESIS_CIERRE)) {
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;

        while(preanalisis.tipo != CORCHETE_APERTURA) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                return;
        }
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }

    BLOCK();
    ELSE_S();
}
// ELSE_S -> else BLOCK | ε
void ELSE_S() {
    cout << "else_s->";
    if(preanalisis.tipo == ELSE) {
        match(ELSE);
        BLOCK();
    }
    // ε
}
// EXPRESION -> LOGIC_AND LOGIC_OR
void EXPRESION() {
    cout << "expresion->";
    LOGIC_AND();
    LOGIC_OR();
}
// LOGIC_OR -> | LOGIC_AND LOGIC_OR | ε
void LOGIC_OR() {
    cout << "logic_or->";
    if(preanalisis.tipo == OR) {
        match(OR);
        LOGIC_AND();
        LOGIC_OR();
    }
    // ε
}
// LOGIC_AND -> EQUALITY LOGIC_AND2
void LOGIC_AND() {
    cout << "logic_and->";
    EQUALITY();
    LOGIC_AND2();
}
// LOGIC_AND2 -> & EQUALITY LOGIC_AND2 | ε
void LOGIC_AND2() {
    cout << "logic_and2->";
    if(preanalisis.tipo == AND) {
        match(AND);
        EQUALITY();
        LOGIC_AND2();
    }
    // ε
}
// EQUALITY -> TERMINO COMPARACION TERMINO
void EQUALITY() {
    cout << "equality->";
    TERMINO();
    COMPARACION();
    TERMINO();
}
// COMPARACION -> < | > | == | <= | >= | !=
void COMPARACION() {
    cout << "comparacion->";
    if(preanalisis.tipo == MENOR)
        match(MENOR);
    else if(preanalisis.tipo == MAYOR)
        match(MAYOR);
    else if(preanalisis.tipo == IGUALDAD)
        match(IGUALDAD);
    else if(preanalisis.tipo == MENOR_IGUAL)
        match(MENOR_IGUAL);
    else if(preanalisis.tipo == MAYOR_IGUAL)
        match(MAYOR_IGUAL);
    else if(preanalisis.tipo == DIFERENTE)
        match(DIFERENTE);
    else {
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba COMPARACION pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return;
        while(!primero_termino.count(preanalisis.tipo) && preanalisis.tipo != PARENTESIS_CIERRE) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        cout << "Estoy en la funcion num_p con valor " << preanalisis.valor << " linea " << preanalisis.linea << endl;
    }
}

/* ------------------------------------------------------------------------------------------------------- */
// para comprobar que el token en el que estás
// es el que deberías recibir 
bool match(tipoToken tt){
    if(preanalisis.tipo == tt){
        pos++; 
        preanalisis = listatokens[pos];
        return true;
    } else{
        // si no es el que se esperaba, error
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba " << tipoTokenString[tt] << " pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        return false;
    }
}
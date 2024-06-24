#include<bits/stdc++.h>
#include<locale.h> 
#include "a_lexico.h"
#define endl '\n'
using namespace std;

void PROGRAMA();
void DECLARACION(bool ejecutar);

void STATEMENT(bool ejecutar);

void VAR_D(bool ejecutar);
int VAR_INIT(bool ejecutar);
int TERMINO();
int TERMINO_2();
int TERMINO_3(int primario);
char TERMINO_4();
int PRIM(int signo);
int PRIMARIO();

void PRINT_S(bool ejecutar);
void CONTENIDO(bool ejecutar);
void CONTENIDO_OP(bool ejecutar);
void CADENA(bool ejecutar);

void BLOCK(bool ejecutar);

void POTENCIA_S(bool ejecutar);
void NUM_P();

void EXPR_S(bool ejecutar);
//void IGUAL_E();
//int EXPR_2();

void IF_S(bool ejecutar);
void ELSE_S(bool ejecutar);
int EXPRESION();
string COMPARACION();
int LOGIC_OR();
int LOGIC_AND();
int LOGIC_AND2();
int EQUALITY();

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

unordered_map<string, int> tablaSimbolos;
unordered_map<string, int> tablaSimGeneral;

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

bool match(tipoToken tt);

string rutaCodigoNuevo;
string rutaCodigoViejo;
int errores = 0;
string salida = "";
bool printDebug = false;

bool ejecutar = false;

void printMap();

int main(int argc, char *argv[]) {

    if (argc == 2) {
        if(atoi(argv[1]) == 1) {
            ejecutar = true;
        }
    }
    else
        cout << " 1/0 " << endl;

    /*
    rutaCodigoNuevo = "./../cpp/archivos/codigoNuevo.txt";
    rutaCodigoViejo = "./../cpp/archivos/codigoViejo.txt";
    */
    rutaCodigoNuevo = "./../cpp/archivos/codigoNuevo.txt";
    rutaCodigoViejo = "./../cpp/archivos/codigoViejo.txt";
    string ruta;
    // Ejecutar ejecuta el código viejo generado por este programa
    if(ejecutar) {
        ruta = rutaCodigoViejo;
    // Compilar genera el código viejo a partir del nuevo
    } else {
        ruta = rutaCodigoNuevo;
    }
    
    ifstream archivo(ruta);
    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo " << ruta << endl;
        return 1;
    }
    stringstream buffer;
    buffer << archivo.rdbuf();
    string content = buffer.str();

    string cadena = "";
    string linea;
    while(getline(buffer, linea)){
        cadena += linea + '\n';
    }
    archivo.close();
    cadena += (char)0;
    // analizador léxico
    AFD(cadena);
    if(printDebug) {
        cout << "\033[92mLista de tokens\033[0m" << endl;
        for(TokenProcesado i: tokensProcesados){
            if(i.tipo != TOKEN_NO_RECONOCIDO)
                cout << "<\033[93m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m, \033[95m" << i.linea << "\033[0m >"<< endl;
            else
                cout << "<\033[91m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m, \033[95m" << i.linea << "\033[0m >"<< endl;
        }
    }
    

    // funcion de análisis sintáctico
    parser(tokensProcesados);

    if(ejecutar) {
        if(errores == 0)
            cout << salida;
    } else {
        if(errores == 0) {
            cout << "\033[92m Compilación correcta \033[0m" << endl;
            ofstream outputFile(rutaCodigoViejo);
            if (!outputFile) {
                cerr << "Error al abrir el archivo de salida" << endl;
                return 1;
            }
            outputFile << content;
            archivo.close();
        }
    }

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
    printMap();

    // si no hemos llegado al final de la cadena después de terminar con la gramática
    if(preanalisis.tipo != FIN){
        cout << "\033[91mCadena no reconocida\033[0m" << endl;
        //exit(0);
    }
}

// PROGRAMA -> DECLARACION
void  PROGRAMA() {
    bool ejecutar = true;
    DECLARACION(ejecutar);
}

// DECLARACION  -> VAR_D DECLARACION
//              -> STATEMENT DECLARACION
//              -> ε
void DECLARACION(bool ejecutar) {
    if(printDebug) cout << "declaracion->";

    if(preanalisis.tipo == VAR) {
        if(printDebug) cout << "var->"<<endl;
        VAR_D(ejecutar);
        DECLARACION(ejecutar);
    }else if(primero_statement.count(preanalisis.tipo)) {
        STATEMENT(ejecutar);
        DECLARACION(ejecutar);
    }
    // ε
}

// STATEMENT    -> PRINT_S
//              -> POTENCIA_S
//              -> EXPR_S
//              -> IF_S
void STATEMENT(bool ejecutar) {
    if(printDebug) cout << "statement->"<<endl;
    if(preanalisis.tipo == PRINT) {
        PRINT_S(ejecutar);
    }else if(preanalisis.tipo == POTENCIA) {
        POTENCIA_S(ejecutar);
    }else if(preanalisis.tipo == CORCHETE_APERTURA) {
        BLOCK(ejecutar);
    }else if(preanalisis.tipo == IDENTIFICADOR) {
        EXPR_S(ejecutar);
    }else if(preanalisis.tipo == IF) {
        IF_S(ejecutar);
    }
}

/****************************************************************
 *                      DECLARACION VARIABLE                    *
 ****************************************************************/
// VAR_D -> var id VAR_INIT ;
void VAR_D(bool ejecutar) {
    if(printDebug) cout << "var_d->";
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
        errores++;
    }

    int valor = VAR_INIT(ejecutar);
    if(ejecutar) {
        tablaSimbolos[nombreVar] = valor;
        if(printDebug) cout << "\033[92mSe asigno " << valor << " a la variable " << nombreVar << "\033[0m";
    }
    tablaSimGeneral[nombreVar] = valor;

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
        if(printDebug) cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }  
}

// VAR_INIT -> = TERM | ε
int VAR_INIT(bool ejecutar) {
    if(printDebug) cout << "var_init->";
    if(preanalisis.tipo == ASIGNACION) {
        match(ASIGNACION);
        return TERMINO();
    }
    return 0;
    // ε
}
/****************************************************************
 *                      DECLARACION PRINT                       *
 ****************************************************************/
// PRINT_S -> print >> CONTENIDO ;
void PRINT_S(bool ejecutar) {
    if(printDebug) cout << "print_s->";
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
    CONTENIDO(ejecutar);
    if(ejecutar) salida += "\n";
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
        if(printDebug) cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}

// CONTENIDO -> CADENA CONTENIDO_OP
void CONTENIDO(bool ejecutar) {
    if(printDebug) cout << "contenido->";
    CADENA(ejecutar);
    CONTENIDO_OP(ejecutar);
}

// CADENA -> " string " | TERMINO
void CADENA(bool ejecutar) {
    if(printDebug) cout << "cadena->";
    if(preanalisis.tipo == COMILLAS) {
        match(COMILLAS);
        match(STRING);
        if(ejecutar) {
            string cadena = listatokens[pos-1].valor;
            salida += cadena;
        }
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
            if(printDebug) cout << "Estoy en la funcion cadena con valor " << preanalisis.valor << "linea" << preanalisis.linea << endl;
            return;
        }
    }else if(primero_termino.count(preanalisis.tipo)) {
        int num = TERMINO();
        if(ejecutar) salida += to_string(num);
    }else {
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba TERMINO o COMILLAS pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        errores++;
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
void CONTENIDO_OP(bool ejecutar) {
    if(printDebug) cout << "contenido_op->";
    if(preanalisis.tipo == CORR_IZQ) {
        match(CORR_IZQ);
        CADENA(ejecutar);
        CONTENIDO_OP(ejecutar);
    }
}

/****************************************************************
 *                      DECLARACION BLOCK                       *
 ****************************************************************/
// BLOCK -> { DECLARACION }
void BLOCK(bool ejecutar) {
    match(CORCHETE_APERTURA);
    if(printDebug) cout << "..." << preanalisis.valor << "----";
    DECLARACION(ejecutar);

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
        if(printDebug) cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}
/****************************************************************
 *                      DECLARACION EXPRESION                   *
 ****************************************************************/
// EXPR_S -> id = TERMINO ;
void EXPR_S(bool ejecutar) {
    if(printDebug) cout << "expr_s->";
    if (tablaSimbolos.find(preanalisis.valor) == tablaSimbolos.end()) {
        cout << "\033[91mError semantico en la linea " << preanalisis.linea << ": Variable " << preanalisis.valor << " no declarada.\033[0m" << endl;
        errores++;
    }
    match(IDENTIFICADOR);
    string id = listatokens[pos-1].valor;

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
            BLOCK(ejecutar);
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
        if(printDebug) cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }

    int expr = TERMINO();
    if(ejecutar) {
        tablaSimbolos[id] = expr;
        if(printDebug) cout << "\033[92mSe asignó " << expr << " a la variable " << id << "\033[0m";
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
        if(printDebug) cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
}
/****************************************************************
 *                         IF STATEMENT                         *
 ****************************************************************/
// IF_S -> if ( EXPRESION ) BLOCK ELSE_S
void IF_S(bool ejecutar) {
    if(printDebug) cout << "if->";
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
        if(printDebug) cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;
    }
    bool expresion = EXPRESION();
    if(!ejecutar) expresion = false;

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

    BLOCK(expresion);
    ELSE_S(!expresion);
}
// ELSE_S -> else BLOCK | ε
void ELSE_S(bool ejecutar) {
    if(printDebug) cout << "else_s->";
    if(preanalisis.tipo == ELSE) {
        match(ELSE);
        BLOCK(ejecutar);
    }
    // ε
}

// TERM -> TERM_2 PRIM
int TERMINO() {
    if(printDebug) cout << "termino->" << endl;
    int signo = TERMINO_2();
    int prim = PRIM(signo);
    return prim;
}

// TERM_2 -> - | + | ε
int TERMINO_2() {
    if(printDebug) cout << "termino_2->";
    if(preanalisis.tipo == RESTA) {
        match(RESTA);
        return -1;
    }
    else if(preanalisis.tipo == SUMA)
        match(SUMA);
    return 1;
    // ε
}
// PRIM -> PRIMARIO TERM_3
int PRIM(int signo) {
    if(printDebug) cout << "prim->";
    int primario = signo*PRIMARIO();
    return TERMINO_3(primario);
}
// PRIMARIO -> id | numero | ( TERMINO )
int PRIMARIO() {
    if(printDebug) cout << "primario->";
    if(preanalisis.tipo == IDENTIFICADOR){
        int valor = 0;
        if (tablaSimbolos.find(preanalisis.valor) == tablaSimbolos.end()) {
            if(tablaSimGeneral.find(preanalisis.valor) == tablaSimGeneral.end()) {
                cout << "\033[91mError semantico en la linea " << preanalisis.linea << ": Variable " << preanalisis.valor << " no declarada.\033[0m" << endl;
                errores++;
            }
        } else {
            valor = tablaSimbolos[preanalisis.valor];
        }
        match(IDENTIFICADOR);
        return valor;
    }
    else if(preanalisis.tipo == NUMERO) {
        int valor = stoi(preanalisis.valor);
        match(NUMERO);
        return valor;
    }
    else if (preanalisis.tipo == PARENTESIS_APERTURA) {
        match(PARENTESIS_APERTURA);
        int valor = TERMINO();
        if(!match(PARENTESIS_CIERRE)) {
            lineaactual = preanalisis.linea;
            aux = listatokens[pos-1];
            if(aux.linea != lineaactual)
                return 0;

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
            
            valor = 0;
        }
        return valor;
    }
    else{
        cout << preanalisis.valor << "LOOOOL?";
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba IDENTIFICADOR, NUMERO, PARENTESIS _APERTURA pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        errores++;
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return 0;

        while(!siguiente_primario.count(preanalisis.tipo)) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        cout << "El valor de la pos actual es: " << preanalisis.valor << " en la linea: " << preanalisis.linea;

        return 0;
    }
}
// TERM_3 -> TERM_4 PRIM | ε
int TERMINO_3(int primario) {
    if(printDebug) cout << "termino_3->";
    if(operador.count(preanalisis.tipo)) {
        char operador = TERMINO_4();
        int derecha = PRIM(1);
        if(operador == '+')
            return primario+derecha;
        else if(operador == '-')
            return primario-derecha;
        else if(operador == '*')
            return primario*derecha;
        else if(operador == '/')
            return primario/derecha;
    }
    return primario;
    // ε
}
// TERM_4 -> * | + | - | /
char TERMINO_4() {
    if(printDebug) cout << "termino->4";
    if(preanalisis.tipo == SUMA) {
        match(SUMA);
        return '+';
    }
    else if(preanalisis.tipo == RESTA) {
        match(RESTA);
        return '-';
    }
    else if(preanalisis.tipo == MULTIPLICACION) {
        match(MULTIPLICACION);
        return '*';
    }
    else if(preanalisis.tipo == DIVISION) {
        match(DIVISION);
        return '/';
    }
    else{
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba OPERADOR pero se encontro " << tipoTokenString[preanalisis.tipo] << "\033[0m"<< endl;
        errores++;
        return '+';
    }
}


/****************************************************************
 *                      DECLARACION POTENCIA                    *
 ****************************************************************/
// POTENCIA_S -> potencia ( NUM_P , numero ) ;
void POTENCIA_S(bool ejecutar) {
    if(printDebug) cout << "potencia->";
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
    if(printDebug) cout << "num_p->";
    if(preanalisis.tipo == NUMERO)
        match(NUMERO);
    else if(preanalisis.tipo == IDENTIFICADOR){
        if (tablaSimbolos.find(preanalisis.valor) == tablaSimbolos.end()) {
            cout << "\033[91mError semantico en la linea " << preanalisis.linea << ": Variable " << preanalisis.valor << " no declarada.\033[0m" << endl;
            errores++;
        }
        match(IDENTIFICADOR);
    }else {
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba NUMERO o IDENTIFICADOR pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        errores++;
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

// EXPRESION -> LOGIC_AND LOGIC_OR
int EXPRESION() {
    if(printDebug) cout << "expresion->";
    int izquierda = LOGIC_AND();
    int derecha = LOGIC_OR();
    return izquierda || derecha;
}
// LOGIC_OR -> || LOGIC_AND LOGIC_OR | ε
int LOGIC_OR() {
    if(printDebug) cout << "logic_or->";
    if(preanalisis.tipo == OR) {
        match(OR);
        int izquierda = LOGIC_AND();
        int derecha = LOGIC_OR();
        return izquierda || derecha;
    }
    return 0;
    // ε
}
// LOGIC_AND -> EQUALITY LOGIC_AND2
int LOGIC_AND() {
    if(printDebug) cout << "logic_and->";
    int izquierda = EQUALITY();
    return izquierda && LOGIC_AND2();
}
// LOGIC_AND2 -> && EQUALITY LOGIC_AND2 | ε
int LOGIC_AND2() {
    if(printDebug) cout << "logic_and2->";
    if(preanalisis.tipo == AND) {
        match(AND);
        int izquierda = EQUALITY();
        return izquierda && LOGIC_AND2();;
    }
    return 1;
    // ε
}
// EQUALITY -> TERMINO COMPARACION TERMINO
int EQUALITY() {
    if(printDebug) cout << "equality->";
    int izquierda = TERMINO();
    string operador = COMPARACION();
    int derecha = TERMINO();

    int comparacion;
    if(operador == "<")
        comparacion = izquierda < derecha;
    else if(operador == ">")
        comparacion = izquierda > derecha;
    else if(operador == "==")
        comparacion = izquierda == derecha;
    else if(operador == "<=")
        comparacion = izquierda <= derecha;
    else if(operador == ">=")
        comparacion = izquierda >= derecha;
    else
        comparacion = izquierda != derecha;
    
    return comparacion;
}
// COMPARACION -> < | > | == | <= | >= | !=
string COMPARACION() {
    if(printDebug) cout << "comparacion->";
    if(preanalisis.tipo == MENOR) {
        match(MENOR);
        return "<";
    }
    else if(preanalisis.tipo == MAYOR) {
        match(MAYOR);
        return ">";
    }
    else if(preanalisis.tipo == IGUALDAD) {
        match(IGUALDAD);
        return "==";
    }
    else if(preanalisis.tipo == MENOR_IGUAL) {
        match(MENOR_IGUAL);
        return "<=";
    }
    else if(preanalisis.tipo == MAYOR_IGUAL) {
        match(MAYOR_IGUAL);
        return ">=";
    }
    else if(preanalisis.tipo == DIFERENTE) {
        match(DIFERENTE);
        return "!=";
    }
    else {
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba COMPARACION pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        errores++;
        lineaactual = preanalisis.linea;
        aux = listatokens[pos-1];
        if(aux.linea != lineaactual)
            return "==";
        while(!primero_termino.count(preanalisis.tipo) && preanalisis.tipo != PARENTESIS_CIERRE) {
            pos++;
            preanalisis = listatokens[pos];
            if(preanalisis.tipo == FIN)
                break;
            if(preanalisis.linea != lineaactual)
                break;
        }
        cout << "Estoy en la funcion num_p con valor " << preanalisis.valor << " linea " << preanalisis.linea << endl;

        return "==";
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
    } else {
        // si no es el que se esperaba, error
        cout << "\033[91mError de sintaxis en la linea " << preanalisis.linea << " se esperaba " << tipoTokenString[tt] << " pero se encontro " << preanalisis.valor << "\033[0m" << endl;
        errores++;
        return false;
    }
}

void printMap() {
    if(printDebug) {
        cout << "------------\n";
        for_each(tablaSimbolos.begin(), tablaSimbolos.end(), 
            [](pair<string, int> p) { 
                cout << p.first 
                        << " :: " << p.second 
                        << endl; 
            });
        cout << "------------\n";
    }
}
#include<bits/stdc++.h>
#define endl '\n'
using namespace std;


void E();
void F();
void T();
void Eprima();
void Tprima();

enum tipoToken {
    NUMERO,
    IDENTIFICADOR,
    RESTA,
    MULTIPLICACION,
    DIVISION,
    TOKEN_NO_RECONOCIDO,
    PARENTESIS_APERTURA,
    PARENTESIS_CIERRE,
    SUMA,
    FIN
};

unordered_map<tipoToken, string> tipoTokenString = {
    {NUMERO,"NUMERO"},
    {IDENTIFICADOR,"IDENTIFICADOR"},
    {SUMA,"SUMA"},
    {RESTA,"RESTA"},
    {MULTIPLICACION,"MULTIPLICACION"},
    {DIVISION,"DIVISION"},
    {TOKEN_NO_RECONOCIDO,"TOKEN_NO_RECONOCIDO"},
    {PARENTESIS_APERTURA,"PARENTESIS_APERTURA"},
    {PARENTESIS_CIERRE,"PARENTESIS_CIERRE"},
    {FIN,"FIN"}
};

class TokenProcesado{
    public:
    string valor;
    tipoToken tipo;

    TokenProcesado(tipoToken tipo, string valor){
        this->tipo = tipo;
        this->valor = valor;
    }
};

void parser(vector<TokenProcesado> tokensProcesados);

unordered_set<char> signos = {'+','-','*','/'};
vector<TokenProcesado> tokensProcesados;
string nombreArchivo;
void match(tipoToken tt);

void AFD(string cadena){
    int estado = 0; 
    string lexema = ""; 
    tipoToken t; 
    char c; 

    for(int i=0; i<cadena.size(); i++){
        c = cadena[i]; 

        switch(estado){

            case 0:     
                if(isdigit(c) && c != '0'){
                    lexema += c;
                    estado = 1;
                }
                else if(isalpha(c)){
                    lexema += c;        
                    estado = 2;
                }
                else if(c == '+'){
                    lexema += c;
                    t = SUMA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '-'){
                    lexema += c;
                    t = RESTA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '*'){
                    lexema += c;
                    t = MULTIPLICACION;
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '/'){
                    lexema += c;
                    t = DIVISION;
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '0'){
                    lexema += c;
                    estado = 4;
                }else if(c == ' ' || c == '\t' || c == '\n' || c == 13){
                    	
                }else if(c == '.'){
                    lexema += c;
                    estado = 5;
                }else if(c == '('){
                    lexema += c;
                    t = PARENTESIS_APERTURA;

                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == ')'){
                    lexema += c;
                    t = PARENTESIS_CIERRE;

                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else{
                    lexema += c;
                    estado = 6;
                }
                break;

            case 1:     // NUMERO
                if(isdigit(c)){
                    lexema += c;            
                    estado = 1;
                }else if(c == '.'){
                    lexema += c;
                    estado = 5;
                }else{
                    t = NUMERO;
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;

            case 2:     // IDENTIFICADOR
                if(isalpha(c) || isdigit(c) || c == '_'){
                    lexema += c;
                    estado = 2;
                }else{
                    t = IDENTIFICADOR;
                    tokensProcesados.push_back(TokenProcesado(t,lexema));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;

            case 4:     // CASO ES CERO SOLITO
                if(isdigit(c) || c == '.'){
                    lexema += c;
                    estado = 5;
                }else{
                    t = NUMERO;
                    tokensProcesados.push_back(TokenProcesado(t,lexema));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;
            case 5: // NUMERO INVALIDO
                if(isdigit(c) || c == '.'){
                    lexema += c;
                    estado = 5;
                }else{
                    t = TOKEN_NO_RECONOCIDO;
                    tokensProcesados.push_back(TokenProcesado(t,lexema));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;

            case 6: // ESTADO MUERTO
                if(!isdigit(c) && !signos.count(c) && !isalpha(c) && c != 0 && !(c = ' ')){
                    lexema += c;
                    estado = 6;
                }else{
                    t = TOKEN_NO_RECONOCIDO;
                    tokensProcesados.push_back(TokenProcesado(t,lexema));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                
                break;
        }

    }
    t = FIN;
    tokensProcesados.push_back(TokenProcesado(t,"FIN"));
}

int main(int argc, char *argv[]) {
    if (argc > 1) { // recibe como argumento
        // el nombre del archivo que contiene la 
        // expresion a evaluar
        nombreArchivo = argv[1];
    } else {
        cout << "nombreArchivo.txt" << endl;
    }

    ifstream archivo(nombreArchivo);
    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    string linea;
    string cadena = "";

    while(getline(archivo, linea)){
        cadena += linea;
    }

    cadena += (char)0;
    // analizador léxico
    AFD(cadena);
    
    
    // cout << "\033[92mLista de tokens\033[0m" << endl;
    // for(TokenProcesado i: tokensProcesados){
    //     if(i.tipo != TOKEN_NO_RECONOCIDO)
    //         cout << "<\033[93m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
    //     else
    //         cout << "<\033[91m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
    // }

    // funcion de análisis sintáctico
    cout << "\033[34mExpresion: \033[0m" << "\033[33m" << cadena << "\033[0m" << endl;
    parser(tokensProcesados);
    
    cout << "\033[92mCadena reconocida\033[0m\n" << endl;

    archivo.close();

    return 0;
}

// declaramos una variables auxiliar llamada
// preanalisis, inicializamos con un valor cualquiera
TokenProcesado preanalisis = {NUMERO, ""};
// lista de tokens de tipo TokenProcesado
vector<TokenProcesado> listatokens;
// para iterar en cada elemento de la lista de tokens
// iniciamos en la posición 0
int pos = 0;

// funcion para nuestro análisis sintáctico
// Recibe: Lista de tokens procesador por el analizador léxico
void parser(vector<TokenProcesado> tokensProcesados){
    // agregamos nuestros tokens a la lista de tokens
    listatokens = tokensProcesados;
    // preanalisis toma el valor del primer elemento de la lista
    preanalisis = listatokens[pos];

    // iniciamos el análisis sintáctico
    E();
    // si no hemos llegado al final de la cadena después
    // de terminar con la gramática
    if(preanalisis.tipo != FIN){
        cout << "\033[91mCadena no reconocida\033[0m" << endl;
        exit(0);
    }
}

// E -> TE´
void E(){
    T();
    Eprima();
}

//T -> FT´
void T(){
    F();
    Tprima();
}

// E´ -> +TE´ | -TE´ | ε
void Eprima(){
    // si nuestro token es +
    // E´ -> +TE´
    if(preanalisis.tipo == SUMA){
        match(SUMA);
        T();
        Eprima();
    }
    // si nuestro token es -
    // E´ -> -TE´
    else if(preanalisis.tipo == RESTA){
        match(RESTA);
        T();
        Eprima();
    }
}

// F -> (E) | # | id
void F(){
    // si nuestro token es ()
    // F -> (E)
    if(preanalisis.tipo == PARENTESIS_APERTURA){
        match(PARENTESIS_APERTURA);
        E();
        match(PARENTESIS_CIERRE);
    }
    // si nuestro token es #
    // F -> #
    else if(preanalisis.tipo == NUMERO){
        match(NUMERO);
    }
    // si nuestro token es id
    // F -> id
    else if(preanalisis.tipo == IDENTIFICADOR){
        match(IDENTIFICADOR);
   
    }else {
        cout << "\033[91mCadena no reconocida\033[0m" << endl;
        exit(0);
    }
}

// T´ -> *FT´ | /FT´ | ε
void Tprima(){
    // si nuestro token es *
    // T´ -> *FT´
    if(preanalisis.tipo == MULTIPLICACION){
        match(MULTIPLICACION);
        F();
        Tprima();
    }
    // si nuestro token es /
    // T´ -> /FT´
    else if(preanalisis.tipo == DIVISION){
        match(DIVISION);
        F();
        Tprima();
    }
}

// para comprobar que el token en el que estás
// es el que deberías recibir 
void match(tipoToken tt){
    if(preanalisis.tipo == tt){
        pos++; // aumentamos en 1 la posición
        // igualamos preanalisis al valor de la
        // siguiente posición en la lista de tokens
        preanalisis = listatokens[pos];
    } else{
        // si no es el que se esperaba, error
        cout << "\033[91mCadena no reconocida\033[0m" << endl;
        exit(0);
    }
}






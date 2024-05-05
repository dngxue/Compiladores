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
    int estado = 0; // inicializa el estado en 0
    string lexema = ""; // auxiliar
    tipoToken t; // para definir posteriormente el tipo de token
    char c; // auxiliar

    for(int i=0; i<cadena.size(); i++){
        c = cadena[i]; // auxiliar

        switch(estado){

            case 0:     // INICIO
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
                    // agregar el token al vector de tokensProcesados
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '-'){
                    lexema += c;
                    t = RESTA;
                    // agregar el token al vector de tokensProcesados
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '*'){
                    lexema += c;
                    t = MULTIPLICACION;
                    // agregar el token al vector de tokensProcesados
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '/'){
                    lexema += c;
                    t = DIVISION;
                    // agregar el token al vector de tokensProcesados
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
    if (argc > 1) {
        nombreArchivo = argv[1];
    } else {
        cout << "nombreArchivo.txt" << endl;
    }

    ifstream archivo(nombreArchivo);
    // si el archivo no se pudo abrir
    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    string linea;
    string cadena = "";
    // agrega a nuestra cadena cada elemento
    // de la linea
    while(getline(archivo, linea)){
        cadena += linea;
    }
    // agrega el final de la cadena
    cadena += (char)0;

    AFD(cadena);
    // imprime la lista de tokens
    cout << "\033[92mLista de tokens\033[0m" << endl;
    for(TokenProcesado i: tokensProcesados){
        if(i.tipo != TOKEN_NO_RECONOCIDO)
            cout << "<\033[93m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
        else
            cout << "<\033[91m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
    }

    parser(tokensProcesados);

    cout << "\033[92mCadena reconocida\033[0m" << endl;
    // cierra el archivo
    archivo.close();

    return 0;
}

TokenProcesado preanalisis = {NUMERO, ""};
vector<TokenProcesado> listatokens;
int pos = 0;

void parser(vector<TokenProcesado> tokensProcesados){
    
    listatokens = tokensProcesados;
    preanalisis = listatokens[pos];

    E();
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
    if(preanalisis.tipo == SUMA){
        match(SUMA);
        T();
        Eprima();
    } else if(preanalisis.tipo == RESTA){
        match(RESTA);
        T();
        Eprima();
    }
}

// F -> (E) | # | id
void F(){
    if(preanalisis.tipo == PARENTESIS_APERTURA){
        match(PARENTESIS_APERTURA);
        E();
        match(PARENTESIS_CIERRE);
    } else if(preanalisis.tipo == NUMERO){
        match(NUMERO);
    } else if(preanalisis.tipo == IDENTIFICADOR){
        match(IDENTIFICADOR);
    } else{
        cout << "\033[91mCadena no reconocida\033[0m" << endl;
        exit(0);
    }
}

// T´ -> *FT´ | /FT´ | ε
void Tprima(){
    if(preanalisis.tipo == MULTIPLICACION){
        match(MULTIPLICACION);
        F();
        Tprima();
    } else if(preanalisis.tipo == DIVISION){
        match(DIVISION);
        F();
        Tprima();
    }
}

void match(tipoToken tt){
    if(preanalisis.tipo == tt){
        pos++;
        preanalisis = listatokens[pos];
    } else{
        cout << "\033[91mCadena no reconocida\033[0m" << endl;
        exit(0);
    }
}






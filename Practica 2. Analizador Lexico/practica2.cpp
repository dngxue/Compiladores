#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

// tipos de tokens que puede reconocer
// el analizador léxico
enum tipoToken{
    NUMERO,
    IDENTIFICADOR,
    OPERADOR,
    TOKEN_NO_RECONOCIDO,
};

// mapena cada tipo de token a su nombre
// como una cadena de texto
unordered_map<tipoToken, string> tipoTokenString = {
    {NUMERO,"NUMERO"},
    {IDENTIFICADOR,"IDENTIFICADOR"},
    {OPERADOR,"OPERADOR"},
    {TOKEN_NO_RECONOCIDO,"TOKEN_NO_RECONOCIDO"},
};

// inicializa los miembros tipo y valor
// de la clase con los valores pasados como argumentos
class TokenProcesado{
    public:
    string valor;
    tipoToken tipo;

    TokenProcesado(tipoToken tipo, string valor){
        this->tipo = tipo;
        this->valor = valor;
    }
};
// signos que pueden ser reconocidos
unordered_set<char> signos = {'+','-'};
vector<TokenProcesado> tokensProcesados;
string nombreArchivo;

// recibe la cadena
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
                else if(c >= 'a' && c <= 'z'){
                    lexema += c;        
                    estado = 2;
                }
                else if(c >= 'A' && c <= 'Z'){
                    lexema += c;
                    estado = 7;
                }
                else if(signos.count(c)){
                    lexema += c;
                    t = OPERADOR;
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

            case 2:     // IDENTIFICADOR UN CARACTER
                if(c >= 'a' && c <= 'z'){
                    lexema += c;
                    estado = 3;
                }else if(c >= 'A' && c <= 'Z'){
                    lexema += c;
                    estado = 7;
                }else{
                    t = IDENTIFICADOR;
                    tokensProcesados.push_back(TokenProcesado(t,lexema));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;

            case 3:     // IDENTIFICARDOR DOS CARACTERES
                if(isalpha(c)){
                    lexema += c;
                    estado = 7;
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
                if(!isdigit(c) && !signos.count(c) && !(c >= 'a' && c <= 'z') && c != 0 && !(c = ' ')){
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

            case 7: // IDENTIFICADOR INVALIDO
                if(isalpha(c)){
                    lexema += c;
                    estado = 7;
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
   
}

int main(int argc, char *argv[]) {
    // recibe un argumento, nuestro archivo de texto
    // que contiene la expresión
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

    // for(char c : cadena) {
    //     cout << c << "_" << (int) c << "  ";
    // }
    // cout << endl;
    // manda a llamar a nuestro AFD para analizar cada caracter
    AFD(cadena);
    // imprime la lista de tokens
    cout << "\033[92mLista de tokens\033[0m" << endl;
    for(TokenProcesado i: tokensProcesados){
        if(i.tipo != TOKEN_NO_RECONOCIDO)
            cout << "<\033[93m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
        else
            cout << "<\033[91m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
    }
    // cierra el archivo
    archivo.close();

    return 0;
}
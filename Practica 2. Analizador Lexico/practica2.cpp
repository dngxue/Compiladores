#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

enum TipoToken{
    INICIO,
    NUMERO,
    ID_1,
    ID_2,
    OPERADOR,
    ID_INVALIDO,
    NUM_INVALIDO,
    MUERTO
};

unordered_map<TipoToken, string> tipoTokenString = {
    {INICIO, "INICIO"},
    {NUMERO,"NUMERO"},
    {ID_1,"ID_1"},
    {ID_2,"ID_2"},
    {OPERADOR,"OPERADOR"},
    {ID_INVALIDO,"ID_INVALIDO"},
    {NUM_INVALIDO,"NUM_INVALIDO"},
    {MUERTO, "MUERTO"}
};

class TokenProcesado
{
    public:
    int linea;
    string valor;
    TipoToken tipo;
    TipoToken posibleError;

    TokenProcesado(TipoToken tipo, string valor, int linea){
        this->tipo = tipo;
        this->valor = valor;
        this->linea = linea;
        this->posibleError = INICIO;
    }
};

unordered_set<char> signos = {'+','-'};
TipoToken posibleError = INICIO;
vector<string> tokens;
vector<TokenProcesado> tokensProcesados;
vector<TokenProcesado> errores;
string nombreArchivo;

TipoToken AFD(string token){
    TipoToken Estado = INICIO;

    for(char c: token){
        
        if(Estado == MUERTO) return MUERTO;

        if(Estado != MUERTO){
            /*  se actualiza la variable global con
                el valor de Estado */
            posibleError = Estado;
        }

        switch(Estado){

            case INICIO:
                if(isdigit(c) && c != '0'){ // 1-9
                    Estado = NUMERO;
                }
                else if(c == '0'){          // 0
                    Estado = NUM_INVALIDO;
                }
                else if(signos.count(c)){   // +,-
                    Estado = OPERADOR;
                }
                else if(isalpha(c)){        // a-z
                    Estado = ID_1;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case NUMERO:
                if(isdigit(c)){             // 0-9
                    Estado = NUMERO;
                }
                else if(signos.count(c)){   // +,-
                    Estado = OPERADOR;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case ID_1:
                if(isalpha(c)){             // a-z
                    Estado = ID_2;
                }else if(signos.count(c)){  // +,-
                    Estado = OPERADOR;
                }else{
                    Estado = MUERTO;
                }
                break;
            case ID_2:
                if(isalpha(c)){             // mas de dos letras
                    Estado = ID_INVALIDO;
                }
                else if(signos.count(c)){
                    Estado = OPERADOR;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case ID_INVALIDO:
                if(signos.count(c)){
                    Estado = OPERADOR;
                }else if(isalpha(c)){
                    Estado = ID_INVALIDO;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case OPERADOR:
                if(isdigit(c) && c != '0'){
                    Estado = NUMERO;
                }
                else if(c == '0'){
                    Estado = NUM_INVALIDO;
                }
                else if(isalpha(c)){
                    Estado = ID_1;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case NUM_INVALIDO:
                if(isdigit(c)){
                    Estado = NUM_INVALIDO;
                }
                else if(signos.count(c)){
                    Estado = OPERADOR;
                }
                else{
                    Estado = MUERTO;
                }
                break;
        }

    }

    return Estado;
}

void procesarCadena(string cadena, int num_linea){
    if(cadena.size() == 0) return;
    string aux = "";

    // Por cada caracter especial, divide la cadena y agrega el resultado a tokens
    /* itera a través de cada caracter c de la cadena */
    for(char c: cadena){
        if(signos.count(c)){
            aux += c;
        }
        else{
            aux += c;
        }

    }

    if(!aux.empty()){
        tokens.push_back(aux);
        tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
        tokensProcesados.rbegin()->posibleError = posibleError;
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
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
    string cadena;
    int num_linea = 1;

    tokensProcesados.push_back(TokenProcesado(INICIO, "", num_linea));

    while(getline(archivo, linea)){
        istringstream ss(linea);

        while(ss >> cadena){
            procesarCadena(cadena, num_linea);
        }
        num_linea++;
    }

    for(TokenProcesado i: tokensProcesados){
        cout << "<\033[93m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
    }

    // Se hacen aparte para 0 y tokensProcesados.size()-1 por el tema de los indices
    // for(int i = 1; i < tokensProcesados.size()-1; i++){
        
    //     /*  se declara una referencia t a un elemento del vector
    //         tokensProcesado en la posicion i
    //         asi facilitamos el acceso al elemento actual 
    //         sin necesidad de utilizar la notación del puntero */ 
    //     TokenProcesado &t = tokensProcesados[i];
    //     TokenProcesado &t_ant = tokensProcesados[i-1];
    //     TokenProcesado &t_sig = tokensProcesados[i+1];
        
    //     //cout << "Palabra: " << t.valor << ", Tipo: " << tipoTokenString[t.tipo] <<  ", Linea: " << t.linea << endl;

    //     /*  se declara una variable tipo y se le asigna el valor del
    //         miembro tipo del token actual t */
    //     TipoToken tipo = t.tipo;
       
    //     /* si no llego a ningún estado de aceptación */
    //     if(tipo == MUERTO){
    //         errores.push_back(t);
    //         lineasError.push_back(t.linea);
    //     }
    //     else if(estadosAceptacion.count(tipo) == 0){
    //         t.posibleError = t.tipo;
    //         errores.push_back(t);
    //         lineasError.push_back(t.linea);
    //     }

    // }
    
    // for(TokenProcesado i: imprimir){
    //     cout << "<" << i.tipo << ", " << i.valor << ">" << endl;
    // }


    archivo.close();

    return 0;
}
#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

enum tipoToken {
    IDENTIFICADOR,
    MULTIPLICACION,
    TOKEN_NO_RECONOCIDO,
    PARENTESIS_APERTURA,
    PARENTESIS_CIERRE,
    SUMA,
    FIN
};

unordered_map<tipoToken, string> tipoTokenString = {
    {IDENTIFICADOR,"IDENTIFICADOR"},
    {SUMA,"SUMA"},
    {MULTIPLICACION,"MULTIPLICACION"},
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


/* TABLA DE PRODUCCIONES LL1 -------------------------------------------------------------------------------------- */
struct produccion{
    string left;
    vector<string> right;
};

// definimos la tabla del análisis sintáctico
unordered_map<string, unordered_map<string,produccion>> m;

unordered_set<char> signos = {'+','*'};
unordered_set<string> terminales = {"IDENTIFICADOR", "SUMA", "MULTIPLICACION", "PARENTESIS_APERTURA", "PARENTESIS_CIERRE", "FIN"};
vector<TokenProcesado> tokensProcesados;
string nombreArchivo;

void agregarProduccion(string noTerminal, tipoToken terminal, produccion p);
void ll1(vector<TokenProcesado> tokensProcesados);
void producciones();
void imprimirPila(stack<string> pila);
void match(string x, TokenProcesado a);

/* -------------------------------------------------------------------------------------------------------------- */

void AFD(string cadena){
    int estado = 0; 
    string lexema = ""; 
    tipoToken t; 
    char c; 

    for(int i=0; i<cadena.size(); i++){
        c = cadena[i]; 

        switch(estado){

            case 0: 
                if(isalpha(c)){
                    lexema += c;        
                    estado = 1;
                }
                else if(c == '+'){
                    lexema += c;
                    t = SUMA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == '*'){
                    lexema += c;
                    t = MULTIPLICACION;
                    tokensProcesados.push_back(TokenProcesado(t, lexema));
                    lexema = "";
                    estado = 0;
                }else if(c == ' ' || c == '\t' || c == '\n' || c == 13){
                    	
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
                    estado = 2;
                }
                break;

            case 1:     // IDENTIFICADOR
                if(isalpha(c) || isdigit(c) || c == '_'){
                    lexema += c;
                    estado = 1;
                }else{
                    t = IDENTIFICADOR;
                    tokensProcesados.push_back(TokenProcesado(t,lexema));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;

            case 2: // ESTADO MUERTO
                if(!isdigit(c) && !signos.count(c) && !isalpha(c) && c != 0 && !(c = ' ')){
                    lexema += c;
                    estado = 2;
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
    tokensProcesados.push_back(TokenProcesado(t,"$"));
}

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
        cadena += linea;
    }

    cadena += (char)0;
    // analizador léxico
    AFD(cadena);
    // funcion para agregar mis producciones
    producciones();
    cout << "\n\033[34mExpresion: \033[0m" << "\033[33m" << cadena << "\033[0m" << endl;
    ll1(tokensProcesados);
    
    
    // cout << "\033[92mLista de tokens\033[0m" << endl;
    // for(TokenProcesado i: tokensProcesados){
    //     if(i.tipo != TOKEN_NO_RECONOCIDO)
    //         cout << "<\033[93m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
    //     else
    //         cout << "<\033[91m " << tipoTokenString[i.tipo] << "\033[0m, \033[96m" << i.valor << "\033[0m >" << endl;
    // }

    // funcion de análisis sintáctico
    archivo.close();

    return 0;
}

TokenProcesado preanalisis = {IDENTIFICADOR, ""};
vector<TokenProcesado> listatokens;
int pos = 0;

void ll1(vector<TokenProcesado> tokensProcesados){
    stack<string> s;
    s.push("FIN");
    s.push("E");

    string x,a;
    listatokens = tokensProcesados;
    preanalisis = listatokens[pos];

    int count = 0;
    while(!s.empty()){
        x = s.top();
        // cout << "Tope de la pila: " << x << endl;
        // cout << "a: " << tipoTokenString[preanalisis.tipo] << endl;

        if(terminales.count(x)){
            if(x == "FIN"){
                if(x == tipoTokenString[preanalisis.tipo])
                    s.pop();
                else{
                    cout << "\033[91mCADENA INVALIDA\033[0m" << endl;
                    if(pos-1 != -1)
                        pos--;
                    preanalisis = listatokens[pos];
                    match(x,preanalisis);
                    return;
                }
            }else if(x == tipoTokenString[preanalisis.tipo]){
                s.pop();
                pos++;
                preanalisis = listatokens[pos];
            }else{
                cout << "\033[91mCADENA INVALIDA\033[0m" << endl;
                if(pos-1 != -1)
                    pos--;
                preanalisis = listatokens[pos];
                match(x,preanalisis);
                return;
            }
        }else{
            auto it = m[x].find(to_string(preanalisis.tipo));
            if(it != m[x].end()){
                s.pop();
                const auto& p = it->second;
                for(auto rit = p.right.rbegin(); rit != p.right.rend(); ++rit){
                    if(*rit != "")
                        s.push(*rit);
                }
                // cout << "Tope de la pila despues de buscar: " << s.top() << endl;
                // imprimirPila(s);
            }else{
                cout << "\033[91mCADENA INVALIDA\033[0m" << endl;
                if(pos-1 != -1)
                    pos--;
                preanalisis = listatokens[pos];
                match(x,preanalisis);
                return;
            }
        }
    }

    cout << "\033[92mCADENA VALIDA\033[0m" << endl;
}

void imprimirPila(stack<string> pila) {
    while (!pila.empty()) {
        cout << pila.top() << " "; // Imprime el elemento en la parte superior de la pila
        pila.pop(); // Elimina el elemento en la parte superior de la pila
    }
    cout << endl;
}

void match(string x, TokenProcesado a){
    cout << "\nError despues de \033[96m" << a.valor << "\033[0m\n";
    cout << "Se esperaba un ";
    if(x == "E"){
        cout << "\033[93mIDENTIFICADOR\033[0m o \033[93m(\033[0m \n";
    }else if(x == "E'"){
        cout << "\033[93m+\033[0m, \033[93m)\033[0m o \033[93mFIN DE CADENA\033[0m\n";
    }else if(x == "T"){
        cout << "\033[93mIDENTIFICADOR\033[0m o \033[93m(\033[0m \n";
    }else if(x == "T'"){
        cout << "\033[93m+\033[0m, \033[93m*\033[0m, \033[93m)\033[0m o \033[93mFIN DE CADENA\033[0m\n";
    }else if(x == "F"){
        cout << "\033[93mIDENTIFICADOR\033[0m o \033[93m(\033[0m \n";
    }else{
        cout << "\033[93m" << x << "\033[0m\n";
    }
}



/*  Funcion para agregar producciones a la tabla
    Recibe: string de mi no terminal
            string de mi terminal
            produccion */
void agregarProduccion(string noTerminal, tipoToken terminal, produccion p){
    m[noTerminal][to_string(terminal)] = p;
}

void producciones(){
    agregarProduccion("E",IDENTIFICADOR,{"E",{"T","E'"}});
    agregarProduccion("E",PARENTESIS_APERTURA,{"E",{"T","E'"}});

    agregarProduccion("E'",SUMA,{"E'",{"SUMA","T","E'"}});
    agregarProduccion("E'",PARENTESIS_CIERRE,{"E'",{""}});
    agregarProduccion("E'",FIN,{"E'",{""}});

    agregarProduccion("T",IDENTIFICADOR,{"T",{"F","T'"}});
    agregarProduccion("T",PARENTESIS_APERTURA,{"T",{"F","T'"}});

    agregarProduccion("T'",SUMA,{"T'",{""}});
    agregarProduccion("T'",MULTIPLICACION,{"T'",{"MULTIPLICACION","F","T'"}});
    agregarProduccion("T'",PARENTESIS_CIERRE,{"T'",{""}});
    agregarProduccion("T'",FIN,{"T'",{""}});

    agregarProduccion("F",IDENTIFICADOR,{"F",{"IDENTIFICADOR"}});
    agregarProduccion("F",PARENTESIS_APERTURA,{"F",{"PARENTESIS_APERTURA","E","PARENTESIS_CIERRE"}});
}

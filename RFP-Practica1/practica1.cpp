#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

vector<vector<string>> transiciones;
vector<char> alfabeto;
vector<string> estados;
string estadoInicial;
vector<string> estadosFinales;

string cadena,nombreArchivo;

void agregarAlfabeto(char simbolo){
    alfabeto.push_back(simbolo);
}

void agregarInicial(const string& inicial){
    estadoInicial = inicial;
}

void agregarEstados(const string& simbolo){
    estados.push_back(simbolo);
}

void agregarFinales(const string& simbolo){
    estadosFinales.push_back(simbolo);
}

// void imprimir() {
//     cout << "\n\033[96mAlfabeto:\033[0m";
//     for(int i=0; i<alfabeto.size(); i++) {
//         cout << " " << alfabeto.at(i);
//     }

//     cout << "\n\033[93mEstado Inicial:\033[0m " << estadoInicial;

//     cout << "\n\033[93mEstados:\033[0m";
//     for(int i=0; i<estados.size(); i++) {
//         cout << " " << estados.at(i);
//     }

//     cout << "\n\033[93mEstados Finales:\033[0m";
//     for(int i=0; i<estadosFinales.size(); i++) {
//         cout << " " << estadosFinales.at(i);
//     }
// }
// para obtener el índice de la fila
size_t getFila(string estadoActual) {
    // busca el estado en el vector
    auto estado = find(estados.begin(), estados.end(), estadoActual);
    // calcula la distancia (el índice)
    // entre el inicio del vector y el estado
    return distance(estados.begin(), estado);
}
// para obtener el índice de la columna
size_t getColumna(char c) {
    auto caracter = find(alfabeto.begin(), alfabeto.end(), c);
    return distance(alfabeto.begin(), caracter);
}

bool automata(){
    // Primero se busca el índice del estado inicial (fila)
    size_t estadoActual = getFila(estadoInicial);

    // itera sobre cada símbolo de la cadena de entrada
    for(int i=0; i<cadena.size(); i++){
        // busca el símbolo en la posición i de la cadena a través
        // de nuestro vector que contiene el alfabeto 
        if(find(alfabeto.begin(), alfabeto.end(), cadena.at(i)) != alfabeto.end()){
            // se busca el estado al que transita en la fila de nuestro 
            // estado actual y en la columna en donde se escuentra el 
            // símbolo dentro de nuestro alfabeto, se guarda en una variable
            // ese estado
            string s = transiciones.at(estadoActual).at(getColumna(cadena.at(i)));
            // se actualiza el valor del índice de la fila de
            // nuestro estado actual
            estadoActual = getFila(s);
        }else{
            // si no encuentra el símbolo dentro del alfabeto
            cout << "\n\033[91mCadena NO valida\033[0m" << endl;
            return false;
        }
    }

    // Una vez que termina de evaluar la cadena, iteramos a lo
    // largo de nuestro vector de estados finales para
    // verificar si el estado actual es uno de los estados finales
    for(int i=0; i<estadosFinales.size(); i++){
        if(estados.at(estadoActual) == estadosFinales.at(i)) {
            cout << "\n\033[92mCadena aceptada\033[0m";
            return true;
        }
    }
    // si no es parte de los estados finales
    cout << "\n\033[91mCadena NO aceptada\033[0m";
    return false;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        nombreArchivo = argv[1];
        cadena = argv[2];
    } else {
        cout << "nombreArchivo.txt cadena" << endl;
    }

    ifstream archivo(nombreArchivo);

    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    // para almacenar cada línea leída del archivo
    string linea;
    // para saber el número de línea actual
    int num_linea = 1;

    // lee cada línea del archivo
    while(getline(archivo,linea)){

        if(num_linea == 1){ // alfabeto
            istringstream alfa(linea);
            char simbolo;
            // para cada palabra de la línea
            while(alfa >> simbolo) agregarAlfabeto(simbolo);

        }else if(num_linea == 2){ // estado inicial
            istringstream inicial(linea);
            string estado;
            while(inicial >> estado) agregarInicial(estado);

        }else if(num_linea == 3){ // conjunto de estados
            istringstream estado(linea);
            string estados;
            while(estado >> estados) agregarEstados(estados);

        }else if(num_linea == 4){ // estados finales
            istringstream final(linea);
            string finales;
            while(final >> finales) agregarFinales(finales);
            
        }else{ // transiciones
            vector<string> aux;
            istringstream fila(linea);
            string casilla;
            while(fila >> casilla) aux.push_back(casilla);
            // para no agregar filas vacías
            if(aux.size() != 0) transiciones.push_back(aux);
        }

        num_linea++;
    }

    // imprimir();
    automata();

    cout << "\n";

    archivo.close();

    return 0;
}
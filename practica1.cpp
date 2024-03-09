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

void imprimir() {
    cout << "\n\033[96mAlfabeto:\033[0m";
    for(int i=0; i<alfabeto.size(); i++) {
        cout << " " << alfabeto.at(i);
    }

    cout << "\n\033[93mEstado Inicial:\033[0m " << estadoInicial;

    cout << "\n\033[93mEstados:\033[0m";
    for(int i=0; i<estados.size(); i++) {
        cout << " " << estados.at(i);
    }

    cout << "\n\033[93mEstados Finales:\033[0m";
    for(int i=0; i<estadosFinales.size(); i++) {
        cout << " " << estadosFinales.at(i);
    }

    // cout << "\n\033[96mTabla de transiciones:\n\033[0m";
    // for(int i=0; i<transiciones.size(); i++) {
    //     for(int j=0; j<transiciones.at(0).size(); j++) {
    //         cout << transiciones.at(i).at(j) << " ";
    //     }
    //     cout << "\n";
    // }
}

size_t getFila(string estadoActual) {
    auto estado = find(estados.begin(), estados.end(), estadoActual);
    return distance(estados.begin(), estado);
}

size_t getColumna(char c) {
    auto caracter = find(alfabeto.begin(), alfabeto.end(), c);
    return distance(alfabeto.begin(), caracter);
}

bool automata(){
    // Encontrar el índice de un estado
    size_t estadoActual = getFila(estadoInicial);
    //cout << "El elemento " << estadoInicial << " se encuentra en el índice " << estadoActual << "\tEstado: "<< estados.at(estadoActual) <<endl;

    for(int i=0; i<cadena.size(); i++){
        if(find(alfabeto.begin(), alfabeto.end(), cadena.at(i)) != alfabeto.end()){
            string s = transiciones.at(estadoActual).at(getColumna(cadena.at(i)));
            estadoActual = getFila(s);
        }else{
            cout << "\n\033[91mCadena NO valida\033[0m" << endl;
            return false;
        }
    }

    for(int i=0; i<estadosFinales.size(); i++){
        if(estados.at(estadoActual) == estadosFinales.at(i)) {
            cout << "\n\033[92mCadena aceptada\033[0m";
            return true;
        }
    }
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

    string linea;
    int num_linea = 1;

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

    imprimir();
    automata();

    cout << "\n";

    archivo.close();

    return 0;
}
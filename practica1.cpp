#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

vector<vector<string>> transiciones;
vector<char> alfabeto;
vector<string> estados;
string estadoInicial;
vector<string> estadosFinales;

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
    cout << "\033[92mAlfabeto:\033[0m";
    for(int i=0; i<alfabeto.size(); i++) {
        cout << " " << alfabeto.at(i);
    }
    cout << "\n\033[92mTabla de transiciones:\n\033[0m";
    for(int i=0; i<transiciones.size(); i++) {
        for(int j=0; j<transiciones.at(0).size(); j++) {
            cout << transiciones.at(i).at(j) << " ";
        }
        cout << "\n";
    }
}

int main(){
    string nombreArchivo = "ejemplo.txt";
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
        }else{
            vector<string> aux;
            istringstream fila(linea);
            string casilla;
            while(fila >> casilla) aux.push_back(casilla);
            if(aux.size() != 0) transiciones.push_back(aux);
        }

        num_linea++;
    }

    imprimir();

    archivo.close();

    return 0;
}
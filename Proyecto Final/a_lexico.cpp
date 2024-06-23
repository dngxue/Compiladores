#include "a_lexico.h"

vector<TokenProcesado> tokensProcesados;

TokenProcesado::TokenProcesado(tipoToken tipo, string valor, int linea) {
    this->tipo = tipo;
    this->valor = valor;
    this->linea = linea;
}

unordered_set<char> signos = {'+', '-', '*', '/'};
unordered_set<string> palabra_reservada = {"var", "if", "else", "print", "potencia", "for", "while"};

void AFD(string cadena){
    int estado = 0; 
    int linea = 1;
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
                // Signos -------------------------------------------------------
                else if(c == '+'){
                    lexema += c;
                    t = SUMA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '-'){
                    lexema += c;
                    t = RESTA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '*'){
                    lexema += c;
                    t = MULTIPLICACION;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '/'){
                    lexema += c;
                    estado = 3;
                }else if(c == '<'){
                    lexema += c;
                    estado = 8;
                }else if(c == '>'){
                    lexema += c;
                    estado = 9;
                }else if(c == '!'){
                    lexema += c;
                    estado = 11;
                }
                // Fin Signos ---------------------------------------------------
                else if(c == '0'){
                    lexema += c;
                    estado = 4;
                }else if(c == ' ' || c == '\t' || c == 13){
                    	
                }else if(c == '.'){
                    lexema += c;
                    estado = 5;
                // Bloques ------------------------------------------------------
                }else if(c == '('){
                    lexema += c;
                    t = PARENTESIS_APERTURA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == ')'){
                    lexema += c;
                    t = PARENTESIS_CIERRE;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '{'){
                    lexema += c;
                    t = CORCHETE_APERTURA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '}'){
                    lexema += c;
                    t = CORCHETE_CIERRE;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '='){
                    lexema += c;
                    estado = 7;
                }else if(c == ';'){
                    lexema += c;
                    t = PUNTO_COMA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == ','){
                    lexema += c;
                    t = COMA;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '"'){
                    lexema += c;
                    t = COMILLAS;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 10;
                }else if(c == '|') {
                    lexema += c;
                    t = OR;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }
                else if(c == '&') {
                    lexema += c;
                    t = AND;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }
                // Fin Bloques --------------------------------------------------
                // IDENTIFICAR NUMERO DE LINEA ----------------------------------
                else if(c == '\n'){
                        linea++;
                }
                // --------------------------------------------------------------
                else{
                    lexema += c;
                    estado = 6;
                }
                break;

            case 1:     // NUMERO ENTERO
                if(isdigit(c)){
                    lexema += c;            
                    estado = 1;
                }/*else if(c == '.'){
                    lexema += c;
                    estado = 5;
                }*/
                else{
                    t = NUMERO;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;

            case 2:     // IDENTIFICADOR - PALABRA RESERVADA
                if((isalpha(c) || isdigit(c) || c == '_')){
                    lexema += c;
                    estado = 2;
                }else{
                    if(palabra_reservada.count(lexema)){
                        if(lexema == "var")
                            t = VAR;
                        else if(lexema == "if")
                            t = IF;
                        else if(lexema == "else")
                            t = ELSE;
                        else if(lexema == "print")
                            t = PRINT;
                        else if(lexema == "potencia")
                            t = POTENCIA;
                        else if(lexema == "while")
                            t = WHILE;
                        else if(lexema == "for")
                            t = FOR;

                        tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                        lexema = "";
                        estado = 0;
                        i--;
                    }else{
                        t = IDENTIFICADOR;
                        tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                        lexema = "";
                        estado = 0;
                        i--;
                    }
                }
                break;

            case 3:     // COMENTARIO
                if(c == '/'){
                    lexema +=c;
                    t = COMENTARIO;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else{
                    t = DIVISION;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
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
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
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
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
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
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;
            
            case 7:     // IGUALDAD == | =
                if(c == '='){
                    lexema += c;
                    t = IGUALDAD;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else{
                    t = ASIGNACION;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;
            
            case 8:     // MENOR IGUAL <= | < | <<
                if(c == '='){
                    lexema += c;
                    t = MENOR_IGUAL;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '<'){
                    lexema += c;
                    t = CORR_IZQ;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else{
                    t = MENOR;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;
            
            case 9:     // MAYOR IGUAL >= | > | >>
                if(c == '='){
                    lexema += c;
                    t = MAYOR_IGUAL;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else if(c == '>'){
                    t = CORR_DER;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                }else{
                    t = MAYOR;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;
            
            case 10:
                if(c != '"' && c != ';' && c != '\n' && c != '<') {
                    lexema += c;
                    estado = 10;
                }else{
                    t = STRING;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    lexema += c;
                    if(c == '"'){
                        tokensProcesados.push_back(TokenProcesado(COMILLAS, lexema, linea));
                    }else{
                        i--;
                    }
                    lexema = "";
                    estado = 0;
                }
                break;
            
            case 11:
                if(c == '=') {
                    lexema += c;
                    t = DIFERENTE;
                    tokensProcesados.push_back(TokenProcesado(t,lexema,linea));
                    estado = 0;
                }else {
                    t = TOKEN_NO_RECONOCIDO;
                    tokensProcesados.push_back(TokenProcesado(t, lexema, linea));
                    lexema = "";
                    estado = 0;
                    i--;
                }
                break;
        }

    }
    t = FIN;
    tokensProcesados.push_back(TokenProcesado(t,"FIN", linea));
}

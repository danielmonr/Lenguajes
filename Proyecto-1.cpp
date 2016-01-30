#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define INT_MIN 48
#define INT_MAX 57
#define SLASH 47
#define CHAR_MAYUS_MIN 65
#define CHAR_MAYUS_MAX 90
#define CHAR_MINUS_MIN 97
#define CHAR_MINUS_MAX 122
#define PAR_ABIERTA 40
#define PAR_CERRADA 41
#define IGUAL 61
#define MAS 43
#define MULTIPLICACION 42
#define MENOS 45
#define POTENCIA 94
#define ESPACIO 32

using namespace std;

// Estructura para guardar la informacion de un Token
struct par{
    string valor;
    string tipo;
};

typedef struct par Par;

// Declaracion de las funciones
int NuevoToken(string l, int it);
int NuevaVariable(string l, int it);
int NuevaAsignacion(string l, int it);
int NuevaSuma(string l, int it);
int NuevaResta(string l, int it);
int NuevaMultiplicacion(string l, int it);
int NuevaDivision(string l, int it);
int NuevaPotencia(string l, int it);
int NuevoSimboloEspecial(string l, int it);
int NuevoComentario(string l, int it);
int NuevoEntero(string l, int it);
int NuevoFlotante(string l, int it);

void imprimirTokens();

// Vector global en el que se guardan los Tokens generados
vector<Par> tokens;

// Funcion main
int main(int argc, char** argv){
    string myfile;
	// Checar que se halla ejecutado el programa con un argumento
    if (argc != 2){
        cout << "Eror no eligió algún archivo" << endl;
        return 0;
    }
    else{
        myfile = argv[1];
    }
    
    string linea;

	// Abrir archivo especificado
    ifstream archivo(myfile);
	// Comprobar si el archivo fue abierto
    if(archivo){
        cout << "Apertura exitosa..." << endl;
    }
    else{
        cout << "No se puede abrir el archivo" << endl;
        return 0;
    }
	// Recorrer todo el archivo linea por linea
    while (getline( archivo, linea)){
		// Recorrer la linea por tokens
        for (int i= 0; i < linea.length(); ++i){
			// asignar a i el ultimo caracter del token en la posición original de i
            i = NuevoToken(linea, i);
        }
    }
	// Cerrar archivo
    archivo.close();
    // Imprimir tokens
    imprimirTokens();
    return 0;
}

// funcion para la creacion de un Token
int NuevoToken(string l, int it){
	// Comprobar la posición del iterador
    if(it >= l.length())
        return it;
    char temp = l[it];
    
	// Ejecutar alguna funcion de acuerdo al caracter en la posición del iterador
    if(temp == ESPACIO){
        return it;
    }
    else if((temp >= CHAR_MAYUS_MIN && temp <= CHAR_MAYUS_MAX) || (temp >= CHAR_MINUS_MIN && temp <= CHAR_MINUS_MAX)){
        return NuevaVariable(l,it);
    }
    else if(temp == SLASH){
        return NuevaDivision(l, it);
    }
    else if(temp == MAS){
        return NuevaSuma(l,it);
    }
    else if(temp == MENOS){
        return NuevaResta(l,it);
    }
    else if(temp == MULTIPLICACION){
        return NuevaMultiplicacion(l,it);
    }
    else if(temp == IGUAL){
        return NuevaAsignacion(l,it);
    }
    else if(temp >=INT_MIN && temp <= INT_MAX){
        return NuevoEntero(l,it);
    }
    else if(temp == POTENCIA){
        return NuevaPotencia(l, it);
    }
    else if(temp == PAR_ABIERTA || temp == PAR_CERRADA){
        return NuevoSimboloEspecial(l,it);
    }
    else{
		// En caso de que no se reconozca el simbolo regresar la misma posición
        return it;
    }
    
}

// Funcion para la creacion de un token de tipo Variable
int NuevaVariable(string l, int it){
    int i = it;
    string var = "";
    while (i < l.length() && ((l[i] >= CHAR_MAYUS_MIN && l[i] <= CHAR_MAYUS_MAX) || (l[i] >= INT_MIN && l[i] <= INT_MAX) || (l[i] >= CHAR_MINUS_MIN && l[i] <= CHAR_MINUS_MAX))){
        var += l[i];
        ++i;
    }
    --i;
    Par nuevo;
    nuevo.tipo = "Variable";
    nuevo.valor = var;
    tokens.push_back(nuevo);
    return i;
}


// Funcion para la creacion de un token de tipo Asignacion
int NuevaAsignacion(string l, int it){
    Par nuevo;
    nuevo.tipo = "Asignacion";
    nuevo.valor = "=";
    tokens.push_back(nuevo);
    return it;
}

// Funcion para la creacion de un token de tipo Suma
int NuevaSuma(string l, int it){
    Par nuevo;
    nuevo.tipo = "Suma";
    nuevo.valor = "+";
    tokens.push_back(nuevo);
    return it;
}

// Funcion para la creacion de un token de tipo Resta
int NuevaResta(string l, int it){
    Par nuevo;
    nuevo.tipo = "Resta";
    nuevo.valor = "-";
    tokens.push_back(nuevo);
    return it;
}

// Funcion para la creacion de un token de tipo Multiplicacion
int NuevaMultiplicacion(string l, int it){
    Par nuevo;
    nuevo.tipo = "Multiplicacion";
    nuevo.valor = "*";
    tokens.push_back(nuevo);
    return it;
}

// Funcion para la creacion de un token de tipo Division
int NuevaDivision(string l, int it){
	// En caso de que el siguiente caracter sea otro '/' se llama a la funcion que crear un token de tipo comentario
    if (l[it+1] == SLASH)
        return NuevoComentario(l, it);
        
    Par nuevo;
    nuevo.tipo = "Division";
    nuevo.valor = "/";
    tokens.push_back(nuevo);
    return it;
}

// Funcion para la creacion de un token de tipo Potencia
int NuevaPotencia(string l, int it){
    Par nuevo;
    nuevo.tipo = "Potencia";
    nuevo.valor = "^";
    tokens.push_back(nuevo);
    return it;
}

// Funcion para la creacion de un token de tipo Caracter especial
int NuevoSimboloEspecial(string l, int it){
    Par nuevo;
	if (l[it] == '('){
		nuevo.tipo = "Parentesis abre";
		nuevo.valor = "(";
	}
	else{
		nuevo.tipo = "Parentesis cierra";
		nuevo.valor = ")";
	}
    tokens.push_back(nuevo);
    return it;
}

// Funcion para la creacion de un token de tipo Comentario
int NuevoComentario(string l, int it){
    string var = "";
    for (int i = it; i < l.length(); ++i){
        var += l[i];
    }
    Par nuevo;
    nuevo.tipo = "Comentario";
    nuevo.valor = var;
    tokens.push_back(nuevo);
	// Se regresa la ultima posicion de la linea
    return l.length();
}

// Funcion para la creacion de un token de tipo Entero
int NuevoEntero(string l, int it){
    int i = it;
    string var = "";
    while (i < l.length() && l[i] >= INT_MIN && l[i] <= INT_MAX){
        var += l[i++];
		// En caso de encontrar un '.' se llama a la funcion que crea un Flotante
        if (l[i] == '.')
            return NuevoFlotante(l,it);
    }
    Par nuevo;
    nuevo.tipo = "Entero";
    nuevo.valor = var;
    tokens.push_back(nuevo);
    return (i-1);
    
}

// Funcion para la creacion de un token de tipo Flotante
int NuevoFlotante(string l, int it){
    int i = it;
    string var = "";
    while (i < l.length() && ((l[i] >= INT_MIN && l[i] <= INT_MAX) || (l[i] == '.') || (l[i] == 'E'))){
        var += l[i++];
    }
    Par nuevo;
    nuevo.tipo = "Flotante";
    nuevo.valor = var;
    tokens.push_back(nuevo);
    return i;
}

// Funcion para imprimir los Tokens
void imprimirTokens(){
    cout << "Token\tTipo\n";
    for (auto it:tokens){
        cout << it.valor <<'\t' << it.tipo << endl;
    }
}

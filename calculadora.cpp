// David Tanarro de las Heras (grupo F)
// Javier Lanchas Hernández (grupo F)
// Se ha hecho la version 4, Lab.: ninguno

#include <iostream>
#include <string>
#include <cstdlib> // srand() y rand()
#include <ctime>   //time
#include <fstream>
#include <iomanip>
#include <stdlib.h> // system("pause") y system("CLS")

using namespace std;

const int META = 31;
typedef enum {nadie, automata, persona} tJugador;

tJugador pasaCalculadora(int nivel);
tJugador quienEmpieza();
bool mismaFila(int ultimo, int nuevo);
bool mismaColumna(int ultimo, int nuevo);
bool digitoValido(int ultimo, int nuevo);
int digitoAleatorio();
int digitoAutomata(int ultimo);
int digitoPersona();
int digitoPersona(int ultimo);
int menu();
bool mostrar(string nombArch);
bool actInforme(int jugadas, int ganadas, int abandonos);
//para la version 4
int digitoAutomataNivelado(int ultimo, int suma);
bool predictor(int nuevo, int suma);
int submenu();

int main() {
	int opcion, nivel;
	int jugadas, ganadas, abandonos;
	string usuario;
	tJugador ganador;
	bool esMostrado, esActualizado;

	srand(time(NULL)); // Se establece la semilla
	jugadas = ganadas = abandonos = 0; // Se inicializan a cero los contadores

	cout << "¡Bienvenido a pasa la calculadora! \n";
	cout << "¿Como te llamas? ";
	cin >> usuario;
	cout << "Hola " << usuario << endl;

	opcion = menu();
	while (opcion != 0) {
		switch (opcion) {
		case 1:
			{
				nivel = submenu();
				jugadas = ganadas = abandonos = 0;
				ganador = pasaCalculadora(nivel);
				if(ganador == persona){
					cout << "¡Enhorabuena has ganado! \n";
				}
				else if (ganador == nadie){
					cout << "¡Has abandonado! \n";
					abandonos = 1; // partida abandonada por la persona
				}
				else{
					cout << "¡Has perdido! \n";
					ganadas = 1; // partida ganada por el automata
				}
				jugadas = 1;
			}
			break;
		case 2:
			{
				esMostrado = mostrar("versionPC.txt");
				if (!esMostrado)
					cout << "No existe ningun archivo nombrado como 'versionPC.txt' \n";
			}
			break;
		case 0:
			break;
		}

		esActualizado = actInforme(jugadas, ganadas, abandonos);

		opcion = menu();
	}

	cout << "Hasta la proxima"<< usuario << endl;
	// (*) 2 posibilidades mas de hacer una pausa (comentado en el final del codigo)
	system("pause");
	
	return 0;
}

//Conduce el desarrollo del juego y devuelve el ganador. Si se abandona devuelve Nadie.
//Utiliza, directa o indirectamente, las funciones que siguen.
tJugador pasaCalculadora(int nivel) {
	tJugador ganador, empieza;
	int nuevo, ultimo, suma;
	bool salir = false;
	bool abandonar = false;

	suma = 0;
	ultimo = 0; // Valor inicial del ultimo digito
	empieza = quienEmpieza();
	if(empieza == persona){
		cout << "Empiezas tu \n";

		while (!abandonar && !salir){
			ultimo = digitoPersona(ultimo);
			if (ultimo == 0)
				abandonar = true;
			else{
				suma += ultimo;
				cout << "Suma: " << suma << endl;
				if(suma >= META){
					ganador = automata;
					salir = true;
				}
				else{
					ultimo = (nivel == 1) ? digitoAutomata(ultimo) : digitoAutomataNivelado(ultimo, suma);
					cout << "Yo pulso: "<< ultimo << endl;
					suma += ultimo;
					cout << "Suma: " << suma << endl;
					if(suma >= META){
						ganador = persona;
						salir = true;
					}
				}
			}
		}
	}
	else{
		cout << "Empiezo yo \n";

		ultimo = digitoAleatorio(); // Valor inicial a la variable ultimo
		while (!abandonar && !salir){
			ultimo = (nivel == 1) ? digitoAutomata(ultimo) : digitoAutomataNivelado(ultimo, suma);
			cout << "Yo pulso: "<< ultimo << endl;
			suma += ultimo;
			cout << "Suma: " << suma << endl;
			if(suma >= META){
				ganador = persona;
				salir = true;
			}

			else{
				ultimo = digitoPersona(ultimo);
				if (ultimo == 0)
					abandonar = true;
				else{
					suma += ultimo;
					cout << "Suma: " << suma << endl;
					if(suma >= META){
						ganador = automata;
						salir = true;
					}
				}
			}
		}
	}

	if (abandonar)
		ganador = nadie;

	return ganador;
}

//Decide aleatoriamente quien empieza.
tJugador quienEmpieza(){
	tJugador empiezaJugador;
	int aleatorio;

	aleatorio = rand() % 2;
	if(aleatorio == 0)
		empiezaJugador = automata;
	else
		empiezaJugador = persona;

	return empiezaJugador;
}

//Devuelve true si nuevo está en la misma fila que ultimo;
//en caso contrario devuelve false.
//Observa que la fila que ocupa un dígito, numeradas desde 0, corresponde con (dígito - 1) / 3.
bool mismaFila(int ultimo, int nuevo){
	bool encontrado = false;
	int filaUltimo, filaNuevo;

	if((ultimo >= 1) && (ultimo <= 9)){
		filaUltimo = (ultimo - 1) / 3;
		filaNuevo = (nuevo - 1) / 3;
		if(filaUltimo == filaNuevo)
			encontrado = true;
	}

	return encontrado;
}

//Devuelve true si nuevo está en la misma columna que ultimo;
//en caso contrario devuelve false.
//Observa que la columna que ocupa un dígito, numeradas desde 0, corresponde con (dígito - 1)% 3.
bool mismaColumna(int ultimo, int nuevo){
	bool encontrado = false;
	int columnaUltimo, columnaNuevo;
	
	if((ultimo >= 1) && (ultimo <= 9)){
		columnaUltimo = (ultimo - 1) % 3;
		columnaNuevo = (nuevo - 1) % 3;
		if(columnaUltimo == columnaNuevo)
			encontrado = true;
	}

	return encontrado;
}

//Devuelve true si nuevo cumple las reglas del juego con respecto a ultimo; en caso contrario devuelve false.
bool digitoValido(int ultimo, int nuevo){
	bool validez = false;

	if((nuevo != ultimo) && ((mismaFila(ultimo, nuevo)) || (mismaColumna(ultimo, nuevo))))
		validez = true;

	return validez;
}

//Devuelve un dígito.
int digitoAleatorio(){
	int digitoAleatorio;

	digitoAleatorio = (rand() % 9) + 1;

	return digitoAleatorio;
}

//Devuelve un dígito que cumpla las reglas del juego con respecto a ultimo.
//Por ejemplo digitoAutomata(2) puede devolver 1,3,5 u 8.
int digitoAutomata(int ultimo){
	int nuevo;

	do {
		nuevo = digitoAleatorio();
	} while (!(digitoValido(ultimo, nuevo)));

	return nuevo;
}

//Pide un dígito al jugador. Sólo devolverá un valor válido (entre 0 y 9). Para cada valor no válido, mostrará un error.
int digitoPersona(){
	int digitoUsuario;

	cin >> digitoUsuario;
	while ((digitoUsuario < 0) || (digitoUsuario > 9)){
		cout << "Error. Introduce un digito entre 0 y 9: \n";
		cin >> digitoUsuario;
	}

	return digitoUsuario;
}

//Pide un dígito al jugador mostrando el teclado.
//Sólo devolverá un valor que cumpla las reglas del juego. Para cada valor no válido, mostrará un error.
int digitoPersona(int ultimo){
	int nuevo;

	cout << "	7	8	9 \n";
	cout << "	4	5	6 \n";
	cout << "	1	2	3 \n";
	cout << "Introduce un digito (0 para abandonar): ";
	nuevo = digitoPersona();

	while((!digitoValido(ultimo, nuevo)) && (ultimo != 0) && (nuevo != 0)){
		cout << "Error: tiene que ser distinto de " << ultimo << " y estar en la misma fila o columna \n";
		cout << "	7	8	9 \n";
		cout << "	4	5	6 \n";
		cout << "	1	2	3 \n";
		cout << "Introduce un digito (0 para abandonar): ";
		nuevo = digitoPersona();
	}

	return nuevo;
}

//Muestra el menú, pide la opción y la devuelve como resultado.
//Sólo devolverá una opción válida. Para cada valor no válido, mostrará un error.
int menu() {
	int op = -1; // Cualquiera no válida

	while ((op < 0) || (op > 2)) {
		cout << "Selecciona una opcion \n\n";
		cout << "1 - Jugar \n";
		cout << "2 - Acerca de Pasa la calculadora \n";
		cout << "0 - Salir \n";
		cout << "Opcion: ";
		cin >> op;
		if ((op < 0) || (op > 2)) {
			cout << "¡Opcion no valida! \n";
		}
	}

	return op;
}

//Muestra en la consola el contenido del archivo de texto nombArch.
//Si el archivo no se encuentra, devuelve false, en otro caso true.
bool mostrar(string nombArch){
	bool encontrado = false;
	ifstream archivo;
	string linea;

	archivo.open(nombArch);
	if(archivo.is_open()){
		encontrado = true;

		while (getline (archivo, linea) != NULL)
		{
			cout << linea << endl;
		}
		archivo.close();
	}

	return encontrado;
}

//Actualiza el archivo informePC.txt con los tres argumentos.
//En caso de no encontrar el archivo, lo crea y devuelve false; en otro caso devuelve true.
bool actInforme(int jugadas, int ganadas, int abandonos){
	bool encontrado = false;
	int vecesUtilizado, acumulaJugadas, acumulaGanadas, acumulaAbandonos;
	ifstream archivo;
	ofstream actualizarArch;
	string nombArch = "informePC.txt";

	vecesUtilizado = acumulaJugadas = acumulaGanadas = acumulaAbandonos = 0;
	archivo.open(nombArch);
	if (archivo.is_open()){
		encontrado = true;
		archivo >> vecesUtilizado;
		archivo >> acumulaJugadas;
		archivo >> acumulaGanadas;
		archivo >> acumulaAbandonos;
		archivo.close();
	}

	actualizarArch.open(nombArch);
	actualizarArch << vecesUtilizado + 1 << endl;			//"Número de veces que se ha utilizado el programa"
	actualizarArch << jugadas + acumulaJugadas << endl;		//"Número total de partidas jugadas (incluidos los abandonos)"
	actualizarArch << ganadas + acumulaGanadas << endl;		//"Número total de partidas ganadas por el programa"
	actualizarArch << abandonos + acumulaAbandonos << endl;	//"Número total de abandonos"
	actualizarArch.close();

	return encontrado;
}

//Devuelve un dígito que cumpla las reglas del juego con respecto a ultimo.
//Ademas el automata consigue realizar movimientos para ganar
int digitoAutomataNivelado(int ultimo, int suma){
	int nuevo, contador, registro;
	bool ganaPersona, salir, registrado;

	ganaPersona = true;
	salir = registrado = false;
	
	for (contador = 1; !salir && ganaPersona; contador++) { // Mientras gana la persona (puede haber otro digito que haga que no gane) y no me tenga que salir
		nuevo = (META - contador) - suma;
		if (nuevo < 1) { // (suma == 30) => Pierdes
			salir = true;
			nuevo = (registrado) ? registro : digitoAutomata(ultimo);
			// El automata juega con un mal movimiento registrado, no necesariamente perdedor
		}
		else if (nuevo > 9) {
			// incrementa contador hasta llegar a 9 y poder evaluar si es un digito valido
		}
		else if (digitoValido(ultimo, nuevo)) {
			ganaPersona = predictor(nuevo, suma);
			registro = nuevo;
			// el automata registra un digito nuevo valido
			// si (ganaPersona == true), registro almacena el menor digito valido para que este no sea necesariamente perdedor
			// si (ganaPersona == false), nuevo sera un digito valido y como no predice que vaya a ganar la persona pues realizara ese movimiento
			registrado = true; // se ha registrado correctamente
		}
	}

	return nuevo;
}

// El automata predice si con un movimiento de la persona es capaz de ganarle
// Devuelve un booleano si puede ganar la persona o no
bool predictor(int nuevo, int suma){
	int adivinador, comprobador;
	bool ganaPersona = false;
	int cota = 4; // 31 - cota => Valor predeterminado para la Suma Total que analizara las posibilidades al llegar a [27, 30]

	for (int i=1; !ganaPersona && (i <= cota); i++){ // la variable i se utiliza para ver si se gana en casos donde la (27 <= suma acumulada <= 30)
		adivinador = (META - i) - suma - nuevo;
		if ((adivinador > 0) && (adivinador < 10) && digitoValido(nuevo, adivinador)){
			comprobador = (META - 1) - suma - nuevo - adivinador;
			if ((comprobador == 0) || !(digitoValido(adivinador, comprobador)))
				ganaPersona = true;
		}
	}

	return ganaPersona;
}

//Muestra el submenú, pide el nivel y lo devuelve como resultado.
//Sólo devolverá un nivel válido. Para cada valor no válido, mostrará un error.
int submenu() {
	int nivel = -1; // Cualquiera no válida

	while ((nivel < 1) || (nivel > 2)) {
		cout << "   Selecciona un nivel \n";
		cout << "   Nivel: 1 - Jugar por probar \n"; // Juega de manera aleatoria
		cout << "   Nivel: 2 - Jugar contra un automata \n";
		cout << "   Nivel: ";
		cin >> nivel;
		if ((nivel < 1) || (nivel > 2)) {
			cout << "   ¡Nivel no valido! \n\n";
		}
	}

	return nivel;
}


/*
------------------------------------------------------------------------------
				(*) 2 posibilidades mas de hacer una pausa
------------------------------------------------------------------------------
	1) En el propio main():
	string pausa;
	cout << "Hasta la proxima " << usuario << " (pulsa una tecla)"<< endl;
	cin >> pausa;
	
	2) En una funcion:
	void pausa(){
		cin.sync();
		cin.get();
	}
------------------------------------------------------------------------------
*/
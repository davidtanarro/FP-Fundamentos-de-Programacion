// David Tanarro de las Heras (grupo F)
// Javier Lanchas Hernández (grupo F)
// Se ha hecho la version opcional

#include <iostream>
#include <string>
#include <cstdlib>	// srand() y rand()
#include <ctime>	//time
#include <fstream>
#include <iomanip>
//#include <stdlib.h> // system("pause") y system("CLS")

using namespace std;

const int CARTASPORPALO = 13;
const int MAXCARTAS = 53;
const int CENTINELA = 52;
const int MAXINTENTOS = 3;
typedef enum {picas, treboles, diamantes, corazones} tPalo; // Baraja francesa
typedef enum {A, dos, tres, cuatro, cinco, seis, siete, ocho, nueve, diez, J, Q, K} tNumero;
typedef int tCarta;
typedef tCarta tMazo [MAXCARTAS];

// Versión 1: Manipulación de mazos de cartas
void crearMazoVacio(tMazo mazo);
int cuantasEnMazo(const tMazo mazo);
tPalo darPalo(tCarta carta);
tNumero darNumero(tCarta carta);
void escribirCarta(tCarta carta);
void escribirMazo(const tMazo mazo);
bool cargarMazo(tMazo mazo);
void barajarMazo(tMazo mazo);
bool unirMazos(tMazo mazoOriginal, const tMazo nuevoMazo);
bool partirMazo(tMazo mazoOrigen, int cuantasCoger, tMazo mazoDestino);
void cortarMazo(tMazo mazo, int cuantas);
void guardarMazo(const tMazo mazo);
int menu();
// Versión 2: Repartos de mazos de cartas
void repartirNegroRojo(const tMazo mazo, tMazo mazoNegro, tMazo mazoRojo);
void repartirBajaAlta(const tMazo mazo, tMazo mazoBajas, tMazo mazoAltas);
void repartirIntercalando(const tMazo mazo, int enCuantosMazos, int queMazoPido, tMazo mazoNuevo);
// Versión 3: Truco de los tres montones
void trucoTresMontones();
// Versión 4: Truco de la posada
void trucoPosada();
// Parte opcional: El truco del jugador desconfiado
void repartirParImpar(const tMazo mazo, tMazo izq, tMazo der);
void repartirFiguraNoFigura(const tMazo mazo, tMazo izq, tMazo der);
void trucoJugadorDesconfiado();

int main() {
	bool concatena, carga;
	int opcion, intentos, cuantas;
	tMazo mazo, nuevoMazo;
	
	srand(time(NULL)); // Se establece la semilla
	crearMazoVacio(mazo);
	crearMazoVacio(nuevoMazo);

	opcion = menu();
	while (opcion != 0) {
		switch (opcion) {
		case 1:
			{
				intentos = 0;
				carga = false;
				while(!carga && (intentos < MAXINTENTOS)) {
					cout << "Indique el fichero con el mazo a cargar:\n";
					carga = cargarMazo(mazo);
					intentos++;
				}
				if (carga)
					escribirMazo(mazo); // mostrar mazo cargado por pantalla
				else
					cout << "Error, no se ha podido cargar el mazo del fichero\n";
			}
			break;
		case 2:
			{
				barajarMazo(mazo);
				escribirMazo(mazo); // mostrar el mazo barajado por pantalla
			}
			break;
		case 3:
			{
				cout << "Indique el fichero con el nuevo mazo a a" << char(164) << "adir:\n"; // char(164) ) = ñ
				carga = cargarMazo(nuevoMazo);
				if (carga) {
					concatena = unirMazos(mazo, nuevoMazo);
					if (concatena)
						escribirMazo(mazo); // mostrar mazo concatenado por pantalla
					else
						cout << "Se ha superado el numero maximo de cartas al a" << char(164) << "adir el mazo\n";
				}
				else
					cout << "Error, no se ha podido cargar el mazo del fichero\n";
			}
			break;
		case 4:
			{
				cout << "Introduce cuantas cartas coges para cortar:\n";
				cin >> cuantas;
				while (cuantas < 0) {
					cout << "Debe ser un valor positivo, introduzcalo de nuevo:\n";
					cin>> cuantas;
				}
				cortarMazo(mazo, cuantas);
				escribirMazo(mazo); // mostrar el mazo cortado por pantalla
			}
			break;
		case 5:
			{
				cout << "Introduzca el nombre del fichero donde desea guardar el mazo:\n";
				guardarMazo(mazo);
			}
			break;
		case 6:
			{
				trucoTresMontones();
			}
			break;
		case 7:
			{
				trucoPosada();
			}
			break;
		case 8:
			{
				trucoJugadorDesconfiado();
			}
			break;
		case 0:
			break; // Salimos del programa.
		}
		cout << endl;
		opcion = menu();
	}


	return 0;
}

// Hace que mazo pase a representar un mazo vacío colocando el centinela en la primera posición del array.
void crearMazoVacio(tMazo mazo){
	tCarta carta;

	carta = CENTINELA;
	mazo[0] = carta;
}

// Devuelve cuántas cartas hay en mazo.
int cuantasEnMazo(const tMazo mazo){
	int contador = 0;
	
	while ((mazo[contador] != CENTINELA) && (contador < (MAXCARTAS-1)))
		contador++;

	return contador;
}

// Devuelve el palo de la carta.
// Por ejemplo, si le llega la carta 2 (correspondiente al 3 de picas) devuelve picas.
tPalo darPalo(tCarta carta){
	tPalo palo = ((tPalo) (carta / 13)); //{picas, treboles, diamantes, corazones}

	return palo;
}

// Devuelve el número de carta.
// Por ejemplo si le llega la carta 11 (correspondiente a la reina de picas) devuelve Q.
tNumero darNumero(tCarta carta){
	tNumero numero = ((tNumero) (carta % 13));

	return numero;
}

// Muestra por pantalla una carta.
// Por ejemplo, si le llega la carta 2 (correspondiente al 3 de picas) escribirá “3 de picas” por pantalla.
void escribirCarta(tCarta carta) {
	string cadPalo, cadCarta;
	tPalo elPalo = darPalo(carta); // {picas, treboles, diamantes, corazones}
	tCarta laCarta = darNumero(carta);

	if (elPalo == 0)
		cadPalo = "picas";
	else if (elPalo == 1)
		cadPalo = "treboles";
	else if (elPalo == 2)
		cadPalo = "diamantes";
	else // elPalo == 3
		cadPalo = "corazones";

	if (laCarta == 0)
		cadCarta = "A";
	else if (laCarta == 1)
		cadCarta = "2";
	else if (laCarta == 2)
		cadCarta = "3";
	else if (laCarta == 3)
		cadCarta = "4";
	else if (laCarta == 4)
		cadCarta = "5";
	else if (laCarta == 5)
		cadCarta = "6";
	else if (laCarta == 6)
		cadCarta = "7";
	else if (laCarta == 7)
		cadCarta = "8";
	else if (laCarta == 8)
		cadCarta = "9";
	else if (laCarta == 9)
		cadCarta = "10";
	else if (laCarta == 10)
		cadCarta = "J";
	else if (laCarta == 11)
		cadCarta = "Q";
	else // laCarta == 12
		cadCarta = "K";

	cout << cadCarta + " de " + cadPalo << endl;
}

// Muestra por pantalla el contenido de mazo.
void escribirMazo(const tMazo mazo){
	int cota = cuantasEnMazo(mazo);
	int i = 0;
	while (i < cota) {
		escribirCarta(mazo[i]);
		i++;
	}
}

/* Si el archivo no puede abrirse, devuelve false.
En otro caso, una vez abierto el archivo, leerá las cartas del fichero y las guardará en mazo.
Cada renglón del fichero contendrá una carta indicada con una letra (p, t, d, c) seguida de un número (entre 1 y 13).
Por ejemplo, “c 4” será el 4 de corazones, “t 10” será el 10 de tréboles y “c 13” será el rey de corazones.
El programa dejará de leer el fichero cuando encuentre el centinela "x" o se supere el máximo de cartas.
Si se supera el máximo de cartas permitido se guardarán solo las que entren en el mazo. */
bool cargarMazo(tMazo mazo){
	bool encontrado = true;
	int numero, contador;
	ifstream fichero;
	string nomFichero;
	char centinelaArch = 'x';
	char palo = 'p'; // Inicializar a cualquier caracter diferente de 'x'

	contador = 0;
	cin >> nomFichero;
	fichero.open(nomFichero);
	if (fichero.is_open()) {
		while((palo != centinelaArch) && (contador < (MAXCARTAS-1))) {
			fichero >> palo;
			if (palo != centinelaArch) {
				fichero >> numero;
				// guardar en el mazo las cartas segun su palo
				if (palo == 'p')
					mazo[contador] = numero-1;
				else if (palo == 't')
					mazo[contador] = numero+12;
				else if (palo == 'd')
					mazo[contador] = numero+25;
				else // (palo == 'c')
					mazo[contador] = numero+38;
				contador++;
			}
		}
	}
	else
		encontrado = false;
	mazo[contador] = CENTINELA;

	return encontrado;
}

// Baraja el mazo. Para ello se intercambian cartas dos a dos de forma aleatoria. Se harán 3*numCartas intercambios.
void barajarMazo(tMazo mazo){
	int aleatorio, numCartas;
	tCarta aux;
	int barajar = 0;

	numCartas = cuantasEnMazo(mazo);
	while (barajar < 3) { // Se repite el proceso de intercambiar cartas hasta 3 veces
		for(int i=0; i < numCartas; i++) {
			aleatorio = (rand() % numCartas); // 0 <= aleatorio < numCartas
			aux = mazo[i];
			mazo[i] = mazo[aleatorio];
			mazo[aleatorio] = aux;
		}
		barajar++;
	}
}

// Añade al final de mazoOriginal las cartas de nuevoMazo (en el mismo orden).
// Si la concatenación supera el máximo de cartas permitido en un mazo devuelve false (en otro caso true).
bool unirMazos(tMazo mazoOriginal, const tMazo nuevoMazo){
	bool concatena = true;
	int cartasMazoOriginal	= cuantasEnMazo(mazoOriginal);
	int cartasNuevoMazo		= cuantasEnMazo(nuevoMazo);
	int sumaDeMazos = cartasMazoOriginal + cartasNuevoMazo;

	if (sumaDeMazos < MAXCARTAS)
		for (int i = 0; i <= cartasNuevoMazo; i++)				// comenzamos pisando la posicion CENTINELA de mazoOriginal
			mazoOriginal[cartasMazoOriginal+i] = nuevoMazo[i];	// y terminamos copiando el CENTINElA del nuevoMazo
	else
		concatena = false;

	return concatena;
}

// Quita a mazoOrigen sus primeras cuantasCoger cartas, formando con ellas, por orden, el mazo mazoDestino.
// Fíjate en que esta función modifica también mazoOrigen. En caso de no haber suficientes cartas
// en mazoOrigen devuelve false, no modifica mazoOrigen y mazoDestino se devuelve vacío.
bool partirMazo(tMazo mazoOrigen, int cuantasCoger, tMazo mazoDestino){
	int i;
	int cuantasTengo = cuantasEnMazo(mazoOrigen);
	bool parto = true;

	if(cuantasCoger <= cuantasTengo) {
		for (i=0; i < cuantasCoger; i++)
			mazoDestino[i] = mazoOrigen[i];
		mazoDestino[i] = CENTINELA; // Le coloco el CENTINELA en la ultima posicion
		for (i=0; i <= cuantasTengo-cuantasCoger; i++) // Copio el CENTINELA
			mazoOrigen[i] = mazoOrigen[i+cuantasCoger];
	}
	else
		parto = false;

	return parto;
}

// Corta mazo por cuantas. Para ello, se parte en un mazo auxiliar, que después se le concatena.
// Si cuantas no es válido (no hay tantas cartas), se entenderá que no se quiere modificar mazo.
// Utilizará la función anterior para partir el mazo.
void cortarMazo(tMazo mazo, int cuantas){
	tMazo mazoAux;
	bool parto = partirMazo( mazo, cuantas, mazoAux);

	if (parto)
		unirMazos(mazo, mazoAux);
}

//Pide al usuario el nombre del fichero en que guardar el mazo y lo escribe en dicho fichero,
//siguiendo exactamente el mismo formato que usamos en los ficheros de los que leemos mazos.
void guardarMazo(const tMazo mazo){
	ofstream fichero;
	string nomFichero;
	char centinelaArch = 'x';
	char cPalo;
	int numero;
	int contador = cuantasEnMazo(mazo);
	tCarta carta;
	tPalo palo;

	cin >> nomFichero;
	fichero.open(nomFichero);
	for (int i = 0; i < contador; i++) {
		carta = mazo[i];
		palo = darPalo(carta);
		if (palo == 0) {
			cPalo = 'p';
			numero = carta+1;
		}
		else if (palo == 1) {
			cPalo = 't';
			numero = carta-12;
		}
		else if (palo == 2) {
			cPalo = 'd';
			numero = carta-25;
		}
		else { // (palo == 3)
			cPalo = 'c';
			numero = carta-38;
		}
		fichero << cPalo << " " << numero << endl;
	}
	fichero << centinelaArch << endl;
}

//Muestra al usuario el menú de opciones y le pide una opción.
//Repite la petición al usuario mientras la opción no sea válida.
//Cuando obtiene una opción válida, la devuelve.
int menu() {
	int op = -1; // Cualquiera no válida

	while ((op < 0) || (op > 8)) {
		cout << "Selecciona una opcion: \n";
		cout << "1 - Cargar un mazo de cartas de fichero.\n";
		cout << "2 - Barajar mazo de cartas.\n";
		cout << "3 - A" << char(164) << "adir mazo.\n"; // char(164) = ñ
		cout << "4 - Cortar mazo.\n";
		cout << "5 - Guardar mazo de cartas en fichero.\n";
		cout << "6 - Ejecutar el truco de los tres montones.\n";
		cout << "7 - Ejecutar el truco de la posada.\n";
		cout << "8 - Ejecutar el truco del jugador desconfiado.\n";
		cout << "0 - Salir.\n";
		cout << "Opcion: ";
		cin >> op;
		if ((op < 0) || (op > 8)) {
			cout << "¡Opcion no valida! \n\n";
		}
	}

	return op;
}

// Separa mazo en dos mazos, uno que contenga las cartas negras (picas y tréboles)
// y otro que contenga las cartas rojas (diamantes y corazones).
void repartirNegroRojo(const tMazo mazo, tMazo mazoNegro, tMazo mazoRojo) {
	int i, j, k;

	i = j = k = 0;
	while (mazo[i] != CENTINELA) {
		if(mazo[i] <= 25) { // picas, treboles
			mazoNegro[j] = mazo[i];
			j++;
		}
		else { // diamantes, corazones
			mazoRojo[k] = mazo[i];
			k++;
		}
		i++;
	}
	mazoNegro[j] = CENTINELA;
	mazoRojo[k] = CENTINELA;
}

// Separa mazo en dos mazos, uno que contenga las cartas bajas (desde A hasta 7)
// y otro que contenga las cartas altas (desde 8 hasta K).
void repartirBajaAlta(const tMazo mazo, tMazo mazoBajas, tMazo mazoAltas) {
	int i, j, k;
	tNumero numero;

	i = j = k = 0;
	while (mazo[i] != CENTINELA) {
		numero = darNumero(mazo[i]);
		if((numero >= 0) && (numero <= 6)) { // desde A hasta 7
			mazoBajas[j] = mazo[i];
			j++;
		}
		else { // desde 8 hasta K
			mazoAltas[k] = mazo[i];
			k++;
		}
		i++;
	}
	mazoBajas[j] = CENTINELA;
	mazoAltas[k] = CENTINELA;
}

// mazoNuevo pasa a contener el queMazoPido-ésimo mazo que se obtendría al repartir mazo alternamente
// en enCuantosMazos mazos. Por ejemplo, si el mazo contuviera las cartas 1, 2, 3, 4, 5, 6, 7,
// entonces repartirIntercalando(mazo, 3, 1, mazoNuevo) haría que mazoNuevo se convirtiera en un mazo con 1, 4, 7.
// Por otro lado, repartirIntercalando(mazo, 3, 2, mazoNuevo) haría que mazoNuevo contuviera las cartas 2, 5.
void repartirIntercalando(const tMazo mazo, int enCuantosMazos, int queMazoPido, tMazo mazoNuevo) {
	int i, j;

	i = j = 0;
	while (mazo[i] != CENTINELA) {
		if ((enCuantosMazos != 0) && ((i % enCuantosMazos) == (queMazoPido-1))) {
			mazoNuevo[j] = mazo[i];
			j++;
		}
		i++;
	}
	mazoNuevo[j] = CENTINELA;
}

/* Descripcion:
	El mago baraja un mazo con 21 cartas y las reparte en tres mazos, pidiendo a
un espectador que memorice una de las 21 cartas que ha repartido sin decir
cuál es, pero que diga en cuál de los tres mazos ha caído dicha carta.
	Entonces el mago junta los tres mazos en uno solo, concatenando los tres y
poniendo el mazo que ha indicado el espectador en medio de los otros dos.
	Después el mago reparte el nuevo mazo en tres mazos repartiendo las cartas
de manera alterna (mazo 1, mazo 2, mazo 3, mazo 1, mazo 2...) y vuelve a
pedir al espectador que diga en qué mazo está ahora la carta que escogió antes.
	El mago vuelve a concatenar los tres montones, poniendo el mazo indicado
por el espectador en medio.
	Entonces el mago vuelve a hacer lo mismo con el espectador por tercera vez.
	Ahora, la carta elegida por el espectador es la que está en la posición 11 del
mazo, así que el mago se la enseña al espectador. */
void trucoTresMontones() {
	tMazo mazo, monton1, monton2, monton3;
	bool cargar = false;
	bool inicio = true;
	int enCuantosMazos = 3;
	int queMazoPido, nVeces;
	string cad1, cad2, cadfinal;

	cout << "Indique el fichero con el mazo que desea cargar con 21 cartas:\n";
	while (!cargar) {
		cargar = cargarMazo(mazo);
		if (!cargar)
			cout << "Indique de nuevo el fichero con el mazo que desea cargar con 21 cartas:\n";
	}
	barajarMazo(mazo);

	nVeces = 0;
	while (nVeces < 3) {
		repartirIntercalando(mazo, enCuantosMazos, 1, monton1);
		repartirIntercalando(mazo, enCuantosMazos, 2, monton2);
		repartirIntercalando(mazo, enCuantosMazos, 3, monton3);

		cout << "\nMonton 1:\n";
		escribirMazo(monton1);
		cout << "\nMonton 2:\n";
		escribirMazo(monton2);
		cout << "\nMonton 3:\n";
		escribirMazo(monton3);

		cad1 = "Escoja mentalmente una carta y que diga en que mazo (1, 2 o 3) esta su carta:\n";
		cad2 = "En que mazo esta ahora la carta que escogio antes:\n";
		cadfinal = inicio ? cad1: cad2;
		cout << cadfinal;
		inicio = false;
		cin >> queMazoPido;
		while ((queMazoPido < 1) || queMazoPido > 3) {
			cout << "El mazo que eliges tiene que estar entre 1 y 3:\n";
			cin >> queMazoPido;
		}
		if (queMazoPido == 1) {
			unirMazos(monton2, monton1);
			unirMazos(monton2, monton3);
			for (int i = 0; i < 21; i++) // Hay 21 cartas
				mazo[i] = monton2[i];
		}
		else if (queMazoPido == 2) {
			unirMazos(monton1, monton2);
			unirMazos(monton1, monton3);
			for (int i = 0; i < 21; i++)
				mazo[i] = monton1[i];
		}
		else { // queMazoPido == 3
			unirMazos(monton1, monton3);
			unirMazos(monton1, monton2);
			for (int i = 0; i < 21; i++)
				mazo[i] = monton1[i];
		}
		nVeces++;
	}

	cout << "La carta que eligio fue el ";
	escribirCarta(mazo[10]); // Es la carta 11 (contando desde el 0)
	cout << endl;
}

/* Descripcion:
	El mago cuenta que había una posada con cuatro habitaciones. Llegaron
cuatro caballeros (las cuatro J) y cada uno se puso en una habitación diferente
(cada carta se pone en un sitio de la mesa). Luego llegaron cuatro señoras (Q)
y, para no dejarlas sin habitación, ubicaron a cada una en una de dichas
habitaciones, con los caballeros (cada Q se pone sobre cada J). Luego llegaron
cuatro reyes (K) con sus cuatro peones (A), y pusieron cada rey y cada peón en
alguna de dichas cuatro habitaciones. Así que en cada habitación quedó una J,
una Q, una K y una A (es decir, tenemos cuatro mazos, cada uno con una J,
una Q, una K y una A). Entonces se fueron todos a dormir.
	El mago recoge los cuatro mazos, concatenándolos, y pide a un espectador que
corte el mazo por donde quiera.
	Después el mago reparte las cartas del mazo resultante de manera alterna en
cuatro mazos (mazo 1, 2, 3, 4, 1, 2,...), que vuelven a representar las cuatro
habitaciones. Estos muestran que, a la mañana siguiente, los cuatro reyes
amanecieron juntos en la misma habitación (es decir, en el mismo mazo), las
cuatro señoras en otra (en otro mazo), los cuatro caballeros en otra, y los
cuatro peones en otra.*/
void trucoPosada(){
	tMazo mazo, monton1, monton2, monton3, monton4;
	bool cargar = false;
	int cuantas;
	int enCuantosMazos = 4;

	cout << "Indique el fichero con el mazo que desea cargar para efectuar el truco:\n";
	while (!cargar) {
		cargar = cargarMazo(mazo);
		if (!cargar)
			cout << "Indique de nuevo el fichero con el mazo que desea cargar para efectuar el truco:\n";
	}
	repartirIntercalando(mazo, enCuantosMazos, 1, monton1);
	repartirIntercalando(mazo, enCuantosMazos, 2, monton2);
	repartirIntercalando(mazo, enCuantosMazos, 3, monton3);
	repartirIntercalando(mazo, enCuantosMazos, 4, monton4);

	cout << "\nMonton 1:\n";
	escribirMazo(monton1);
	cout << "\nMonton 2:\n";
	escribirMazo(monton2);
	cout << "\nMonton 3:\n";
	escribirMazo(monton3);
	cout << "\nMonton 4:\n";
	escribirMazo(monton4);

	unirMazos(monton1, monton2);
	unirMazos(monton1, monton3);
	unirMazos(monton1, monton4);
	for (int i = 0; i < 16; i++) // Hay 16 cartas (4 palos x 4 figuras)
		mazo[i] = monton1[i];

	cout << "Introduce cuantas cartas coges para cortar:\n";
	cin >> cuantas;
	cortarMazo(mazo, cuantas);

	repartirIntercalando(mazo, enCuantosMazos, 1, monton1);
	repartirIntercalando(mazo, enCuantosMazos, 2, monton2);
	repartirIntercalando(mazo, enCuantosMazos, 3, monton3);
	repartirIntercalando(mazo, enCuantosMazos, 4, monton4);

	cout << "\nMonton 1:\n";
	escribirMazo(monton1);
	cout << "\nMonton 2:\n";
	escribirMazo(monton2);
	cout << "\nMonton 3:\n";
	escribirMazo(monton3);
	cout << "\nMonton 4:\n";
	escribirMazo(monton4);
}

// Hace que izq contenga las cartas pares de mazo (incluida Q) y der las cartas impares de mazo (incluidas J y K).
void repartirParImpar(const tMazo mazo, tMazo izq, tMazo der) {
	int i, j, k;

	i = j = k = 0;
	while (mazo[i] != CENTINELA) { // orden de la baraja: {picas, treboles, diamantes, corazones}
		if ((darPalo(mazo[i]) == 0) || (darPalo(mazo[i]) == 2)) {
			if (mazo[i] % 2 == 0) { // cartas impares de picas y diamantes
				der[j] = mazo[i];
				j++;
			}
			else { // cartas pares de picas y diamantes
				izq[k] = mazo[i];
				k++;
			}
		}
		else {
			if(mazo[i] % 2 == 0) { // cartas pares de treboles y corazones
				izq[k] = mazo[i];
				k++;
			}
			else { // cartas impares de treboles y corazones
				der[j] = mazo[i];
				j++;
			}
		}
		i++;
	}
	izq[k] = CENTINELA;
	der[j] = CENTINELA;
}

// Hace que izq contenga las cartas figuras o as de mazo (A, J, Q, K) y der las demás cartas de mazo (2-10).
void repartirFiguraNoFigura(const tMazo mazo, tMazo izq, tMazo der) {
	int i, j, k;
	tNumero numero;

	i = j = k = 0;
	while (mazo[i] != CENTINELA) {
		numero = darNumero(mazo[i]);
		if (numero == 0 || (numero >= 10 && numero <= 12)) { // cartas: figuras o as (A, J, Q, K)
			izq[j] = mazo[i];
			j++;
		}
		else { // resto de cartas
			der[k] = mazo[i];
			k++;
		}
		i++;
	}
	izq[j] = CENTINELA;
	der[k] = CENTINELA;
}

// Lleva a cabo el truco del jugador desconfiado.
void trucoJugadorDesconfiado() {
	bool cargar = false;
	tMazo mazo, jugador1, jugador2, jugador3, jugador4, mazoNegro, mazoRojo, mazoBajas, mazoAltas, pares, impares, figuras, resto;
	int i, cartas1, cartas2, cartas3, cartas4;
	int enCuantosMazos = 4;
	int nVeces = 0;

	cout << "Indique el fichero con el mazo que desea cargar para efectuar el truco:\n";
	while (!cargar) {
		cargar = cargarMazo(mazo);
		if (!cargar)
			cout << "Indique de nuevo el fichero con el mazo que desea cargar para efectuar el truco:";
	}
	barajarMazo(mazo);
	escribirMazo(mazo);

	repartirIntercalando(mazo, enCuantosMazos, 1, jugador1);
	repartirIntercalando(mazo, enCuantosMazos, 2, jugador2);
	repartirIntercalando(mazo, enCuantosMazos, 3, jugador3);
	repartirIntercalando(mazo, enCuantosMazos, 4, jugador4);

	cout << "\nSe ha barajeado las cartas y se reparten a cada jugador:\n";
	cout << "\nJugador 1:\n";
	escribirMazo(jugador1);
	cout << "\nJugador 2:\n";
	escribirMazo(jugador2);
	cout << "\nJugador 3:\n";
	escribirMazo(jugador3);
	cout << "\nJugador 4:\n";
	escribirMazo(jugador4);

	while (nVeces < 2) { // Primera y segunda division de cartas
		repartirNegroRojo (jugador1, mazoNegro, mazoRojo);
		repartirBajaAlta (jugador2, mazoBajas, mazoAltas);
		repartirParImpar (jugador3, pares, impares);
		repartirFiguraNoFigura (jugador4, figuras, resto);

		// Se une el mazo der. del jugador de la izq. con el mazo izquierdo del jugador de la derecha
		unirMazos( mazoBajas, resto);	// Cartas que van a jugador1
		unirMazos( pares, mazoRojo);	// Cartas que van a jugador2
		unirMazos( figuras, mazoAltas);	// Cartas que van a jugador3
		unirMazos( mazoNegro, impares);	// Cartas que van a jugador4

		cartas1 = cuantasEnMazo(mazoBajas);
		cartas2 = cuantasEnMazo(pares);
		cartas3 = cuantasEnMazo(figuras);
		cartas4 = cuantasEnMazo(mazoNegro);

		for (i = 0; i < cartas1; i++)
			jugador1[i] = mazoBajas[i];
		jugador1[i] = CENTINELA;
		for (i = 0; i < cartas2; i++)
			jugador2[i] = pares[i];
		jugador2[i] = CENTINELA;
		for (i = 0; i < cartas3; i++)
			jugador3[i] = figuras[i];
		jugador3[i] = CENTINELA;
		for (i = 0; i < cartas4; i++)
			jugador4[i] = mazoNegro[i];
		jugador4[i] = CENTINELA;

		cout << "\nLas cartas de cada jugador se dividira en dos mazos, izquierdo y derecho,\ntal y como indica el criterio del Jugador 1\n";
		cout << "\nJugador 1:\n";
		escribirMazo(jugador1);
		cout << "\nJugador 2:\n";
		escribirMazo(jugador2);
		cout << "\nJugador 3:\n";
		escribirMazo(jugador3);
		cout << "\nJugador 4:\n";
		escribirMazo(jugador4);
		nVeces++;
	}

	// Tercera division de cartas
	repartirNegroRojo (jugador1, mazoNegro, mazoRojo);
	repartirBajaAlta (jugador2, mazoBajas, mazoAltas);
	repartirParImpar (jugador3, pares, impares);
	repartirFiguraNoFigura (jugador4, figuras, resto);

	// Se une el mazo derecho del jugador con el mazo izquierdo del jugador de la derecha
	unirMazos( mazoBajas, mazoRojo);// Cartas que van a jugador1
	unirMazos( pares, mazoAltas);	// Cartas que van a jugador2
	unirMazos( figuras, impares);	// Cartas que van a jugador3
	unirMazos( mazoNegro, resto);	// Cartas que van a jugador4

	cout << "\nLas cartas de cada jugador se han repartido de forma 'caotica' y 'aleatoria'\n";
	cout << "\nJugador 1:\n";
	escribirMazo(mazoBajas);
	cout << "\nJugador 2:\n";
	escribirMazo(pares);
	cout << "\nJugador 3:\n";
	escribirMazo(figuras);
	cout << "\nJugador 4:\n";
	escribirMazo(mazoNegro);
}
// David Tanarro de las Heras (grupo F)
// Javier Lanchas Hernández (grupo F)

#include <iostream>
#include <Windows.h>
#include <stdlib.h> // system("CLS") y system("pause")
#include <cstdlib>	// srand() y rand()
#include <ctime>	// time
#include <string>
#include <fstream>
#include <iomanip> // setw()
#include <conio.h> // _getch()

using namespace std;

const int MAXDIM = 8;
const int DEFMETA = 2048;
const int DEFDIM = 4;
const int PROBDE2 = 95;
const int NRECORDS = 10;
const string CENTINELA = "???";
const bool MODOPRUEBAS = true;

typedef enum {arriba, abajo, derecha, izquierda, salir, nada} tAccion;
typedef enum {
	black,			// 0
	dark_blue,		// 1
	dark_green,		// 2
	dark_cyan,		// 3
	dark_red,		// 4
	dark_magenta,	// 5
	dark_yellow,	// 6
	light_gray,		// 7
	dark_gray,		// 8
	light_blue,		// 9
	light_green,	// 10
	light_cyan,		// 11
	light_red,		// 12
	light_magenta,	// 13
	light_yellow,	// 14
	white			// 15
} tColor;

typedef int tMatriz[MAXDIM][MAXDIM];
typedef struct {
	tMatriz matriz;
	int dim;
} tTablero;

typedef struct {
	tTablero tablero;
	int puntuacionTotal;
	int ultimaPuntuacion;
	int meta;
} tJuego;

typedef struct { // cambiar nombres por nombre y puntuaciones por puntuacion
	string nombre;
	int puntuacion;
} tRecord;
typedef tRecord tGaleria[NRECORDS];

bool esAfirmativo(char afirmativo);
// Version 1
void iniciaJuego(tJuego& juego);
void nuevaPartida(tJuego& juego);
void visualiza(const tJuego& juego);
void pintarLinea(const int dim, const char izq, const char centro, const char der);
void pintarFila(const tTablero& tablero, const int fila, const bool filaIntermedia);
void colorFondo(int color);
int log2(int num);
bool carga(tJuego& juego);
// Version 2
tAccion leeAccion();
void inclinaTablero(tTablero& tablero, tAccion accion);
void desplazaArriba(tTablero& tablero);
void desplazaAbajo(tTablero& tablero);
void desplazaDerecha(tTablero& tablero);
void desplazaIzquierda(tTablero& tablero);
// Version 3
void nuevaFicha(tTablero& tablero);
bool tableroMeta(const tTablero& tablero, const int meta);
bool tableroLleno(const tTablero& tablero);
void combinaFichas(tJuego& juego, tAccion accion);
int combinaArriba(tTablero& tablero);
int combinaAbajo(tTablero& tablero);
int combinaDerecha(tTablero& tablero);
int combinaIzquierda(tTablero& tablero);
// Version 4
void reportarResultado(const tJuego& juego, const bool escape, const bool lleno, const bool meta);
void nuevoRecord(const int puntos);
int cargarRecords(tGaleria& galeria);
void insertarRecords(tGaleria& galeria, int &nelements, const int puntos);
void guardarRecords(const tGaleria& galeria);
void muestraRecords();
void guarda(const tJuego& juego);

int main(){
	tJuego juego;
	tAccion accion;
	bool escape, lleno, meta;
	char afirmativo;
	escape = lleno = meta = false;
	srand(time(NULL)); // Se establece la semilla

	iniciaJuego(juego);
	visualiza(juego);
	while (!escape && !lleno && !meta) {
		accion = leeAccion();
		if (accion == salir) {
			cout << "Seguro que quieres salir [S/N]?";
			cin >> afirmativo;
			if (esAfirmativo(afirmativo))
				escape = true;
			else
				visualiza(juego);
		}
		else if (accion != nada) {
			inclinaTablero(juego.tablero, accion);
			combinaFichas(juego, accion);
			if (tableroLleno(juego.tablero) && juego.ultimaPuntuacion == 0)
				lleno = true;
			else if (tableroMeta(juego.tablero, juego.meta))
				meta = true;
			else
				nuevaFicha(juego.tablero);
			visualiza(juego);
		}
	}
	reportarResultado(juego, escape, lleno, meta);

	return 0;
}

bool esAfirmativo(char afirmativo) {
	return (afirmativo == 'S' || afirmativo == 's');
}

void iniciaJuego(tJuego& juego) {
	char afirmativo;

	cout << "Quieres cargar una partida [S/N]? ";
	cin >> afirmativo;
	if (esAfirmativo(afirmativo)) {
		cout << "Nombre del archivo con la partida: ";
		if (!carga(juego))
			nuevaPartida(juego);
	}
	else
		nuevaPartida(juego);
}

// Inicializa el juego haciendo que todas las baldosas del tablero esten libres, excepto dos elegidas aleatoriamente.
// Esas dos baldosas contendrán fichas con valor 2 o 4,siendo la probabilidad de ser un 2 del 95% y no pudiendo ser ambas 4.
void nuevaPartida(tJuego& juego) {
	int numFilAleatoria, numColAleatoria;

	juego.tablero.dim = DEFDIM; // Se podria preguntar al usuario elegirla
	juego.meta = DEFMETA;		// Se podria preguntar al usuario elegirla
	juego.puntuacionTotal = juego.ultimaPuntuacion = 0;
	for(int i = 0; i < juego.tablero.dim; i++)
		for (int j = 0; j < juego.tablero.dim; j++)
			juego.tablero.matriz[i][j] = 1; // 2^0 = 1
	// Agregar dos fichas iniciales
	numFilAleatoria = rand() % juego.tablero.dim;
	numColAleatoria = rand() % juego.tablero.dim;
	juego.tablero.matriz[numFilAleatoria][numColAleatoria] = 2;
	nuevaFicha(juego.tablero);
}

// Muestra el estado del juego,el cual incluye el tablero, los puntos de la última jugada, el total de puntos y la meta.
void visualiza(const tJuego& juego) {
	if (!MODOPRUEBAS)
		system("cls");
	colorFondo(0); // Color de fondo negro
	cout << endl << setw(6) << juego.ultimaPuntuacion << setw(15) << "Total:" << setw(7) << juego.puntuacionTotal << endl;
	pintarLinea(juego.tablero.dim, char(218), char(194), char(191)); // Borde Superior '┌', '┬', '┐'
	int i = 0;
	while (i < juego.tablero.dim-1) {
		pintarFila(juego.tablero, i, false);
		pintarFila(juego.tablero, i, true);
		pintarFila(juego.tablero, i, false);
		pintarLinea(juego.tablero.dim, char(195), char(197), char(180)); // Separador '├', '┼' ,'┤'
		i++;
	}
	pintarFila(juego.tablero, i, false);
	pintarFila(juego.tablero, i, true);
	pintarFila(juego.tablero, i, false);
	pintarLinea(juego.tablero.dim, char(192), char(193), char(217)); // Borde Inferior '└', '┴', '┘'
	cout << "\nUsa las teclas de flecha (Esc para salir)...\n";
}

/*	ESQUEMA
				┌					┬					┐
	cout << char(218) << " " << char(194) << " " << char(191);
				├					┼					┤
	cout << char(195) << " " << char(197) << " " << char(180);
				└					┴					┘
	cout << char(192) << " " << char(193) << " " << char(217);
				█						│					─
	cout << char(219)  << " " << 	char(179) << " " << char(196);
*/
void pintarLinea(const int dim, const char izq, const char centro, const char der) {
	cout << izq;
	for (int j=0; j < dim-1; j++)
		cout << char(196) << char(196) << char(196) << char(196) << char(196) << char(196) << centro;
	cout << char(196) << char(196) << char(196) << char(196) << char(196) << char(196) << der << endl;
}

void pintarFila(const tTablero& tablero, const int fila, const bool filaIntermedia) {
	tColor colorea;
	for (int j = 0; j < tablero.dim; j++) {
		cout << char(179); // │
		colorea = tColor(log2(tablero.matriz[fila][j]));
		colorFondo(colorea);
		(filaIntermedia && (tablero.matriz[fila][j]!=1)) ? cout << " " << setw(4) << tablero.matriz[fila][j] << " ": cout << setw(6) << "";
		colorFondo(black);
	}
	cout << char(179) << endl; // │
}

void colorFondo(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

// Devuelve la potencia de 2 que da como resultado num (1 = 2^0, 2 = 2^1, 4 = 2^2, 8 = 2^3, ..., 2048 = 2^11)
int log2(int num) {
	int resultado = 0;
	int i = num;

	while (i > 1) {
		i = i/2;
		resultado++;
	}

	return resultado;
}

// Pide un nombre de archivo e intenta cargar el juego usando los datos del archivo.
// Si no se puede cargar devolverá false.
bool carga(tJuego& juego) {
	bool encontrado = true;
	ifstream inFich;
	string nomFichero;

	cin >> nomFichero;
	inFich.open(nomFichero);
	if (inFich.is_open()) {
		inFich >> juego.tablero.dim;
		if (juego.tablero.dim < 2 || juego.tablero.dim > MAXDIM)
			encontrado = false;
		for(int i = 0; i < juego.tablero.dim; i++)
			for(int j = 0; j < juego.tablero.dim; j++)
				inFich >> juego.tablero.matriz[i][j];
		inFich >> juego.puntuacionTotal >> juego.meta;
		juego.ultimaPuntuacion = 0;
		if (inFich.fail())
			encontrado = false;
	}
	else
		encontrado = false;
	inFich.close();

	return encontrado;
}

// Devuelve un valor de tAccion, distinto de Nada, que será Salir, si se pulsa la tecla Esc, o la dirección en la que mover el tablero si se pulsa una flecha.
// Se usará _getch() para detectar las teclas de flecha (o Esc). Se seguirán leyendo teclas mientras no se lea una acción válida.
tAccion leeAccion() {
	tAccion accion; // enum {arriba, abajo, derecha, izquierda, salir, nada}
	int dir;

	cin.sync();
	dir =_getch(); // dir:tipo int
	if (dir == 0xe0) {
		dir = _getch();
		if (dir == 72)
			accion = arriba;
		else if (dir == 80)
			accion = abajo;
		else if (dir == 77)
			accion = derecha;
		else // dir == 75
			accion = izquierda;
	}
	else if (dir == 27) //Si no, si dir es 27, es la tecla Esc
		accion = salir;
	else
		accion = nada;

	return accion;
}

//Desplaza las fichas del tablero en la dirección indicada por accion.
void inclinaTablero(tTablero& tablero, tAccion accion) {
	if (accion == arriba)
		desplazaArriba(tablero);
	else if (accion == abajo)
		desplazaAbajo(tablero);
	else if (accion == derecha)
		desplazaDerecha(tablero);
	else // (accion == izquierda)
		desplazaIzquierda(tablero);
}

void desplazaArriba(tTablero& tablero) {
	int huecos;

	for (int j = 0; j < tablero.dim; j++) { // Recorrido de columnas
		huecos = 0;
		for (int i = 0; i < tablero.dim; i++) { // Recorrido de filas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (huecos > 0) {
				tablero.matriz[i-huecos][j] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}
}

void desplazaAbajo(tTablero& tablero) {
	int huecos;

	for (int j = 0; j < tablero.dim; j++) { // Recorrido de columnas
		huecos = 0;
		for (int i = tablero.dim-1; i >= 0; i--) { // Recorrido de filas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (huecos > 0) {
				tablero.matriz[i+huecos][j] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}
}

void desplazaDerecha(tTablero& tablero) {
	int huecos;

	for (int i = 0; i < tablero.dim; i++) { // Recorrido de filas
		huecos = 0;
		for (int j = tablero.dim-1; j >= 0; j--) { // Recorrido de columnas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (huecos > 0) {
				tablero.matriz[i][j+huecos] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}
}

void desplazaIzquierda(tTablero& tablero) {
	int huecos;

	for (int i = 0; i < tablero.dim; i++) { // Recorrido de filas
		huecos = 0;
		for (int j = 0; j < tablero.dim; j++) { // Recorrido de columnas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (huecos > 0) {
				tablero.matriz[i][j-huecos] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}
}

// Crea, en una posición del tablero elegida aleatoriamente y que esté libre, una nueva ficha
// (con un 95% de probabilidad de que sea un 2 y un 5% de que sea un 4).
void nuevaFicha(tTablero& tablero) {
	int i, j;
	do {
		i = rand() % tablero.dim;
		j = rand() % tablero.dim;
	} while (tablero.matriz[i][j] != 1);

	if (rand() % 100 >= PROBDE2)
		tablero.matriz[i][j] = 4;
	else
		tablero.matriz[i][j] = 2;
}

// Indica si el tablero contiene una casilla con el valor de la meta indicada.
bool tableroMeta(const tTablero& tablero, const int meta) {
	bool encontrado = false;
	for (int i = 0; i < tablero.dim && !encontrado; i++)
		for (int j = 0; j < tablero.dim && !encontrado; j++)
			if (tablero.matriz[i][j] == meta)
				encontrado = true;
	return encontrado;
}

// Devuelve true si no quedan baldosas libres en el tablero; false en otro caso.
bool tableroLleno(const tTablero& tablero) {
	bool lleno = true;
	for (int i = 0; i < tablero.dim && lleno; i++)
		for (int j = 0; j < tablero.dim && lleno; j++)
			if (tablero.matriz[i][j] == 1)
				lleno = false;
	return lleno;
}

/*	Combina las fichas del tablero que hayan quedado juntas tras haberlo movido en la dirección indicada por acción,
	actualizando correspondientemente las puntuaciones del juego.
	Observa que se podría fusionar con la función inclinaTablero dando lugar a una implementación más eficiente
	(no es obligatorio pero se tendrá en cuenta). */
void combinaFichas(tJuego& juego, tAccion accion) {
	if (accion == arriba)
		juego.ultimaPuntuacion = combinaArriba(juego.tablero);
	else if (accion == abajo)
		juego.ultimaPuntuacion = combinaAbajo(juego.tablero);
	else if (accion == derecha)
		juego.ultimaPuntuacion = combinaDerecha(juego.tablero);
	else // (accion == izquierda)
		juego.ultimaPuntuacion = combinaIzquierda(juego.tablero);
	juego.puntuacionTotal += juego.ultimaPuntuacion;
}

int combinaArriba(tTablero& tablero) {
	int huecos, ultimaPuntuacion = 0;

	for (int j = 0; j < tablero.dim; j++) { // Recorrido de columnas
		huecos = 0;
		for (int i = 0; i < tablero.dim; i++) { // Recorrido de filas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (tablero.matriz[i][j] == tablero.matriz[i+1][j] && tablero.matriz[i][j] != 1) {
				if (huecos > 0) {
					tablero.matriz[i-huecos][j] = tablero.matriz[i][j] * 2;
					tablero.matriz[i][j] = 1;
					ultimaPuntuacion += tablero.matriz[i-huecos][j];
				}
				else {
					tablero.matriz[i][j] = tablero.matriz[i][j] * 2;
					ultimaPuntuacion += tablero.matriz[i][j];
				}
				tablero.matriz[i+1][j] = 1;
			}
			else if (huecos > 0) { // && tablero.matriz[i][j] != 1
				tablero.matriz[i-huecos][j] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}

	return ultimaPuntuacion;
}

int combinaAbajo(tTablero& tablero) {
	int huecos, ultimaPuntuacion = 0;

	for (int j = 0; j < tablero.dim; j++) { // Recorrido de columnas
		huecos = 0;
		for (int i = tablero.dim-1; i >= 0; i--) { // Recorrido de filas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (tablero.matriz[i][j] == tablero.matriz[i-1][j] && tablero.matriz[i][j] != 1) {
				if (huecos > 0) {
					tablero.matriz[i+huecos][j] = tablero.matriz[i][j] * 2;
					tablero.matriz[i][j] = 1;
					ultimaPuntuacion += tablero.matriz[i+huecos][j];
				}
				else {
					tablero.matriz[i][j] = tablero.matriz[i][j] * 2;
					ultimaPuntuacion += tablero.matriz[i][j];
				}
				tablero.matriz[i-1][j] = 1;
			}
			else if (huecos > 0) { // && tablero.matriz[i][j] != 1
				tablero.matriz[i+huecos][j] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}

	return ultimaPuntuacion;
}

int combinaDerecha(tTablero& tablero) {
	int huecos, ultimaPuntuacion = 0;

	for (int i = 0; i < tablero.dim; i++) { // Recorrido de filas
		huecos = 0;
		for (int j = tablero.dim-1; j >= 0; j--) { // Recorrido de columnas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (tablero.matriz[i][j] == tablero.matriz[i][j-1] && tablero.matriz[i][j] != 1) {
				if (huecos > 0) {
					tablero.matriz[i][j+huecos] = tablero.matriz[i][j] * 2;
					tablero.matriz[i][j] = 1;
					ultimaPuntuacion += tablero.matriz[i][j+huecos];
				}
				else {
					tablero.matriz[i][j] = tablero.matriz[i][j] * 2;
					ultimaPuntuacion += tablero.matriz[i][j];
				}
				tablero.matriz[i][j-1] = 1;
			}
			else if (huecos > 0) { // && tablero.matriz[i][j] != 1
				tablero.matriz[i][j+huecos] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}

	return ultimaPuntuacion;
}

int combinaIzquierda(tTablero& tablero) {
	int huecos, ultimaPuntuacion = 0;

	for (int i = 0; i < tablero.dim; i++) { // Recorrido de filas
		huecos = 0;
		for (int j = 0; j < tablero.dim; j++) { // Recorrido de columnas
			if (tablero.matriz[i][j] == 1)
				huecos++;
			else if (tablero.matriz[i][j] == tablero.matriz[i][j+1] && tablero.matriz[i][j] != 1) {
				if (huecos > 0) {
					tablero.matriz[i][j-huecos] = tablero.matriz[i][j] * 2;
					tablero.matriz[i][j] = 1;
					ultimaPuntuacion += tablero.matriz[i][j-huecos];
				}
				else {
					tablero.matriz[i][j] = tablero.matriz[i][j] * 2;
					ultimaPuntuacion += tablero.matriz[i][j];
				}
				tablero.matriz[i][j+1] = 1;
			}
			else if (huecos > 0) { // && tablero.matriz[i][j] != 1
				tablero.matriz[i][j-huecos] = tablero.matriz[i][j];
				tablero.matriz[i][j] = 1;
			}
		}
	}

	return ultimaPuntuacion;
}

void reportarResultado(const tJuego& juego, const bool escape, const bool lleno, const bool meta) {
	int enter = -1; // Cualquier valor distinto de 13
	char afirmativo;
	if (escape) {
		cout << "Quieres guardar la partida [S/N])? ";
		cin >> afirmativo;
		if (esAfirmativo(afirmativo))
			guarda(juego);
		cout << "Has abandonado la partida...Hasta otra!\n";
	}
	else if (lleno)
		cout << "Lo siento! No te quedan movimientos!\n";
	else if (meta) {
		cout << "Enhorabuena! Has ganado!\n";
		nuevoRecord(juego.puntuacionTotal);
	}
	cout << "Pulsa intro para salir...";
	while (enter != 13) { // 13 es el valor ASCII del ENTER
		cin.sync();
		enter =_getch(); // intro:tipo int
	}
}

/*	Carga del archivo de mejores puntuaciones (records.txt) una lista de nombres y puntuaciones,
	pide al usuario su nombre, inserta en la lista, en la posición correspondiente, el nombre y los puntos
	del nuevo record, y finalmente guarda en el archivo la lista actualizada (hasta diez records; si
	ya había diez, el de menor puntuación desaparece de la lista). */
void nuevoRecord(const int puntos) {
	tGaleria galeria;
	int nelements = cargarRecords(galeria);
	insertarRecords(galeria, nelements, puntos);
	guardarRecords(galeria);
	muestraRecords();
}

int cargarRecords(tGaleria& galeria) {
	ifstream inFich;
	string nombrefich = "records.txt";
	int i = 0;

	inFich.open(nombrefich);
	if (inFich.is_open()) {
		inFich >> galeria[i].nombre;
		while (i < NRECORDS && (galeria[i].nombre != CENTINELA)) {
			inFich >> galeria[i].puntuacion;
			i++;
			if (i < NRECORDS)
				inFich >> galeria[i].nombre;
		}
		if (inFich.fail()) {
			i = 0;
			galeria[i].nombre = CENTINELA;
		}
	}
	else
		galeria[0].nombre = CENTINELA; // Será el primer record hallado
	inFich.close();

	return i;
}

void insertarRecords(tGaleria& galeria, int &nelements, const int puntos) {
	bool esRecord = ((nelements < NRECORDS) || (puntos >= galeria[nelements-1].puntuacion)) ? true : false;
	bool encontrado = false;
	int pos = 0;
	tRecord ultimo;

	if (esRecord) {
		for (int j = 0; (j < nelements) && !encontrado; j++)
			if (galeria[j].puntuacion <= puntos) {
				encontrado = true;
				pos = j;
			}
			else
				pos++;

		if (nelements > 0)
			ultimo = galeria[nelements-1];
		for (int i = nelements-2; i >= pos; i--)
			galeria[i+1] = galeria[i];
		if (nelements < NRECORDS) {
			galeria[nelements] = ultimo;
			nelements++;
			if (nelements < NRECORDS)
				galeria[nelements].nombre = CENTINELA;
		}
		cout << "Estupendo! Tienes una de las 10 mejores puntuaciones!\n";
		cout << "Por favor, introduce tu 'nick': ";
		cin >> galeria[pos].nombre;
		galeria[pos].puntuacion = puntos;
	}
	else
		cout << "Lo siento! No es una de las 10 mejores puntuaciones!\n";
}

void guardarRecords(const tGaleria& galeria) {
	ofstream outFich;
	string nombrefich = "records.txt";
	int i = 0;

	outFich.open(nombrefich);
	outFich << galeria[i].nombre;
	while (i < NRECORDS && (galeria[i].nombre != CENTINELA)) {
		outFich << " " << galeria[i].puntuacion << endl;
		i++;
		if (i < NRECORDS)
			outFich << galeria[i].nombre;
	}
	outFich.close();
}

// Muestra el contenido del archivo records.txt.
void muestraRecords() {
	int i = 0;
	ifstream inFich;
	string nombrefich = "records.txt";
	tGaleria galeria;

	cout << "*** H A L L   O F   F A M E ***\n";
	inFich.open(nombrefich);
	if (inFich.is_open()) {
		inFich >> galeria[i].nombre;
		while (i < NRECORDS && (galeria[i].nombre != CENTINELA)) {
			inFich >> galeria[i].puntuacion;
			i++;
			if (i < NRECORDS && (galeria[i].nombre != CENTINELA))
				inFich >> galeria[i].nombre;
		}
		for (int j = 0; j < i; j++)
			cout << left << setw(15) << galeria[j].nombre << right << setw(7) << galeria[j].puntuacion << endl;
	}

}

//Pide un nombre de archivo y guarda en él el estado del juego actual en el formato de fichero especificado.
void guarda(const tJuego& juego) {
	ofstream outFich;
	string nomFichero;

	cout << "Nombre del archivo: ";
	cin >> nomFichero;
	outFich.open(nomFichero);
	outFich << juego.tablero.dim << endl;
	for(int i = 0; i < juego.tablero.dim; i++)
		for(int j = 0; j < juego.tablero.dim; j++)
			outFich << juego.tablero.matriz[i][j] << endl;
	outFich << juego.puntuacionTotal << " " << juego.meta;
	outFich.close();
}
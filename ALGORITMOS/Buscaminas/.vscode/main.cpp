#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

// Constantes fijas para el tablero de 7x7 y 3 minas
const int FILAS = 7;
const int COLUMNAS = 7;
const int NUM_MINAS = 3;

// Prototipos de funciones
void mostrarResumen();
void crearMatriz(vector<vector<char>> &matriz, int filas, int columnas, char simbolo);
void colocarMinas(vector<vector<char>> &matrizMinas, int filas, int columnas, int numMinas);
void mostrarTablero(const vector<vector<char>> &matriz, int filas, int columnas);
bool abrirCasilla(vector<vector<char>> &tablero, const vector<vector<char>> &matrizMinas, int f, int c, int &intentos);
bool hayVictoria(const vector<vector<char>> &tablero, int filas, int columnas, int numMinas);
void guardarHistorial(int filas, int columnas, const string &resultado, int intentos);

// Lee historial.txt y muestra el resumen al iniciar el programa[cite: 8]
void mostrarResumen() {
    ifstream archivo("historial.txt");
    int partidas = 0, ganadas = 0, perdidas = 0;

    if (archivo.is_open()) {
        int f, c, i;
        string res;
        while (archivo >> f >> c >> res >> i) {
            partidas++;
            if (res == "gano") {
                ganadas++;
            } else if (res == "perdio") {
                perdidas++;
            }
        }
        archivo.close();
    }

    cout << "Partidas: " << partidas << " | Ganadas: " << ganadas << " | Perdidas: " << perdidas << "\n\n";
}

// Crea y llena una matriz con un símbolo específico[cite: 6]
void crearMatriz(vector<vector<char>> &matriz, int filas, int columnas, char simbolo) {
    matriz.assign(filas, vector<char>(columnas, simbolo));
}

// Esconde las minas al azar sin repetir posiciones[cite: 6]
void colocarMinas(vector<vector<char>> &matrizMinas, int filas, int columnas, int numMinas) {
    int colocadas = 0;
    while (colocadas < numMinas) {
        int f = rand() % filas;
        int c = rand() % columnas;

        if (matrizMinas[f][c] != '*') {
            matrizMinas[f][c] = '*';
            colocadas++;
        }
    }
}

// Muestra el tablero con los números de fila y columna[cite: 6]
void mostrarTablero(const vector<vector<char>> &matriz, int filas, int columnas) {
    cout << "\n  ";
    for (int j = 1; j <= columnas; j++) {
        cout << j << " ";
    }
    cout << "\n";

    for (int i = 0; i < filas; i++) {
        cout << i + 1 << " ";
        for (int j = 0; j < columnas; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

// Abre una casilla y determina si había mina[cite: 7]
bool abrirCasilla(vector<vector<char>> &tablero, const vector<vector<char>> &matrizMinas, int f, int c, int &intentos) {
    if (matrizMinas[f][c] == '*') {
        tablero[f][c] = '*';
        return true; 
    } else {
        tablero[f][c] = '+';
        intentos++;
        return false; 
    }
}

// Verifica si se abrieron todas las casillas sin mina[cite: 7]
bool hayVictoria(const vector<vector<char>> &tablero, int filas, int columnas, int numMinas) {
    int casillasAbiertas = 0;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (tablero[i][j] == '+') {
                casillasAbiertas++;
            }
        }
    }
    int casillasObjetivo = (filas * columnas) - numMinas;
    return casillasAbiertas == casillasObjetivo;
}

// Agrega una línea al archivo historial.txt al terminar la partida[cite: 8]
void guardarHistorial(int filas, int columnas, const string &resultado, int intentos) {
    ofstream archivo("historial.txt", ios::app);
    if (archivo.is_open()) {
        archivo << filas << " " << columnas << " " << resultado << " " << intentos << "\n";
        archivo.close();
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    mostrarResumen();

    vector<vector<char>> matrizMinas;
    vector<vector<char>> tablero;

    crearMatriz(matrizMinas, FILAS, COLUMNAS, '.');
    crearMatriz(tablero, FILAS, COLUMNAS, '?');
    colocarMinas(matrizMinas, FILAS, COLUMNAS, NUM_MINAS);

    int intentos = 0;
    bool enJuego = true;

    while (enJuego) {
        mostrarTablero(tablero, FILAS, COLUMNAS);

        int f, c;
        cout << "Escribe la fila (0 para salir, -1 para ver minas): ";
        cin >> f;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Entrada invalida. Ingresa un numero.\n";
            continue;
        }

        if (f == 0) {
            cout << "Juego terminado por el usuario.\n";
            break;
        }

        if (f == -1) {
            cout << "\n--- MATRIZ DE MINAS (MODO PRUEBA) ---";
            mostrarTablero(matrizMinas, FILAS, COLUMNAS);
            continue;
        }

        cout << "Escribe la columna: ";
        cin >> c;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Entrada invalida. Ingresa un numero.\n";
            continue;
        }

        // Validar límites de 7x7[cite: 4, 6]
        if (f < 1 || f > FILAS || c < 1 || c > COLUMNAS) {
            cout << "Error: Posicion fuera del tablero. Intenta de nuevo.\n";
            continue;
        }

        int idxF = f - 1;
        int idxC = c - 1;

        // Validar si la casilla ya estaba abierta[cite: 4, 6]
        if (tablero[idxF][idxC] != '?') {
            cout << "Error: Esa casilla ya esta abierta. Intenta otra.\n";
            continue;
        }

        bool pisoMina = abrirCasilla(tablero, matrizMinas, idxF, idxC, intentos);

        if (pisoMina) {
            cout << "\nPisaste una mina!\n";
            for (int i = 0; i < FILAS; i++) {
                for (int j = 0; j < COLUMNAS; j++) {
                    if (matrizMinas[i][j] == '*') {
                        tablero[i][j] = '*';
                    }
                }
            }
            mostrarTablero(tablero, FILAS, COLUMNAS);
            guardarHistorial(FILAS, COLUMNAS, "perdio", intentos);
            enJuego = false;
        } else {
            if (hayVictoria(tablero, FILAS, COLUMNAS, NUM_MINAS)) {
                cout << "\nGanaste!\n";
                mostrarTablero(tablero, FILAS, COLUMNAS);
                guardarHistorial(FILAS, COLUMNAS, "gano", intentos);
                enJuego = false;
            }
        }
    }

    return 0;
}
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

//const
const int PUNTOS_GANADORES = 21;
const int PUNTOS_MINIMOS_BANCA = 17;
const int MAX_CARTAS = 52;

//estruct de datos
struct Carta {
    string valor;
    string palo;
    int puntos;
};

struct Jugador{
    string nombre;
    Carta mano[MAX_CARTAS];
    int numCartas;
    int puntos;
    bool plantado;
};

//proto de funciones
void inicializarMazo(Carta mazo[], int& mazoSize);
void barajarMazo(Carta mazo[], int mazoSize);
void repartirCarta(Jugador& jugador, Carta mazo[], int& mazoSize);
void mostrarMano(const Jugador& jugador, bool esBanca);
int calcularPuntos(Jugador& jugador);
void turnoJugador(Jugador& jugador, Carta mazo[], int& mazoSize);
void turnoBanca(Jugador& banca, Carta mazo[], int& mazoSize);
string determinarGanador(Jugador& jugador, Jugador& banca);
void guardarPartida(const Jugador& jugador, const string& resultado);
void mostrarHistorial();
string obtenerFechaActual();
void limpiarMano(Jugador& jugador);
bool preguntarRepetirJuego();

int main(){
    srand(static_cast<unsigned int>(time(0)));

    bool jugarOtraVez = true;
    string nombreJugador;

    cout << "Bienvenido a Blackjack!\nIngresa tu nombre: ";
    getline(cin, nombreJugador);

    while (jugarOtraVez){
        Carta mazo[MAX_CARTAS];
        int mazoSize=0;
        inicializarMazo(mazo, mazoSize);
        barajarMazo(mazo, mazoSize);

        Jugador jugador;
        Jugador banca;
        banca.nombre ="Banca";

        // config inicial
        jugador.nombre = nombreJugador;
        jugador.puntos=0;
        jugador.numCartas=0;
        banca.puntos=0;
        banca.numCartas=0;
        jugador.plantado=false;
        banca.plantado=false;

        //reparte cartas iniciales
        for (int i=0; i<2; i++){
            repartirCarta(jugador, mazo, mazoSize);
            repartirCarta(banca, mazo, mazoSize);
        }

        calcularPuntos(jugador);
        calcularPuntos(banca);

        cout << "\n-- INICIO DEL JUEGO --\n";
        cout << "Tu mano (" << jugador.puntos << " puntos):\n";
        mostrarMano(jugador, false);

        cout << "\nMano de la banca:\n";
        mostrarMano(banca, true);

        turnoJugador(jugador, mazo, mazoSize);

        if(!jugador.plantado && jugador.puntos <= PUNTOS_GANADORES){
            turnoBanca(banca, mazo, mazoSize);
        }

        string resultado=determinarGanador(jugador, banca);
        guardarPartida(jugador, resultado);

        jugarOtraVez=preguntarRepetirJuego();

        limpiarMano(jugador);
        limpiarMano(banca);
    }

    cout << "\n-- HISTORIAL DE PARTIDAS --\n";
    mostrarHistorial();

    cout << "\nGracias por jugar blackjack, " << nombreJugador << "!\n";
    return 0;
}

void inicializarMazo(Carta mazo[], int& mazoSize){
    // codigo
}

void barajarMazo(Carta mazo[], int mazoSize){
    // codigo
}

void repartirCarta(Jugador& jugador, Carta mazo[], int& mazoSize){
    //codigo
}

void mostrarMano(const Jugador& jugador, bool esBanca){
    //codigo
}

int calcularPuntos(Jugador& jugador){
    //codigo
    return 0;
}

void turnoJugador(Jugador& jugador, Carta mazo[], int& mazoSize){
    //codigo
}

void turnoBanca(Jugador& banca, Carta mazo[], int& mazoSize){
    //codigo
}

string determinarGanador(Jugador& jugador, Jugador& banca){
    //codigo
    return "";
}

void guardarPartida(const Jugador& jugador, const string& resultado){
    //codigo
}

void mostrarHistorial(){
    // codigo
}

string obtenerFechaActual(){
    //codigo
    return "";
}

bool preguntarRepetirJuego(){
    // codigo
    return false;
}

void limpiarMano(Jugador& jugador){
    //codigo
}
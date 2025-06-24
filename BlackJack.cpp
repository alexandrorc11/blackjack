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
    string palos[]={"Corazones", "Diamantes", "Treboles", "Picas"};
    string valores[]={"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    for(int i=0; i<4; i++){
        for(int j=0; j<13; j++){
            int puntos=0;
            if(valores[j]=="A"){
                puntos=11;
            }
            else if(valores[j]=="J" || valores[j]=="Q" || valores[j]=="K"){
                puntos=10;
            }
            else{
                puntos=atoi(valores[j].c_str());
            }
            mazo[mazoSize++]={valores[j], palos[i], puntos};
        }
    }
}

void barajarMazo(Carta mazo[], int mazoSize){
    for(int i=0; i<mazoSize; i++){
        int j=rand()%mazoSize;
        Carta temp=mazo[i];
        mazo[i]=mazo[j];
        mazo[j]=temp;
    }
}

void repartirCarta(Jugador& jugador, Carta mazo[], int& mazoSize){
    if(mazoSize==0){
        cout<< "El mazo esta vacio! Reiniciando mazo...\n";
        inicializarMazo(mazo, mazoSize);
        barajarMazo(mazo, mazoSize);
    }

    jugador.mano[jugador.numCartas++]=mazo[--mazoSize];
}

void mostrarMano(const Jugador& jugador, bool esBanca){
    if(esBanca && jugador.numCartas==2 && !jugador.plantado){
        cout<< "1. [Carta Oculta]\n";
        cout<< "2." << jugador.mano[1].valor << " de " << jugador.mano[1].palo<< "\n";
    }
    else{
        for(int i=0; i<jugador.numCartas; i++){
            cout<< i+1 << ". " << jugador.mano[i].valor<< " de " <<jugador.mano[i].palo << "\n";
        }
        if(jugador.plantado || !esBanca){
            cout << "Total: " << jugador.puntos << " puntos\n";
        }
    }
}

int calcularPuntos(Jugador& jugador){
    int total=0;
    int ases=0;

    for(int i=0; i<jugador.numCartas; i++){
        total+= jugador.mano[i].puntos;
        if(jugador.mano[i].valor=="A"){
            ases++;
        }
    }
    while(total>PUNTOS_GANADORES&&ases>0){
        total-=10;
        ases--;
    }
    jugador.puntos=total;
    return total;
}

void turnoJugador(Jugador& jugador, Carta mazo[], int& mazoSize){
    char opcion;

    while(jugador.puntos<PUNTOS_GANADORES && !jugador.plantado){
        cout<< "\nQuieres pedir carta (P) o plantarte (S)? ";
        cin >> opcion;
        cin.ignore(1000, '\n');

        if(toupper(opcion)== 'P'){
            repartirCarta(jugador, mazo, mazoSize);
            calcularPuntos(jugador);
            
            cout<<"\nTu nueva mano (" << jugador.puntos << " puntos):\n";
            mostrarMano(jugador, false);

            if(jugador.puntos >PUNTOS_GANADORES){
                cout << "\nTe has pasado de 21! Pierdes automaticamente.\n";
                return;
            }
        }
	    else if(toupper(opcion)== 'S'){
            jugador.plantado =true;
            cout <<"\nTe has plantado con " << jugador.puntos << " puntos\n";
        }
	    else{
            cout<< "Opcion no valida. Intenta nuevamente.\n";
        }
    }
}

void turnoBanca(Jugador& banca, Carta mazo[], int& mazoSize){
    cout <<"\n--- TURNO DE LA BANCA ---\n";
    cout<< "Mano completa de la banca:\n";
    mostrarMano(banca, false);

    while(banca.puntos<PUNTOS_MINIMOS_BANCA && banca.puntos<PUNTOS_GANADORES){
        cout << "\nLa banca pide otra carta...\n";
        repartirCarta(banca, mazo, mazoSize);
        calcularPuntos(banca);
        
        cout<<"Nueva mano de la banca (" << banca.puntos << " puntos):\n";
        mostrarMano(banca, false);

        if(banca.puntos > PUNTOS_GANADORES){
            cout <<"\nLa banca se ha pasado de 21. Ganas automaticamente!!\n";
            return;
        }
    }
    if(banca.puntos>=PUNTOS_MINIMOS_BANCA){
        cout << "\nLa banca se planta con "<< banca.puntos << " puntos.\n";
        banca.plantado=true;
    }
}

string determinarGanador(Jugador& jugador, Jugador& banca){
    cout<<"\n--- RESULTADO FINAL ---\n";
    cout<<"Tu puntuacion final: "<< jugador.puntos<< "\n";
    cout<<"Puntuacion final de la banca: "<< banca.puntos << "\n\n";
    string resultado;

    if(jugador.puntos>PUNTOS_GANADORES){
        cout<< "Te has pasado de 21. La banca gana!\n";
        resultado= "Derrota";
    } 
    else if(banca.puntos > PUNTOS_GANADORES){
        cout <<"La banca se ha pasado de 21 Tu ganas!\n";
        resultado ="Victoria";
    }
    else if(jugador.puntos>banca.puntos){
        cout<< "Tienes mas puntos que la banca. Tu ganas!\n";
        resultado ="Victoria";
    } 
    else if(banca.puntos > jugador.puntos){
        cout<< "La banca tiene mas puntos La banca gana!\n";
        resultado ="Derrota";
    } 
    else{
        cout<< "Empate.La banca gana por reglas del juego!\n";
        resultado ="Derrota";
    }
    return resultado;
}

void guardarPartida(const Jugador& jugador, const string& resultado){
    ofstream archivo("historial.txt", ios::app);
    if(archivo.is_open()){
        archivo<<jugador.nombre << "," << resultado<< ","<< obtenerFechaActual()<< "\n";
        archivo.close();
    } 
    else{
        cout<<"Error al guardar el historial de partidas\n";
    }
}

void mostrarHistorial(){
    ifstream archivo("historial.txt");
    if(archivo.is_open()){
        string linea;
        cout<< left << setw(20)<< "Jugador" << setw(10)<< "Resultado" << "Fecha\n";
        cout<<"----------------------------------------\n";
        
        while(getline(archivo, linea)){
            size_t pos1=linea.find(',');
            size_t pos2=linea.find(',', pos1 + 1);
            string nombre=linea.substr(0, pos1);
            string resultado= linea.substr(pos1 + 1, pos2 - pos1 - 1);
            string fecha= linea.substr(pos2 + 1);
            
            cout <<left<< setw(20) << nombre << setw(10) << resultado << fecha << "\n";
        }
        archivo.close();
    } 
    else{
        cout <<"No hay historial de partidas guardadas\n";
    }
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
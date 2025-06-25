#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

//constantes
const int PUNTOS_GANADORES = 21; //puntatje maximo de los jugadores
const int PUNTOS_MINIMOS_BANCA = 17; //puntaje que necesita la banca para plantarse
const int MAX_CARTAS = 52; //Total de cartas

//estruct de datos
struct Carta {
    string valor; //para los valores de las cartas
    string palo; //para el tipo de carta 
    int puntos; //puntos de la carta
};

struct Jugador{
    string nombre; //nombre jugador
    Carta mano[MAX_CARTAS]; //cartas del jugador
    int numCartas; //cantidad de cartas
    int puntos; //puntos del jugador
    bool plantado; //para indicar cuando el jugador se plante
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
    srand(static_cast<unsigned int>(time(0))); //inicializa los aleatorios

    bool jugarOtraVez = true; //para controlar cuando se repite el juego
    string nombreJugador; //nombre del jugador

    cout << "Bienvenido a Blackjack!\nIngresa tu nombre: ";
    getline(cin, nombreJugador); //capturar nombre

    while (jugarOtraVez){
        Carta mazo[MAX_CARTAS]; //declarar el mazo
        int mazoSize=0; //inicializar el tamaño del mazo
        inicializarMazo(mazo, mazoSize); //llenar el mazo de cartas
        barajarMazo(mazo, mazoSize); //mezclar todas las cartas

        Jugador jugador;
        Jugador banca;
        banca.nombre ="Banca";

        //estado de la banca y del jugador
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

        //calcular los puntos
        calcularPuntos(jugador);
        calcularPuntos(banca);

        cout << "\n-- INICIO DEL JUEGO --\n";
        cout << "Tu mano (" << jugador.puntos << " puntos):\n";
        mostrarMano(jugador, false);

        cout << "\nMano de la banca:\n";
        mostrarMano(banca, true);

        //turno del jugador 
        turnoJugador(jugador, mazo, mazoSize);

        //si no se paso y no se planto juega la banca
        if(!jugador.plantado && jugador.puntos <= PUNTOS_GANADORES){
            turnoBanca(banca, mazo, mazoSize);
        }

        //guardar el resultado de la partida 
        string resultado=determinarGanador(jugador, banca);
        guardarPartida(jugador, resultado);

        //preguntar para repetir el juego
        jugarOtraVez=preguntarRepetirJuego();

        //limpiar las manos del jugador y de la banca
        limpiarMano(jugador);
        limpiarMano(banca);
    }
    //muestra el historial de partidas
    cout << "\n-- HISTORIAL DE PARTIDAS --\n";
    mostrarHistorial();

    cout << "\nGracias por jugar blackjack, " << nombreJugador << "!\n";
    return 0;
}

void inicializarMazo(Carta mazo[], int& mazoSize){
    //arreglos con todos los tipos de cartas y los valores posible
    string palos[]={"Corazones", "Diamantes", "Treboles", "Picas"};
    string valores[]={"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    //recorre cada palo
    for(int i=0; i<4; i++){
        //recorre el valor dentro del palo 
        for(int j=0; j<13; j++){
            int puntos=0; //puntos que vale esta carta
            //asignar puntos segun el valor de la carta
            if(valores[j]=="A"){
                puntos=11;//si la carta es as
            }
            else if(valores[j]=="J" || valores[j]=="Q" || valores[j]=="K"){
                puntos=10;//Las figuras valen 10
            }
            else{
                puntos=atoi(valores[j].c_str()); //convierte el valor de la carta a entero
            }
            mazo[mazoSize++]={valores[j], palos[i], puntos}; //crea la carta y la añade al mazo
        }
    }
}

void barajarMazo(Carta mazo[], int mazoSize){
    //recorre todas las cartas del mazo
    for(int i=0; i<mazoSize; i++){
        int j=rand()%mazoSize;//genera una posicion aleatoria entre 0 y mazoSize-1
        //intercambia el valor de la carta actual con una aleatoria
        Carta temp=mazo[i];
        mazo[i]=mazo[j];
        mazo[j]=temp;
    }
}

void repartirCarta(Jugador& jugador, Carta mazo[], int& mazoSize){
    //ver si el mazo esta vacio
    if(mazoSize==0){
        cout<< "El mazo esta vacio! Reiniciando mazo...\n";
        inicializarMazo(mazo, mazoSize);//reiniciar el mazo con todas las carta
        barajarMazo(mazo, mazoSize); //mezclar el mazo
    }

    //pone la ultima carta del mazo a la mano del jugador
    jugador.mano[jugador.numCartas++]=mazo[--mazoSize];
}

void mostrarMano(const Jugador& jugador, bool esBanca){
    //si es la banca tiene solo dos cartas y una oculta si no se ha plantado
    if(esBanca && jugador.numCartas==2 && !jugador.plantado){
        cout<< "1. [Carta Oculta]\n";//oculta una carta
        cout<< "2." << jugador.mano[1].valor << " de " << jugador.mano[1].palo<< "\n";//muestra la otra carta
    }
    else{
        //muestra las cartas del jugador
        for(int i=0; i<jugador.numCartas; i++){
            cout<< i+1 << ". " << jugador.mano[i].valor<< " de " <<jugador.mano[i].palo << "\n";
        }
        //muestra el total de si es el jugador y ya se planto
        if(jugador.plantado || !esBanca){
            cout << "Total: " << jugador.puntos << " puntos\n";
        }
    }
}

int calcularPuntos(Jugador& jugador){
    //contador de puntos
    int total=0;
    //contador de ases
    int ases=0;

    //recorre todas las cartas que tiene el jugador
    for(int i=0; i<jugador.numCartas; i++){
        total+= jugador.mano[i].puntos; //sumar los puntos
        //ver si la carta es un as
        if(jugador.mano[i].valor=="A"){
            ases++; //aumentar el contador de ases
        }
    }
    //si el total de puntos es mayor a 21 cambiar el valor de los ases a 1 en vez de 11
    while(total>PUNTOS_GANADORES&&ases>0){
        total-=10;//restar 10 puntos para corregir el valor del as
        ases--;//disminuir el comntador del ases
    }
    jugador.puntos=total; //actualizar el total de puntos del jugador
    return total; //regresar el total de puntos
}

void turnoJugador(Jugador& jugador, Carta mazo[], int& mazoSize){
    char opcion;// para guardar el valor de plantarse o pedir carta

    //seguir mientras el jugador no se haya plantado y no supere los 21 puntos
    while(jugador.puntos<PUNTOS_GANADORES && !jugador.plantado){
        cout<< "\nQuieres pedir carta (P) o plantarte (S)? ";//preguntar si se quiere plantar o pedir carta
        cin >> opcion;//leer la opcion del jugador
        cin.ignore(1000, '\n');//limpiar bufer 

        //si la opcion es p
        if(toupper(opcion)== 'P'){
            repartirCarta(jugador, mazo, mazoSize);//repartir una carta del mazo
            calcularPuntos(jugador);//calcular sus puntos
            
            cout<<"\nTu nueva mano (" << jugador.puntos << " puntos):\n";//mostrar las cartas
            mostrarMano(jugador, false);

            //si el jugador pasa de 21 pierde automaticamente
            if(jugador.puntos >PUNTOS_GANADORES){
                cout << "\nTe has pasado de 21! Pierdes automaticamente.\n";
                return;//salimos de la funcion
            }
        }
        //si la opcion es s
	    else if(toupper(opcion)== 'S'){
            jugador.plantado =true;//marcar al jugador como plantado
            cout <<"\nTe has plantado con " << jugador.puntos << " puntos\n";//mostrar al jugador que se planto y sus puntos
        }
        //si la opcion no es valida
	    else{
            cout<< "Opcion no valida. Intenta nuevamente.\n";
        }
    }
}

void turnoBanca(Jugador& banca, Carta mazo[], int& mazoSize){
    cout <<"\n--- TURNO DE LA BANCA ---\n";//indicar que es turno de la banca
    cout<< "Mano completa de la banca:\n";//mostrar las cartas de la banca
    mostrarMano(banca, false); //se ponde false para que muestre todas las cartas

    //mientras la banca denga menos del minimo para plantarse y no pase de 21
    while(banca.puntos<PUNTOS_MINIMOS_BANCA && banca.puntos<PUNTOS_GANADORES){
        cout << "\nLa banca pide otra carta...\n";//pide otra carta
        repartirCarta(banca, mazo, mazoSize);//repartir una carta del mazo
        calcularPuntos(banca);//calcular sus puntos
        
        //mostrar la nuevas cartas de la banca
        cout<<"Nueva mano de la banca (" << banca.puntos << " puntos):\n";
        mostrarMano(banca, false);

        //si se pasa de 21 pierde automaticamente
        if(banca.puntos > PUNTOS_GANADORES){
            cout <<"\nLa banca se ha pasado de 21. Ganas automaticamente!!\n";
            return;
        }
    }
    //si alcanza los puntos para plantarse
    if(banca.puntos>=PUNTOS_MINIMOS_BANCA){
        //mostrar que la banca se planto
        cout << "\nLa banca se planta con "<< banca.puntos << " puntos.\n";
        banca.plantado=true;//se marca la banca plantada
    }
}

string determinarGanador(Jugador& jugador, Jugador& banca){
    //se muestran los resultados
    cout<<"\n--- RESULTADO FINAL ---\n";
    cout<<"Tu puntuacion final: "<< jugador.puntos<< "\n";//se muestra el puntaje del jugador
    cout<<"Puntuacion final de la banca: "<< banca.puntos << "\n\n";//se muestra el puntaje de la banca
    string resultado;//variable para guardar el resultado

    //si el jugador se pasa de 21 puntos
    if(jugador.puntos>PUNTOS_GANADORES){
        //pierde automaticamente
        cout<< "Te has pasado de 21. La banca gana!\n";
        resultado= "Derrota";
    } 
    //si la banca se pasa de 21 gana el jugador
    else if(banca.puntos > PUNTOS_GANADORES){
        cout <<"La banca se ha pasado de 21 Tu ganas!\n";
        resultado ="Victoria";
    }
    //si el jugador tiene mas puntos que la banca y no se pasa de 21
    else if(jugador.puntos>banca.puntos){
        cout<< "Tienes mas puntos que la banca. Tu ganas!\n";
        resultado ="Victoria";
    } 
    //si la banca tiene mas puntos que el jugador
    else if(banca.puntos > jugador.puntos){
        cout<< "La banca tiene mas puntos La banca gana!\n";
        resultado ="Derrota";
    } 
    //si ambos tiene la msima cantidad de puntos gana la banca
    else{
        cout<< "Empate.La banca gana por reglas del juego!\n";
        resultado ="Derrota";
    }
    //se retorna el resultado 
    return resultado;
}

void guardarPartida(const Jugador& jugador, const string& resultado){
    //se abre el archivo
    ofstream archivo("historial.txt", ios::app);
    //se verifica que se abrio correctamente
    if(archivo.is_open()){
        //se escribe el nombre del jugador el resultado y la fecha
        archivo<<jugador.nombre << "," << resultado<< ","<< obtenerFechaActual()<< "\n";
        archivo.close();//se cierra el archivo despues de escribir
    } 
    else{
        //si no se pudo abrir el archivo se muestra el error
        cout<<"Error al guardar el historial de partidas\n";
    }
}

void mostrarHistorial(){
    //se abre el archivo
    ifstream archivo("historial.txt");
    //se verifica que el archivo se abrio
    if(archivo.is_open()){
        string linea;//variable para almacenar cada linea del archivo
        //se imprime los encabezados de la tabla
        cout<< left << setw(20)<< "Jugador" << setw(10)<< "Resultado" << "Fecha\n";
        cout<<"----------------------------------------\n";
        
        //se recorre cada linea del archivo
        while(getline(archivo, linea)){
            size_t pos1=linea.find(',');//se busca la posicion de la primer coma
            size_t pos2=linea.find(',', pos1 + 1);//se busca la posicion de la segunda coma
            string nombre=linea.substr(0, pos1);//se extrae el nombre hasta la primer coma
            string resultado= linea.substr(pos1 + 1, pos2 - pos1 - 1);//se extrae el resultado entre la primer coma y la segunda
            string fecha= linea.substr(pos2 + 1);//se extrae la fecha despues de la segunda coma
            //se muestra la informacion
            cout <<left<< setw(20) << nombre << setw(10) << resultado << fecha << "\n";
        }
        //se cierra el archivo
        archivo.close();
    } 
    //si no se puede abrir el archivo
    else{
        cout <<"No hay historial de partidas guardadas\n";
    }
}

string obtenerFechaActual(){
    //se obtiene el tiempo actual en el formato de tiempo UNIX
    time_t ahora=time(0);
    tm* tiempo=localtime(&ahora);//se convierte el tiempo a la estructura de fecha y hora actual
    char buffer[80];//se declara un arreglo de caracteres para guardar fecha y hora
    //se da formato a la fecha y hora y se guarda en el bufer
    // %d = día, %m = mes, %Y = año con 4 dígitos, %H:%M:%S = hora:minutos:segundos
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tiempo);
    //se convierte el arreglo en un string y se retorna
    return string(buffer);
}

bool preguntarRepetirJuego(){
    char respuesta;//variable para guardar la respuesta
    //bucle infinito hasta que el usuario de una respuesta invalida
    while(true){
        cout<< "\nQuieres jugar otra vez? (S/N): ";//se pregunta si quiere jugar otra vez
        cin >>respuesta;//leer respuesta
        cin.ignore(1000, '\n');//se limpia el bufer

        respuesta=toupper(respuesta);//se convierte la respuesta a mayuscula
        //si la respuesta es s
        if(respuesta=='S'){
            return true;//se devuelve verdadero
        } 
        //si la respuesta es n
        else if(respuesta=='N'){
            return false;//se devuelve falso
        } 
        //si la opcion no es valida
        else{
            cout<< "Opcion no valida. Por favor ingrese S o N.\n";
        }
    }
}

void limpiarMano(Jugador& jugador){
    jugador.numCartas=0;//se reinicia el numero de cartas del jugador a 0
    jugador.puntos=0;//se reinicia los puntos acumulados a 0
    jugador.plantado=false;//se marca que el jugador no esta plantado
}
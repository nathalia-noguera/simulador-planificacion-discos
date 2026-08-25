#include <iostream>
#include "Disk.hpp"

int main() {
    std::cout << ">>> SIMULADOR DE DISCO - FASE 1 <<<\n\n";

    // 1. Definir valores por defecto iniciales por si el usuario no quiere personalizar
    int cilindrosDefault = 200;
    int posicionDefault = 50;
    double tiempoDefault = 2.0;

    int userCilindros, userPosicion;
    double userTiempo;

    // 2. Permitir que el usuario decida y escriba los valores por teclado
    std::cout << "Configuración del Disco Duro Virtual:\n";
    
    std::cout << "Ingrese el número total de cilindros (ej. 200): ";
    std::cin >> userCilindros;

    std::cout << "Ingrese la posición inicial del cabezal (ej. 50): ";
    std::cin >> userPosicion;

    std::cout << "Ingrese el tiempo en ms por cada salto de cilindro (ej. 2.0): ";
    std::cin >> userTiempo;

    // 3. Creamos el objeto disco con los valores introducidos por el usuario
    Disk miDisk(userCilindros, userPosicion, userTiempo);

    std::cout << "\n[OK] Disco creado exitosamente con tus valores.\n";
    miDisk.printStatus();

    // 4. Probar mover el brazo a un cilindro destino ingresado también por el usuario
    int destino;
    std::cout << "\n¿A qué cilindro deseas mover el brazo lector ahora? ";
    std::cin >> destino;

    double tiempoGastado = miDisk.moveHeadTo(destino);

    if (tiempoGastado > 0.0) {
        std::cout << "\n-> ¡Movimiento exitoso!\n";
        std::cout << "-> Tiempo total que tardó el brazo: " << tiempoGastado << " ms.\n\n";
        miDisk.printStatus();
    }

    std::cout << "\n>>> FASE 1 FINALIZADA CORRECTAMENTE <<<\n";
    return 0;
}
#include "../include/Disk.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>

// Constructor, valida los valores recibidos y asigna los del disco.
Disk::Disk(int cylinders, int initialPosition, double msPerCylinder, int tracksPerCyl) {
    if(cylinders > 0){
        totalCylinders = cylinders;
    }else{
        totalCylinders = 200;
    }

    if(initialPosition >= 0 && initialPosition < totalCylinders){
        currentHeadPosition = initialPosition;
    }else{
        currentHeadPosition = 0;
    }

    if(msPerCylinder >= 0.0){
        timePerCylinder = msPerCylinder;
    }else{
        timePerCylinder = 1.0;
    }
    direction = 1;

    if(tracksPerCyl > 0){
        tracksPerCylinder = tracksPerCyl;
    }else{
        tracksPerCylinder = 4;
    }
}

// Devuelve el total de cilindros del disco.
int Disk::getTotalCylinders() const {
    return totalCylinders;
}
// Devuelve la posición actual del cabezal.
int Disk::getCurrentHeadPosition() const {
    return currentHeadPosition;
}
// Devuelve la dirección en la que se mueve el cabezal.
int Disk::getDirection() const {
    return direction;
}
// Devuelve el tiempo que tarda el cabezal en moverse un cilindro.
double Disk::getTimePerCylinder() const {
    return timePerCylinder;
}
// Devuelve el número de pistas por cilindro.
int Disk::getTracksPerCylinder() const {
    return tracksPerCylinder;
}

// Cambia la posición actual del cabezal si está dentro del rango.
void Disk::setCurrentHeadPosition(int newPosition){
    if(newPosition >= 0 && newPosition < totalCylinders){
        currentHeadPosition = newPosition;
    }else{
        std::cout << "  [ADVERTENCIA] Posición fuera de rango. No se pudo cambiar.\n";
    }
}

// Cambia la dirección de movimiento del cabezal.
void Disk::setDirection(int newDirection){
    if(newDirection == 1 || newDirection == -1){
        direction = newDirection;
    }
}

// Cambia el tiempo que tarda el cabezal en moverse un cilindro.
void Disk::setTimePerCylinder(double newTime){
    if(newTime >= 0.0){
        timePerCylinder = newTime;
    }
}

// Cambia el número de pistas por cilindro.
void Disk::setTracksPerCylinder(int newTracks){
    if(newTracks > 0){
        tracksPerCylinder = newTracks;
    }else{
        std::cout << "  [ADVERTENCIA] Número de pistas inválido. No se pudo cambiar.\n";
    }
}

// Calcula la distancia entre dos cilindros.
int Disk::calculateDistance(int cylinderA, int cylinderB) const {
    return std::abs(cylinderA - cylinderB);
}

// Mueve el cabezal a un cilindro destino y calcula el tiempo que tomó.
double Disk::moveHeadTo(int targetCylinder){
    if(targetCylinder < 0 || targetCylinder >= totalCylinders){
        std::cerr << "  [ERROR] Cilindro " << targetCylinder << " fuera de rango.\n";
        return 0.0;
    }

    int distance = calculateDistance(currentHeadPosition, targetCylinder);
    double timeElapsed = distance * timePerCylinder;
    currentHeadPosition = targetCylinder;

    return timeElapsed;
}

// Muestra por pantalla el estado actual del disco.
void Disk::printStatus() const {
    std::string direccionTexto;
    if(direction == 1){
        direccionTexto = "Derecha / Subiendo";
    }else{
        direccionTexto = "Izquierda / Bajando";
    }

    std::cout << "-------------------------------------------------------------------------------\n\n"
              << "  E s t a d o  a c t u a l  d e l  d i s c o\n\n"
              << "  Total de cilindros      : " << totalCylinders << "\n"
              << "  Pistas por cilindro     : " << tracksPerCylinder << "\n"
              << "  Posición actual cabezal : " << currentHeadPosition << "\n"
              << "  Dirección de movimiento : " << direccionTexto << "\n"
              << "  Tiempo por cilindro     : " << timePerCylinder << " ms\n\n"
              << "-------------------------------------------------------------------------------\n";
}
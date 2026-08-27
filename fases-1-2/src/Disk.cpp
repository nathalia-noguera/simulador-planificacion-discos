#include "../include/Disk.hpp" // Ajuste vital: ruta correcta
#include <iostream>            // Ajuste vital: para usar std::cout y std::cerr
#include <cmath>
#include <stdexcept>

Disk::Disk(int cylinders, int initialPosition, double msPerCylinder) {
    totalCylinders = (cylinders > 0) ? cylinders : 200;

    if (initialPosition >= 0 && initialPosition < totalCylinders) {
        currentHeadPosition = initialPosition;
    } else {
        currentHeadPosition = 0;
    }

    timePerCylinder = (msPerCylinder >= 0.0) ? msPerCylinder : 1.0;
    direction = 1;
}

int Disk::getTotalCylinders() const { return totalCylinders; }
int Disk::getCurrentHeadPosition() const { return currentHeadPosition; }
int Disk::getDirection() const { return direction; }
double Disk::getTimePerCylinder() const { return timePerCylinder; }

// Implementación real de los setters
void Disk::setCurrentHeadPosition(int newPosition) {
    if (newPosition >= 0 && newPosition < totalCylinders) {
        currentHeadPosition = newPosition;
    } else {
        std::cout << "[ADVERTENCIA] Posición fuera de rango. No se pudo cambiar.\n";
    }
}

void Disk::setDirection(int newDirection) {
    if (newDirection == 1 || newDirection == -1) {
        direction = newDirection;
    }
}

void Disk::setTimePerCylinder(double newTime) {
    if (newTime >= 0.0) {
        timePerCylinder = newTime;
    }
}

int Disk::calculateDistance(int cylinderA, int cylinderB) const {
    return std::abs(cylinderA - cylinderB);
}

double Disk::moveHeadTo(int targetCylinder) {
    if (targetCylinder < 0 || targetCylinder >= totalCylinders) {
        std::cerr << "[ERROR] Cilindro " << targetCylinder << " fuera de rango.\n";
        return 0.0;
    }

    int distance = calculateDistance(currentHeadPosition, targetCylinder);
    double timeElapsed = distance * timePerCylinder;
    currentHeadPosition = targetCylinder;

    return timeElapsed;
}

void Disk::printStatus() const {
    std::cout << "----------------------------------------\n"
              << "       ESTADO ACTUAL DEL DISCO          \n"
              << "----------------------------------------\n"
              << "Total de cilindros     : " << totalCylinders << "\n"
              << "Posición actual cabezal: " << currentHeadPosition << "\n"
              << "Dirección de movimiento: " << (direction == 1 ? "Derecha / Subiendo" : "Izquierda / Bajando") << "\n"
              << "Tiempo por cilindro    : " << timePerCylinder << " ms\n"
              << "----------------------------------------\n";
}
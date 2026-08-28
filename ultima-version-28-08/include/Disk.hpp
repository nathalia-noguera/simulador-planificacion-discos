#ifndef DISK_HPP
#define DISK_HPP

#include <iostream>

class Disk{
private:
    int totalCylinders;       
    int currentHeadPosition;  
    int direction;            
    double timePerCylinder;   
    int tracksPerCylinder; // NUEVO: número de pistas (superficies) por cilindro

public:
    Disk(int cylinders, int initialPosition, double msPerCylinder, int tracksPerCyl = 4);

    // Getters
    int getTotalCylinders() const;
    int getCurrentHeadPosition() const;
    int getDirection() const;
    double getTimePerCylinder() const;
    int getTracksPerCylinder() const; // NUEVO

    // Setters (¡Aquí están los que faltaban para cambiar los valores!)
    void setCurrentHeadPosition(int newPosition);
    void setDirection(int newDirection);
    void setTimePerCylinder(double newTime);
    void setTracksPerCylinder(int newTracks); // NUEVO

    // Funciones del disco
    int calculateDistance(int cylinderA, int cylinderB) const;
    double moveHeadTo(int targetCylinder);
    void printStatus() const;
};

#endif // DISK_HPP
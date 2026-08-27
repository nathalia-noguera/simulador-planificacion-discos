#ifndef DISK_HPP
#define DISK_HPP

#include <iostream>

class Disk {
private:
    int totalCylinders;       
    int currentHeadPosition;  
    int direction;            
    double timePerCylinder;   

public:
    Disk(int cylinders, int initialPosition, double msPerCylinder);

    // Getters
    int getTotalCylinders() const;
    int getCurrentHeadPosition() const;
    int getDirection() const;
    double getTimePerCylinder() const;

    // Setters (¡Aquí están los que faltaban para cambiar los valores!)
    void setCurrentHeadPosition(int newPosition);
    void setDirection(int newDirection);
    void setTimePerCylinder(double newTime);

    // Funciones del disco
    int calculateDistance(int cylinderA, int cylinderB) const;
    double moveHeadTo(int targetCylinder);
    void printStatus() const;
};

#endif // DISK_HPP
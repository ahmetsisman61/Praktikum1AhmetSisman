/**
 * @author Ahmet Sisman
 * @author Muhammed Gürgöze
 * @date   07.04.2023
 */


#include "FestoTransferSystem.h"
#include "TimeHelper.h"
#include <windows.h>
#include <iostream>

// Aufgabe 3.3

void sensor(FestoTransferSystem *festo) {

    if (festo->lightbarrierBegin.getState()) {
        std::cout << "Lichtschranke 1 geschlossen" << std::endl;
    } else {
        std::cout << "Lichtschranke 1 unterbrochen";

    }
    if (festo->lightbarrierHeightSensor.getState()) {
        std::cout << "Lichtschranke 2 geschlossen" << std::endl;
    } else {
        std::cout << "Lichtschranke 2 unterbrochen";
    }
    if (festo->lightbarrierFeedSeparator.getState()) {
        std::cout << "Lichtschranke 3 geschlossen" << std::endl;
    } else {
        std::cout << "Lichtschranke 3 unterbrochen";
    }
    if (festo->lightbarrierBufferFull.getState()) {
        std::cout << "Lichtschranke 4 geschlossen" << std::endl;
    } else {
        std::cout << "Lichtschranke 4 unterbrochen";
    }
    if (festo->lightbarrierEnd.getState()) {
        std::cout << "Lichtschranke 5 geschlossen" << std::endl;
    } else {
        std::cout << "Lichtschranke 5 unterbrochen";
    }
}

// Aufgabe 3.2
int main() {

    FestoTransferSystem festo;
    int i = 1;
    while (i < 6) {

        festo.updateSensors();   // E
        sensor(&festo);
        festo.lampGreen.setState(false); //V
        festo.lampRed.setState(true);// V
        festo.updateActuators(); // A
        Sleep(1000);

        festo.updateSensors();   // E
        sensor(&festo);
        festo.lampRed.setState(false);//V
        festo.lampYellow.setState(true); // V
        festo.updateActuators(); // A
        Sleep(1000);

        festo.updateSensors();   // E
        sensor(&festo);   //V
        festo.lampGreen.setState(true);
        festo.lampYellow.setState(false);// V
        festo.updateActuators(); // A
        Sleep(1000);
        i++;

    }

    // Aufgabe 3.4
    festo.updateSensors();
    sensor(&festo);
    festo.lampYellow.setState(true);
    festo.lampYellow.setState(false);
    festo.updateActuators();

    bool running = true;

    while (running) {
        festo.updateSensors(); // E
        if (!festo.switchEmergency.getState()) {
            festo.drive.setSpeed(0);
            festo.ledQ1.setState(false);
            festo.ledQ2.setState(false);
            festo.feedSeparator.setState(false);
            running = false;
        }else {
            if (festo.pushbuttonStart.getState()) { // V
                festo.ledStart.setState(true);
                festo.drive.setSpeed(CONVEYERBELT_RIGHT_FAST);
            } else {
                festo.ledStart.setState(false);
                festo.drive.setSpeed(CONVEYERBELT_STOP);
            }
            if (!festo.pushbuttonStop.getState()) {
                festo.feedSeparator.setState(true);
                festo.ledReset.setState(true);
            } else {
                festo.feedSeparator.setState(false);
                festo.ledReset.setState(false);
            }
            if (!festo.lightbarrierBegin.getState()) {
                festo.ledQ1.setState(true);
            } else {
                festo.ledQ1.setState(false);
            }
            if (festo.lightbarrierHeightSensor.getState()) {
                festo.ledQ2.setState(true);
            } else {
                festo.ledQ2.setState(false);
            }
        }

        festo.updateActuators();// A
    }
}
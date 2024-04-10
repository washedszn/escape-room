#ifndef POTENTIOMETERLOCK_H
#define POTENTIOMETERLOCK_H

extern void updateTimerDisplay();

class PotentiometerLock {
public:
    PotentiometerLock();
    void run();
    bool isCompleted();

private:
    void createLockCharacters();
    void checkTM1638ButtonPress();
    void updateDisplay();
    void checkCombination();
    void cycleLocks();
    bool completed;
    bool lockState[3]; // Tracks the locked/unlocked state of each lock
};

#endif // POTENTIOMETERLOCK_H

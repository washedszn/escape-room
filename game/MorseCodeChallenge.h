// MorseCodeChallenge.h
#ifndef MORSECODECHALLENGE_H
#define MORSECODECHALLENGE_H

extern void updateTimerDisplay();

class MorseCodeChallenge {
public:
    MorseCodeChallenge();
    void run();
    bool isCompleted();
private:
    void outputMorseCode(int level);
    bool checkButtonSequence(int level);
    bool checkPotentiometerPosition(int level);
    void nextLevel();
    
    int currentLevel = 0;
    bool levelCompleted[3] = {false, false, false};
};

#endif // MORSECODECHALLENGE_H

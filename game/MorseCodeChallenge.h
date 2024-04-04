// MorseCodeChallenge.h
#ifndef MORSECODECHALLENGE_H
#define MORSECODECHALLENGE_H

class MorseCodeChallenge {
public:
    MorseCodeChallenge(); // Constructor
    void run(); // Starts the challenge
    bool isCompleted(); // Checks if the challenge is completed
    void reset(); // Resets the challenge to its initial state

private:
    bool completed; // Keeps track of completion status
    // Add any other member variables and methods needed for the challenge
};

#endif // MORSECODECHALLENGE_H

// MelodyMe.h
#ifndef MELODYME_H
#define MELODYME_H

extern void updateTimerDisplay();

class MelodyMe {
public:
    MelodyMe(); // Constructor
    void run(); // Starts the challenge
    bool isCompleted(); // Checks if the challenge is completed
    void reset(); // Resets the challenge to its initial state

private:
    bool completed; // Keeps track of completion status
    // Add any other member variables and methods needed for the challenge
};

#endif // MELODYME_H

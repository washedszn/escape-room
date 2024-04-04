#ifndef REDLIGHTGREENLIGHT_H
#define REDLIGHTGREENLIGHT_H

class RedLightGreenLight {
public:
    RedLightGreenLight();
    void run(); // Removed the default parameter for simplicity
    bool isCompleted();

private:
    void initializeLevel(int level);
    bool processLevel(int level); // Handles the logic for a single level
    void displayCountdown(int countdownSeconds);
    void displayLevelOutcome(bool success, int level);
    
    bool completed; // Tracks if the entire challenge is completed
};

#endif // REDLIGHTGREENLIGHT_H

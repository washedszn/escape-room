#ifndef MELODYME_H
#define MELODYME_H

extern void updateTimerDisplay();

class MelodyMe {
public:
    MelodyMe();
    void run();
    bool isCompleted();
    void reset();
    void playMelody(int level);
    bool captureInput(int level);
    void replayMelody();
    void clearAllLEDs();

    static const int melodyLengths[3];
    static const int melodies[3][15]; // Maximum 15 notes in the most complex melody

private:
    bool completed;
    int currentLevel;
};

#endif // MELODYME_H
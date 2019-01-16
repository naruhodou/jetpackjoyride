#ifndef TIMER_H
#define TIMER_H


#define GLM_ENABLE_EXPERIMENTAL
class Timer {
public:
    Timer() {
        prev = 0;
    }
    Timer(double interval);
    double interval;
    bool processTick();
private:
    double prev;
};

#endif // TIMER_H

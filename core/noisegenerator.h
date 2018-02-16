#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

class NoiseGenerator {
public:
    explicit NoiseGenerator(int length);
    void reset();
    float get(float alpha);
private:
    const int mSampleCount;
    unsigned int mRandomSeed = 0;
    int mLastIndex = -1;
    float mLastValue = 0;

    float randomRange(float range);
};

#endif // NOISEGENERATOR_H

#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

/**
 * Generates a reproducible sequence of random values between -1 and 1
 * for a period.
 *
 * The generator returns `sampleCount` (the constructor argument) different
 * values, stretched on the period.
 *
 * `alpha` the argument of the get() method, is a number between 0 and 1,
 * where 0 represents the start of the period and 1 the end.
 *
 * To reproduce the same sequence, call reset()
 */
class NoiseGenerator {
public:
    explicit NoiseGenerator(int sampleCount);
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

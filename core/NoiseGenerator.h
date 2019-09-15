#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include <QtGlobal>

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
    qreal get(qreal alpha);

private:
    const int mSampleCount;
    unsigned int mRandomSeed = 0;
    int mLastIndex = -1;
    qreal mLastValue = 0;

    qreal randomRange(qreal range);
};

#endif // NOISEGENERATOR_H

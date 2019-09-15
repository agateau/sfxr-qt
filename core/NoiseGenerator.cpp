#include "NoiseGenerator.h"

#include <stdlib.h>

NoiseGenerator::NoiseGenerator(int sampleCount) : mSampleCount(sampleCount) {
}

void NoiseGenerator::reset() {
    mRandomSeed = 0;
    mLastIndex = -1;
}

qreal NoiseGenerator::get(qreal alpha) {
    int index = int(mSampleCount * alpha);
    if (index != mLastIndex) {
        mLastIndex = index;
        mLastValue = randomRange(2.0) - 1.0;
    }
    return mLastValue;
}

qreal NoiseGenerator::randomRange(qreal range) {
    return rand_r(&mRandomSeed) / qreal(RAND_MAX) * range;
}

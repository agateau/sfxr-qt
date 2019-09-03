#include "NoiseGenerator.h"

#include <stdlib.h>

NoiseGenerator::NoiseGenerator(int sampleCount)
    : mSampleCount(sampleCount) {
}

void NoiseGenerator::reset() {
    mRandomSeed = 0;
    mLastIndex = -1;
}

float NoiseGenerator::get(float alpha) {
    int index = mSampleCount * alpha;
    if (index != mLastIndex) {
        mLastIndex = index;
        mLastValue = randomRange(2.0f) - 1.0f;
    }
    return mLastValue;
}

float NoiseGenerator::randomRange(float range) {
    return rand_r(&mRandomSeed) / float(RAND_MAX) * range;
}

#ifndef MEAN_FILTER_H
#define MEAN_FILTER_H

#include "include/weightedfilter.h"

class MeanFilter : public WeightedFilter {
    public:
        MeanFilter(int size)
            : WeightedFilter(
                std::vector<std::vector<float>>(
                    size, std::vector<float>(size, 1.0f))) {}
};

#endif // MEAN_FILTER_H

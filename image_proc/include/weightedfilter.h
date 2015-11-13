#ifndef WEIGHTED_FILTER_H
#define WEIGHTED_FILTER_H

#include "include/filter.h"

#include <vector>

class WeightedFilter : public Filter {
    public:
        WeightedFilter(const std::vector<std::vector<float>> & weights);
        virtual QImage apply(const QImage & image);

    protected:
        float calculate_sum(const std::vector<std::vector<float>> & v) const;
        static std::vector<std::vector<float>> copy_weights(
            const std::vector<std::vector<float>> & weights);
        QRgb round_neighbors(
            unsigned int x, unsigned int y, const QImage & image) const;
        unsigned int round_value(float v) const;
        unsigned int size;
        float sum;
        std::vector<std::vector<float>> weights;
};

#endif // WEIGHTED_FILTER_H

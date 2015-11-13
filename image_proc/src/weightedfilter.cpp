#include "include/weightedfilter.h"

#include <cmath>
#include <algorithm>
#include <numeric>
#include <exception>

WeightedFilter::WeightedFilter(
        const std::vector<std::vector<float> > & weights) :
            size(weights.size()),
            sum(this->calculate_sum(weights)),
            weights(WeightedFilter::copy_weights(weights)) {
    if(size %2 != 1)
        throw std::exception();
    for(unsigned int i = 0; i < size; ++i)
        if(weights[i].size() != size)
            throw std::exception();
}

/*virtual*/
QImage WeightedFilter::apply(const QImage & image) {
    const unsigned int HALF = this->size / 2;
    QImage copy(image);
    for(unsigned int y = HALF; y < image.height() - HALF; ++y)
        for(unsigned int x = HALF; x < image.width() - HALF; ++x)
            copy.setPixel(x, y, this->round_neighbors(x, y, image));
    return copy;
}

float WeightedFilter::calculate_sum(const std::vector<std::vector<float>> & v)
        const{
    float ret = 0.0f;
    for(unsigned int i = 0; i < this->size; ++i)
        for(unsigned int j = 0; j < this->size; ++j)
            ret += v[i][j];
    return ret;
}

/*static*/
std::vector<std::vector<float>> WeightedFilter::copy_weights(
        const std::vector<std::vector<float>> & weights) {
    auto ret = std::vector<std::vector<float>>(weights.size());
    std::copy(weights.cbegin(), weights.cend(), ret.begin());
    return ret;
}

QRgb WeightedFilter::round_neighbors(
        unsigned int x, unsigned int y, const QImage & image) const {
    const unsigned int HALF = this->size / 2;
    float rgba[] = {0.0f, 0.0f, 0.0f, 0.0f};
    for(unsigned int y2 = 0; y2 < size; ++y2) {
        for(unsigned int x2 = 0; x2 < size; ++x2) {
            float weight = this->weights[y2][x2];
            QRgb neighbor = image.pixel(x + x2 - HALF, y + y2 - HALF);
            rgba[0] +=   qRed(neighbor) * weight;
            rgba[1] += qGreen(neighbor) * weight;
            rgba[2] +=  qBlue(neighbor) * weight;
            rgba[3] += qAlpha(neighbor) * weight;
        }
    }
    return qRgba(
        this->round_value(rgba[0]),
        this->round_value(rgba[1]),
        this->round_value(rgba[2]),
        this->round_value(rgba[3]));
}

unsigned int WeightedFilter::round_value(float v) const {
    return static_cast<unsigned int>(std::floor(v / this->sum) + 0.5f);
}

#include "include/histogram.h"

#include <QImage>

/*static*/
Histogram::channel_map_function Histogram::CHANNEL_MAP[4] =
    {qRed, qGreen, qBlue, qAlpha};

Histogram::Histogram(const QImage & image, int channel)
    : channel(channel), size(image.width() * image.height()), data(256, 0) {
    this->create(image);
}

Histogram::Histogram(int channel, int size, const std::vector<int> & data)
    : channel(channel), size(size), data(data) {}

/*static*/
std::vector<Histogram> Histogram::create_all(const QImage & image) {
    int channels =
        image.isGrayscale() ? 1 :
        image.hasAlphaChannel() ? 4 :
        3;
    std::vector<Histogram> histograms;
    histograms.reserve(channels);
    for(int i = 0; i < channels; ++i)
        histograms.push_back(Histogram(image, i));
    return histograms;
}

void Histogram::create(const QImage & image) {
    for(int y = 0; y < image.height(); y++)
        for(int x = 0; x < image.width(); x++)
            ++this->data[CHANNEL_MAP[channel](image.pixel(x, y))];
}

QString Histogram::toString() const {
    QString s;
    for(unsigned int iRgb = 0; iRgb < 256; ++iRgb)
        s += QString("%1: %2\n").arg(
            QString::number(iRgb),
            QString(
                static_cast<unsigned int>(
                    static_cast<float>(data[iRgb]) / size * 100.0f),
                'o'));
    return s;
}

Histogram Histogram::equalize() const {
    float freq[256];
    for(unsigned int i = 0; i < 256; i++)
        freq[i] = static_cast<float>(data[i]) / static_cast<float>(size);
    std::vector<int> new_data(256);
    float sum = 0;
    for(unsigned int i = 0; i < 256; i++)
        new_data[i] = (sum += freq[i]) * 255;
    return Histogram(this->channel, this->size, new_data);
}

/*static*/
QImage Histogram::equalize_image(const QImage & image) {
    std::vector<Histogram> histograms = Histogram::create_all(image);
    std::vector<std::vector<int>> equalized(histograms.size());
    for(unsigned int i = 0; i < histograms.size(); ++i)
        equalized[i] = histograms[i].equalize().get_data();
    QImage copy = QImage(image);
    bool is_gray = image.isGrayscale();
    bool has_alpha = image.hasAlphaChannel();
    for(int y = 0; y < image.height(); y++) {
        for(int x = 0; x < image.width(); x++) {
            QRgb pixel = image.pixel(x, y);
            copy.setPixel(
                x, y, qRgba(
                    equalized[is_gray   ? 0 : 0][qRed(pixel)],
                    equalized[is_gray   ? 0 : 1][qGreen(pixel)],
                    equalized[is_gray   ? 0 : 2][qBlue(pixel)],
                    equalized[has_alpha ? 3 : 0][qAlpha(pixel)]));
        }
    }
    return copy;
}

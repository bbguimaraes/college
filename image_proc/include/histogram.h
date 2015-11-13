#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>

#include <QString>
#include <QImage>

class Histogram {
    public:
        Histogram(const QImage & image, int channel);
        virtual ~Histogram() {}

        std::vector<int> get_data() const {return this->data;}
        QString toString() const;
        Histogram equalize() const;
        static QImage equalize_image(const QImage & image);
        static std::vector<Histogram> create_all(const QImage & image);

    private:
        typedef int (*channel_map_function)(QRgb);
        Histogram(int channel, int size, const std::vector<int> & data);
        static channel_map_function CHANNEL_MAP[4];
        void create(const QImage & image);
        int channel;
        unsigned int size;
        std::vector<int> data;
};

#endif // HISTOGRAM_H

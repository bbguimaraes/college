#ifndef FILTER_H
#define FILTER_H

#include <QImage>

class Filter {
    public:
        virtual ~Filter() {}
        virtual QImage apply(const QImage & image) = 0;
};

#endif // FILTER_H

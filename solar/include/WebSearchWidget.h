#ifndef WEB_SEARCH_WIDGET_H
#define WEB_SEARCH_WIDGET_H

#include <QWebView>

#include <string>

class WebSearchWidget : public QWebView {
    std::string m_engine;

    public:
        WebSearchWidget(QWidget * parent = nullptr);

        void set_engine(const std::string & engine) {this->m_engine = engine;}
        std::string engine() const {return this->m_engine;}

        void search(const std::string & str);
        std::string url_for(const std::string & str);
};

#endif // WEB_SEARCH_WIDGET_H

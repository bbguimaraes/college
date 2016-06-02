#include <WebSearchWidget.h>

#include <string>

WebSearchWidget::WebSearchWidget(QWidget * parent)
    : QWebView(parent) {}

void WebSearchWidget::search(const std::string & str) {
    this->setUrl(QUrl(QString::fromStdString(this->url_for(str))));
}

std::string WebSearchWidget::url_for(const std::string & str) {
    size_t pos = this->m_engine.find("$1");
    std::string url = this->m_engine;
    if(pos != std::string::npos)
        url.replace(pos, 2, str);
    else
        url.append(str);
    return url;
}

#include "Token.h"

std::ostream & operator<<(std::ostream & os, const Token & token) {
    return os <<
        "Token[" << &token << "]: " <<
        "text(" << token.text() << "), " <<
        "type(" << token.type() << ")";
}

#ifndef INLINEPARSER_H
#define INLINEPARSER_H

#include <QString>

class InlineParser
{
public:
    static QString process(const QString &text);
    static QString escapeHtml(const QString &text);
    static QString parseWikiLinks(const QString &text);
};

#endif // INLINEPARSER_H

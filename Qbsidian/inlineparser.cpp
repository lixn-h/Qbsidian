#include "inlineparser.h"
#include <QRegularExpression>
#include <QRegularExpression>
#include <QUrl>

QString InlineParser::process(const QString &text)
{
    QString html = text;
    html.replace("&", "&amp;");
    html.replace("<", "&lt;");
    html.replace(">", "&gt;");

    //图片
    QRegularExpression imgRe("!\\[(.*?)\\]\\((.*?)\\)");
    html.replace(imgRe, "<img src=\"\\2\" alt=\"\\1\" style=\"max-width:100%;\" />");
    // 超链接
    QRegularExpression linkRe("\\[(.*?)\\]\\((.*?)\\)");
    html.replace(linkRe, "<a href=\"\\2\">\\1</a>");
    QRegularExpression autolinkRe("<(https?://.+?)>");
    html.replace(autolinkRe, "<a href=\"\\1\">\\1</a>");
    //行内代码块
    QRegularExpression codeRe("`(.+?)`");
    html.replace(codeRe, "<code>\\1</code>");
    //粗体
    QRegularExpression boldRe("\\*\\*(.+?)\\*\\*",QRegularExpression::DotMatchesEverythingOption);
    html.replace(boldRe, "<strong>\\1</strong>");
    //斜体
    QRegularExpression italicRe("\\*(.+?)\\*",QRegularExpression::DotMatchesEverythingOption);
    html.replace(italicRe, "<em>\\1</em>");
    // 删除线
    QRegularExpression strikeRe("~~(.+?)~~", QRegularExpression::DotMatchesEverythingOption);
    html.replace(strikeRe, "<del>\\1</del>");


    return html;
}
QString InlineParser::escapeHtml(const QString &text)
{
    QString result = text;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    result.replace("\"", "&quot;");
    return result;
}


QString InlineParser::parseWikiLinks(const QString &text)
{
    QString result = text;
    QRegularExpression re("\\[\\[([^|\\]]+)(?:\\|([^\\]]+))?\\]\\]");

    QRegularExpressionMatchIterator i = re.globalMatch(result);
    QList<QRegularExpressionMatch> matches;
    while (i.hasNext())
    {
        matches.prepend(i.next());
    }

    for (const QRegularExpressionMatch &match : matches)
    {
        QString fullMatch = match.captured(0); // 完整的 [[...]]
        QString noteName = match.captured(1).trimmed(); // 笔记名
        QString alias = match.captured(2).trimmed();    // 别名

        // 如果没有别名，显示文字就是笔记名
        QString displayText = alias.isEmpty() ? noteName : alias;

        QString html = QString("<a href=\"internal://%1\">%2</a>").arg(QString(QUrl::toPercentEncoding(noteName)), displayText);
        result.replace(match.capturedStart(0), match.capturedLength(0), html);
    }

    return result;
}

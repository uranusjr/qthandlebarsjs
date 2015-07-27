#ifndef QTHANDLEBARSJS_HANDLEBARS_H
#define QTHANDLEBARSJS_HANDLEBARS_H

#include <QJSValue>
#include <QObject>
#include <QSharedPointer>
#include "qthandlebarsjs_global.h"
class QJsonObject;

namespace handlebars
{

typedef QJSValue (*HelperFunction)(QJSValue context, QJSValue options);


class HandlebarsPrivate;

class QTHANDLEBARSJS_SHARED_EXPORT Handlebars : public QObject
{
    Q_DECLARE_PRIVATE(Handlebars)
    const QSharedPointer<HandlebarsPrivate> d_ptr;

public:
    explicit Handlebars(QObject *parent = 0);

    void registerHelper(const QString &name, HelperFunction helper);
    void unregisterHelper(const QString &name);

    QString render(const QString &source, const QJsonObject &context);
    QString render(const QString &source, const QVariantHash &context);
};

}   // namespace handlebars

#endif // QTHANDLEBARSJS_HANDLEBARS_H

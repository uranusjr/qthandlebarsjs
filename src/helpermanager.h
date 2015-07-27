#ifndef QTHANDLEBARSJS_HELPERMANAGER_H
#define QTHANDLEBARSJS_HELPERMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include "handlebars.h"

namespace handlebars
{

class HelperManagerPrivate;

class HelperManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HelperManager)
    const QSharedPointer<HelperManagerPrivate> d_ptr;

public:
    explicit HelperManager(QObject *parent = 0);

    void addHelper(const QString &name, HelperFunction helper);
    void removeHelper(const QString &name);

public slots:
    QJSValue runHelper(const QString &name, QJSValue object, QJSValue options);
};

}   // namespace handlebars

#endif // QTHANDLEBARSJS_HELPERMANAGER_H

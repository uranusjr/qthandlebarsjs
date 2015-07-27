#include "helpermanager.h"

namespace handlebars
{

class HelperManagerPrivate
{
    Q_DECLARE_PUBLIC(HelperManager)
    HelperManager * const q_ptr;

public:
    HelperManagerPrivate(HelperManager *q);

    QHash<QString, HelperFunction> helpers;
};

HelperManagerPrivate::HelperManagerPrivate(HelperManager *q) : q_ptr(q)
{
}


HelperManager::HelperManager(QObject *parent) :
    QObject(parent), d_ptr(new HelperManagerPrivate(this))
{
}

void HelperManager::addHelper(const QString &name, HelperFunction helper)
{
    Q_D(HelperManager);
    if (d->helpers.contains(name))
    {
        qWarning("A helper with name \"%s\" already exists. It will be "
                 "overriden.", qPrintable(name));
    }
    d->helpers[name] = helper;
}

void HelperManager::removeHelper(const QString &name)
{
    Q_D(HelperManager);
    if (!d->helpers.contains(name))
    {
        qWarning("Helper \"%s\" does not exist. Not removing.",
                 qPrintable(name));
        return;
    }
    d->helpers.remove(name);
}

QJSValue HelperManager::runHelper(
        const QString &name, QJSValue object, QJSValue options)
{
    Q_D(HelperManager);
    return d->helpers[name](object, options);
}

}   // namespace handlebars

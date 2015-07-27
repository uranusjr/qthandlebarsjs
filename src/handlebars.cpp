#include <QBitArray>
#include <QDateTime>
#include <QFile>
#include <QJSEngine>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>
#include <QRegularExpression>
#include <QUrl>
#include <QUuid>
#include "handlebars.h"
#include "helpermanager.h"

namespace handlebars
{

static const char *HANDLEBARS_PATH = ":/qthandlebarsjs/handlebars-v3.0.3.js";


// TODO: This is not safe because QBitArray can very well overflow uint. Will
// need to do something... Maybe automatically convert to double when it
// happens?
inline uint uintfbits(const QBitArray &bits)
{
    uint val = 0;
    for (int i = 0; i < bits.size(); i++)
    {
        val <<= 1;
        val += bits.at(i);
    }
    return val;
}


class HandlebarsPrivate
{
    Q_DECLARE_PUBLIC(Handlebars)
    Handlebars * const q_ptr;

public:
    HandlebarsPrivate(Handlebars *q);

    QString render(const QString &source, const QJSValue &context);

    // Handlebars.js could not handle wrapped pointers, so we need to
    // serialize it going in. Sad.
    QJSValue buildValue(QObject *object);
    QJSValue buildValue(void *object, int userType);

    QJSValue buildValue(const QJsonDocument &document);
    QJSValue buildValue(const QJsonArray &array);
    QJSValue buildValue(const QJsonObject &object);
    QJSValue buildValue(const QJsonValue &value);

    QJSValue buildValue(const QVariantHash &hash);
    QJSValue buildValue(const QVariantList &list);
    QJSValue buildValue(const QVariantMap &map);
    QJSValue buildValue(const QVariant &variant);

    QJSEngine *engine;
    HelperManager *manager;
};

HandlebarsPrivate::HandlebarsPrivate(Handlebars *q) :
    q_ptr(q), engine(new QJSEngine(q)), manager(new HelperManager(q))
{
    QFile f(HANDLEBARS_PATH);
    bool ok = f.open(QFile::ReadOnly | QFile::Text);
    Q_ASSERT(ok);
    QJSValue retval = engine->evaluate(f.readAll(), HANDLEBARS_PATH);
    Q_ASSERT(!retval.isError());

    engine->globalObject().setProperty(
                "_manager", engine->newQObject(manager));
}

QString HandlebarsPrivate::render(
        const QString &source, const QJSValue &context)
{
    QJSValue g = engine->globalObject();
    g.setProperty("_templateSource", source);
    g.setProperty("_context", context);

    QJSValue value = engine->evaluate(
                "Handlebars.compile(_templateSource)(_context);");
    QString result = value.toString();
    return result;
}

QJSValue HandlebarsPrivate::buildValue(QObject *object)
{
    QJSValue value = engine->newObject();
    const QMetaObject *meta = object->metaObject();
    for (int i = 0; i < meta->propertyCount(); i++)
    {
        QMetaProperty property = meta->property(i);
        value.setProperty(property.name(), buildValue(property.read(object)));
    }
    return value;
}

QJSValue HandlebarsPrivate::buildValue(const QJsonDocument &document)
{
    if (document.isObject())
        return buildValue(document.object());
    else if (document.isArray())
        return buildValue(document.array());
    return QJSValue(QJSValue::NullValue);
}

QJSValue HandlebarsPrivate::buildValue(const QJsonArray &array)
{
    QJSValue val = engine->newArray(array.size());
    QJsonArray::const_iterator begin = array.begin();
    for (QJsonArray::const_iterator it = begin; it != array.end(); it++)
        val.setProperty(it - begin, buildValue(*it));
    return val;
}

QJSValue HandlebarsPrivate::buildValue(const QJsonObject &object)
{
    QJSValue val = engine->newObject();
    for (QJsonObject::const_iterator it = object.constBegin();
            it != object.constEnd(); it++)
        val.setProperty(it.key(), buildValue(it.value()));
    return val;
}

QJSValue HandlebarsPrivate::buildValue(const QJsonValue &value)
{
    QJSValue val(QJSValue::UndefinedValue);
    switch (value.type())
    {
    case QJsonValue::Null:
        val = QJSValue(QJSValue::NullValue);
        break;
    case QJsonValue::Bool:
        val = QJSValue(value.toBool());
        break;
    case QJsonValue::Double:
        val = QJSValue(value.toDouble());
        break;
    case QJsonValue::String:
        val = QJSValue(value.toString());
        break;
    case QJsonValue::Array:
        val = buildValue(value.toArray());
        break;
    case QJsonValue::Object:
        val = buildValue(value.toObject());
        break;
    case QJsonValue::Undefined:
        break;
    }
    return val;
}

QJSValue HandlebarsPrivate::buildValue(const QVariantHash &hash)
{
    QJSValue value = engine->newObject();
    for (QVariantHash::const_iterator it = hash.constBegin();
            it != hash.constEnd(); it++)
        value.setProperty(it.key(), buildValue(it.value()));
    return value;
}

QJSValue HandlebarsPrivate::buildValue(const QVariantList &list)
{
    QJSValue value = engine->newArray(list.size());
    QVariantList::const_iterator begin = list.constBegin();
    for (QVariantList::const_iterator it = begin; it != list.constEnd(); it++)
        value.setProperty(it - begin, buildValue(*it));
    return value;
}

QJSValue HandlebarsPrivate::buildValue(const QVariantMap &map)
{
    QJSValue value = engine->newObject();
    for (QVariantMap::const_iterator it = map.constBegin();
            it != map.constEnd(); it++)
        value.setProperty(it.key(), buildValue(it.value()));
    return value;
}

QJSValue HandlebarsPrivate::buildValue(const QVariant &variant)
{
    QJSValue value(QJSValue::UndefinedValue);

    int userType = variant.userType();
    switch (userType)
    {
    case QMetaType::QBitArray:
        value = uintfbits(variant.toBitArray());
    case QMetaType::Bool:
        value = variant.toBool();
        break;
    case QMetaType::QChar:
        value = QString(1, variant.toChar());
        break;
    case QMetaType::Double:
    case QMetaType::Float:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
    {
        bool ok = false;
        double x = variant.toDouble(&ok);
        if (ok)
            value = x;
        break;
    }
    case QMetaType::Int:
    {
        bool ok = false;
        int x = variant.toInt(&ok);
        if (ok)
            value = x;
        break;
    }
    case QMetaType::UInt:
    {
        bool ok = false;
        uint x = variant.toUInt(&ok);
        if (ok)
            value = x;
        break;
    }
    case QMetaType::QByteArray:
        value = QLatin1String(variant.toByteArray().constData());
        break;
    case QMetaType::QString:
        value = variant.toString();
        break;
    case QMetaType::QDate:
        value = engine->toScriptValue<QDate>(variant.toDate());
        break;
    case QMetaType::QDateTime:
        value = engine->toScriptValue<QDateTime>(variant.toDateTime());
        break;
    case QMetaType::QRegExp:
        value = engine->toScriptValue<QRegExp>(variant.toRegExp());
        break;
    case QMetaType::QUrl:
        value = variant.toUrl().toDisplayString();
        break;
    case QMetaType::QUuid:
        value = variant.toUuid().toString();
        break;
    case QMetaType::QObjectStar:
        value = buildValue(variant.value<QObject *>());
        break;

    // Newer stuff.
    case QMetaType::QRegularExpression:
        value = engine->toScriptValue<QRegularExpression>(
                    variant.toRegularExpression());
        break;

    // JSON compatibility.
    case QMetaType::QJsonArray:
        value = buildValue(variant.toJsonArray());
        break;
    case QMetaType::QJsonDocument:
        value = buildValue(variant.toJsonDocument());
        break;
    case QMetaType::QJsonObject:
        value = buildValue(variant.toJsonObject());
        break;
    case QMetaType::QJsonValue:
        value = buildValue(variant.toJsonValue());
        break;

    // Nested.
    case QMetaType::QVariantHash:
        value = buildValue(variant.toHash());
        break;
    case QMetaType::QVariantList:
    case QMetaType::QStringList:
        value = buildValue(variant.toList());
        break;
    case QMetaType::QVariantMap:
        value = buildValue(variant.toMap());
        break;

    default:
        if (userType >= QMetaType::User && variant.canConvert<QObject *>())
        {
            {
                value = buildValue(variant.value<QObject *>());
                break;
            }
        }
        // TODO: Better error message.
        qWarning("Could not convert value of type %d", userType);
        break;
    }
    return value;
}


Handlebars::Handlebars(QObject *parent) :
    QObject(parent), d_ptr(new HandlebarsPrivate(this))
{
}

void Handlebars::registerHelper(const QString &name, HelperFunction helper)
{
    static QString codeTemplate =
            "Handlebars.registerHelper('%1', function (context, options) {"
                "return _manager.runHelper('%2', context, options);"
            "});";

    Q_D(Handlebars);
    d->manager->addHelper(name, helper);
    d->engine->evaluate(codeTemplate.arg(name, name));
}

void Handlebars::unregisterHelper(const QString &name)
{
    static QString codeTemplate = "Handlebars.unregisterHelper('%1');";

    Q_D(Handlebars);
    d->manager->removeHelper(name);
    d->engine->evaluate(codeTemplate.arg(name));
}

QString Handlebars::render(const QString &source, const QJsonObject &context)
{
    Q_D(Handlebars);
    return d->render(source, d->buildValue(context));
}

QString Handlebars::render(const QString &source, const QVariantHash &context)
{
    Q_D(Handlebars);
    return d->render(source, d->buildValue(context));
}

}   // namespace handlebars


#include "customtest.h"

QString Person::name() const
{
    return "Mosky";
}


CustomTest::CustomTest(QObject *parent) : QObject(parent), hb(nullptr)
{
}

void CustomTest::init()
{
    hb = new handlebars::Handlebars();
}

void CustomTest::cleanup()
{
    hb->deleteLater();
    hb = 0;
}

void CustomTest::testQObjectStar()
{
    Person person;

    QString src = "{{ person.name }}";

    QVariantHash context;
    context["person"] = QVariant::fromValue<QObject *>(&person);

    QString result = hb->render(src, context);
    QCOMPARE(result, QString("Mosky"));
}

void CustomTest::testCustomQObjectSubclassStar()
{
    Person person;

    QString src = "{{ person.name }}";

    QVariantHash context;
    context["person"] = QVariant::fromValue(&person);

    QString result = hb->render(src, context);
    QCOMPARE(result, QString("Mosky"));
}

#ifndef CUSTOMTEST_H
#define CUSTOMTEST_H

#include <QtTest>
#include "handlebars.h"


class Person : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)

public:
    QString name() const;
};


class CustomTest : public QObject
{
    Q_OBJECT

public:
    explicit CustomTest(QObject *parent = 0);

private slots:
    void init();
    void cleanup();

    void testQObjectStar();
    void testCustomQObjectSubclassStar();

private:
    handlebars::Handlebars *hb;
};

#endif // CUSTOMTEST_H

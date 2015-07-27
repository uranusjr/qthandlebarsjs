#ifndef HOMEPAGETEST_H
#define HOMEPAGETEST_H

#include <QtTest>
#include "handlebars.h"

/**
 * @brief Tests based on examples from <http://handlebarsjs.com>.
 */
class HomepageTest : public QObject
{
    Q_OBJECT

public:
    HomepageTest();

private slots:
    void init();
    void cleanup();

    void testGettingStarted();
    void testHtmlEscaping();
    void testBlockExpressions();
    void testHandlebarsPaths();
    void testHandlebarsPathsParent();
    void testHandlebarsPathsConflict();
    void testHandlebarsComments();

private:
    handlebars::Handlebars *hb;
};



#endif  // HOMEPAGETEST_H


#include <QJSValue>
#include <QJsonArray>
#include <QJsonObject>
#include "homepagetest.h"


QJSValue listHelper(QJSValue context, QJSValue options)
{
    QString out = "<ul>";
    for (int i = 0; i < context.property("length").toInt(); i++)
    {
        QJSValueList args;
        args << context.property(i);
        out += "<li>" + options.property("fn").call(args).toString() + "</li>";
    }
    return out + "</ul>";
}


HomepageTest::HomepageTest(QObject *parent) : QObject(parent), hb(nullptr)
{
}

void HomepageTest::init()
{
    hb = new handlebars::Handlebars();
}

void HomepageTest::cleanup()
{
    hb->deleteLater();
    hb = 0;
}

void HomepageTest::testGettingStarted()
{
    QString src = "<div class=\"entry\"><h1>{{title}}</h1><div class=\"body\">"
                  "{{body}}</div></div>";

    QVariantHash context;
    context["title"] = "My New Post";
    context["body"] = "This is my first post!";

    QString result = hb->render(src, context);
    QCOMPARE(result, QString(
                 "<div class=\"entry\"><h1>My New Post</h1>"
                 "<div class=\"body\">This is my first post!</div></div>"));
}

void HomepageTest::testHtmlEscaping()
{
    QString src = "<div class=\"entry\"><h1>{{title}}</h1><div class=\"body\">"
                  "{{{body}}}</div></div>";

    QVariantHash context;
    context["title"] = "All About <p> Tags";
    context["body"] = "<p>This is a post about &lt;p&gt; tags</p>";

    QString result = hb->render(src, context);
    QCOMPARE(result, QString(
                 "<div class=\"entry\"><h1>All About &lt;p&gt; Tags</h1>"
                 "<div class=\"body\">"
                 "<p>This is a post about &lt;p&gt; tags</p></div></div>"));
}

void HomepageTest::testBlockExpressions()
{
    QString src = "{{#list people}}{{firstName}} {{lastName}}{{/list}}";
    hb->registerHelper("list", listHelper);

    QJsonObject context = {
        {"people", QJsonArray({
            QJsonObject({{"firstName", "Yehuda"}, {"lastName", "Katz"}}),
            QJsonObject({{"firstName", "Carl"}, {"lastName", "Lerche"}}),
            QJsonObject({{"firstName", "Alan"}, {"lastName", "Johnson"}}),
        })},
    };

    QString result = hb->render(src, context);
    QCOMPARE(result, QString(
                 "<ul><li>Yehuda Katz</li><li>Carl Lerche</li>"
                 "<li>Alan Johnson</li></ul>"));
}

void HomepageTest::testHandlebarsPaths()
{
    QString src = "<div class=\"entry\"><h1>{{title}}</h1>"
                  "<h2>By {{author.name}}</h2><div class=\"body\">"
                  "{{body}}</div></div>";
    QJsonObject context = {
        {"title", "My First Blog Post!"},
        {"author", QJsonObject({
             {"id", 47},
             {"name", "Yehuda Katz"},
        })},
        {"body", "My first post. Wheeeee!"},
    };

    QString result = hb->render(src, context);
    QCOMPARE(result, QString(
                 "<div class=\"entry\"><h1>My First Blog Post!</h1>"
                 "<h2>By Yehuda Katz</h2>"
                 "<div class=\"body\">My first post. Wheeeee!</div></div>"));
}

void HomepageTest::testHandlebarsPathsParent()
{
    QString src = "<h1>Comments</h1><div id=\"comments\">{{#each comments}}"
                  "<h2><a href=\"/posts/{{../permalink}}#{{id}}\">{{title}}"
                  "</a></h2><div>{{body}}</div>{{/each}}</div>";
    QJsonObject context = {
        {"comments", QJsonArray({
             QJsonObject({{{"id", 1}, {"title", "T1"}, {"body", "First."}}}),
             QJsonObject({{{"id", 2}, {"title", "T2"}, {"body", "Second."}}}),
        })},
        {"permalink", "this-post"},
    };

    QString result = hb->render(src, context);
    QCOMPARE(result, QString(
                 "<h1>Comments</h1><div id=\"comments\">"
                 "<h2><a href=\"/posts/this-post#1\">T1</a></h2>"
                 "<div>First.</div>"
                 "<h2><a href=\"/posts/this-post#2\">T2</a></h2>"
                 "<div>Second.</div></div>"));
}

void HomepageTest::testHandlebarsPathsConflict()
{
    QString src = "<p>{{name}}</p>"
                  "<p>{{./name}} or {{this/name}} or {{this.name}}</p>";
    hb->registerHelper("name", [](QJSValue, QJSValue) -> QJSValue {
        return "Not name";
    });
    QJsonObject context;
    context["name"] = "Name";

    QString result = hb->render(src, context);
    QCOMPARE(result, QString("<p>Not name</p><p>Name or Name or Name</p>"));
}

void HomepageTest::testHandlebarsComments()
{

}

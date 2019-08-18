#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QQuickWindow>

#include "phmodel.h"
#include "dinner.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    app.setApplicationName("N-Philosophers");
    app.setApplicationDisplayName("Philisopher's dinner");
    app.setApplicationVersion("1.0");

    QQmlApplicationEngine engine;


    Dinner      dinner;

    // Add starting group
    dinner.addItem("Alyona the Great");
    dinner.addItem("Sokrates");
    dinner.addItem("Piphagor");
    dinner.addItem("Oleksii Aleksieiev");
    dinner.addItem("Voltaire");

    dinner.startDinner();

    engine.rootContext()->setContextProperty(QStringLiteral("PHModel"), dinner.model());
    qmlRegisterUncreatableType<PHModel>("Philos", 1, 0, "PHModel",
                         QStringLiteral("PHModel"));


    QUrl url = QUrl(QStringLiteral("qrc:/main.qml"));
    // Must come before engine.load()
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                     [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

    engine.load(QUrl(url));

    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    // connect our QML signal to our C++ slot
    QObject::connect(window, SIGNAL(addPerson(QString)), &dinner, SLOT(addItem(const QString &)));
    QObject::connect(window, SIGNAL(delPerson(int)), &dinner, SLOT(delItem(int)));
    QObject::connect(window, SIGNAL(startDinner()), &dinner, SLOT(startDinner()));
    QObject::connect(window, SIGNAL(stopDinner()), &dinner, SLOT(stopDinner()));


    return app.exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>

#include "tictacgame.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    app.setApplicationName("TicTacToe");
    app.setApplicationDisplayName("Famous Game");
    app.setApplicationVersion("1.0");

    TicTacGame  Game;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("Ticmodel"), Game.model());
    qmlRegisterUncreatableType<TicModel>("Tictac", 1, 0, "Ticmodel",
                         QStringLiteral("Ticmodel cannot be created"));

    QUrl url = QUrl(QStringLiteral("qrc:/main.qml"));
    // Must come before engine.load()
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                     [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

    // Load QML object
    engine.load(QUrl(url));

    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    Game.set_qmlwindow( window );

    Game.startGame();

    return app.exec();
}

#include <QApplication>

#include <myclass.h>
#include <clientsmodel.h>
#include <messagemodel.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDir>
#include <QStandardPaths>
static void connectToDatabase()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
    }

    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDir.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));

    // Ensure that we have a writable location on all devices.
    const QString fileName = writeDir.absolutePath() + "/chat-database.sqlite3";

    // When using the SQLite driver, open() will create the SQLite database if it doesn't exist.
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
    }
    qDebug() << database.password() ;
}
int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    /* Register models */
    qmlRegisterType<ClientsModel>("ClientsModel", 1, 0, "ClientsModel");
    qmlRegisterType<MessageModel>("MessageModel", 1, 0, "MessageModel");

    /* Connect to db */
    connectToDatabase();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

        MyClass myClass;
        myClass.engine = &engine;

    QObject::connect(engine.rootObjects().first(), SIGNAL(sendCredentials(QString)),&myClass, SLOT(submit(QString)));
    qDebug() << engine.rootContext()->baseUrl();
    // QQuickView view(QUrl(QStringLiteral("qrc:/client.qml")));




    //qmlObject->setProperty("color", QColor("green"));
    //QQmlProperty property);

    return app.exec();
}

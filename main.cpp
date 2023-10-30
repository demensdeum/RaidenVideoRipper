#include "editorwindow.h"

#include <QLibraryInfo>
#include <QTranslator>
#include <QApplication>
#include <QDir>
#include "constants.h"

QTranslator* getTranslator(QString locale)
{
    auto translator = new QTranslator();
    auto filepath = QString(":/resources/translation/raidenvideoripper_%1.qm").arg(locale);
    auto result = translator->load(filepath);
    if (!result) {
        qDebug() << "Failed to load translation file";
    }
    return translator;
}

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setOrganizationName(companyName);
    application.setOrganizationDomain(companyDomain);
    application.setApplicationName(applicationName);
    application.setApplicationVersion(applicationVersion);
    auto localeIdentifier = QLocale::system().name();
    localeIdentifier.truncate(localeIdentifier.lastIndexOf('_'));
    auto translator = getTranslator(localeIdentifier);
    auto locale = QLocale(localeIdentifier);
    application.installTranslator(translator);
    QLocale::setDefault(locale);
    EditorWindow window;
    window.show();
    return application.exec();
}

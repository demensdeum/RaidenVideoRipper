#include "editorwindow.h"

#include <QApplication>
#include "constants.h"

int main(int argc, char *argv[])
{

    QApplication application(argc, argv);
    application.setOrganizationName(companyName);
    application.setOrganizationDomain(companyDomain);
    application.setApplicationName(applicationName);
    application.setApplicationVersion(applicationVersion);
    EditorWindow window;
    window.show();
    return application.exec();
}

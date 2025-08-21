#include <successwindow.h>
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QPushButton>
#include <QHBoxLayout>
#include <QProcess>

SuccessWindow::SuccessWindow(const QString &filePath, QWidget *parent) : QDialog(parent), filePath(filePath)
{
    setWindowTitle(tr("Success"));

    QTextBrowser *textBrowser = new QTextBrowser(this);
    textBrowser->setOpenExternalLinks(false);

    QString emoji = "✅";
    QString htmlMessage = QString("<p style='font-size: 72px; text-align: center;'>%1</p>"
                                  "<p style='text-align: center;'>%2</p>"
                                  "<p style='text-align: center;'><a href='https://demensdeum.com'>%3</a></p>")
                                .arg(
                                    emoji,
                                    tr("All files were successfully cut."),
                                    tr("Click here to open the location.")
                                );
    textBrowser->setHtml(htmlMessage);
    textBrowser->setAlignment(Qt::AlignCenter);

    connect(textBrowser, &QTextBrowser::anchorClicked, this, &SuccessWindow::onAnchorClicked);

    QPushButton *okButton = new QPushButton(tr("OK"), this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(textBrowser);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void SuccessWindow::onAnchorClicked(const QUrl &)
{
    QStringList args;
    args << "/select," << QDir::toNativeSeparators(filePath);
    QProcess::startDetached("explorer.exe", args);
    close();
}

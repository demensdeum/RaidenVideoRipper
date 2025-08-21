#include <successwindow.h>
#include <QDebug>
#include <QDesktopServices>
#include <QPushButton> // Include QPushButton header
#include <QHBoxLayout> // Include QHBoxLayout header

SuccessWindow::SuccessWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Success"));

    QTextBrowser *textBrowser = new QTextBrowser(this);
    textBrowser->setOpenExternalLinks(false);
    textBrowser->setHtml(tr("All files were successfully cut. <a href='file:///C:/'>Click here to open the location.</a>"));

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

void SuccessWindow::onAnchorClicked(const QUrl &link)
{
    QDesktopServices::openUrl(link);
    close();
}

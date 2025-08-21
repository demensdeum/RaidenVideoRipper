#pragma once

#include <QDialog>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QUrl>

class SuccessWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SuccessWindow(const QString &filePath, QWidget *parent = nullptr);

public slots:
    void onAnchorClicked(const QUrl &link);

private:
    const QString &filePath;
};

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QBitArray>
#include <QCryptographicHash>
#include <form.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool open();
    void begin();
    void commit();
    void rollback();
    bool exec(QSqlQuery &query);



private slots:
    void on_pushButton_clicked();

private:
    QString getdate();
    QString wk;
    void updatedate();
    //QString decryptToString(const QString& plaintext);

    Ui::MainWindow *ui;
    bool lg;
    void loadusr(QString nm, QString usr);
    QString nama;
    QString pas;
    QString lvl;
    QString type;
    QString id;

};
#endif // MAINWINDOW_H

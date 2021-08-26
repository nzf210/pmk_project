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

 QString nama_main;
 QString pass_main;

   // Q_SIGNAL void myTextChanged(const QString &);
   //QString myText() const { return nama_main ;}


public slots:

    QString getdate();
    //QStringList getListDatauser();




private slots:
    void on_pushButton_clicked();


private:

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
    QString namaL;
    QString id_type;
    QStringList listDatauser;



};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include <QObject>


bool openDB2(QSqlDatabase &db2)
{
    //=================== Baca Setting =============
    QString path("data/");
    QDir dir(path);
    if(!dir.exists()){ qInfo()<< "File Setting tdk tersedia"; }
    QFile file( path+"setting.txt");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readAll();
            QStringList list = line.split(":");
            QString hostName(list.at(0));
            QString port(list.at(1));
            QString nm(list.at(2));
            QString pass(list.at(3));
            int porto = port.toInt();
            file.close();
    // =================== Baca Setting =============
    // qInfo() << "oooooooooooooooo  Opening Database boollllllllllllllllllllllll" << line;
            db2.setHostName(hostName);
            db2.setPort(porto);
            db2.setDatabaseName("yhk_2021");
            db2.setUserName(nm); //Change the username
            //db2.setPassword("megarezst_yhk"); //Change the password
            db2.setPassword(pass); } //Change the password
    //db2.setConnectOptions("connect_timeout=8");
    bool ok = db2.open();
    if(ok)
    {
        qInfo() << "Opening Database bool main window" ;
        return true;
    }
    qInfo() << "Failed to open connection!";
    qInfo() << db2.lastError().text();return false;
 }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //if(this->isVisible()){QMessageBox::information(this,"Info...","Aplikasi Telah Terbuka"); return;}
    //if(this->isActiveWindow()){return;}

    QDateTime wkt = QDateTime::currentDateTime();
    QDate thn = QDate::currentDate();
    QString thn_ = thn.toString("yyyy");
    this->setWindowTitle("Sistem Infomasi Pengelolaan Keuangan Desa Kab. Yahukimo "+thn_+" ");
    this->setFixedSize(this->width(),this->height());
    QIcon logo(":/gbr/html/gbr/yhk.png");
    this->setWindowIcon(logo);

    QSqlDatabase db2 = QSqlDatabase::addDatabase("QPSQL");
    openDB2(db2);

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit->setPlaceholderText("username");
    ui->lineEdit_2->setPlaceholderText("password");
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit_2->setClearButtonEnabled(true);

    const QIcon usrIcon(":/gbr/html/gbr/icons8-user-male-50-2.png");
    ui->lineEdit->addAction(usrIcon, QLineEdit::LeadingPosition);
    const QIcon passwordIcon(":/gbr/html/gbr/icons8-lock-50-2.png");
    ui->lineEdit_2->addAction(passwordIcon, QLineEdit::LeadingPosition);


    //connect(ui->lineEdit_2->editingFinished(), &QLineEdit::returnPressed, this, MainWindow::on_pushButton_clicked());

    QDateTime wkt2 = QDateTime::fromString(getdate(),"yyyy-MM-dd");
    //QDateTime wkt3 = QDateTime::fromString(getdate(),"yyyy-MM-dd");
   QDateTime wkt3 = QDateTime::fromString("2022-10-23","yyyy-MM-dd"); // Dari waktu Set Manual

    QString scDate1_ = wkt.toString("yyyy");
    QString scDate1_1 = wkt.toString("MM");
    QString scDate1_2 = wkt.toString("dd");
    QDate scDate1a(scDate1_.toInt(),scDate1_1.toInt(), scDate1_2.toInt()); // Waktu Saat ini

    QString scDate2_ = wkt2.toString("yyyy");
    QString scDate2_1 = wkt2.toString("MM");
    QString scDate2_2 = wkt2.toString("dd");
    QDate scDate2a(scDate2_.toInt(),scDate2_1.toInt(), scDate2_2.toInt()); // Waktu Saat ini

    QString scDate3_ = wkt3.toString("yyyy");
    QString scDate3_1 = wkt3.toString("MM");
    QString scDate3_2 = wkt3.toString("dd");
    QDate scDate3a(scDate3_.toInt(),scDate3_1.toInt(), scDate3_2.toInt()); // Set Manual

    int days = scDate1a.daysTo(scDate2a);
    int days1 = scDate3a.daysTo(scDate1a);

    qInfo() << "int days =================" << days;
    qInfo() << "int day1 =================" << days1;

//  if(days<20&&days1<0){QMessageBox::information(this,"Info...","Mohon Perbaharui Aplikasi anda untuk tetap menggunakannya..."); }
//    if(){QMessageBox::information(this,"Info...","Mohon perbaharui Aplikasi anda..."); return;
//    }
   // if(days1>0){updatedate();}
    //MainWindow *a = new MainWindow;
   // this->close();

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getdate()
{
    //open();
    QSqlQuery query;
    QString cmd = "SELECT date FROM pmk_yhk.ver WHERE id=1 ";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Info....","Gagal query Waktu"); return "";}
    while (query.next() ) {
        wk = query.value(0).toString();
    }
    return wk;
}


void MainWindow::updatedate()
{
    //QDateTime wkt = QDateTime::currentDateTime();
    QDate wkt = QDate::currentDate();
    QString wkt_ = wkt.toString("yyyy-MM-dd");
    //qInfo() << "Waktu" << wkt_;
    QSqlQuery query;
    QString cmd = " UPDATE pmk_yhk.ver SET date = :date ";
    query.prepare(cmd);
    query.bindValue(":date",wkt_);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Info....","Gagal Mengupdate Waktu"); return;}
}

// =================================== DB Conection ====================================
bool MainWindow::open()
{
    QSqlDatabase db2 = QSqlDatabase::database();
    bool isOpen = db2.isOpen();
    if(!isOpen){QMessageBox::information(this,"Error...","Gagal Koneksi Kedatabase");}
    qInfo() << "isOpen: " << isOpen;
    return isOpen;
}

bool MainWindow::exec(QSqlQuery &query)
{
    QSqlDatabase db2 = QSqlDatabase::database();
    qInfo() << "db2 " << db2.isOpen();
    if(!db2.isOpen())

    {
        qInfo() << "The connection is not open! win";
        return false;
    }
    qInfo() << "Exec: " << query.executedQuery();
    bool ok =  query.exec();

    if(!ok)
    {
        qInfo() << db2.lastError().text();
        qInfo() << query.lastError().text();
    }
    return ok;
}



void MainWindow::begin()
{
    QSqlDatabase::database().transaction();
}

void MainWindow::commit()
{
    QSqlDatabase::database().commit();
}

void MainWindow::rollback()
{
    QSqlDatabase::database().rollback();
}
//================================ DB Conection ===============================

void MainWindow::on_pushButton_clicked()  // Button Login ...
{

    QString nama_i = ui->lineEdit->text();
    QString pass = ui->lineEdit_2->text();

    if(nama_i=="" && pass==""){QMessageBox::information(this,"Perhatian...","Nama atau Password tdk boleh kosong"); return;}
    QByteArray hash = QCryptographicHash::hash(nama_i.toLocal8Bit(),QCryptographicHash::Sha256).toHex();
    QByteArray hash2 = QCryptographicHash::hash(pass.toLocal8Bit(),QCryptographicHash::Sha256).toHex();
    QString pass_= QString::fromLocal8Bit(hash);
    QString nama_ = QString::fromLocal8Bit(hash2);

    loadusr(nama_i,pass_);

    if(nama_i==nama_i  && pass_==pas)
    {
        Form  *form = new Form();
        form->show();
        this->hide();
    } else {if(!lg) {return;}  QMessageBox::information(this, "Gagal Login" , "User atau Pass Salah...!!! ");}
}

void MainWindow::loadusr(QString nm, QString ps)
{
    QStringList list_data_coba;
    if(!open()){open();}
    QSqlQuery query;
    QString cmd ="SELECT nama,pass,level,type,id,jabatan FROM pmk_yhk.usr WHERE nama = :nm AND pass = :ps " ;
    query.prepare(cmd);
    query.bindValue(":nm",nm);
    query.bindValue(":ps",ps);
    lg= exec(query);
    if(!lg) { QMessageBox::information(this,"Error..."," Gagal Memuat Data User... "+query.lastError().text()+""); return; }
    while (query.next()) {
            nama = query.value(0).toString();
            pas = query.value(1).toString();
            lvl = query.value(2).toString();
            type = query.value(3).toString();
            id = query.value(4).toString();
            namaL = query.value(5).toString();
            list_data_coba <<nama<<pas<<lvl<<type<<id<<namaL;
                                  }
    listDatauser = list_data_coba;
    //qInfo() << "NIlai List Data User" << listDatauser;
    QString path("doc/temp/");
    QDir dir(path);
    QFile fOut(path+"lvl_type.txt");
    if(!fOut.exists()){fOut.remove(); }
    fOut.open(QFile::WriteOnly | QFile::Text);
    if(fOut.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"Gagal Menyimpan lvl_type.txt"; return;} else
    {
        QTextStream stream(&fOut);
        stream << lvl <<"/n"<<type << "/n" << id <<"/n" <<namaL;
        fOut.flush();
        fOut.close(); }

    //getListDatauser();
}

QStringList MainWindow::getListDatauser()
{
    QString xx = nama_main;
    QString cc =pass_main;
    //qDebug ()<< "Data User Main WIndow di getList User"<< nm <<"===" <<ps;
    QStringList list_data_coba;
    if(!open()){open();}
    QSqlQuery query;
    QString cmd ="SELECT nama,pass,level,type,id,jabatan FROM pmk_yhk.usr WHERE nama = :nm AND pass = :ps " ;
    query.prepare(cmd);
    query.bindValue(":nm",xx);
    query.bindValue(":ps",cc);
    lg= exec(query);
    if(!lg) { QMessageBox::information(this,"Error..."," Gagal Memuat Data User... "+query.lastError().text()+""); }
    while (query.next()) {
            nama = query.value(0).toString();
            pas = query.value(1).toString();
            lvl = query.value(2).toString();
            type = query.value(3).toString();
            id = query.value(4).toString();
            namaL = query.value(5).toString();
            list_data_coba <<nama<<pas<<lvl<<type<<id<<namaL;
                                  }
    return list_data_coba;
}











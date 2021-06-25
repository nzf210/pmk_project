#include "form.h"
#include "ui_form.h"
#include "QPdfWriter"
#include "pdf_dok.h"
#include "qrencode.h"
#include "pdf_dds.h"
#include "pdf_add.h"
#include "pdf_sppd_dds.h"
#include "pdf4.h"
#include "mainwindow.h"

// =======================================================================================
#include "QDebug"
#include <QFile>
#include <QDate>
#include <QSqlDatabase>
#include <QFile>
#include <QString>
#include <QDir>
#include <QSqlQuery>
#include <QTableWidget>
#include <QAbstractItemView>
#include <QClipboard>
#include <QDate>
#include <QDateEdit>
#include <QDateTime>
#include <QTime>
#include <QKeyEvent>
#include <QSignalMapper>
#include <QtCore>
#include <QPixmap>
#include <QAbstractButton>
#include <QRadioButton>
#include <QString>
#include <QPainter>
#include <QUrl>
// =======================================================================================


#define EXPORT_IMAGE_SIZE 256
static const QString SAVE_TO_PNG(QString("Save imagefile to\nQRCode.png"));

#ifdef Q_OS_WINDOWS
static const QString DLL_NAME("qrencode.dll");
QLibrary *Form::m_qrencodelib = Q_NULLPTR;

extern "C"
{
    typedef QRcode* (*Func_QRcode_encodeString)(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive);
}
#endif

QString satuan[] = { "", "satu ", "dua ", "tiga ", "empat ", "lima ", "enam ", "tuju ", "delapan ", "sembilan " };
QString sat[] = { "Nol", "satu ", "dua ", "tiga ", "empat ", "lima ", "enam ", "tuju ", "delapan ", "sembilan " };

QString konvertAng(int n) {
  QString st = QString::number('.',n);
  return st;
}

QString konvertAngka(qint64 n) {
    if (n < 0) {
        return "negatif " + konvertAngka(-n);
    } else if (n < 10) {
        return satuan[n];
    } else if (n == 10) { // khusus untuk sepuluh
        return "sepuluh ";
    } else if (n == 11) { // khusus untuk sebelas
        return "sebelas ";
    } else if (n < 20) {
        return satuan[n-10] + "belas ";
    } else if (n < 100) {
        return satuan[(n-(n%10))/10] + "puluh " + konvertAngka(n % 10);
    } else if (n < 1000) {
        return (n < 200 ? "seratus " : satuan[(n-(n%100))/100] + "ratus ") + konvertAngka(n % 100);
    } else if (n < 1000000) {
        return (n < 2000 ? "seribu " : konvertAngka((n-(n%1000))/1000) + "ribu ") + konvertAngka(n % 1000);
    } else if (n < 1000000000) {
        return konvertAngka((n-(n%1000000))/1000000) + "juta " + konvertAngka(n % 1000000);
    } else if (n < 1000000000000) {
        return konvertAngka((n-(n%1000000000))/1000000000) + "Miliar " + konvertAngka(n % 1000000000);
    }  else if (n < 1000000000000000) {
        return konvertAngka((n-(n%1000000000000))/1000000000000) + "Triliun" + konvertAngka(n % 1000000000000);}
    else if (n < 1000000000000000000) {
            return konvertAngka((n-(n%1000000000000000))/1000000000000000) + "Quad Billion" + konvertAngka(n % 1000000000000000);}
    else {
        return "Angka lebih besar dari 999.999.999.999,999,999 (harus kurang dari 1jt T)";
    }
}

bool openDB(QSqlDatabase &db2)
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
    //=================== Baca Setting =============
            db2.setHostName(hostName);
            db2.setPort(porto);
            db2.setDatabaseName("yhk_2021");
            db2.setUserName(nm); //Change the username
            //db2.setPassword("megarezst_yhk"); //Change the password
            db2.setPassword(pass); }//Change the password
    //db2.setConnectOptions("connect_timeout=8");
    bool ok = db2.open();
    if(ok)
    {
        return true;
    }
    qInfo() << db2.lastError().text();
    return false;
 }

Form::Form(QWidget *parent) :
    QWidget(parent),data("ssssss"),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QDateTime wkt = QDateTime::currentDateTime();
    QDate thn = QDate::currentDate();
    QString thn_ = thn.toString("yyyy");
    this->setWindowTitle("Sistem Infomasi Pengelolaan Keuangan Desa Kab. Yahukimo "+thn_+" ");

    QIcon logo(":/gbr/html/gbr/yhk.png");
    this->setWindowIcon(logo);

    QSqlDatabase db2 = QSqlDatabase::addDatabase("QPSQL");
    openDB(db2);
    qbx_id_dis = new QComboBox;
    qbx_id_kam = new QComboBox;
    qbx_jml_kk = new QComboBox;
 // Sppd ========================
    btnsppd = new QPushButton;
    btnsppd_2 = new QPushButton;
    btnsppd_dk = new QPushButton;
 // Sppd ========================

 //Muat Daftar Filter Bamuskam
    muatListFilter();
    muat_dis();
    muat_kampung();
    ui->label_realisasi->setVisible(false);
    ui->label_s_d->setVisible(false);
    ui->comboBox_realisasi->setVisible(false);
    ui->toolButton_pdf_main->setVisible(false);

    ui->dateEdit->setVisible(false);
    ui->dateEdit_2->setVisible(false);
//   ======================================= SIGNAL AND SLOT ==========================================
    conect_slot();
    ui->label_barCode->setVisible(false);
//  =======================================  SIGNAL AND SLOT ==========================================
header_wt1(); header_wt2(); header_wt9(); header_wt6(); header_wt11();

menu="1";
ui->label_4->setText(""); ui->label_5->setText(" Dana Desa"); ui->label_13->setText(""); ui->label_14->setText("Alokasi Dana Desa");

 // === Memua Level Type ===
modeTampilan_usr();
ui->label_space_1->setText("Selamat Datang : "+nama_l);
 // === Memuat Level Type ===


//Percobaan Signal and Slot

#ifdef Q_OS_WINDOWS
    Form::Initialize();
#endif
 // ============================
}

Form::~Form()
{    delete ui; }


QString decryptToString(const QString plaintext)
{    QString a = plaintext;
    return a;
}

void Form::onTabChanged(int tabIndex) {
    if (tabIndex == 0) {
    } else if (tabIndex == 1) {
     }
}

//QStringList Form::getSend_cmd2() const
//{
//    return send_cmd2;
//}

// === StringList dari list daftar nama ===S===
QStringList Form::getLspdf1() const
{    return  lspdf1; }

void Form::rundatapdf1()
{
    Form *a = new Form;
    a->datapdf1();
  //  qInfo() << " Running Dari run data pdf ";
}

QString Form::getPdfdt1() const
{
    return pdfdt1;
}
//ini Percobaan Send CMD
void Form::sendCMD()
{
    emit cmd();
}

// === StringList dari list daftar nama ===E===

#ifdef Q_OS_WINDOWS
void Form::Initialize()
{
    if (Q_NULLPTR == m_qrencodelib){
        m_qrencodelib = new QLibrary(DLL_NAME);
        bool loadResult = m_qrencodelib->load();

        if (true == loadResult){
            qDebug() << "QRCode_Generator:" << DLL_NAME << "load success.";
        }
        else{
            qDebug() << "QRCode_Generator:" << DLL_NAME << "load failure!!!";
        }
    }
}

void Form::Deinitialize()
{
    if (m_qrencodelib != Q_NULLPTR){
        m_qrencodelib->unload();
        delete m_qrencodelib;
    }
}
#endif

bool Form::GeneratePixmapFromText(QString &text, QPixmap &pixmap_, int width, int height)
{

    bool result = false;
    if ((false == text.isEmpty())
            && (width == height)
            && (width > 0)
            && (height > 0)){
        QRcode *qrcode;
#ifdef Q_OS_WINDOWS
        if (m_qrencodelib != Q_NULLPTR){
            Func_QRcode_encodeString QRcode_encodeStringFunc = (Func_QRcode_encodeString)m_qrencodelib->resolve("QRcode_encodeString");
            if (!QRcode_encodeStringFunc)
            {
                qDebug("Can not get QRcode_encodeString() function pointer");
                return false;
            }
            qrcode = QRcode_encodeStringFunc(text.toUtf8().constData(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);
        }
        else{
            qDebug("qrencode library pointer null !!! ");
            return false;
        }

#else
        qrcode = QRcode_encodeString(text.toUtf8().constData(), 0, QR_ECLEVEL_H, QR_MODE_8, 1);
#endif

        qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
        double scale_x = static_cast<double>(width) / static_cast<double>(qrcode_width);
        double scale_y = static_cast<double>(height) / static_cast<double>(qrcode_width);
        QImage mainimg=QImage(width,height,QImage::Format_ARGB32);
        QPainter painter(&mainimg);
        QColor background(Qt::white);
        painter.setBrush(background);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, width, height);
        QColor foreground(Qt::black);
        painter.setBrush(foreground);
        for( qint32 y = 0; y < qrcode_width; y ++)
        {
            for(qint32 x = 0; x < qrcode_width; x++)
            {
                unsigned char b = qrcode->data[y * qrcode_width + x];
                if(b & 0x01)
                {
                    QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                    painter.drawRects(&r, 1);
                }
            }
        }
        pixmap_ = QPixmap::fromImage(mainimg);
        if (false == pixmap_.isNull()){
            result = true;
        }
    }
    return result;
}

QString Form::jumlahcair()
{
    return tot_sppd;
}

bool Form::saveDocument(QString filePath)
{
    qInfo()<< filePath;
    return true;
}

void Form::on_saveFileButton_pressed(QString cd)
{

    on_generateButton_pressed(cd);
//    if ((ui->label_barCode->pixmap() != Q_NULLPTR)  && (ui->label_barCode->pixmap()->isNull() != true))
//    {

    QString filetypeStr;
    QString imageType = "PNG";
    if (imageType == QString("PNG")){
        filetypeStr = QString(".png");
    }
    QRCodePixmap = ui->label_barCode->pixmap();
    bool saveResult;
    QString savefileName = QString("doc/temp/gbr/QRCode") + filetypeStr;
    saveResult = QRCodePixmap->save(savefileName);

    if (true == saveResult){
        //QMessageBox::information(this, tr("QRCode Generator"), QString("Save to ") + savefileName + QString(" success"));
                                        } else{ QMessageBox::warning(this, tr("QRCode Generator"), QString("Save to ") + savefileName + QString(" failure"));}
   // } else{QMessageBox::warning(this, tr("QRCode Generator"), tr("Invalid QRCode Image"));}
}


void Form::on_generateButton_pressed( QString te)
{
    QString text = te;
    if (text.isEmpty()) {
       return;
    }

    int qrcode_width = 150 /*ui->label_barCode->width()*/;
    int qrcode_height = 150/*ui->label_barCode->height()*/;
    QPixmap qrcode_pixmap;
    bool result;
    result = Form::GeneratePixmapFromText(text, qrcode_pixmap, qrcode_width, qrcode_height);
    if (true == result){
        ui->label_barCode->setPixmap(qrcode_pixmap);
    }
    else{
        QMessageBox::warning(this, tr("QRCode Generator"), tr("Invalid QRCodeImage."));
        return;
    }
}

QString Form::muatreal()
{
   int j = li_realdds.count();
   qInfo() << j;
   QString a="d";
   return a;

}

void Form::on_comboBox_currentIndexChanged(int index)
{
    qInfo () << " Indexx Combo Box " << index;
    ui->comboBox->currentText();
}

void Form::muat_bm(QString skam) // Memuat data nama Bamuskam
{

    QSqlQuery query;
    QString cmd = "SELECT nama FROM pmk_yhk.m_bam WHERE id_kam= :id ORDER BY id_dis ASC";
    query.prepare(cmd);
    query.bindValue(":id",skam);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal memuat Daftar Bamuskam... "+query.lastError().text()+"..."); return;}
    while (query.next()) {
           QString nm=query.value(0).toString() ;
            li_bam << nm; }
}

void Form::databam(QString skam)
{
    muat_bm(skam);

    QString pt("doc/temp/");
    QFile fl(pt+"nmbam.txt");
    if(fl.exists()){ fl.remove();};
    if(fl.exists()){QMessageBox::information(this,"Error","Gagal Menghapus data bamuskam"); return;};
    //if(!fl.exists()){  }
    if(!fl.open(QIODevice::WriteOnly | QIODevice::Text)){QMessageBox::information(this,"Error","Gagal Memuat Menyimpan data bamuskam"); return; }

    for(int i=0; i<li_bam.count(); i++ )
    {
//  QString a = li_bam.at(i);
      QTextStream stream(&fl);
      QString a = ";";
       stream << li_bam.at(i) +a;
       //fl.flush();
    }
    fl.close();
}


void Form::muat_dis()
{
    id_dis.clear();
    Distrik.clear();

    Kampung.clear();
    id_dis<<"";
    Distrik<<"";
    Kampung<<"";


    QSqlQuery query;
    QString cmd = " SELECT * FROM pmk_yhk.v_dis ORDER BY id_distrik";
    query.prepare(cmd);
    qInfo() << "exec Querty 2" << exec(query);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal memuat data Distrik... "+query.lastError().text()+"..."); return;}
    while (query.next()) {
           QString id=query.value(1).toString() ; QString nm_dis=query.value(0).toString() ; /*QString nm_kam=query.value(2).toString() ;*/
            id_dis << id;
            Distrik << nm_dis;
 }
    ui->comboBox->addItems(Distrik);
    ui->comboBox->setEditable(true);
    qbx_id_dis->addItems(id_dis);
                QCompleter *completer = new QCompleter(Distrik, this);
                completer->setCaseSensitivity(Qt::  CaseInsensitive);
                completer->setFilterMode(Qt::MatchContains);
                ui->comboBox->setCompleter(completer);
}


void Form::muat_kampung()
{
    id_kam.clear();
    Kampung.clear();

    ui->comboBox_nmKampung->clear();
    qbx_id_kam->clear();
    qbx_jml_kk->clear();

    Kampung<<"";
    id_kam<<"";

    QSqlQuery query;
    QString cmd = " SELECT * FROM pmk_yhk.v_kam ORDER BY no";
    query.prepare(cmd);
    qInfo() << "exec Querty 2" << exec(query);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal memuat data Kampung... "+query.lastError().text()+"..."); return;}
    while (query.next()) {
           QString kam=query.value(0).toString() ; QString id=query.value(1).toString() ;
           id_kam<< id;
           Kampung << kam;

 }
    ui->comboBox_nmKampung->addItems(Kampung);
    ui->comboBox_nmKampung->setEditable(true);
    qbx_id_kam->addItems(id_kam);

                QCompleter *completer = new QCompleter(Kampung, this);
                completer->setCaseSensitivity(Qt::  CaseInsensitive);
                completer->setFilterMode(Qt::MatchContains);
                ui->comboBox_nmKampung->setCompleter(completer);
}

void Form::even_distrik_combo(QString &id_kam_s)
{
    qInfo()<< "Even Distrik COmbo ......................... <> "  ;
    if(id_kam_s==""){qInfo()<<"Nilai 0 pada sis combo";
                                 muat_kampung();
                                 return;}
    qInfo()<< "Even Distrik COmbo ......................... <Lanjut> " << id_kam_s ;
    Kampung.clear();
    ui->comboBox_nmKampung->clear();
    id_kam.clear();
    qbx_id_kam->clear();
    Kampung<<"";
    id_kam <<"";
    QSqlQuery query;
    QString cmd= " SELECT kampung,no FROM pmk_yhk.m_kampung WHERE id_distrik = :id ORDER BY no ";
    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);

    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data Kapmpung...!!!");}
    while (query.next()) {
            Kampung << query.value(0).toString();
            id_kam << query.value(1).toString();
    }
    ui->comboBox_nmKampung->addItems(Kampung);
    qbx_id_kam->addItems(id_kam);
    QCompleter *completer = new QCompleter(Kampung, this);
    completer->setCaseSensitivity(Qt::  CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->comboBox_nmKampung->setCompleter(completer);

    if(menu=="2"&&ui->comboBox_nmKampung->currentText()==""){
        while(ui->tableWidget_cetak_dds->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_cetak_dds->removeRow(0);}}

    if(menu=="3"&&ui->comboBox_nmKampung->currentText()==""){
        while(ui->tableWidget_cetak_add->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_cetak_add->removeRow(0);}}

}

//===================================DB Conection ====================================
bool Form::open()
{
    QSqlDatabase db2 = QSqlDatabase::database();
    bool isOpen = db2.isOpen();
    qInfo() << "isOpen: " << isOpen;
    return isOpen;
}

bool Form::exec(QSqlQuery &query)
{
    QSqlDatabase db2 = QSqlDatabase::database();
    qInfo() << "db2 " << db2.isOpen();
    if(!db2.isOpen())

    {
        qInfo() << "The connection is not open!";
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

void Form::begin()
{
    QSqlDatabase::database().transaction();
}

void Form::commit()
{
    QSqlDatabase::database().commit();
}

void Form::rollback()
{
    QSqlDatabase::database().rollback();
}

//================================ DB Conection ===============================

void Form::qbx_id_dis_conn() // Connect qbx ke ke even id_distrik
{
     int aa = ui->comboBox->currentIndex();
     qbx_id_dis->setCurrentIndex(aa);
     //qInfo() << " qbx_id_dis " << qbx_id_dis->currentText();
     QString id_kam_s = qbx_id_dis->currentText();
     even_distrik_combo(id_kam_s);
}

void Form::qbx_id_kam_conn() // Connect qbx ke ke even id_kampung
{
    int aa = ui->comboBox_nmKampung->currentIndex();
    qbx_id_kam->setCurrentIndex(aa);

   // qInfo() << " qbx_id_kam " << qbx_id_kam->currentText();
    QString id_kp = qbx_id_kam->currentText();
    if(menu=="2"){
        while(ui->tableWidget_cetak_dds->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_cetak_dds->removeRow(0);}
        muat_v_bam(id_kp);}

    if(menu=="3"){
        while(ui->tableWidget_cetak_add->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_cetak_add->removeRow(0);}
        muat_v_bam_2(id_kp);}

    if(id_kp==""){return;}
    if(menu=="1"){muat_bamuskam(id_kp);}  //Muat data kampung di menu 1

}










//Muat Tahap Pencairan

void Form::muatTahap(QString a)
{
    QSqlQuery query;
    QString cmd = "SELECT persen,laporan,thp,tahap FROM pmk_yhk.tahap_dds WHERE thp = :a  ";
    query.prepare(cmd);
    query.bindValue(":a", a);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error","Gagal Memuat Tahapan dds");return;}
    while (query.next()) {
           persen_2=query.value(0).toString() ; laporan=query.value(1).toString(); thp=query.value(2).toString(); tahap_=query.value(3).toString();
                                     }
}

void Form::muatTahap_2(QString a)
{
    QSqlQuery query;
    QString cmd = "SELECT persen,laporan,thp,tahap FROM pmk_yhk.tahap_add WHERE thp = :a  ";
    query.prepare(cmd);
    query.bindValue(":a", a);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error","Gagal Memuat Tahapan dds");return;}
    while (query.next()) {
           persen_2_2=query.value(0).toString() ; laporan_2=query.value(1).toString(); thp_2=query.value(2).toString(); tahap__2=query.value(3).toString();
                                     }
}

void Form::act()
{
//    disconnect(btn1, SIGNAL(pressed()), this, SLOT(click_btn1()));
//    disconnect(c, SIGNAL(pressed()), this, SLOT(active_eb_v()));

if(menu=="2"){ menu2="1";

    QString id_dis = qbx_id_dis->currentText();
    QString id_kam = qbx_id_kam->currentText();
    if(id_kam!="" && id_dis!=""){

         muat_rek(id_kam);
         muat_bend_kp(id_kam);
         muat_k_kp(id_kam);
         tahap();
         no_srt1();
         no_srt2();
         persen();
         sk_bup();
         //sk_menteri();
         kp_dns();

         QDate dt = QDate::currentDate();
         QDateEdit *de = new QDateEdit;
         de->setDate(dt);
         de->setDisplayFormat("dd-MM-yyyy");

         QString terbil = terbilang;
         QString no_rek = li_norek.at(0);
         QString nm_rek  = li_nm_rek.at(0);
         QString nm_bank = li_nm_bank.at(0);
         QString j_bend = s_j_bend;
         QString j_kp = s_j_kp;
         QString nm_kp = s_nm_kp;
         //nfo() << "Nama Kepala Kampung <<" << nm_kp;
         QString nm_bend = s_nm_bend;
         QString tahap = li_tahap.at(2);
         QString tgl_ter = de->text();
         QString no_srt1 =li_srt1.at(2);
         QString no_srt2 = li_srt2.at(3);
         QString persen = li_persen.at(2);
         QString sk_bup = li_sk_bup.at(0);
         QString sk_kam = s_sk_kp;
         QString nm_kp_dns = li_kp_dns.at(0);
         QString pg_kp_dns = li_kp_dns.at(1);
         QString nip_kp_dns = li_kp_dns.at(2);
//     QString sk_pmk = li_menteri.at(1);
//     QString sk_keu =li_menteri.at(0);
         QString j_kk = li_j_kk.at(0);
         QString thn2 = thp.right(4);

//     qInfo() <<ui->comboBox->currentText()<<"++"<<ui->comboBox_nmKampung->currentText()<<"++" <<id_dis<<"++" <<id_kam<<"++"<<no_rek<<"++" << nm_rek << "++" << nm_bank
//     <<"++"<< j_bend <<"++" << j_kp <<"++" << nm_kp << "++" << nm_bend <<"++" << tahap << "\n"
//     <<tgl_ter <<"++"<< no_srt1 <<"++"<< no_srt2<<"++" << persen<<"++" << sk_bup<<"++" << sk_kam<<"++" << nm_kp_dns<<"++" << pg_kp_dns <<"++"<< nip_kp_dns
//     <<"++ \n" << pg_kp_dns <<"++" << sk_pmk << "++" << sk_keu  << "++" << j_kk <<"++" << terbil;

         c =new QPushButton;
         c->setText("c");
         c->setMaximumWidth(22);

         qbx_thp_penc = new QComboBox;
         qbx_thp_penc->setEditable(true);
         qbx_thp_l = new QComboBox;
         qbx_thp_l->setEditable(true);
         qbx_thp_penc->addItems(li_tahap);
         qbx_thp_l->addItems(li_tahap);
         de_tgl_terima = new QDateEdit;
         de_tgl_terima->setDate(dt);
         de_tgl_terima->setDisplayFormat("dd-MM-yyyy");
         de_tgl_terima->setCalendarPopup(true);
         de_tgl_terima->setMaximumWidth(80);

         le_jml = new QLineEdit;
         le_jml->setValidator(new QDoubleValidator);

         le_jml->setClearButtonEnabled(true);
         le_jml->setText("Rp 0,00");
         le_jml->setMaximumWidth(190);
 //    le_jml->installEventFilter(eb_v);
         qbx_no_srt1 = new QComboBox;
         qbx_no_srt1->addItems(li_srt1);
         qbx_no_srt1->setEditable(true);
         qbx_no_srt2 = new QComboBox;
         qbx_no_srt2->addItems(li_srt2);
         qbx_no_srt2->setEditable(true);
         qbx_persen= new QComboBox;
         qbx_persen->addItems(li_persen);
         qbx_persen->setEditable(true);
//     qbx_sk_pmk= new QComboBox;
//     qbx_sk_pmk->addItems(li_menteri);
//     qbx_sk_keu= new QComboBox;
//     qbx_sk_keu->addItems(li_menteri);

         btn1= new QPushButton;
         btn1->setText("TAMBAH");
         btn1->setStyleSheet("QPushButton{font-weight: bold; }");
         btn1->setMaximumWidth(150);
         QLabel *ksg = new QLabel;
         ksg->setText("");
         ksg->setMaximumWidth(135);
         QLabel *thp= new QLabel; thp->setText("Tahap Pencairan :"); thp->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         thp->setMaximumWidth(150);
         QLabel *thp_l= new QLabel; thp_l->setText("Laporan Realisasi :"); thp_l->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         thp_l->setMaximumWidth(150);
         QLabel *tgl= new QLabel; tgl->setText("Tanggal :"); tgl->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         tgl->setMaximumWidth(150);
         QLabel *Jumlah= new QLabel; Jumlah->setText("Jumlah Pencairan :"); Jumlah->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         Jumlah->setMaximumWidth(150);
         QLabel *No_1= new QLabel; No_1->setText("NO SPPD :"); No_1->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         No_1->setMaximumWidth(150);
         QLabel *No_2= new QLabel; No_2->setText("NO SPPS :"); No_2->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         No_2->setMaximumWidth(150);
         QLabel *per= new QLabel; per->setText("Persentase Pencairan :"); per->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         per->setMaximumWidth(150);
         QLabel *sk1= new QLabel; sk1->setText("SK Menteri PMK :"); sk1->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         sk1->setMaximumWidth(150);
         QLabel *sk2= new QLabel; sk2->setText("SK Menteri Keu :"); sk2->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         sk2->setMaximumWidth(150);

         //Tambahan Nama Kepala Kampung dan Bendahara
         QLabel *nmKkam = new QLabel;nmKkam->setText("Kepala Kampung :"); nmKkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         nmKkam->setMaximumWidth(150);
         QLabel *nmBkam = new QLabel;nmBkam->setText("Bendahara :"); nmBkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         nmBkam->setMaximumWidth(150);
         QLabel *nmKkam_ = new QLabel; nmKkam_->setStyleSheet(" QLabel { font-weight: bold;}");
         QLabel *nmBkam_ = new QLabel; nmBkam_->setStyleSheet(" QLabel { font-weight: bold;}");
         nmKkam_->setText(nm_kp);
         nmBkam_->setText(nm_bend);

         eb_v = new QWidget;
         QIcon logo(":/gbr/html/gbr/yhk.png");
         eb_v->setWindowIcon(logo);
         eb_v->setWindowTitle("Realisasi Anggaran Desa");
         eb_v->setMinimumWidth(400);
         eb_v->setMaximumWidth(400);
         eb_v->setMaximumHeight(830);

         QVBoxLayout *VL = new QVBoxLayout(eb_v);
         QHBoxLayout *HL1 = new QHBoxLayout;
         QHBoxLayout *HL2 = new QHBoxLayout;
         QHBoxLayout *HL3 = new QHBoxLayout;
         QHBoxLayout *HL4 = new QHBoxLayout;
         QHBoxLayout *HL5 = new QHBoxLayout;
         QHBoxLayout *HL6 = new QHBoxLayout;
         QHBoxLayout *HL7 = new QHBoxLayout;
         QHBoxLayout *HL8 = new QHBoxLayout;
         QHBoxLayout *HL9 = new QHBoxLayout;
         QHBoxLayout *HL10 = new QHBoxLayout;

         VL->addLayout(HL1);
         VL->addLayout(HL2);
         VL->addLayout(HL3);
         VL->addLayout(HL4);
         VL->addLayout(HL5);
         VL->addLayout(HL6);
         VL->addLayout(HL7);
         VL->addLayout(HL8);
         VL->addLayout(HL9);
         VL->addLayout(HL10);

         HL1->addWidget(thp);
         HL1->addWidget(qbx_thp_penc);

         HL2->addWidget(tgl);
         HL2->addWidget(de_tgl_terima);
         HL2->addWidget(ksg);

         HL3->addWidget(Jumlah);
         HL3->addWidget(le_jml);
         HL3->addWidget(c);

         HL4->addWidget(No_1);
         HL4->addWidget(qbx_no_srt1);

         HL5->addWidget(No_2);
         HL5->addWidget(qbx_no_srt2);

         HL6->addWidget(per);
         HL6->addWidget(qbx_persen);

//     HL7->addWidget(sk1);
//     HL7->addWidget(qbx_sk_pmk);

//     HL8->addWidget(sk2);
//     HL8->addWidget(qbx_sk_keu);

         HL7->addWidget(thp_l);
         HL7->addWidget(qbx_thp_l);

         //Tambahan K Kampung dan B Kampung
         HL8->addWidget(nmKkam);
         HL8->addWidget(nmKkam_);
         HL9->addWidget(nmBkam);
         HL9->addWidget(nmBkam_);

         HL10->addWidget(btn1);
         eb_v->show();

        connect(btn1, SIGNAL(pressed()), this, SLOT(click_btn1()));
        connect(c, SIGNAL(pressed()), this, SLOT(active_eb_v()));

        //Tambahan untuk auto lap dan persentase
        connect(qbx_thp_penc, SIGNAL (currentIndexChanged(int)), this, SLOT(eventQbxadd()));
    } }
}


void Form::act_2()
{


if(menu=="3"){
     menu2="3";
    QString id_dis = qbx_id_dis->currentText();
    QString id_kam = qbx_id_kam->currentText();
    if(id_kam!="" && id_dis!=""){

         muat_rek(id_kam);
         muat_bend_kp(id_kam);
         muat_k_kp(id_kam);
         tahap_add();
         no_srt1();
         no_srt2();
         persen();
         sk_bup();
         //sk_menteri();
         kp_dns();
         QDate dt = QDate::currentDate();
         QDateEdit *de = new QDateEdit;
         de->setDate(dt);
         de->setDisplayFormat("dd-MM-yyyy");

         QString terbil = terbilang;
         QString no_rek = li_norek.at(0);
         QString nm_rek  = li_nm_rek.at(0);
         QString nm_bank = li_nm_bank.at(0);
         QString j_bend = s_j_bend;
         QString j_kp = s_j_kp;
         QString nm_kp = s_nm_kp;
         QString nm_bend = s_nm_bend;
         //QString tahap = li_tahap_add.at(0);
         QString tgl_ter = de->text();
         QString no_srt1 =li_srt1.at(2);
         QString no_srt2 = li_srt2.at(3);
         QString persen = li_persen.at(2);
         QString sk_bup = li_sk_bup.at(0);
         QString sk_kam = s_sk_kp;
         QString nm_kp_dns = li_kp_dns.at(0);
         QString pg_kp_dns = li_kp_dns.at(1);
         QString nip_kp_dns = li_kp_dns.at(2);
//     QString sk_pmk = li_menteri.at(1);
//     QString sk_keu =li_menteri.at(0);
         QString j_kk = li_j_kk.at(0);
         QString thn2 = thp_2.right(4);

         c =new QPushButton;
         c->setText("c");
         c->setMaximumWidth(22);
         qbx_thp_penc = new QComboBox;
         qbx_thp_l = new QComboBox;
         qbx_thp_penc->setEditable(true);
         qbx_thp_l->setEditable(true);
         qbx_thp_l->addItems(li_tahap_add);
         qbx_thp_penc->addItems(li_tahap_add);
         de_tgl_terima = new QDateEdit;
         de_tgl_terima->setDate(dt);
         de_tgl_terima->setDisplayFormat("dd-MM-yyyy");
         de_tgl_terima->setCalendarPopup(true);
         de_tgl_terima->setMaximumWidth(80);

         le_jml = new QLineEdit;
         le_jml->setValidator(new QDoubleValidator);
         le_jml->setClearButtonEnabled(true);
         le_jml->setText("Rp 0,00");
         le_jml->setMaximumWidth(190);
        // le_jml->installEventFilter(eb_v);

         qbx_no_srt1 = new QComboBox;
         qbx_no_srt1->addItems(li_srt1);
         qbx_no_srt1->setEditable(true);
         qbx_no_srt2 = new QComboBox;         
         qbx_no_srt2->addItems(li_srt2);
         qbx_no_srt2->setEditable(true);
         qbx_persen= new QComboBox;
         qbx_persen->addItems(li_persen);
         qbx_persen->setEditable(true);

         btn1= new QPushButton;
         btn1->setText("TAMBAH");
         btn1->setStyleSheet("QPushButton{font-weight: bold; }");
         btn1->setMaximumWidth(150);
         QLabel *ksg = new QLabel;
         ksg->setText("");
         ksg->setMaximumWidth(135);
         QLabel *thp= new QLabel; thp->setText("Tahap Pencairan :"); thp->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         thp->setMaximumWidth(150);   
         QLabel *thp_l= new QLabel; thp_l->setText("Laporan Realisasi :"); thp_l->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         thp_l->setMaximumWidth(150);
         QLabel *tgl= new QLabel; tgl->setText("Tanggal :"); tgl->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         tgl->setMaximumWidth(150);
         QLabel *Jumlah= new QLabel; Jumlah->setText("Jumlah Pencairan :"); Jumlah->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         Jumlah->setMaximumWidth(150);
         QLabel *No_1= new QLabel; No_1->setText("NO SPPD :"); No_1->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         No_1->setMaximumWidth(150);
         QLabel *No_2= new QLabel; No_2->setText("NO SPPS :"); No_2->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         No_2->setMaximumWidth(150);
         QLabel *per= new QLabel; per->setText("Persentase Pencairan :"); per->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         per->setMaximumWidth(150);
         QLabel *sk1= new QLabel; sk1->setText("SK Menteri PMK :"); sk1->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         sk1->setMaximumWidth(150);
         QLabel *sk2= new QLabel; sk2->setText("SK Menteri Keu :"); sk2->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         sk2->setMaximumWidth(150);

         //Tambahan Nama Kepala Kampung dan Bendahara
         QLabel *nmKkam = new QLabel;nmKkam->setText("Kepala Kampung :"); nmKkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         nmKkam->setMaximumWidth(150);
         QLabel *nmBkam = new QLabel;nmBkam->setText("Bendahara :"); nmBkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
         nmBkam->setMaximumWidth(150);
         QLabel *nmKkam_ = new QLabel; nmKkam_->setStyleSheet(" QLabel { font-weight: bold;}");
         QLabel *nmBkam_ = new QLabel; nmBkam_->setStyleSheet(" QLabel { font-weight: bold;}");
         nmKkam_->setText(nm_kp);
         nmBkam_->setText(nm_bend);

         eb_v = new QWidget;
         QIcon logo(":/gbr/html/gbr/yhk.png");
         eb_v->setWindowIcon(logo);
         eb_v->setWindowTitle("Realisasi Anggaran Desa");
         eb_v->setMinimumWidth(400);
         eb_v->setMaximumWidth(400);
         eb_v->setMaximumHeight(800);

         QVBoxLayout *VL = new QVBoxLayout(eb_v);
         QHBoxLayout *HL1 = new QHBoxLayout;
         QHBoxLayout *HL2 = new QHBoxLayout;
         QHBoxLayout *HL3 = new QHBoxLayout;
         QHBoxLayout *HL4 = new QHBoxLayout;
         QHBoxLayout *HL5 = new QHBoxLayout;
         QHBoxLayout *HL6 = new QHBoxLayout;
         QHBoxLayout *HL7 = new QHBoxLayout;
         QHBoxLayout *HL8 = new QHBoxLayout;
          QHBoxLayout *HL9 = new QHBoxLayout;
          QHBoxLayout *HL10 = new QHBoxLayout;

         VL->addLayout(HL1);
         VL->addLayout(HL2);
         VL->addLayout(HL3);
         VL->addLayout(HL4);
         VL->addLayout(HL5);
         VL->addLayout(HL6);
         VL->addLayout(HL7);
         VL->addLayout(HL8);
          VL->addLayout(HL9);
          VL->addLayout(HL10);

         HL1->addWidget(thp);
         HL1->addWidget(qbx_thp_penc);

         HL2->addWidget(tgl);
         HL2->addWidget(de_tgl_terima);
         HL2->addWidget(ksg);

         HL3->addWidget(Jumlah);
         HL3->addWidget(le_jml);
         HL3->addWidget(c);

         HL4->addWidget(No_1);
         HL4->addWidget(qbx_no_srt1);

         HL5->addWidget(No_2);
         HL5->addWidget(qbx_no_srt2);

         HL6->addWidget(per);
         HL6->addWidget(qbx_persen);

         HL7->addWidget(thp_l);
         HL7->addWidget(qbx_thp_l);

         //Tambahan K Kampung dan B Kampung
         HL8->addWidget(nmKkam);
         HL8->addWidget(nmKkam_);
         HL9->addWidget(nmBkam);
         HL9->addWidget(nmBkam_);

         HL10->addWidget(btn1);
         eb_v->show();

        connect(btn1, SIGNAL(pressed()), this, SLOT(click_btn2()));
        connect(c, SIGNAL(pressed()), this, SLOT(active_eb_v_2()));

        //Tambahan untuk auto lap dan persentase
        connect(qbx_thp_penc, SIGNAL (currentIndexChanged(int)), this, SLOT(eventQbxadd_2()));

    }
}}


void Form::active_eb_v()
{
if(menu=="2"){
QLocale indo = QLocale(QLocale::Indonesian,QLocale::Indonesia);
QString j = le_jml->text();
if(j==""){le_jml->setText("Rp 0,00");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong"); return;}
qint64 jj = j.toDouble();
qint64 a =jj;
QString rp =QString::number(a);
bilang(rp);
QString j_ = indo.toCurrencyString(jj,"Rp ");
if(j.at(0) !="R"){
le_jml->setText(j_);}
//nfo() << "Active eb v......,,,llllllllll" << terbilang;
}}

void Form::active_eb_v_2()
{
if(menu=="3"){
QLocale indo = QLocale(QLocale::Indonesian,QLocale::Indonesia);
QString j = le_jml->text();
if(j==""){le_jml->setText("Rp 0,00");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong"); return;}
qint64 jj = j.toDouble();
qint64 a =jj;
QString rp =QString::number(a);
bilang(rp);
QString j_ = indo.toCurrencyString(jj,"Rp ");
if(j.at(0) !="R"){
le_jml->setText(j_);}
}}

bool Form::eventFilter(QObject* object, QEvent* event)
  {
    if(object == eb_v && event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *k = static_cast<QMouseEvent *> (event);
      if( k->button() == Qt::LeftButton ) {
        //ebug() << "Left click";
      } else if ( k->button() == Qt::RightButton ) {
       //Debug() << "Right click";
      }
    }
    return false;
  }



void Form::klik(QKeyEvent *k) {
 qInfo() << "Tess Even Mouse clil";

if(k->key()== Qt::Key_U){qInfo () << "xxxxxxxxxxxxxxxxxxxxxxx";}

if(k->key()== Qt::Key_Tab)
   {
       qInfo() << "Tess Even Mouse clil...............";
      // k->accept();
   }

if(k->type() == QEvent::MouseButtonRelease )
     {
        qDebug() << "Got Mouse Event..........";
     }

if(k->key()== Qt::Key_Escape)
 {
     qInfo() << "Tess Even Mouse clil...............";
   }
}

void Form::keyvent(QEvent *e) {
 qInfo() << "Tess Even Mouse clil";

   if(e->type() == QEvent::MouseButtonPress)
   {
       qInfo() << "Tess Even Mouse clil...............";
       e->accept();
   }
 if(e->type() == QEvent::MouseButtonRelease )
     {
        qDebug() << "Got Mouse Event..........";
        e->accept();
     }
}


QString Form::nosurat() //================================= No Surat =================================
{
QString nsrt ="";
QSqlQuery query;
//QString cmd = "SELECT count(*) AS j1 ,( SELECT count(*) FROM pmk_yhk.t_real_2) AS j2 , (SELECT count(*) FROM pmk_yhk.sppdd) AS j3 , "
//              "(SELECT count(*) FROM pmk_yhk.sppdd_2) AS j4  FROM pmk_yhk.t_real" ;
QString cmd = "SELECT sum(nos) AS no1,(SELECT sum(nos) FROM pmk_yhk.sppdd_2) AS no2,(SELECT sum(nos) FROM pmk_yhk.t_real) AS no3,"
                           "(SELECT sum(nos) FROM pmk_yhk.t_real_2) AS no4 FROM pmk_yhk.sppdd";
query.prepare(cmd);
//int j1 =0; int j2 =0; int j3 =0; int j4 =0;
//QString j1_; QString j2_; QString j3_; QString j4_;
bool ok = exec(query);
if(!ok){QMessageBox::information(this,"Info","Gagal Memuat nomor surat 1");}
while(query.next())
{
    j1_ = query.value(0).toString(); j2_ = query.value(1).toString(); j3_ = query.value(2).toString(); j4_ = query.value(3).toString();
    qInfo()<<"s" <<query.value(0).toString() <<"==" << query.value(1).toString() << "=="<< query.value(2).toString() <<"==" <<  query.value(3).toString();
}
qInfo()<<"s" <<j1_ <<"==" <<j2_ << "=="<< j3_ <<"==" << j4_;
if(j1_==""){j1_="0";} if(j2_==""){j2_="0";} if(j3_==""){j3_="0";} if(j4_==""){j4_="0";}
int j1 =j1_.toInt();    int j2 =j2_.toInt(); int j3 =j3_.toInt();  int j4 = j4_.toInt();
int no = j1+j2+j3+j4+1;
QString a;
QString aa= QString::number(no);
qInfo()<<"scc" << no;
if(no==0){aa=""; a="0001";}
if(no>=1 && no<=9){ a="000"; }
if(no>=10 && no<=99){ a="00"; }
if(no>=100 && no<=999){ a="0"; }
if(no>=1000 ){a="";}
return a+aa;
}

QString Form::nosurat_2() // ================================= No Surat =================================
{
QString nsrt ="";
QSqlQuery query;
//QString cmd = "SELECT count(*) AS j1 ,( SELECT count(*) FROM pmk_yhk.t_real_2) AS j2 , (SELECT count(*) FROM pmk_yhk.sppdd) AS j3 , "
//              "(SELECT count(*) FROM pmk_yhk.sppdd_2) AS j4  FROM pmk_yhk.t_real" ;
QString cmd = "SELECT sum(nos) AS no1,(SELECT sum(nos) FROM pmk_yhk.sppdd_2) AS no2,(SELECT sum(nos) FROM pmk_yhk.t_real) AS no3,"
                           "(SELECT sum(nos) FROM pmk_yhk.t_real_2) AS no4 FROM pmk_yhk.sppdd";
query.prepare(cmd);
bool ok = exec(query);
if(!ok){QMessageBox::information(this,"Info","Gagal Memuat nomor surat 2");}
//int j1 =0; int j2 =0; int j3 =0; int j4 =0;
//QString j1_; QString j2_; QString j3_; QString j4_;
while(query.next())
{
    j1_ = query.value(0).toString(); j2_ = query.value(1).toString(); j3_ = query.value(2).toString(); j4_ = query.value(3).toString();
}
qInfo()<<"s" <<j1_ <<"==" <<j2_ << "=="<< j3_ <<"==" << j4_;
if(j1_==""){j1_="0";} if(j2_==""){j2_="0";} if(j3_==""){j3_="0";} if(j4_==""){j4_="0";}
int j1 =j1_.toInt();    int j2 =j2_.toInt(); int j3 =j3_.toInt();  int j4 = j4_.toInt();

int no = j1+j2+j3+j4+2;
qInfo()<<"sxx" << no << j4_;
QString a;
QString aa= QString::number(no);
if(no==0) {aa=""; a="0002";}
if(no>=1 && no<=9){ a="000"; }
if(no>=10 && no<=99){ a="00"; }
if(no>=100 && no<=999){ a="0"; }
if(no>=1000){a="";}
return a+aa;
}

void Form::setTot_sppd(const QString &value)
{
    tot_sppd_2 = value;
}

QString Form::getTot_sppd() const
{
    qInfo()<<" Jalan Kah tdj get toot " << tot_sppd_2;
    return tot_sppd_2;
}


void Form::muat_sppd() // muat No Sppd dds
{
    li_sppd.clear();
    qInfo() << "Baca File nosppd" ;
    QString path("data/");
    QFile f_tahap(path+"nosppd_dds.txt");
    if(!f_tahap.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat nosppd_dds..."); return;}
     f_tahap.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_tahap);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_sppd = line.split("\n");
    }
    f_tahap.close();
}
void Form::muat_sppd_2() // muat No Sppd add
{
    li_sppd_2.clear();
    qInfo() << "Baca File nosspd add" ;
    QString path("data/");
    QFile f_tahap(path+"halsppd_dds.txt");
    if(!f_tahap.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat perihal sppd dds..."); return;}
     f_tahap.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_tahap);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_sppd_2 = line.split("\n");
    }
    f_tahap.close();
}

void Form::muat_sppd_() // muat No Sppd dds
{
    li_sppd_.clear();
    qInfo() << "Baca File nosppd add" ;
    QString path("data/");
    QFile f_tahap(path+"nosppd_add.txt");
    if(!f_tahap.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat nosppd add..."); return;}
     f_tahap.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_tahap);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_sppd_ = line.split("\n");
    }
    f_tahap.close();
}

void Form::muat_sppd_2_() // muat No Sppd add
{
    li_sppd_2_.clear();
    qInfo() << "Baca File perihak add" ;
    QString path("data/");
    QFile f_tahap(path+"halsppd_add.txt");
    if(!f_tahap.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat perihal add..."); return;}
     f_tahap.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_tahap);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_sppd_2_ = line.split("\n");
    }
    f_tahap.close();
}


void Form::tahap()    { // Memuat daftar tahap penerimaan
    li_tahap.clear();
    qInfo() << "Baca File daftar Terima" ;
    QString path("data/");
    QFile f_tahap(path+"tahap_pencairan.txt");
    if(!f_tahap.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat Tahap Pencairan..."); return;}
     f_tahap.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_tahap);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_tahap = line.split("\n");
    }
    f_tahap.close();
}

void Form::no_srt1()    {
    li_srt1.clear();
    qInfo() << "Baca File no SRT 1" ;
     QString path("data/");
    QFile f_srt1(path+"no_srt_1.txt");
    if(!f_srt1.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat no Surat 1."); return;}
     f_srt1.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_srt1);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_srt1 = line.split("\n");
    }
    f_srt1.close();
}

void Form::no_srt2()    {
    li_srt2.clear();
    qInfo() << "Baca File no SRT 2" ;
     QString path("data/");
    QFile f_srt2(path+"no_srt_2.txt");
    if(!f_srt2.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat no Surat 2");return;}
     f_srt2.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_srt2);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_srt2 = line.split("\n");
    }
    f_srt2.close();
}

void Form::tahap_add()    {
    li_tahap_add.clear();
    qInfo() << "Baca tahap add" ;
     QString path("data/");
    QFile f_tahap_add(path+"tahap_pencairan_add.txt");
    if(!f_tahap_add.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat tahap pencairan add"); return;}
     f_tahap_add.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_tahap_add);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "tahap Add" << line;
        li_tahap_add = line.split("\n");
    }
    f_tahap_add.close();
}

void Form::no_srt4()    {
    li_srt4.clear();
    qInfo() << "Baca File no SRT 4" ;
     QString path("data/");
    QFile f_srt4(path+"no_srt_4.txt");
    if(!f_srt4.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat no Surat 2");return;}
     f_srt4.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_srt4);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_srt4 = line.split("\n");
    }
    f_srt4.close();
}

void Form::persen()    {
    li_persen.clear();
    qInfo() << "Baca File Persentase" ;
     QString path("data/");
    QFile f_persen(path+"persen.txt");
    if(!f_persen.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat Persentase Tahapan.");return;}
     f_persen.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_persen);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_persen = line.split("\n");
    }
    f_persen.close();
}

void Form::sk_bup()    {
    li_sk_bup.clear();
    qInfo() << "Baca File sk_bup" ;
     QString path("data/");
    QFile f_sk_bup(path+"sk_bup.txt");
    if(!f_sk_bup.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat SK Bupati...");return;}
     f_sk_bup.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_sk_bup);
     while (!str.atEnd()) {
        QString line = str.readAll();
        qInfo()<< "lInessss" << line;
        li_sk_bup = line.split("\n");
    }
    f_sk_bup.close();
}

void Form::sk_kampung()    {
    li_sk_kampung.clear();
    qInfo() << "Baca File sk kampung" ;
     QString path("data/");
    QFile f_sk_kampung(path+"sk_kampung.txt");
    if(!f_sk_kampung.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat SK Kampung.");return;}
     f_sk_kampung.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_sk_kampung);
     while (!str.atEnd()) {
        QString line = str.readAll();
        li_sk_kampung = line.split("\n");
    }
    f_sk_kampung.close();
}

void Form::sk_menteri()    {
    li_menteri.clear();
    qInfo() << "Baca File sk Menteri" ;
     QString path("data/");
    QFile f_menteri(path+"sk_menteri.txt");
    if(!f_menteri.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat SK  Menteri...");return;}
     f_menteri.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_menteri);
     while (!str.atEnd()) {
        QString line = str.readAll();
        li_menteri = line.split("\n");
    }
    f_menteri.close();
}

void Form::kp_dns() {
    li_kp_dns.clear();
    qInfo() << "Baca File kp dns" ;
     QString path("data/");
    QFile f_kp_dns(path+"kepala_dinas.txt");
    if(!f_kp_dns.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat Nama Kepala Dinas PMK YHK.");return;}
     f_kp_dns.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_kp_dns);
     while (!str.atEnd()) {
        QString line = str.readAll();
        li_kp_dns = line.split("\n");
    }
    f_kp_dns.close();
    //qInfo() << "Baca File kp dns 22222222" ;
}

// === Memua Level dan Type User ===
void Form::muat_lvl_type() {
    li_lvl_type.clear();
    QString path("doc/temp/");
    QDir dir(path);
    QFile lvl_type(path+"lvl_type.txt");
    if(!lvl_type.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat Nama Kepala Dinas PMK YHK.");return;}
     lvl_type.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&lvl_type);
     while (!str.atEnd()) {
        QString line = str.readAll();
        li_lvl_type = line.split("/n");
        lvl = li_lvl_type.at(0);
        type = li_lvl_type.at(1);
        id_usr = li_lvl_type.at(2);
        nama_l = li_lvl_type.at(3);
    }
    lvl_type.close();
}


void   Form::muat_bend_kp(QString &id_kam_s)
{
    if(open()==false){open();}

    QSqlQuery query;
    QString cmd = " SELECT nama, jabatan,no_sk FROM pmk_yhk.v_bam_ WHERE id_kam = :id AND id_j = 3" ;
    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal memuat data Bendahara Kampung... "+query.lastError().text()+"..."); return;}
    while (query.next()) {
           QString nama=query.value(0).toString() ; QString jab=query.value(1).toString() ; QString sk=query.value(2).toString() ;
            s_nm_bend = nama;
            s_j_bend = jab;
            s_sk_bend = sk;
}}

void   Form::muat_k_kp(QString &id_kam_s)
{
    if(open()==false){open();}
    QSqlQuery query;
    QString cmd = " SELECT nama, jabatan,no_sk FROM pmk_yhk.v_bam_ WHERE id_kam = :id AND id_j = 1" ;
    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal memuat data Kepala Kampung... "+query.lastError().text()+"..."); return;}
    while (query.next()) {
           QString nama=query.value(0).toString() ; QString jab=query.value(1).toString() ; QString sk=query.value(2).toString() ;
            s_j_kp = jab;
            s_nm_kp = nama;
            s_sk_kp = sk;
}}

void   Form::muat_rek(QString &id_kam_s)
{

    if(open()==false){open();}
    li_norek.clear();
    li_kas_kam.clear();
    li_nm_rek.clear();
    li_nm_bank.clear();
    li_j_kk.clear();

    QSqlQuery query;
    QString cmd = " SELECT * FROM pmk_yhk.m_rek WHERE id = :id" ;
    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal memuat data Rekening Kampung... "+query.lastError().text()+"..."); return;}
    while (query.next()) {
           QString no_rek=query.value(1).toString() ; QString kas=query.value(2).toString() ; QString nm_rek=query.value(3).toString() ;
           QString nm_bank=query.value(4).toString() ; QString j_kk=query.value(5).toString() ;
            li_norek << no_rek;
            li_kas_kam << kas;
            li_nm_rek << nm_rek;
            li_nm_bank << nm_bank;
            li_j_kk << j_kk;
 }
}


void Form::bilang(QString nilai){

qint64 ad = nilai.toDouble();
//qInfo() << "konvertAngka(ad)" << nilai <<"ad" /* add*/;
//qInfo() << konvertAngka(ad);
terbilang = konvertAngka(ad);

}

void Form::bil(QString nilai){

    int ad = nilai.toInt();
    if(nilai=="00") { ad=0; }
    if(nilai=="01")
   qInfo() << "konvertAngka(ad)" << nilai <<"ad" /* add*/;
   qInfo() << konvertAng(ad);
}








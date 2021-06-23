#include "form.h"
#include "ui_form.h"
#include "QPdfWriter"
#include "pdf_dok.h"
#include "qrencode.h"
#include "pdf.h"
#include "pdf2.h"
#include "pdf3.h"
#include "pdf4.h"
#include "mainwindow.h"

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
        return konvertAngka((n-(n%1000000000000))/1000000000000) + "Triliun " + konvertAngka(n % 1000000000000);}
    else if (n < 1000000000000000000) {
            return konvertAngka((n-(n%1000000000000000))/1000000000000000) + "Quad Billion " + konvertAngka(n % 1000000000000000);}
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
            db2.setDatabaseName("megarezst");
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
//   Widget *a = new Widget;
//   a->hide();
    ui->label_realisasi->setVisible(false);
    ui->label_s_d->setVisible(false);
    ui->comboBox_realisasi->setVisible(false);
    ui->toolButton_pdf_main->setVisible(false);

    ui->dateEdit->setVisible(false);
    ui->dateEdit_2->setVisible(false);
//   ======================================= SIGNAL AND SLOT ==========================================
 connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(qbx_id_dis_conn())); /*qbx_id_kam_conn()*/
 connect(ui->comboBox_nmKampung, SIGNAL (currentIndexChanged(int)), this, SLOT(qbx_id_kam_conn()));
 connect(ui->comboBox_realisasi, SIGNAL (currentIndexChanged(int)), this, SLOT(qbx3event()));

 connect(ui->tableWidget_Bamuskam, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw);
 connect(ui->tableWidget_Bamuskam, &QTableWidget::cellClicked , this, &Form::event_klik_tw);
 connect(ui->tableWidget_6, &QTableWidget::cellClicked , this, &Form::event_klik_tw_6);
 connect(ui->tableWidget_11, &QTableWidget::cellClicked , this, &Form::event_klik_tw_11);
 connect(ui->tableWidget_13, &QTableWidget::cellClicked , this, &Form::event_klik_tw_13);
 connect(ui->tableWidget_2, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_2);
 connect(ui->tableWidget_9, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_9);
 //connect(ui->tableWidget_13, &QTableWidget::doubleClicked , this, &Form::headsppd_2);
 connect(ui->tableWidget_13, &QTableWidget::doubleClicked , this, &Form::even_dklik_tw13);
 //connect(ui->tableWidget_13, &QTableWidget::cellClicked , this, &Form::even_klik_tw13);
 connect(this, SIGNAL( currentChanged(int)), this, SLOT(onTabChanged(int)));

//  =======================================  SIGNAL AND SLOT ==========================================
header_wt1();
header_wt2();
header_wt9();
header_wt6();
header_wt11();

menu="1";

 ui->label_4->setText(""); ui->label_5->setText(" Dana Desa"); ui->label_13->setText(""); ui->label_14->setText("Alokasi Dana Desa");

 ui->toolButton_3->setVisible(false);
 ui->toolButton_5->setVisible(false);
 ui->toolButton_9->setVisible(false);
 ui->toolButton_11->setVisible(false);

 // === Memua Level Type ===
 muat_lvl_type();

// qInfo() << "Type data pada lvl: " << lvl;
// qInfo() << "Type data pada Type: " << type;



 if(lvl=="su" && type=="su")
 {
     updateTampilan(SuperMode);
 }
    else if( lvl=="admin_1" && type=="dds" )
                {updateTampilan(AdminMode_1);}
                else if(lvl=="admin_2" && type=="add")
                              {updateTampilan(AdminMode_2);}
                             else if(lvl=="usr" && type=="dds")
                                           {updateTampilan(UserMode_1);}
                                             else if(lvl=="usr" && type=="add")
                                                           {updateTampilan(UserMode_2);}

 // === Memuat Level Type ===

#ifdef Q_OS_WINDOWS
    Form::Initialize();
#endif
 // ============================
}

Form::~Form()
{
    delete ui;
}


QString decryptToString(const QString plaintext)
{
    QString a = plaintext;
    return a;
}

void Form::onTabChanged(int tabIndex) {
    if (tabIndex == 0) {
    } else if (tabIndex == 1) {
     }
}

// === StringList dari list daftar nama ===S===
QStringList Form::getLspdf1() const
{
    return  lspdf1;
}

void Form::rundatapdf1()
{
    Form *a = new Form;
    a->datapdf1();
    qInfo() << " Running Dari run data pdf ";
}

QString Form::getPdfdt1() const
{
    return pdfdt1;
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

bool Form::GeneratePixmapFromText(QString &text, QPixmap &pixmap, int width, int height)
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
        pixmap = QPixmap::fromImage(mainimg);
        if (false == pixmap.isNull()){
            result = true;
        }
    }
    return result;
}

QString Form::jumlahcair()
{
    return tot_sppd;
}


void Form::on_toolButton_12_clicked()
{
    int noBrs = ui->tableWidget_9->currentRow();
    if(noBrs<0){QMessageBox::information(this,"Info...!!!","Pilih Realisasi yang ingin di cetak...");return;}
    if(noBrs>=0){
        datapdf2();
        QString id = ui->tableWidget_9->item(noBrs,2)->text();
        databam(id);

        Widget2 *a = new Widget2;
        QIcon logo(":/gbr/html/gbr/yhk.png");
        a->setWindowIcon(logo);
        a->show();
    }
}

void Form::datapdf1()
{
    QStringList lspdf;
    int noBrs = ui->tableWidget_2->currentRow();
    if(noBrs!=-1 && menu=="2"){
    QString id_kam =  ui->tableWidget_2->item(noBrs,2)->text();
    QString nmdis = ui->tableWidget_2->item(noBrs,3)->text();
    QString nmkamp = ui->tableWidget_2->item(noBrs,4)->text();
    QString terbilang= ui->tableWidget_2->item(noBrs,13)->text();
    QString norek = ui->tableWidget_2->item(noBrs,5)->text();
    QString nmrek = ui->tableWidget_2->item(noBrs,6)->text();
    QString nmbank = ui->tableWidget_2->item(noBrs,7)->text();
    QString nmkkp = ui->tableWidget_2->item(noBrs,9)->text();
    QString nmbenk = ui->tableWidget_2->item(noBrs,10)->text();
    QString jkk = ui->tableWidget_2->item(noBrs,28)->text();
    QString tahap = ui->tableWidget_2->item(noBrs,11)->text();
    QString jml = ui->tableWidget_2->item(noBrs,12)->text();
    QString persen = ui->tableWidget_2->item(noBrs,17)->text();
    QString nmkpdns = ui->tableWidget_2->item(noBrs,22)->text();
    QString tahap2 = ui->tableWidget_2->item(noBrs,26)->text();
    QString pkpldns = ui->tableWidget_2->item(noBrs,24)->text();
    QString nip = ui->tableWidget_2->item(noBrs,25)->text();
    QString nosrt1 = ui->tableWidget_2->item(noBrs,15)->text();
    QString nosrt2 = ui->tableWidget_2->item(noBrs,16)->text();
    QString tgl = ui->tableWidget_2->item(noBrs,14)->text();

    QString unmdis = nmdis.toUpper();
    QString unmkam = nmkamp.toUpper();\

    QString skben= ui->tableWidget_2->item(noBrs,8)->text();
    QString skkkam= ui->tableWidget_2->item(noBrs,19)->text();
    //Tambahan thn 2
    QString thn2 = ui->tableWidget_2->item(noBrs,23)->text();

    lspdf << nmdis << nmkamp << terbilang << norek << nmrek << nmbank <<nmkkp << nmbenk << jkk << tahap << jml << persen << nmkpdns << tahap2 << pkpldns << nip << nosrt1 << nosrt2 << tgl << unmdis << unmkam;
    lspdf1 =lspdf;
    qInfo() << "Data List Dari PDF 1 at 2" << getLspdf1();


    pdfdt1 = nmdis +"/n "+ nmkamp+ "/n" +terbilang +"/n"+norek+"/n"+ nmrek +"/n"+nmbank+"/n"+nmkkp+"/n"+nmbenk+"/n"
                     ""+jkk+"/n"+ tahap+ "/n"+ jml+"/n"+ persen+ "/n" +nmkpdns+ "/n"+ tahap2+ "/n"+ pkpldns +"/n"+ nip +"/n"+ nosrt1 +"/n"+ nosrt2 +"/n"+ tgl +"/n"+ unmdis+"/n"+ unmkam +"/n"+ skben +"/n"+ skkkam +"/n"+thn2;
    QString path = "doc/temp/";
    QString l= pdfdt1;
    QFile fOut(path+"dtpdf1.txt");
    if(fOut.exists()){fOut.remove();}
    if(!fOut.open(QFile::WriteOnly | QFile::Text))
        {qInfo()<<"tdk Bisa Buka file dtpdf1.txt ";} else {
                                                                                        QTextStream stream(&fOut);
                                                                                        stream << l;
                                                                                        fOut.flush();
                                                                                        fOut.close();
                                                                                     }
}}

void Form::datapdf2()
{
    int noBrs = ui->tableWidget_9->currentRow();

if(noBrs!=-1 && menu=="3"){
    QString id_kam =  ui->tableWidget_9->item(noBrs,2)->text();
    QString nmdis = ui->tableWidget_9->item(noBrs,3)->text();
    QString nmkamp = ui->tableWidget_9->item(noBrs,4)->text();
    QString terbilang= ui->tableWidget_9->item(noBrs,13)->text();
    QString norek = ui->tableWidget_9->item(noBrs,5)->text();
    QString nmrek = ui->tableWidget_9->item(noBrs,6)->text();
    QString nmbank = ui->tableWidget_9->item(noBrs,7)->text();
    QString nmkkp = ui->tableWidget_9->item(noBrs,9)->text();
    QString nmbenk = ui->tableWidget_9->item(noBrs,10)->text();
    QString jkk = ui->tableWidget_9->item(noBrs,28)->text();
    QString tahap = ui->tableWidget_9->item(noBrs,11)->text();
    QString jml = ui->tableWidget_9->item(noBrs,12)->text();
    QString persen = ui->tableWidget_9->item(noBrs,17)->text();
    QString nmkpdns = ui->tableWidget_9->item(noBrs,22)->text();
    QString tahap2 = ui->tableWidget_9->item(noBrs,26)->text();
    QString pkpldns = ui->tableWidget_9->item(noBrs,24)->text();
    QString nip = ui->tableWidget_9->item(noBrs,25)->text();
    QString nosrt1 = ui->tableWidget_9->item(noBrs,15)->text();
    QString nosrt2 = ui->tableWidget_9->item(noBrs,16)->text();
    QString tgl = ui->tableWidget_9->item(noBrs,14)->text();
    QString skben= ui->tableWidget_9->item(noBrs,8)->text();
    QString skkkam= ui->tableWidget_9->item(noBrs,19)->text();

    QString unmdis = nmdis.toUpper();
    QString unmkam = nmkamp.toUpper();

    //Tambahan thn 2
    QString thn2 = ui->tableWidget_9->item(noBrs,23)->text();

    QString pdfdt2;

    pdfdt2 = nmdis +"/n "+ nmkamp+ "/n" +terbilang +"/n"+norek+"/n"+ nmrek +"/n"+nmbank+"/n"+nmkkp+"/n"+nmbenk+"/n"
                    ""+jkk+"/n"+ tahap+ "/n"+ jml+"/n"+ persen+ "/n" +nmkpdns+ "/n"+ tahap2+ "/n"+ pkpldns +"/n"+ nip +"/n"+ nosrt1 +"/n"+ nosrt2 +"/n"+ tgl +"/n"+ unmdis +"/n"+ unmkam +"/n"+skben+"/n"+skkkam+"/n" +thn2 ;
    QString path = "doc/temp/";
    QString l= pdfdt2;
    QFile fOut(path+"dtpdf2.txt");
    if(fOut.exists()){fOut.remove();}
    //fOut.open(QFile::WriteOnly | QFile::Text);
    if(!fOut.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"tdk Bisa Buka file dtpdf2.txt ";} else {
        QTextStream stream(&fOut);
        stream << l;
        fOut.flush();
        fOut.close();
     }
}}

void Form::datapdf3()
{

}


bool Form::saveDocument(QString filePath)
{
    qInfo()<< filePath;
    return true;
}



void Form::on_saveFileButton_pressed(QString cd)
{
    on_generateButton_pressed(cd);
    if ((ui->label_16->pixmap() != Q_NULLPTR)  && (ui->label_16->pixmap()->isNull() != true))
    {
     const QPixmap *QRCodePixmap = ui->label_16->pixmap();

    QString imageType = "PNG";
    QString filetypeStr;
    if (imageType == QString("PNG")){
        filetypeStr = QString(".png");
    }

    bool saveResult;
    QString savefileName = QString("doc/temp/gbr/QRCode") + filetypeStr;
    saveResult = QRCodePixmap->save(savefileName);

    if (true == saveResult){
        //QMessageBox::information(this, tr("QRCode Generator"), QString("Save to ") + savefileName + QString(" success"));
    }
    else{ QMessageBox::warning(this, tr("QRCode Generator"), QString("Save to ") + savefileName + QString(" failure"));}
    }
    else{QMessageBox::warning(this, tr("QRCode Generator"), tr("Invalid QRCode Image"));}
}


void Form::on_generateButton_pressed( QString te)
{
    QString text = te;

    if (text.isEmpty()) {
       return;
    }

    int qrcode_width = 120 /*ui->label_16->width()*/;
    int qrcode_height = 120/*ui->label_16->height()*/;
    QPixmap qrcode_pixmap;
    bool result;
    result = Form::GeneratePixmapFromText(text, qrcode_pixmap, qrcode_width, qrcode_height);
    if (true == result){
        ui->label_16->setPixmap(qrcode_pixmap);
    }
    else{
        QMessageBox::warning(this, tr("QRCode Generator"), tr("Invalid QRCodeImage."));
        return;
    }
}

//!================== QToolButton ============================

void Form::on_toolButton_clicked() //tb 1
{
    ui->comboBox->setVisible(true);
    ui->comboBox_nmKampung->setVisible(true);
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->toolButton_refResh->setVisible(true);

    ui->label_realisasi->setVisible(false);
    ui->label_s_d->setVisible(false);
    ui->comboBox_realisasi->setVisible(false);
    ui->toolButton_pdf_main->setVisible(false);

    ui->dateEdit->setVisible(false);
    ui->dateEdit_2->setVisible(false);

    //Tambahan Filter
    ui->comboBox_4->setVisible(true);
    ui->label_26->setVisible(true);
    ui->toolButton_18->setVisible(true);
    ui->comboBox_4->clear();
    muatListFilter();


   ui->stackedWidget->setCurrentIndex(0);
   menu ="1";
   if(menu=="1"){
       if(qbx_id_kam->currentText()!="")
       {
           QString id = qbx_id_kam->currentText();
           muat_bamuskam(id);

       }
   }
}

// ==== Button Menu Dana Desa ==========
void Form::on_toolButton_danaDesa_clicked()
{
    ui->comboBox->setVisible(true);
    ui->comboBox_nmKampung->setVisible(true);
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->toolButton_refResh->setVisible(true);

    ui->label_realisasi->setVisible(false);
    ui->label_s_d->setVisible(false);
    ui->comboBox_realisasi->setVisible(false);
    ui->toolButton_pdf_main->setVisible(false);

    ui->dateEdit->setVisible(false);
    ui->dateEdit_2->setVisible(false);

    //Tambahan Filter
    ui->comboBox_4->setVisible(false);
    ui->label_26->setVisible(false);
    ui->toolButton_18->setVisible(false);

    ui->stackedWidget->setCurrentIndex(1);

   menu ="2";
   muat_kampung();
   muat_dis();
   ui->comboBox->setCurrentIndex(0);
   QString n="";
   muat_v_bam(n);
}

void Form::on_toolButton_8_clicked()
{
    ui->comboBox->setVisible(true);
    ui->comboBox_nmKampung->setVisible(true);
    ui->label->setVisible(true);
    ui->label_2->setVisible(true);
    ui->toolButton_refResh->setVisible(true);

    ui->label_realisasi->setVisible(false);
    ui->label_s_d->setVisible(false);
    ui->comboBox_realisasi->setVisible(false);
    ui->toolButton_pdf_main->setVisible(false);

    ui->dateEdit->setVisible(false);
    ui->dateEdit_2->setVisible(false);

    //Tambahan Filter
    ui->comboBox_4->setVisible(false);
    ui->label_26->setVisible(false);
    ui->toolButton_18->setVisible(false);

    ui->stackedWidget->setCurrentIndex(2);
    menu="3";
    muat_kampung();
    muat_dis();
    ui->comboBox->setCurrentIndex(0);
    QString n="";
    muat_v_bam_2(n);
}


void Form::on_toolButton_13_clicked() // ToolButton 4
{
    //Tambahan Filter
    ui->comboBox_4->setVisible(false);
    ui->label_26->setVisible(false);
    ui->toolButton_18->setVisible(false);

    menu="4";
    ui->comboBox->setVisible(false);
    ui->comboBox_nmKampung->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);

    ui->label_realisasi->setVisible(true);
    ui->label_s_d->setVisible(true);
    ui->comboBox_realisasi->setVisible(true);
    ui->toolButton_pdf_main->setVisible(false);

    ui->dateEdit->setVisible(true);
    ui->dateEdit_2->setVisible(true);
    ui->stackedWidget->setCurrentIndex(3);

    ui->comboBox_realisasi->clear();
    QStringList qb3;
    qb3 << "Realisasi Dana Desa" << "Realisasi Alokasi Dana Desa" ;
    ui->comboBox_realisasi->addItems(qb3);
    ui->comboBox_realisasi->setCurrentIndex(0);
    QDate d =QDate::currentDate();
    QString de = d.toString("yyyy");
    QString de_ = "01-01-"+de;
    QDate d_ =QDate::fromString(de_,"dd-MM-yyyy");
    ui->dateEdit->setDisplayFormat("dd-MMMM-yyyy");
    ui->dateEdit->setDate(d_);
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit_2->setDisplayFormat("dd-MMMM-yyyy");
    ui->dateEdit_2->setCalendarPopup(true);
    ui->dateEdit_2->setDate(d);

    ui->label_18->setText("Realisasi Anggaran Desa Tahun Anggaran "+de);
    //muatrealdds();
    headsppd();
    loadsppd();
    loadsppd_2();

}

//!================== QToolButton ============================

void Form::muatrealdds() // Muat Data Realisasi Dana Desa
{
    li_realdds.clear();
    li_realdds.append("ID");
    li_realdds.append("Nama Distrik");
    li_realdds.append("Nama Kampung");
     li_realdds.append("Pagu Anggaran");
    QSqlQuery query;
    QString cmd = "SELECT thp_cair FROM pmk_yhk.t_real GROUP BY thp_cair ORDER BY thp_cair ";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat data realisasi Dana Desa ."); return;}
    while(query.next())
    {
        li_realdds.append(query.value(0).toString());
    }
    li_realdds.append("Sub Total");
    li_realdds.append("Sisa Pagu");

    muatheadertw12();
    muatrealdds_();
    muatrealdds__();
}


void Form::muatheadertw12()
{
    int j = li_realdds.count();
    ui->tableWidget_12->setColumnCount(j);
    ui->tableWidget_12->setHorizontalHeaderLabels(li_realdds );
    ui->tableWidget_12->setColumnHidden(0,true);
}

void Form::muatrealdds_() // Muat Data Kampung Untuk Realisasi
{
    while(ui->tableWidget_12->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_12->removeRow(0);}

    QSqlQuery query;
    QString cmd = "SELECT no,distrik,kampung,pagu FROM pmk_yhk.m_kampung ORDER BY no";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Nama Distrik dan Kampung di realisasi Dana Desa .."); return;}
    int no=0;
    while(query.next())
    {
        ui->tableWidget_12->insertRow(no);
        QTableWidgetItem *id_ = new QTableWidgetItem;
        QTableWidgetItem *nmdis_ = new QTableWidgetItem;
        QTableWidgetItem *nmkam_ = new QTableWidgetItem;
        QTableWidgetItem *pagu_ = new QTableWidgetItem;

        id_->setText(query.value(0).toString());
        nmdis_->setText(query.value(1).toString());
        nmkam_->setText(query.value(2).toString());
        double pg = query.value(3).toDouble();
        QString pagu = indo.toCurrencyString(pg,"Rp ");
        pagu_->setText(pagu);

        id_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        nmdis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        nmkam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pagu_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

       ui->tableWidget_12->setItem(no,0,id_);
       ui->tableWidget_12->setItem(no,1,nmdis_);
       ui->tableWidget_12->setItem(no,2,nmkam_);
       ui->tableWidget_12->setItem(no,3,pagu_);

        no++;
}}

void Form::muatrealdds__()
{

    QDate tg = QDate::fromString(ui->dateEdit->text(),"dd-MMMM-yyyy");
    QDate tg2 = QDate::fromString(ui->dateEdit_2->text(),"dd-MMMM-yyyy");

    QString tgl = tg.toString("yyyy-MM-dd");
    QString tgl2 = tg2.toString("yyyy-MM-dd");

    int j = ui->tableWidget_12->rowCount();
    int k = ui->tableWidget_12->columnCount();

    for(int i=0; i<j ; i++)
    {
        double sbt=0;
        for(int ii=4; ii<k-2; ii++)
        {
                qInfo()<<"iiiii i" << i;
                qInfo()<<"iiiii Ii" << ii;
                qInfo()<<"iiiii k3" << k-2;

            QString id = ui->tableWidget_12->item(i,0)->text();
            QString a = ui->tableWidget_12->item(i,3)->text();
             if(a==nullptr){a="0";}
            a.replace("Rp ",""); a.replace(".",""); a.replace(",",".");
            double aa = a.toDouble();

            QSqlQuery query;
            // SELECT j_cair FROM pmk_yhk.t_real WHERE id_kam = :id  AND thp_cair = :thp AND tgl >= :tgl AND tgl <= :tgl1
            //QString cmd = "SELECT j_cair FROM pmk_yhk.t_real WHERE id_real = :id  AND thp_cair = :thp ";
            QString cmd = "SELECT j_cair FROM pmk_yhk.t_real WHERE id_kam = :id  AND thp_cair = :thp AND tgl >= :tgl AND tgl <= :tgl2";
            query.prepare(cmd);
            query.bindValue(":id",id);
            query.bindValue(":thp",li_realdds.at(ii));
            query.bindValue(":tgl",tgl);
            query.bindValue(":tgl2",tgl2);
            qInfo()<<"Tahap Cair "<< li_realdds.at(ii);
            bool ok = exec(query);
            if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Nama Distrik dan Kampung di realisasi Dana Desa ..."); return;}
            while(query.next()){
            QTableWidgetItem *jc_ = new QTableWidgetItem;

            QString j = query.value(0).toString();
            if(j==nullptr) {j="0";}
            double jc = j.toDouble();
            sbt += jc;
            QString jcc = indo.toCurrencyString(jc,"Rp ");

            jc_->setText(jcc);
           ui->tableWidget_12->setItem(i,ii,jc_);

           QTableWidgetItem *sb_ = new QTableWidgetItem;
           QString sb = indo.toCurrencyString(sbt,"Rp ");
           sb_->setText(sb);
           ui->tableWidget_12->setItem(i, ui->tableWidget_12->columnCount()-2, sb_);


           double dd_ = aa-sbt;
           QString  dd = indo.toCurrencyString(dd_,"Rp ");
           QTableWidgetItem *sb__ = new QTableWidgetItem;
           sb__->setText(dd);
           ui->tableWidget_12->setItem(i, ui->tableWidget_12->columnCount()-1, sb__);

           /**/ }

        }
        sbt=0;
    }
}

QString Form::muatreal()
{
   int j = li_realdds.count();
   qInfo() << j;
   QString a="d";
   return a;

}

void Form::muatrealadd()
{
    while(ui->tableWidget_12->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_12->removeRow(0);}

    li_realadd.clear();
    li_realadd.append("ID");
    li_realadd.append("Nama Distrik");
    li_realadd.append("Nama Kampung");
    li_realadd.append("Pagu Anggaran");
    QSqlQuery query;
    QString cmd = "SELECT thp_cair FROM pmk_yhk.t_real_2 GROUP BY thp_cair ORDER BY thp_cair";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat data realisasi Dana Desa ."); return;}
    while(query.next())
    {
        li_realadd.append(query.value(0).toString());
    }
    li_realadd.append("Sub Total");
    li_realadd.append("Sisa Pagu");

    muatheadertw12_2();
    muatrealadd_();
    muatrealadd__();

}

void Form::muatheadertw12_2()
{
    int j = li_realadd.count();
    ui->tableWidget_12->setColumnCount(j);
    ui->tableWidget_12->setHorizontalHeaderLabels(li_realadd );
    ui->tableWidget_12->setColumnHidden(0,true);

}

void Form::muatrealadd_() // Muat Data Kampung Untuk Realisasi
{
    while(ui->tableWidget_12->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_12->removeRow(0);}

    QSqlQuery query;
    QString cmd = "SELECT no,distrik,kampung,pagu_add FROM pmk_yhk.m_kampung ORDER BY no";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Nama Distrik dan Kampung di realisasi Dana Desa .."); return;}
    int no=0;
    while(query.next())
    {
        ui->tableWidget_12->insertRow(no);
        QTableWidgetItem *id_ = new QTableWidgetItem;
        QTableWidgetItem *nmdis_ = new QTableWidgetItem;
        QTableWidgetItem *nmkam_ = new QTableWidgetItem;
        QTableWidgetItem *pagu_ = new QTableWidgetItem;

        id_->setText(query.value(0).toString());
        nmdis_->setText(query.value(1).toString());
        nmkam_->setText(query.value(2).toString());
        double pg = query.value(3).toDouble();
        QString pagu = indo.toCurrencyString(pg,"Rp ");
        pagu_->setText(pagu);

        id_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        nmdis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        nmkam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        pagu_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

       ui->tableWidget_12->setItem(no,0,id_);
       ui->tableWidget_12->setItem(no,1,nmdis_);
       ui->tableWidget_12->setItem(no,2,nmkam_);
       ui->tableWidget_12->setItem(no,3,pagu_);

        no++;
}}

void Form::muatrealadd__()
{

    QDate tg = QDate::fromString(ui->dateEdit->text(),"dd-MMMM-yyyy");
    QDate tg2 = QDate::fromString(ui->dateEdit_2->text(),"dd-MMMM-yyyy");

    QString tgl = tg.toString("yyyy-MM-dd");
    QString tgl2 = tg2.toString("yyyy-MM-dd");

    int j = ui->tableWidget_12->rowCount();
    int k = ui->tableWidget_12->columnCount();
    for(int i=0; i<j ; i++)
    {
        double sbt=0;
        for(int ii=4; ii<k-2; ii++)
        {
            QString id = ui->tableWidget_12->item(i,0)->text();
            QString a = ui->tableWidget_12->item(i,3)->text();
             if(a==nullptr){a="0";}
            a.replace("Rp ",""); a.replace(".",""); a.replace(",",".");
            double aa = a.toDouble();

            QSqlQuery query;
            // SELECT j_cair FROM pmk_yhk.t_real WHERE id_kam = :id  AND thp_cair = :thp AND tgl >= :tgl AND tgl <= :tgl1
            //QString cmd = "SELECT j_cair FROM pmk_yhk.t_real WHERE id_real = :id  AND thp_cair = :thp ";
            QString cmd = "SELECT j_cair FROM pmk_yhk.t_real_2 WHERE id_kam = :id  AND thp_cair = :thp AND tgl >= :tgl AND tgl <= :tgl2";
            query.prepare(cmd);
            query.bindValue(":id",id);
            query.bindValue(":thp",li_realadd.at(ii));
            query.bindValue(":tgl",tgl);
            query.bindValue(":tgl2",tgl2);
            qInfo()<<"Tahap Cair "<< li_realadd.at(ii);
            bool ok = exec(query);
            if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Nama Distrik dan Kampung di realisasi Dana Desa ..."); return;}
            while(query.next()){
            QTableWidgetItem *jc_ = new QTableWidgetItem;

            QString j = query.value(0).toString();
            if(j==nullptr) {j="0";}
            double jc = j.toDouble();
            sbt += jc;
            QString jcc = indo.toCurrencyString(jc,"Rp ");

            jc_->setText(jcc);
           ui->tableWidget_12->setItem(i,ii,jc_);

           QTableWidgetItem *sb_ = new QTableWidgetItem;
           QString sb = indo.toCurrencyString(sbt,"Rp ");
           sb_->setText(sb);
           ui->tableWidget_12->setItem(i, ui->tableWidget_12->columnCount()-2, sb_);


           double dd_ = aa-sbt;
           QString  dd = indo.toCurrencyString(dd_,"Rp ");
           QTableWidgetItem *sb__ = new QTableWidgetItem;
           sb__->setText(dd);
           ui->tableWidget_12->setItem(i, ui->tableWidget_12->columnCount()-1, sb__);

           /**/ }

        }
        sbt=0;
    }
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
        while(ui->tableWidget_2->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_2->removeRow(0);}}

    if(menu=="3"&&ui->comboBox_nmKampung->currentText()==""){
        while(ui->tableWidget_9->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_9->removeRow(0);}}

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
        while(ui->tableWidget_2->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_2->removeRow(0);}
        muat_v_bam(id_kp);}

    if(menu=="3"){
        while(ui->tableWidget_9->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
       {ui->tableWidget_9->removeRow(0);}
        muat_v_bam_2(id_kp);}

    if(id_kp==""){return;}
    if(menu=="1"){muat_bamuskam(id_kp);}  //Muat data kampung di menu 1

}

void Form::muat_bamuskam(QString &id_kam_s)
{

//    int noBr = ui->tableWidget->rowCount();
//    while (noBr>0) {ui->tableWidget->removeRow(0); }

    while(ui->tableWidget_Bamuskam->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_Bamuskam->removeRow(0);}

    QSqlQuery query;
    QString cmd= "  SELECT id,id_kam,id_dis,id_j,distrik,kampung, nama, jabatan,no_sk, tgl_sk,ttl,alamat FROM pmk_yhk.v_bam_ WHERE id_kam = :id ORDER BY id_j ";
    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);
    int noBrs=0;
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data Bamuskam..."+ui->comboBox_nmKampung->currentText()+"...error... "+query.lastError().text()+"...!!!"); return;}
    while (query.next()) {
            ui->tableWidget_Bamuskam->insertRow(noBrs);

            QTableWidgetItem *id_ = new QTableWidgetItem;
            QTableWidgetItem *id_kam_ = new QTableWidgetItem;
            QTableWidgetItem *id_dis_ = new QTableWidgetItem;
            QTableWidgetItem *id_j_ = new QTableWidgetItem;
            QTableWidgetItem *nmDis_ = new QTableWidgetItem;
            QTableWidgetItem *nmKam_ = new QTableWidgetItem;
            QTableWidgetItem *nama_ = new QTableWidgetItem;
            QTableWidgetItem *jabatan_ = new QTableWidgetItem;
            QTableWidgetItem *no_sk_ = new QTableWidgetItem;
            QTableWidgetItem *tgl_sk_ = new QTableWidgetItem;
            QTableWidgetItem *ttl_ = new QTableWidgetItem;
            QTableWidgetItem *almt_ = new QTableWidgetItem;

            id_->setText(query.value(0).toString());
            id_kam_->setText(query.value(1).toString());
            id_dis_->setText(query.value(2).toString());
            id_j_->setText(query.value(3).toString());
            nmDis_->setText(query.value(4).toString());
            nmKam_->setText(query.value(5).toString());
            nama_->setText(query.value(6).toString());
            jabatan_->setText(query.value(7).toString());
            no_sk_->setText(query.value(8).toString());
            QDate d = QDate::fromString(query.value(9).toString(),"yyyy-MM-dd");
            QString dd = d.toString("dd-MM-yyyy");

            tgl_sk_->setText(dd);
            ttl_->setText(query.value(10).toString());
            almt_->setText(query.value(11).toString());

            id_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            id_kam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            id_dis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            id_j_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            nmDis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            nmKam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            nama_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            jabatan_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            no_sk_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            tgl_sk_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ttl_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            almt_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableWidget_Bamuskam->setItem(noBrs,0,id_);
            ui->tableWidget_Bamuskam->setItem(noBrs,1,id_kam_);
            ui->tableWidget_Bamuskam->setItem(noBrs,2,id_dis_);
            ui->tableWidget_Bamuskam->setItem(noBrs,3,id_j_);
            ui->tableWidget_Bamuskam->setItem(noBrs,4,nmDis_);
            ui->tableWidget_Bamuskam->setItem(noBrs,5,nmKam_);
            ui->tableWidget_Bamuskam->setItem(noBrs,6,nama_);
            ui->tableWidget_Bamuskam->setItem(noBrs,7,jabatan_);
            ui->tableWidget_Bamuskam->setItem(noBrs,8,no_sk_);
            ui->tableWidget_Bamuskam->setItem(noBrs,9,tgl_sk_);
            ui->tableWidget_Bamuskam->setItem(noBrs,10,ttl_);
            ui->tableWidget_Bamuskam->setItem(noBrs,11,almt_);

            noBrs++;
    }

    if(noBrs==0){QMessageBox::information(this,"Info...!!!","Data Bamuskam pada kampung ini belum tersedia..."); return;}
}


void Form::muat_v_bam(QString &id_kam_s) // Data realisasi kampung
{
    while(ui->tableWidget_6->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_6->removeRow(0);}
    QLocale indo = QLocale(QLocale::Indonesian, QLocale::Indonesia);
    QSqlQuery query;
    QString cmd= " SELECT no, distrik, kampung, pagu, pagu1 AS sisa, realisasi FROM pmk_yhk.v_bam WHERE no= :id ORDER BY no";
    if (id_kam_s==""){cmd= " SELECT no, distrik, kampung, pagu, pagu1 AS sisa, realisasi FROM pmk_yhk.v_bam ORDER BY no ";}

    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);
    int noBrs=0;
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data..."+ui->comboBox_nmKampung->currentText()+"...error... "+query.lastError().text()+"!!!");}
    double Tp=0;
    double Tr=0;
    double Ts=0;
    while (query.next()) {
            ui->tableWidget_6->insertRow(noBrs);

            QTableWidgetItem *no_ = new QTableWidgetItem;
            QTableWidgetItem *dis_ = new QTableWidgetItem;
            QTableWidgetItem *kam_ = new QTableWidgetItem;
            QTableWidgetItem *pagu_ = new QTableWidgetItem;
            QTableWidgetItem *real_ = new QTableWidgetItem;
            QTableWidgetItem *sisa_ = new QTableWidgetItem;

            no_->setText(query.value(0).toString());
            dis_->setText(query.value(1).toString());
            kam_->setText(query.value(2).toString());
            double p = query.value(3).toDouble();
            pagu_->setText(indo.toCurrencyString(p, "Rp "));
            double r = query.value(4).toDouble();
            real_->setText(indo.toCurrencyString(r, "Rp "));
            double s = query.value(5).toDouble();
            sisa_->setText(indo.toCurrencyString(s, "Rp "));
            Tp +=p;
            Tr +=r;
            Ts +=s;

            no_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            dis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            kam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pagu_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            real_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            sisa_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableWidget_6->setItem(noBrs,0,no_);
            ui->tableWidget_6->setItem(noBrs,1,dis_);
            ui->tableWidget_6->setItem(noBrs,2,kam_);
            ui->tableWidget_6->setItem(noBrs,3,pagu_);
            ui->tableWidget_6->setItem(noBrs,4,real_);
            ui->tableWidget_6->setItem(noBrs,5,sisa_);
            noBrs++;
    }

    if(noBrs==0){QMessageBox::information(this,"Info...!!!","Gagal Memuat Data...Error"+query.lastError().text()+""); return;}

    QString tt = QString::number(Tp);
    //qInfo() << "Tppp" << Tp << "++++" << tt;
    bilang(tt);
//    qInfo() << "Tp" << indo.toCurrencyString(Tp, "Rp ");
//    qInfo() << "Tr" << indo.toCurrencyString(Tr, "Rp ");
//    qInfo() << "Ts" << indo.toCurrencyString(Ts, "Rp ");

     ui->lineEdit->setText(indo.toCurrencyString(Tp, "Rp "));
     ui->lineEdit_3->setText(indo.toCurrencyString(Tr, "Rp "));
     ui->lineEdit_2->setText(indo.toCurrencyString(Ts, "Rp "));

     muat_real(id_kam_s);
}


void Form::muat_v_bam_2(QString &id_kam_s) // Data realisasi kampung
{
    while(ui->tableWidget_11->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_11->removeRow(0);}
    QLocale indo = QLocale(QLocale::Indonesian, QLocale::Indonesia);
    QSqlQuery query;
    QString cmd= " SELECT no, distrik, kampung, pagu_add, pagu_add1 AS sisa, realisasi_add FROM pmk_yhk.v_bam WHERE no= :id ORDER BY no ";
    if (id_kam_s==""){cmd= " SELECT no, distrik, kampung, pagu_add, pagu_add1 AS sisa, realisasi_add FROM pmk_yhk.v_bam ORDER BY no  ";}

    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);
    int noBrs=0;
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data..."+ui->comboBox_nmKampung->currentText()+"...error... "+query.lastError().text()+"!!!");}
    double Tp=0;
    double Tr=0;
    double Ts=0;
    while (query.next()) {

            ui->tableWidget_11->insertRow(noBrs);

            QTableWidgetItem *no_ = new QTableWidgetItem;
            QTableWidgetItem *dis_ = new QTableWidgetItem;
            QTableWidgetItem *kam_ = new QTableWidgetItem;
            QTableWidgetItem *pagu_ = new QTableWidgetItem;
            QTableWidgetItem *real_ = new QTableWidgetItem;
            QTableWidgetItem *sisa_ = new QTableWidgetItem;

            no_->setText(query.value(0).toString());
            dis_->setText(query.value(1).toString());
            kam_->setText(query.value(2).toString());
            double p = query.value(3).toDouble();
            pagu_->setText(indo.toCurrencyString(p, "Rp "));
            double r = query.value(4).toDouble();
            real_->setText(indo.toCurrencyString(r, "Rp "));
            double s = query.value(5).toDouble();
            sisa_->setText(indo.toCurrencyString(s, "Rp "));

            no_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            dis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            kam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            pagu_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            real_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            sisa_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            Tp +=p;
            Tr +=r;
            Ts +=s;
            ui->tableWidget_11->setItem(noBrs,0,no_);
            ui->tableWidget_11->setItem(noBrs,1,dis_);
            ui->tableWidget_11->setItem(noBrs,2,kam_);
            ui->tableWidget_11->setItem(noBrs,3,pagu_);
            ui->tableWidget_11->setItem(noBrs,4,real_);
            ui->tableWidget_11->setItem(noBrs,5,sisa_);
            noBrs++;
    }

    if(noBrs==0){QMessageBox::information(this,"Info...!!!","Gagal Memuat Data...Error"+query.lastError().text()+""); return;}

    QString tt = QString::number(Tp);
   // qInfo() << "Tppp" << Tp << "++++" << tt;
   bilang(tt);

//    qInfo() << "Tp" << indo.toCurrencyString(Tp, "Rp ");
//    qInfo() << "Tr" << indo.toCurrencyString(Tr, "Rp ");
//    qInfo() << "Ts" << indo.toCurrencyString(Ts, "Rp ");

      ui->lineEdit_4->setText(indo.toCurrencyString(Tp, "Rp "));
      ui->lineEdit_6->setText(indo.toCurrencyString(Tr, "Rp "));
      ui->lineEdit_5->setText(indo.toCurrencyString(Ts, "Rp "));

      muat_real_2(id_kam_s);

}

void Form::header_wt1() // Header table widget 1
{
    QStringList headerWidget;
    ui->tableWidget_Bamuskam->setColumnCount(12);
    headerWidget <<"id" <<"id Kam"<<"id_dis"<<"id_j"<<"Distrik" <<"Kampung" <<"Nama" <<"Jabatan "<<"No SK " << "Tgl SK" <<"Tempat Tanggal Lahir  " << "Alamat ";
    ui->tableWidget_Bamuskam->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_Bamuskam->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_Bamuskam->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_Bamuskam->setColumnWidth(0,8);
    ui->tableWidget_Bamuskam->setColumnHidden(0,true);
    ui->tableWidget_Bamuskam->setColumnHidden(1,true);
    ui->tableWidget_Bamuskam->setColumnHidden(2,true);
    ui->tableWidget_Bamuskam->setColumnHidden(3,true);
    ui->tableWidget_Bamuskam->setColumnHidden(11,true);
    ui->tableWidget_Bamuskam->setColumnHidden(10,true);

    ui->tableWidget_Bamuskam->setColumnWidth(4,90);
    ui->tableWidget_Bamuskam->setColumnWidth(5,120);
    ui->tableWidget_Bamuskam->setColumnWidth(6,130);
    ui->tableWidget_Bamuskam->setColumnWidth(7,250);
    ui->tableWidget_Bamuskam->setColumnWidth(8,250);
    ui->tableWidget_Bamuskam->setColumnWidth(9,250);
    ui->tableWidget_Bamuskam->setColumnWidth(10,250);
    ui->tableWidget_Bamuskam->setColumnWidth(11,250);
}


void Form::header_wt6() // Header table widget 6
{
    QStringList headerWidget;
    ui->tableWidget_6->setColumnCount(6);
    headerWidget <<"id" <<"Nama Distrik"<<"Nama Kampung" <<"Pagu Anggaran " <<"Sisa Anggaran "<<"Realisasi ";
    ui->tableWidget_6->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_6->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_6->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_6->setColumnWidth(0,8);
    ui->tableWidget_6->setColumnHidden(0,true);
    ui->tableWidget_6->setColumnWidth(1,140);
    ui->tableWidget_6->setColumnWidth(2,150);
    //ui->tableWidget->setColumnHidden(2,true);
    ui->tableWidget_6->setColumnWidth(3,120);
    ui->tableWidget_6->setColumnWidth(4,120);
    ui->tableWidget_6->setColumnWidth(5,120);

}

void Form::header_wt11() // Header table widget 6
{
    QStringList headerWidget;
    ui->tableWidget_11->setColumnCount(6);
    headerWidget <<"id" <<"Nama Distrik"<<"Nama Kampung" <<"Pagu Anggaran " <<"Sisa Anggaran "<<"Realisasi ";
    ui->tableWidget_11->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_11->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_11->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_11->setColumnWidth(0,8);
    ui->tableWidget_11->setColumnHidden(0,true);
    ui->tableWidget_11->setColumnWidth(1,140);
    ui->tableWidget_11->setColumnWidth(2,150);
    //ui->tableWidget->setColumnHidden(2,true);
    ui->tableWidget_11->setColumnWidth(3,120);
    ui->tableWidget_11->setColumnWidth(4,120);
    ui->tableWidget_11->setColumnWidth(5,120);

}

void Form::header_wt2()
{
    QStringList headerWidget;
    ui->tableWidget_2->setColumnCount(29);
    headerWidget <<"cetak"<<"Id Dis"<<"Id Kam"<<" Nama Distrik"<<"Nama Kampung"<<"No Rekening"<<"Nama Rekening"<<"Nama Bank"<<"SK Bendahara"<<"Nama Kepala Kampung"<<" Nama Bendahara "
                 <<"Tahap Pencairan"<<"Jumlah Pencairan"<<"Terbilang"<<"Tanggal Terima"<<"No Srt 1"<<"No Srt 2"<<"%"<<"SK Bupati"<<"SK Kep.Kampung"
                <<"SK Men PMK"<<"SK Men Keu"<<"Kepala Dinas" << " Ket " << "Pangkat" << "NIP" <<"Laporan Realisasi"<<"id"<<"jkk";
    ui->tableWidget_2->setHorizontalHeaderLabels(headerWidget);

    ui->tableWidget_2->setColumnHidden(0,true);
    ui->tableWidget_2->setColumnHidden(1,true);
    ui->tableWidget_2->setColumnHidden(2,true);
    //ui->tableWidget_2->setColumnHidden(3,true);
    //ui->tableWidget_2->setColumnHidden(4,true);
    ui->tableWidget_2->setColumnHidden(5,true);
    ui->tableWidget_2->setColumnHidden(6,true);
    ui->tableWidget_2->setColumnHidden(7,true);
    ui->tableWidget_2->setColumnHidden(8,true);
    ui->tableWidget_2->setColumnHidden(9,true);
    ui->tableWidget_2->setColumnHidden(10,true);
    ui->tableWidget_2->setColumnHidden(18,true);
    ui->tableWidget_2->setColumnHidden(19,true);
    ui->tableWidget_2->setColumnHidden(20,true);
    ui->tableWidget_2->setColumnHidden(21,true);
    ui->tableWidget_2->setColumnHidden(22,true);
    ui->tableWidget_2->setColumnHidden(23,true);
    ui->tableWidget_2->setColumnHidden(24,true);
    ui->tableWidget_2->setColumnHidden(25,true);
    ui->tableWidget_2->setColumnHidden(27,true);
    ui->tableWidget_2->setColumnHidden(28,true);

    ui->tableWidget_2->setColumnWidth(3,120);
    ui->tableWidget_2->setColumnWidth(4,120);
   ui->tableWidget_2->setColumnWidth(11,150);
   ui->tableWidget_2->setColumnWidth(12,120);
   ui->tableWidget_2->setColumnWidth(13,340);
   ui->tableWidget_2->setColumnWidth(14,90);
   ui->tableWidget_2->setColumnWidth(15,230);
   ui->tableWidget_2->setColumnWidth(16,230);
   ui->tableWidget_2->setColumnWidth(17,50);
   ui->tableWidget_2->setColumnWidth(26,120);

}

void Form::header_wt9()
{
    QStringList headerWidget;
    ui->tableWidget_9->setColumnCount(29);
    headerWidget <<"cetak"<<"Id Dis"<<"Id Kam"<<" Nama Distrik"<<"Nama Kampung"<<"No Rekening"<<"Nama Rekening"<<"Nama Bank"<<"SK Bendahara"<<"Nama Kepala Kampung"<<" Nama Bendahara "
                 <<"Tahap Pencairan"<<"Jumlah Pencairan"<<"Terbilang"<<"Tanggal Terima"<<"No Srt 1"<<"No Srt 2"<<"%"<<"SK Bupati"<<"SK Kep.Kampung"
                <<"SK Men PMK"<<"SK Men Keu"<<"Kepala Dinas" << " Ket " << "Pangkat" << "NIP" <<"Laporan Realisasi"<<"id"<<"jkk";
    ui->tableWidget_9->setHorizontalHeaderLabels(headerWidget);

    ui->tableWidget_9->setColumnHidden(0,true);
    ui->tableWidget_9->setColumnHidden(1,true);
    ui->tableWidget_9->setColumnHidden(2,true);
    //ui->tableWidget_9->setColumnHidden(3,true);
    //ui->tableWidget_9->setColumnHidden(4,true);
    ui->tableWidget_9->setColumnHidden(5,true);
    ui->tableWidget_9->setColumnHidden(6,true);
    ui->tableWidget_9->setColumnHidden(7,true);
    ui->tableWidget_9->setColumnHidden(8,true);
    ui->tableWidget_9->setColumnHidden(9,true);
    ui->tableWidget_9->setColumnHidden(10,true);
    //ui->tableWidget_9->setColumnHidden(11,true);
    //ui->tableWidget_9->setColumnHidden(12,true);
    //ui->tableWidget_9->setColumnHidden(13,true);
    //ui->tableWidget_9->setColumnHidden(14,true);
    //ui->tableWidget_9->setColumnHidden(15,true);
    //ui->tableWidget_9->setColumnHidden(16,true);
    //ui->tableWidget_9->setColumnHidden(17,true);
    ui->tableWidget_9->setColumnHidden(18,true);
    ui->tableWidget_9->setColumnHidden(19,true);
    ui->tableWidget_9->setColumnHidden(20,true);
    ui->tableWidget_9->setColumnHidden(21,true);
    ui->tableWidget_9->setColumnHidden(22,true);
    ui->tableWidget_9->setColumnHidden(23,true);
    ui->tableWidget_9->setColumnHidden(24,true);
    ui->tableWidget_9->setColumnHidden(25,true);
    //ui->tableWidget_9->setColumnHidden(26,true);
    ui->tableWidget_9->setColumnHidden(27,true);
    ui->tableWidget_9->setColumnHidden(28,true);

   ui->tableWidget_9->setColumnWidth(3,120);
   ui->tableWidget_9->setColumnWidth(4,120);
   ui->tableWidget_9->setColumnWidth(11,150);
   ui->tableWidget_9->setColumnWidth(12,120);
   ui->tableWidget_9->setColumnWidth(13,340);
   ui->tableWidget_9->setColumnWidth(14,90);
   ui->tableWidget_9->setColumnWidth(15,230);
   ui->tableWidget_9->setColumnWidth(16,230);
   ui->tableWidget_9->setColumnWidth(17,50);
   ui->tableWidget_9->setColumnWidth(26,120);

}


void Form::event_doubleklik_tw() // Double Klik tw data bamuskam menu 1
{
    qInfo()<< "Even Double Kli tw 1";
    if(menu=="1"){ int nBrs = ui->tableWidget_Bamuskam->currentRow();

                                btn = new QPushButton;
                                btn->setMaximumSize(150,25);
                                btn->setText(" SIMPAN ");
                                 id= ui->tableWidget_Bamuskam->item(nBrs,0)->text();
                                 QString nm = ui->tableWidget_Bamuskam->item(nBrs,6)->text();
                                 QString jbtn = ui->tableWidget_Bamuskam->item(nBrs,7)->text();
                                 QString no_sk = ui->tableWidget_Bamuskam->item(nBrs,8)->text();
                                 QString tgl_sk = ui->tableWidget_Bamuskam->item(nBrs,9)->text();
                                 QDate de_ = QDate::fromString(tgl_sk,"dd-MM-yyyy");
                                 de = new QDateEdit;
                                 de->setDate(de_);
                                 de->setMinimumSize(40,25);
                                 de->setDisplayFormat("dd-MM-yyyy");
                                 de->setCalendarPopup(true);
                                 QString ttl = ui->tableWidget_Bamuskam->item(nBrs,10)->text();
                                 QString almt = ui->tableWidget_Bamuskam->item(nBrs,11)->text();

                                 //qInfo()<< "Even Double Kli tw 1... " << id << "="<< nm  <<"="<<jbtn<<"="<<no_sk << "+++ " << " " << "=" << de->text() << "=" << ttl << "=" << almt <<"ddd" <<tgl_sk;

                                 connect(btn, SIGNAL(pressed()), this, SLOT(click_btn()));

                                 QLabel *L_nm = new QLabel;
                                 QLabel *L_jbtn = new QLabel;
                                 QLabel *L_no_sk = new QLabel;
                                 QLabel *L_tgl_sk = new QLabel;

                                 Le_nm = new QLineEdit;
                                 Le_jbtn = new QLineEdit;
                                 Le_no_sk = new QLineEdit;

                                 L_nm->setMaximumSize(200,25); Le_nm->setMaximumSize(200,25);
                                 L_nm->setMinimumSize(200,25); Le_nm->setMinimumSize(200,25);

                                 L_jbtn->setMaximumSize(170,25); Le_jbtn->setMaximumSize(170,25);
                                 L_jbtn->setMinimumSize(170,25); Le_jbtn->setMinimumSize(170,25);

                                 L_no_sk->setMaximumSize(260,25); Le_no_sk->setMaximumSize(260,25);
                                 L_no_sk->setMinimumSize(260,25); Le_no_sk->setMinimumSize(260,25);

                                 L_tgl_sk->setMinimumSize(40,25);

                                 L_nm->setText("Nama");
                                 L_nm->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignCenter; }");
                                 L_jbtn->setText("Jabatan");
                                 L_jbtn->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignCenter; }");
                                 L_no_sk->setText("No SK");
                                 L_no_sk->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignCenter; }");
                                 L_tgl_sk->setText("Tanggal SK");
                                 L_tgl_sk->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignCenter; }");

                                 Le_nm->clear();
                                 Le_jbtn->clear();
                                 Le_no_sk->clear();

                                 Le_nm->setText(nm);
                                 Le_jbtn->setText(jbtn);
                                 Le_no_sk->setText(no_sk);


                                 //this->setWindowIcon(logo);

                                 QWidget *eb_v = new QWidget;
                                 QIcon logo(":/gbr/html/gbr/yhk.png");
                                 eb_v->setWindowIcon(logo);
                                 eb_v->setWindowTitle("Ubah Data "+jbtn+" Kampung "+ui->comboBox_nmKampung->currentText()+"");
                                 eb_v->setMinimumWidth(800);
                                 eb_v->setMaximumWidth(800);
                                 eb_v->setMaximumHeight(150);


                                 QVBoxLayout *VL = new QVBoxLayout(eb_v);
                                 QHBoxLayout *HL1 = new QHBoxLayout;
                                 QHBoxLayout *HL2 = new QHBoxLayout;
                                 QHBoxLayout *HL3 = new QHBoxLayout;

                                 VL->addLayout(HL1);
                                 VL->addLayout(HL2);
                                 VL->addLayout(HL3);
                                 HL1->addWidget(L_nm);
                                 HL1->addWidget(L_jbtn);
                                 HL1->addWidget(L_no_sk);
                                 HL1->addWidget(L_tgl_sk);
                                HL2->addWidget(Le_nm);
                                HL2->addWidget(Le_jbtn);
                                HL2->addWidget(Le_no_sk);
                                HL2->addWidget(de);
                                HL3->addWidget(btn);
                                eb_v->show();
                            }
}


void Form::event_doubleklik_tw_2() //Double Klik Edit data realisasi dds
{
    qInfo() << "Doubleklik tw 2";


if(menu=="2"){  menu2="2";
    QString id_dis = qbx_id_dis->currentText();
    QString id_kam = qbx_id_kam->currentText();
    if(id_dis==""&& id_kam==""){QMessageBox::information(this,"Info","Pilih Distrik dan Kampung..."); return;}
    int noBrs = ui->tableWidget_2->currentRow();

    if(noBrs>=0){

        le_jml = new QLineEdit;
        qbx_no_srt1 = new QComboBox;
        qbx_no_srt2 = new QComboBox;
        qbx_persen= new QComboBox;
//        qbx_sk_pmk= new QComboBox;
//        qbx_sk_keu= new QComboBox;

        le_jml->setClearButtonEnabled(true);
        le_jml->setText( ui->tableWidget_2->item(noBrs,12)->text());
        le_jml->setMaximumWidth(190);


        //========== Data dari table widget ================
        QString thp_tw = ui->tableWidget_2->item(noBrs,11)->text();

//        QString jcair = le_jml->text();
//        jcair.replace("Rp ",""); jcair.replace(".",""); jcair.replace(",",".");
//        double jc = jcair.toDouble();

//        QString jcair2 = ui->tableWidget_2->item(noBrs,12)->text();
//        jcair2.replace("Rp ",""); jcair2.replace(".",""); jcair2.replace(",",".");
//        double jc2 = jcair2.toDouble();

//        QString jcair3 = ui->tableWidget_6->item(0,4)->text();
//        jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
//        double jc3 = jcair3.toDouble();
//        qInfo() << "Doubleklik tw 2";
//        if((jc3+jc2)-(jc3+jc)<0){QMessageBox::information(eb_v,"Info....","Nilai yang anda masukkan melebihi pagu"); return;}

        QString tl = ui->tableWidget_2->item(noBrs, 14)->text();
        QDate dt = QDate::fromString(tl,"dd-MM-yyyy");
//        QDateEdit *de = new QDateEdit;
//        de->setDate(dt);
//        de->setDisplayFormat("dd-MM-yyyy");


        QString nosr1 = ui->tableWidget_2->item(noBrs,15)->text();
        QString nosr2 = ui->tableWidget_2->item(noBrs,16)->text();
        QString persent = ui->tableWidget_2->item(noBrs,17)->text();
//        QString pmk = ui->tableWidget_2->item(noBrs,20)->text();
//        QString keu = ui->tableWidget_2->item(noBrs,21)->text();
        QString lap = ui->tableWidget_2->item(noBrs,26)->text();


         //==========================================

         muat_rek(id_kam);
         muat_bend_kp(id_kam);
         muat_k_kp(id_kam);
         tahap();
         no_srt1();
         no_srt2();
         persen();
         sk_bup();
        // sk_menteri();
         kp_dns();

        // le_jml->installEventFilter(eb_v);
         qbx_no_srt1->addItem(nosr1);
         qbx_no_srt1->addItems(li_srt1);
         qbx_no_srt1->setEditable(true);

         qbx_no_srt2->addItem(nosr2);
         qbx_no_srt2->addItems(li_srt2);
         qbx_no_srt2->setEditable(true);

         qbx_persen->addItem(persent);
         qbx_persen->addItems(li_persen);
         qbx_persen->setEditable(true);

//     qbx_sk_pmk->addItem(pmk);
//     qbx_sk_pmk->addItems(li_menteri);

//     qbx_sk_keu->addItem(lap);
//     qbx_sk_keu->addItems(li_menteri);

         c =new QPushButton;
         c->setText("c");
         c->setMaximumWidth(22);

         qbx_thp_penc = new QComboBox;
         qbx_thp_l = new QComboBox;
         qbx_thp_penc->setEditable(true);
         qbx_thp_l->setEditable(true);
         qbx_thp_l->addItem(lap);
         qbx_thp_penc->addItem(thp_tw);
         qbx_thp_penc->addItems(li_tahap);
         qbx_thp_l->addItems(li_tahap);
         de_tgl_terima = new QDateEdit;
         de_tgl_terima->setDate(dt);
         de_tgl_terima->setDisplayFormat("dd-MM-yyyy");
         de_tgl_terima->setCalendarPopup(true);
         de_tgl_terima->setMaximumWidth(80);

         btn1= new QPushButton;
         btn1->setText("UBAH");
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
//         QLabel *sk1= new QLabel; sk1->setText("SK Menteri PMK :"); sk1->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
//         sk1->setMaximumWidth(150);
//         QLabel *sk2= new QLabel; sk2->setText("SK Menteri Keu :"); sk2->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
//         sk2->setMaximumWidth(150);

         eb_v = new QWidget;
         QIcon logo(":/gbr/html/gbr/yhk.png");
         eb_v->setWindowIcon(logo);
         eb_v->setWindowTitle("Mengubah Data....");
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
//     QHBoxLayout *HL9 = new QHBoxLayout;
//     QHBoxLayout *HL10 = new QHBoxLayout;

         VL->addLayout(HL1);
         VL->addLayout(HL2);
         VL->addLayout(HL3);
         VL->addLayout(HL4);
         VL->addLayout(HL5);
         VL->addLayout(HL6);
         VL->addLayout(HL7);
         VL->addLayout(HL8);
//     VL->addLayout(HL9);
//     VL->addLayout(HL10);

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

         HL8->addWidget(btn1);
         eb_v->show();

        connect(btn1, SIGNAL(pressed()), this, SLOT(click_btn1()));
        connect(c, SIGNAL(pressed()), this, SLOT(active_eb_v()));

}
}}

void Form::event_doubleklik_tw_9()  //Double Klik Edit data realisasi add double klik tw 9
{

//    disconnect(btn1, SIGNAL(pressed()), this, SLOT(click_btn2()));
//    disconnect(c, SIGNAL(pressed()), this, SLOT(active_eb_v_2()));

   //Info() << "Doubleklik tw 9";
    if(menu=="3"){ menu2="4";
        QString id_dis = qbx_id_dis->currentText();
        QString id_kam = qbx_id_kam->currentText();
        if(id_dis==""&& id_kam==""){QMessageBox::information(this,"Info","Pilih Distrik dan Kampung..."); return;}
        int noBrs = ui->tableWidget_9->currentRow();

        if(noBrs>=0){
// Qbox untuk di Tambah Pencairan
            le_jml = new QLineEdit;
            qbx_no_srt1 = new QComboBox;
            qbx_no_srt2 = new QComboBox;
            qbx_persen= new QComboBox;
            qbx_thp_penc = new QComboBox;
            qbx_thp_l = new QComboBox;

            qbx_sk_pmk= new QComboBox;
            qbx_sk_keu= new QComboBox;

            le_jml->setClearButtonEnabled(true);
            le_jml->setText( ui->tableWidget_9->item(noBrs,12)->text());
            le_jml->setMaximumWidth(190);


            //========== Data dari table widget ================
            QString thp_tw = ui->tableWidget_9->item(noBrs,11)->text();

//            QString jcair = le_jml->text();
//            jcair.replace("Rp ",""); jcair.replace(".",""); jcair.replace(",",".");
//            double jc = jcair.toDouble();

//            QString jcair2 = ui->tableWidget_9->item(noBrs,12)->text();
//            jcair2.replace("Rp ",""); jcair2.replace(".",""); jcair2.replace(",",".");
//            double jc2 = jcair2.toDouble();

//            QString jcair3 = ui->tableWidget_11->item(0,4)->text();
//            jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
//            double jc3 = jcair3.toDouble();
//            qInfo() << "Doubleklik tw 2";
//            if((jc3+jc2)-(jc3+jc)<0){QMessageBox::information(eb_v,"Info....","Nilai yang anda masukkan melebihi pagu"); return;}

            QString tl = ui->tableWidget_9->item(noBrs, 14)->text();
            QDate dt = QDate::fromString(tl,"dd-MM-yyyy");
//        QDateEdit *de = new QDateEdit;
//        de->setDate(dt);
//        de->setDisplayFormat("dd-MM-yyyy");

            QString nosr1 = ui->tableWidget_9->item(noBrs,15)->text();
            QString nosr2 = ui->tableWidget_9->item(noBrs,16)->text();
            QString persent = ui->tableWidget_9->item(noBrs,17)->text();
            QString lap = ui->tableWidget_9->item(noBrs,26)->text();

             //==========================================

             muat_rek(id_kam);
             muat_bend_kp(id_kam);
             muat_k_kp(id_kam);
             tahap_add();
             no_srt1();
             no_srt2();
             persen();
             sk_bup();
            // sk_menteri();
             kp_dns();

            // le_jml->installEventFilter(eb_v);
             qbx_no_srt1->addItem(nosr1);
             qbx_no_srt1->addItems(li_srt1);
             qbx_no_srt1->setEditable(true);

             qbx_no_srt2->addItem(nosr2);
             qbx_no_srt2->addItems(li_srt2);
             qbx_no_srt2->setEditable(true);

             qbx_persen->addItem(persent);
             qbx_persen->addItems(li_persen);
             qbx_persen->setEditable(true);

             c =new QPushButton;
             c->setText("c");
             c->setMaximumWidth(22);


             qbx_thp_penc->setEditable(true);
             qbx_thp_l->setEditable(true);
             qbx_thp_l->addItem(lap);
             qbx_thp_penc->addItem(thp_tw);
             qbx_thp_penc->addItems(li_tahap_add);
             qbx_thp_l->addItems(li_tahap_add);
             de_tgl_terima = new QDateEdit;
             de_tgl_terima->setDate(dt);
             de_tgl_terima->setDisplayFormat("dd-MM-yyyy");
             de_tgl_terima->setCalendarPopup(true);
             de_tgl_terima->setMaximumWidth(80);

             btn1= new QPushButton;
             btn1->setText("UBAH");
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

             eb_v = new QWidget;
             QIcon logo(":/gbr/html/gbr/yhk.png");
             eb_v->setWindowIcon(logo);
             eb_v->setWindowTitle("Mengubah Data....");
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
    //         QHBoxLayout *HL9 = new QHBoxLayout;
    //         QHBoxLayout *HL10 = new QHBoxLayout;

             VL->addLayout(HL1);
             VL->addLayout(HL2);
             VL->addLayout(HL3);
             VL->addLayout(HL4);
             VL->addLayout(HL5);
             VL->addLayout(HL6);
             VL->addLayout(HL7);
             VL->addLayout(HL8);
    //         VL->addLayout(HL9);
    //         VL->addLayout(HL10);

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

    //         HL7->addWidget(sk1);
    //         HL7->addWidget(qbx_sk_pmk);

    //         HL8->addWidget(sk2);
    //         HL8->addWidget(qbx_sk_keu);

             HL7->addWidget(thp_l);
             HL7->addWidget(qbx_thp_l);

             HL8->addWidget(btn1);

            //connect(qbx_thp_penc, SIGNAL( currentChanged(int)), eb_v, SLOT(eventQxadd()));
            //connect(qbx_thp_penc, SIGNAL (currentIndexChanged(int)), this, SLOT(eventQxadd()));
            connect(qbx_thp_penc, SIGNAL(activated(int)), this, SLOT(eventQbxadd()));
            connect(btn1, SIGNAL(pressed()), this, SLOT(click_btn2()));
            connect(c, SIGNAL(pressed()), this, SLOT(active_eb_v_2()));
            //connect(c, SIGNAL(pressed()), this, SLOT(eventQbxadd()));

            eb_v->show();
    }}
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

void Form::eventQbxadd()
{
    //Info() << "event Qbx dds";
      QString   thp = qbx_thp_penc->currentText();

      muatTahap(thp);
      qbx_thp_l->clear();
      qbx_persen->clear();
      qbx_thp_l->addItem(laporan);
      qbx_persen->addItem(persen_2);
      le_jml->clear();
      QString per = persen_2.left(2);
      QString thn2 = thp.right(4);
      QString pagu = ui->tableWidget_6->item(0,3)->text();
      pagu.replace("Rp ",""); pagu.replace(".",""); pagu.replace(",",".");

      double pg = pagu.toDouble();
      double pe = per.toDouble();
      double hs1 = pe/100;
      double hs = hs1 * pg;
      qInfo() << pg;
      QString j = indo.toCurrencyString(hs, "Rp ");
      le_jml->setText(j);

}

void Form::eventQbxadd_2()
{
    ///nfo() << "event Qbx add";
     QString   thp = qbx_thp_penc->currentText();

     muatTahap_2(thp);
     qbx_thp_l->clear();
     qbx_persen->clear();
     qbx_thp_l->addItem(laporan_2);
     qbx_persen->addItem(persen_2_2);
     le_jml->clear();
     QString per = persen_2_2.left(2);
     QString thn2 = thp_2.right(4);
    //Info() << "Tahap thn 2 add " << thn2;
     QString pagu = ui->tableWidget_11->item(0,3)->text();
     pagu.replace("Rp ",""); pagu.replace(".",""); pagu.replace(",",".");

     double pg = pagu.toDouble();
     double pe = per.toDouble();
     double hs1 = pe/100;
     double hs = hs1 * pg;
     //nfo() << pg;
     QString j = indo.toCurrencyString(hs, "Rp ");
     le_jml->setText(j);
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

//disconnect(btn1, SIGNAL(pressed()), this, SLOT(click_btn2()));
//disconnect(c, SIGNAL(pressed()), this, SLOT(active_eb_v_2()));

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

//         qInfo() <<ui->comboBox->currentText()<<"++"<<ui->comboBox_nmKampung->currentText()<<"++" <<id_dis<<"++" <<id_kam<<"++"<<no_rek<<"++" << nm_rek << "++" << nm_bank
//                     <<"++"<< j_bend <<"++" << j_kp <<"++" << nm_kp << "++" << nm_bend <<"++" << tahap << "\n"
//                     <<tgl_ter <<"++"<< no_srt1 <<"++"<< no_srt2<<"++" << persen<<"++" << sk_bup<<"++" << sk_kam<<"++" << nm_kp_dns<<"++" << pg_kp_dns <<"++"<< nip_kp_dns
//                     <<"++ \n" << pg_kp_dns <<"++" << sk_pmk << "++" << sk_keu  << "++" << j_kk <<"++" << terbil;

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
//      qbx_sk_pmk= new QComboBox;
//      qbx_sk_pmk->addItems(li_menteri);
//      qbx_sk_keu= new QComboBox;
//      qbx_sk_keu->addItems(li_menteri);

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


void Form::sppd() // Form Cetak Surat Pengantar Pencairan
{

//disconnect(btnsppd, SIGNAL(pressed()), this, SLOT(sppdbtc()));
muat_sppd();
muat_sppd_2();
//nfo()<<"Menu cetak Pengantar dds";
QDate dt = QDate::currentDate();
desppd = new QDateEdit;
desppd->setDate(dt);
desppd->setDisplayFormat("dd-MM-yyyy");
desppd->setCalendarPopup(true);

QString nosrt;
QString tgl = desppd->text();
QString thn = tgl.right(4);
qbnosppd = new QComboBox;
qbhal = new QComboBox;
qbnosppd->setEditable(true);
qbhal->setEditable(true);
qbnosppd->addItems(li_sppd);
qbhal->addItems(li_sppd_2);

wg = new QWidget;

QLabel *nsrt = new QLabel;
nsrt->setText("No Surat :"); nsrt->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
nsrt->setMaximumWidth(95);
QLabel *hal= new QLabel; hal->setText("Perihal :"); hal->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
hal->setMaximumWidth(95);
QLabel *tgll= new QLabel; tgll->setText("Tanggal :"); tgll->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
tgll->setMaximumWidth(95);

QLabel *ls1 = new QLabel; ls1->setMinimumWidth(65);
QIcon logo(":/gbr/html/gbr/yhk.png");
wg->setWindowIcon(logo);
wg->setWindowTitle("Surat Pengantar Pencairan Dana Desa "+thn+"");
wg->setMinimumWidth(400);
wg->setMaximumWidth(400);
wg->setMaximumHeight(800);

QVBoxLayout *VL = new QVBoxLayout(wg);
QHBoxLayout *HL1 = new QHBoxLayout;
QHBoxLayout *HL2 = new QHBoxLayout;
QHBoxLayout *HL3 = new QHBoxLayout;
QHBoxLayout *HL4 = new QHBoxLayout;

VL->addLayout(HL1);
VL->addLayout(HL2);
VL->addLayout(HL3);
VL->addLayout(HL4);

HL1->addWidget(nsrt);
HL1->addWidget(qbnosppd);

HL2->addWidget(hal);
HL2->addWidget(qbhal);

HL3->addWidget(tgll);
desppd->setMaximumWidth(140);
HL3->addWidget(desppd);
HL3->addWidget(ls1);

btnsppd->setText("SIMPAN");
btnsppd->setMaximumWidth(140);
HL4->addWidget(btnsppd);
wg->show();

connect(btnsppd, SIGNAL(pressed()), this, SLOT(sppdbtc()));

}

void Form::sppdbtc()
{
    QString nosrt = qbnosppd->currentText();
    QString hal = qbhal->currentText();
    QString date = desppd->text();

    instsppd(nosrt, hal, date);
    disconnect(btnsppd, SIGNAL(pressed()), this, SLOT(sppdbtc()));
}

void Form::instsppd(QString nosrt, QString hal,QString date)
{
QString a = nosrt;
a.replace("****",nosurat());
QString b = hal;
QString c = date;
QDate dt = QDate::fromString(c,"dd-MM-yyyy");
QString d = dt.toString("yyyy-MM-dd");

//nfo() << "sppd btc ..................."<< a <<"==" << b << "==" << c;
QSqlQuery query;
QString cmd = "INSERT INTO pmk_yhk.sppdd (no_srt,perihal,tgl,nos) VALUES ( :nosrt, :hal, :tgl,'1') ";
query.prepare(cmd);
query.bindValue(":nosrt", a);
query.bindValue(":hal",b);
query.bindValue(":tgl",d);
bool ok = exec(query);
if(!ok){QMessageBox::information(this,"Error","Gagal Menyimpan data nosppd dds");return;} wg->close();
loadsppd();
wg->close();
}

void Form::sppd_2()
{

muat_sppd_();
muat_sppd_2_();
//nfo()<<"Menu cetak Pengantar add";
QDate dt = QDate::currentDate();
desppd_2 = new QDateEdit;
desppd_2->setDate(dt);
desppd_2->setDisplayFormat("dd-MM-yyyy");
desppd_2->setCalendarPopup(true);

QString nosrt;
QString tgl = desppd_2->text();
QString thn = tgl.right(4);
qbnosppd_2 = new QComboBox;
qbhal_2 = new QComboBox;
qbnosppd_2->setEditable(true);
qbhal_2->setEditable(true);
qbnosppd_2->addItems(li_sppd_);
qbhal_2->addItems(li_sppd_2_);
wg_2 = new QWidget;

QLabel *nsrt = new QLabel;
nsrt->setText("No Surat :"); nsrt->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
nsrt->setMaximumWidth(95);
QLabel *hal= new QLabel; hal->setText("Perihal :"); hal->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
hal->setMaximumWidth(95);
QLabel *tgll= new QLabel; tgll->setText("Tanggal :"); tgll->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
tgll->setMaximumWidth(95);

QLabel *ls1 = new QLabel; ls1->setMinimumWidth(65);

QIcon logo(":/gbr/html/gbr/yhk.png");
wg_2->setWindowIcon(logo);
wg_2->setWindowTitle("Surat Pengantar Pencairan Alokasi Dana Desa "+thn+"");
wg_2->setMinimumWidth(400);
wg_2->setMaximumWidth(400);
wg_2->setMaximumHeight(800);

QVBoxLayout *VL = new QVBoxLayout(wg_2);
QHBoxLayout *HL1 = new QHBoxLayout;
QHBoxLayout *HL2 = new QHBoxLayout;
QHBoxLayout *HL3 = new QHBoxLayout;
QHBoxLayout *HL4 = new QHBoxLayout;

VL->addLayout(HL1);
VL->addLayout(HL2);
VL->addLayout(HL3);
VL->addLayout(HL4);

HL1->addWidget(nsrt);
HL1->addWidget(qbnosppd_2);

HL2->addWidget(hal);
HL2->addWidget(qbhal_2);

HL3->addWidget(tgll);
desppd_2->setMaximumWidth(140);
HL3->addWidget(desppd_2);
HL3->addWidget(ls1);

btnsppd_2->setText("SIMPAN");
btnsppd_2->setMaximumWidth(140);
HL4->addWidget(btnsppd_2);

connect(btnsppd_2, SIGNAL(pressed()), this, SLOT(sppdbtc_2()));
wg_2->show();
}

void Form::sppdbtc_2()
{
    QString nosrt = qbnosppd_2->currentText();
    QString hal = qbhal_2->currentText();
    QString date = desppd_2->text();
    instsppd_2(nosrt,hal,date);
    disconnect(btnsppd_2, SIGNAL(pressed()), this, SLOT(sppdbtc_2()));
}

void Form::qbx3event()
{
if(menu=="4" && ui->comboBox_realisasi->currentIndex()==0){ qInfo() << "sppd 1111111"; loadsppd(); }
if(menu=="4" && ui->comboBox_realisasi->currentIndex()==1){ qInfo() << "sppd 2222222"; loadsppd_2();}

}


void Form::instsppd_2(QString nosrt, QString hal,QString date)
{

QString a = nosrt;
a.replace("****",nosurat());
QString b = hal;
QString c = date;
QDate dt = QDate::fromString(c,"dd-MM-yyyy");
QString d = dt.toString("yyyy-MM-dd");

//nfo() << "sppd btc 2 2 ..................."<< a <<"==" << b << "==" << c;
QSqlQuery query;
QString cmd = "INSERT INTO pmk_yhk.sppdd_2 (no_srt,perihal,tgl,nos) VALUES (:nosrt, :hal, :tgl, '1') ";
query.prepare(cmd);
query.bindValue(":nosrt", a);
query.bindValue(":hal",b);
query.bindValue(":tgl",d);
bool ok = exec(query);
if(!ok){QMessageBox::information(this,"Error","Gagal Menyimpan data nosppd add");return;} wg_2->close();
loadsppd_2();
wg_2->close();
}

void Form::loadsppd()
{
    if(ui->comboBox_realisasi->currentIndex()==0){
    while(ui->tableWidget_13->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_13->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.sppdd ORDER BY id";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error","Gagal Memuat data nosppd dds");return;}
    int i=0;
    while(query.next())
    {
                ui->tableWidget_13->insertRow(i);
                QTableWidgetItem *no_ = new QTableWidgetItem;
                QTableWidgetItem *nosrt_ = new QTableWidgetItem;
                QTableWidgetItem *hal_ = new QTableWidgetItem;
                QTableWidgetItem *tgl_ = new QTableWidgetItem;

                no_->setText(query.value(0).toString());
                nosrt_->setText(query.value(1).toString());
                hal_->setText(query.value(2).toString());
                QDate dt = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
                tgl_->setText(dt.toString("dd-MM-yyyy"));

                no_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                nosrt_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                hal_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                tgl_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                ui->tableWidget_13->setItem(i,0,no_);
                ui->tableWidget_13->setItem(i,1,nosrt_);
                ui->tableWidget_13->setItem(i,2,hal_);
                ui->tableWidget_13->setItem(i,3,tgl_);
                i++;
    }}
}

void Form::loadsppd_2()
{
    if(ui->comboBox_realisasi->currentIndex()==1){
    while(ui->tableWidget_13->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_13->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.sppdd_2 ORDER BY id";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error","Gagal Memuat data nosppd dds");return;}
    int i=0;
    while(query.next())
    {
                ui->tableWidget_13->insertRow(i);
                QTableWidgetItem *no_ = new QTableWidgetItem;
                QTableWidgetItem *nosrt_ = new QTableWidgetItem;
                QTableWidgetItem *hal_ = new QTableWidgetItem;
                QTableWidgetItem *tgl_ = new QTableWidgetItem;
                no_->setText(query.value(0).toString());
                nosrt_->setText(query.value(1).toString());
                hal_->setText(query.value(2).toString());
                QDate dt = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
                tgl_->setText(dt.toString("dd-MM-yyyy"));

                no_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                nosrt_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                hal_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                tgl_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                ui->tableWidget_13->setItem(i,0,no_);
                ui->tableWidget_13->setItem(i,1,nosrt_);
                ui->tableWidget_13->setItem(i,2,hal_);
                ui->tableWidget_13->setItem(i,3,tgl_);
                i++;
    }}
}


void Form::headsppd()
{
    QStringList headerWidget;
    ui->tableWidget_13->setColumnCount(4);
    headerWidget <<"id" <<"No Surat"<<"Perihal" <<"Tanggal ";
    ui->tableWidget_13->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_13->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_13->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget_13->setColumnHidden(0,true);
    ui->tableWidget_13->setColumnWidth(1,230);
    ui->tableWidget_13->setColumnWidth(2,330);
    ui->tableWidget_13->setColumnWidth(3,70);
}

void Form::headsppd_2() // di gunakan double klik tw 13 ..........................................
{
    //nfo() << "qbx  ....................... ";
}

void Form::even_klik_tw13()
{
 //Info() << "qbx klik  ....................... ";
}

void Form::even_dklik_tw13() // Double Klik Event ...................................... ubah sppd
{
//disconnect(btnsppd_dk, SIGNAL(pressed()), this, SLOT(updatesppd()));
muat_sppd();
muat_sppd_2();
  if(ui->comboBox_realisasi->currentIndex()==0)
  {
    //qInfo() << "qbx  double klik 11....................... ";
      int b = ui->tableWidget_13->currentRow();
      QString id = ui->tableWidget_13->item(b,0)->text();
      QString nosrt = ui->tableWidget_13->item(b,1)->text();
      QString hal = ui->tableWidget_13->item(b,2)->text();
      QString tgl = ui->tableWidget_13->item(b,3)->text();

      QDate dt = QDate::fromString(tgl,"dd-MM-yyyy");
      desppd = new QDateEdit;
      desppd->setDate(dt);
      desppd->setDisplayFormat("dd-MM-yyyy");
      desppd->setCalendarPopup(true);

      QString thn = tgl.right(4);
      qbnosppd = new QComboBox;
      qbhal = new QComboBox;
      qbnosppd->setEditable(true);
      qbhal->setEditable(true);
      qbnosppd->addItem(nosrt);
      qbnosppd->addItems(li_sppd);
      qbhal->addItem(hal);
      qbhal->addItems(li_sppd_2);

      wg = new QWidget;

      QLabel *nsrt = new QLabel;
      nsrt->setText("No Surat :"); nsrt->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
      nsrt->setMaximumWidth(95);
      QLabel *hal_= new QLabel; hal_->setText("Perihal :"); hal_->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
      hal_->setMaximumWidth(95);
      QLabel *tgll= new QLabel; tgll->setText("Tanggal :"); tgll->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
      tgll->setMaximumWidth(95);

      QLabel *ls1 = new QLabel; ls1->setMinimumWidth(65);
      QIcon logo(":/gbr/html/gbr/yhk.png");
      wg->setWindowIcon(logo);
      wg->setWindowTitle("Ubah Surat Pengantar Pencairan Dana Desa "+thn+"");
      wg->setMinimumWidth(400);
      wg->setMaximumWidth(400);
      wg->setMaximumHeight(800);

      QVBoxLayout *VL = new QVBoxLayout(wg);
      QHBoxLayout *HL1 = new QHBoxLayout;
      QHBoxLayout *HL2 = new QHBoxLayout;
      QHBoxLayout *HL3 = new QHBoxLayout;
      QHBoxLayout *HL4 = new QHBoxLayout;

      VL->addLayout(HL1);
      VL->addLayout(HL2);
      VL->addLayout(HL3);
      VL->addLayout(HL4);

      HL1->addWidget(nsrt);
      HL1->addWidget(qbnosppd);

      HL2->addWidget(hal_);
      HL2->addWidget(qbhal);

      HL3->addWidget(tgll);
      desppd->setMaximumWidth(140);
      HL3->addWidget(desppd);
      HL3->addWidget(ls1);

      btnsppd_dk->setText("UBAH");
      btnsppd_dk->setMaximumWidth(140);
      HL4->addWidget(btnsppd_dk);
      wg->show();
      connect(btnsppd_dk, SIGNAL(pressed()), this, SLOT(updatesppd()));

  }

  if(ui->comboBox_realisasi->currentIndex()==1)
  {

    //qInfo() << "qbx  double klik 22....................... ";
      int b = ui->tableWidget_13->currentRow();
      QString id = ui->tableWidget_13->item(b,0)->text();
      QString nosrt = ui->tableWidget_13->item(b,1)->text();
      QString hal = ui->tableWidget_13->item(b,2)->text();
      QString tgl = ui->tableWidget_13->item(b,3)->text();

      QDate dt = QDate::fromString(tgl,"dd-MM-yyyy");
      desppd_2 = new QDateEdit;
      desppd_2->setDate(dt);
      desppd_2->setDisplayFormat("dd-MM-yyyy");
      desppd_2->setCalendarPopup(true);

      QString thn = tgl.right(4);
      qbnosppd_2 = new QComboBox;
      qbhal_2 = new QComboBox;
      qbnosppd_2->setEditable(true);
      qbhal_2->setEditable(true);
      qbnosppd_2->addItem(nosrt);
      qbnosppd_2->addItems(li_sppd_);
      qbhal_2->addItem(hal);
      qbhal_2->addItems(li_sppd_2_);

      wg_2 = new QWidget;

      QLabel *nsrt = new QLabel;
      nsrt->setText("No Surat :"); nsrt->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
      nsrt->setMaximumWidth(95);
      QLabel *hal_= new QLabel; hal_->setText("Perihal :"); hal_->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
      hal_->setMaximumWidth(95);
      QLabel *tgll= new QLabel; tgll->setText("Tanggal :"); tgll->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
      tgll->setMaximumWidth(95);

      QLabel *ls1 = new QLabel; ls1->setMinimumWidth(65);

      QIcon logo(":/gbr/html/gbr/yhk.png");
      wg_2->setWindowIcon(logo);
      wg_2->setWindowTitle("Ubah Surat Pengantar Pencairan Dana Desa "+thn+"");
      wg_2->setMinimumWidth(400);
      wg_2->setMaximumWidth(400);
      wg_2->setMaximumHeight(800);

      QVBoxLayout *VL = new QVBoxLayout(wg_2);
      QHBoxLayout *HL1 = new QHBoxLayout;
      QHBoxLayout *HL2 = new QHBoxLayout;
      QHBoxLayout *HL3 = new QHBoxLayout;
      QHBoxLayout *HL4 = new QHBoxLayout;

      VL->addLayout(HL1);
      VL->addLayout(HL2);
      VL->addLayout(HL3);
      VL->addLayout(HL4);

      HL1->addWidget(nsrt);
      HL1->addWidget(qbnosppd_2);

      HL2->addWidget(hal_);
      HL2->addWidget(qbhal_2);

      HL3->addWidget(tgll);
      desppd_2->setMaximumWidth(140);
      HL3->addWidget(desppd_2);
      HL3->addWidget(ls1);

      btnsppd_dk->setText("UBAH");
      btnsppd_dk->setMaximumWidth(140);
      HL4->addWidget(btnsppd_dk);
      wg_2->show();
      connect(btnsppd_dk, SIGNAL(pressed()), this, SLOT(updatesppd()));
  }
}

void Form::updatesppd() // Update sppd
{
    if(ui->comboBox_realisasi->currentIndex()==0)
    {
       int b = ui->tableWidget_13->currentRow();
       QString id = ui->tableWidget_13->item(b,0)->text();
       QString nosrt = qbnosppd->currentText();
       QString hal = qbhal->currentText();
       QString tgl =  desppd->text();
       QDate dt = QDate::fromString(tgl,"dd-MM-yyyy");
       QString tgl_ = dt.toString("yyyy-MM-dd");
       //nfo()<< " info 000" << nosrt << "==" << hal << "==" << tgl;
       QSqlQuery  query;
       QString cmd = " UPDATE pmk_yhk.sppdd SET no_srt = :no_srt, perihal= :hal, tgl= :tgl  WHERE id= :id " ;
       query.prepare(cmd);
       query.bindValue(":no_srt",nosrt);
       query.bindValue(":hal",hal);
       query.bindValue(":tgl",tgl_);
       query.bindValue(":hal",hal);
       query.bindValue(":id",id);
       bool ok = exec(query);
       if(!ok){QMessageBox::information(this,"Error","Gagal Update data Surat Pencairan dds "+query.lastError().text()+""); return;}
       wg->close();
       loadsppd();

    }

    if(ui->comboBox_realisasi->currentIndex()==1)
    {
        int b = ui->tableWidget_13->currentRow();
        QString id = ui->tableWidget_13->item(b,0)->text();
        QString nosrt = qbnosppd_2->currentText();
        QString hal = qbhal_2->currentText();
        QString tgl =  desppd_2->text();
        QDate dt = QDate::fromString(tgl,"dd-MM-yyyy");
        QString tgl_ = dt.toString("yyyy-MM-dd");

        //nfo()<< " info 111" << nosrt << "==" << hal << "==" << tgl;
        QSqlQuery  query;
        QString cmd = " UPDATE pmk_yhk.sppdd_2 SET no_srt = :no_srt, perihal= :hal, tgl= :tgl  WHERE id= :id " ;
        query.prepare(cmd);
        query.bindValue(":no_srt",nosrt);
        query.bindValue(":hal",hal);
        query.bindValue(":tgl",tgl_);
        query.bindValue(":id",id);
        bool ok = exec(query);
        if(!ok){QMessageBox::information(this,"Error","Gagal Update data Surat Pencairan add "+query.lastError().text()+""); return;}
        wg_2->close();
        loadsppd_2();
    }
    disconnect(btnsppd_dk, SIGNAL(pressed()), this, SLOT(updatesppd()));
}



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
//nfo() << "Active eb v.....2222222222,,,,,,,,,,,,,,,,,,,,,,,,,,,,,llllllllllllll" << terbilang;
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


void Form::click_btn1() // Even Klik Save di tambah realisasi dana desa
{
    if(menu2=="1"){
        if(menu=="2"){
        active_eb_v();
        QString id_dis = qbx_id_dis->currentText();
        QString id_kam = qbx_id_kam->currentText();
        QString j = le_jml->text();
        if(j=="Rp 0,00"){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh Nilai Rp 0,00,-"); return;}
        if(j==""){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong,-"); return;}
       //Info()  << "qinfo j pada btn1" << j;
        QString jj_ = j;
        j.replace("Rp ",""); j.replace(".",""); j.replace(",",".");
       //qInfo()  << "qinfo j pada btn1 replace" << j;
        bilang(j);
         QString jcair3 = ui->tableWidget_6->item(0,4)->text();
        jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
        double jc3 = jcair3.toDouble();
        double jj = j.toDouble();

        if((jc3-jj)<0){QMessageBox::information(eb_v,"Info...","Mohon Periksa Ketersediaan Dana"); return;}
        //QString jj_ = QString::number(jj);
        //bilang(jj_);
        jj_.replace("Rp ",""); jj_.replace(".",""); jj_.replace(",",".");
       //Info()  << "qinfo j pada btn1 pada jj_" << jj_;
        muat_rek(id_kam);
        muat_bend_kp(id_kam);

        tahap();
        no_srt1();
        no_srt2();
        persen();
        sk_bup();
        //sk_menteri();
        kp_dns();

        QDate dt = QDate::fromString(de_tgl_terima->text(),"dd-MM-yyyy");
        QDateEdit *de = new QDateEdit;
        de->setDate(dt);
        de->setDisplayFormat("yyyy-MM-dd");

        QString terbil = terbilang;
        QString no_rek = li_norek.at(0);
        QString nm_rek  = li_nm_rek.at(0);
        QString nm_bank = li_nm_bank.at(0);
        QString j_bend = s_sk_bend;
        QString j_kp = s_sk_kp;
        QString nm_kp = s_nm_kp;
       //Info()<< "Nama Kepala Kampung<<" << nm_kp;
        QString nm_bend = s_nm_bend;

        //QString tahap = qbx_thp_penc->currentText();
        QString tahap = tahap_;

        QString tahap_l = qbx_thp_l->currentText();
        QString tgl_ter = de->text();

        QString no_srt1 =qbx_no_srt1->currentText();
        QString no_srt2 =qbx_no_srt2->currentText();
        QString persen = qbx_persen->currentText();
        QString sk_bup = li_sk_bup.at(0); //=====================
        QString sk_kam = s_sk_kp;

        QString nm_kp_dns = li_kp_dns.at(0);
        QString pg_kp_dns = li_kp_dns.at(1);
        QString nip_kp_dns = li_kp_dns.at(2);

        QString thn2 = thp.right(4);
    //    QString sk_pmk = qbx_sk_pmk->currentText();
    //    QString sk_keu =qbx_sk_keu->currentText();
        QString j_kk = li_j_kk.at(0); //====================
        QString rp ="rupiah";
        QPixmap logo;
        QIcon icon(":/icon/gbr/kcl.png");
        QMessageBox boxPesan;
        boxPesan.setWindowIcon(icon);
        boxPesan.setWindowTitle("Info...!!!");
        boxPesan.setText("<b>Menambah data Realisasi Kampung "+ui->comboBox_nmKampung->currentText()+" </b>");
        boxPesan.setInformativeText("Pastikan data yang anda input sudah sesuai...");
        QAbstractButton * pBhapus = boxPesan.addButton(tr(" LANJUT "), QMessageBox::YesRole) ;
         boxPesan.addButton(tr(" BATAL "), QMessageBox::NoRole);
         if (logo.load(":/icon/gbr/kcl.png"))
         boxPesan.setIconPixmap(logo);
         boxPesan.exec();
         if(boxPesan.clickedButton() == pBhapus)
           {

             if(open()==false){open();}
             QSqlQuery query;
             begin();
             QString cmd = "INSERT INTO pmk_yhk.t_real (id_kam,nm_dis,nm_kam, no_rek, nm_rek,nm_bank, sk_bend, sk_kp, nm_ben, nm_kp, thp_cair, j_cair, j_terbilang, "
                                       "tgl, no_srt1, no_srt2,persentase,sk_bup,sk_kam,sk_pmk,sk_keu, nm_kpd, j_kpd, nip_kpd, ket, thp_l, jkk, nos) VALUES "
                                        "(:id_kam, :nm_dis, :nm_kam, :no_rek, :nm_rek, :nm_bank, :sk_bend, :sk_kp, :nm_ben, :nm_kp, :thp_cair, :j_cair, :j_terbilang, "
                                        ":tgl, :no_srt1, :no_srt2, :persentase, :sk_bup, :sk_kam, :sk_pmk, :sk_keu, :nm_kpd, :j_kpd, :nip_kpd, :ket, :thp_l, :jkk, '2' )";

             query.prepare(cmd);
             query.bindValue(":id_kam",qbx_id_kam->currentText());
             query.bindValue(":nm_dis", ui->comboBox->currentText());
             query.bindValue(":nm_kam",ui->comboBox_nmKampung->currentText());
             query.bindValue(":no_rek",no_rek);
             query.bindValue(":nm_rek",nm_rek);
             query.bindValue(":nm_bank",nm_bank);
             query.bindValue(":sk_bend",j_bend);
             query.bindValue(":sk_kp",j_kp);
             query.bindValue(":nm_ben",nm_bend);
             query.bindValue(":nm_kp",nm_kp);
             query.bindValue(":thp_cair", tahap );
             query.bindValue(":j_cair",jj_);
             query.bindValue(":j_terbilang",terbilang+rp);
             query.bindValue(":tgl",tgl_ter);
             no_srt1.replace("****",nosurat());
             query.bindValue(":no_srt1",no_srt1);
             no_srt2.replace("####",nosurat_2());
             query.bindValue(":no_srt2",no_srt2);
             query.bindValue(":persentase",persen);
             query.bindValue(":sk_bup",sk_bup);
             query.bindValue(":sk_kam",sk_kam);
    //         query.bindValue(":sk_pmk",sk_pmk);
    //         query.bindValue(":sk_keu",sk_keu);
             query.bindValue(":nm_kpd",nm_kp_dns);
             query.bindValue(":j_kpd",pg_kp_dns);
             query.bindValue(":nip_kpd",nip_kp_dns);
             query.bindValue(":ket",thn2);
             query.bindValue(":thp_l",tahap_l);
             query.bindValue(":jkk",j_kk);

             bool ok = exec(query);
             if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Menambah realisasi , "+query.lastError().text()+"");}
             commit();
                eb_v->close();
            }else { return; }
         menu="2";

         if(menu=="2"){
         QString id = qbx_id_kam->currentText();
         muat_real(id);}
        // Qnfo()<<"Menu2 = 2 dan menu=2";
                  } }

    //================ 2 ======================

    if(menu2=="2"){
        int noBrs = ui->tableWidget_2->currentRow();

        // if(menu=="2"){
        active_eb_v();
        QString id_dis = qbx_id_dis->currentText();
        QString id_kam = qbx_id_kam->currentText();
        QString j = le_jml->text();

        QString jcair = le_jml->text();
        jcair.replace("Rp ",""); jcair.replace(".",""); jcair.replace(",",".");
        double jc = jcair.toDouble();

        QString jcair2 = ui->tableWidget_2->item(noBrs,12)->text();
        jcair2.replace("Rp ",""); jcair2.replace(".",""); jcair2.replace(",",".");
        double jc2 = jcair2.toDouble();

        QString jcair3 = ui->tableWidget_6->item(0,4)->text();
        jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
        double jc3 = jcair3.toDouble();
        //nfo() << "Doubleklik tw 2";
        if(jc<jc2){ if(((jc3+jc2)-jc)<0){QMessageBox::information(eb_v,"Info....","Nilai yang anda masukkan melebihi pagu .. "); return;}}
        if(jc>jc2){ if(0>((jc3+jc2)-jc)){QMessageBox::information(eb_v,"Info....","Nilai yang anda masukkan melebihi pagu .. "); return;}}

        if(j=="Rp 0,00"){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh Nilai Rp 0,00,-"); return;}
        if(j==""){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong,-"); return;}

        j.replace("Rp ",""); j.replace(".",""); j.replace(",",".");
       //Info()<< "isi Bilang j" << j;
        QString jj_ = j;
        bilang(j);
        // QString jcair3 = ui->tableWidget_6->item(0,4)->text();
        jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
        //double jc3 = jcair3.toDouble();
        //double jj = j.toDouble();
        // if((jc3-jj)<0){QMessageBox::information(eb_v,"Info...","Mohon Periksa Ketersediaan Dana"); return;}
        //QString jj_ = QString::number(jj);
       //Info()<< "isi Bilang jj_" << jj_;
        //bilang(jj_);

        QString id=ui->tableWidget_2->item(noBrs,27)->text();

        QString terbil = terbilang;
        QString thp_cair = qbx_thp_penc->currentText();
        QString j_cair  = jj_;

        QDate dtt = QDate::fromString(de_tgl_terima->text(),"dd-MM-yyyy");
        QString tgl = dtt.toString("yyyy-MM-dd");

        QString no_srt1 = qbx_no_srt1->currentText();
        qbx_no_srt1->setEditable(true);
        QString no_srt2 = qbx_no_srt2->currentText();
        qbx_no_srt2->setEditable(true);
        QString persen = qbx_persen->currentText();
        qbx_persen->setEditable(true);
        QString thp_l = qbx_thp_l->currentText();
        qbx_thp_l->setEditable(true);

        QString rp ="rupiah";
        QPixmap logo;
        QIcon icon(":/icon/gbr/kcl.png");
        QMessageBox boxPesan;
        boxPesan.setWindowIcon(icon);
        boxPesan.setWindowTitle("Info...!!!");
        boxPesan.setText("<b>Mengubah data Realisasi Kampung "+ui->comboBox_nmKampung->currentText()+" </b>");
        boxPesan.setInformativeText("Pastikan data yang anda input sudah sesuai...");
        QAbstractButton * pBhapus = boxPesan.addButton(tr(" UBAH "), QMessageBox::YesRole) ;
         boxPesan.addButton(tr(" BATAL "), QMessageBox::NoRole);
         if (logo.load(":/icon/gbr/kcl.png"))
         boxPesan.setIconPixmap(logo);
         boxPesan.exec();
         if(boxPesan.clickedButton() == pBhapus)
           {

             if(open()==false){open();}
             QSqlQuery query;
             begin();
             QString cmd = "UPDATE pmk_yhk.t_real SET thp_cair = :thp_cair,  j_cair = :j_cair, tgl = :tgl , no_srt1 = :no_srt1 , no_srt2= :no_srt2 , persentase= :persen , thp_l = :thp_l, j_terbilang= :terbil WHERE id = :id";

             query.prepare(cmd);
             query.bindValue(":id", id);
             query.bindValue(":thp_cair", thp_cair);
             query.bindValue(":j_cair",jj_);
            query.bindValue(":tgl",tgl);
             query.bindValue(":no_srt1",no_srt1);
             query.bindValue(":no_srt2",no_srt2);
             query.bindValue(":persen",persen);
             query.bindValue(":thp_l",thp_l);
             query.bindValue(":terbil", terbilang+" rupiah");

             bool ok = exec(query);
             if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Mengubah data realisasi , "+query.lastError().text()+""); return;}

             commit();
             QMessageBox::information(eb_v,"Info....","Berhasil Mengubah data realisasi");  eb_v->close();

            }else { return; }
         //menu="2";
         if(menu=="2"){
         QString id = qbx_id_kam->currentText();
         muat_real(id);
         //nfo()<<"menu=2 Saja";
         } // }
}
on_toolButton_refResh_clicked();
}


void Form::click_btn2()// Even Klik Save di tambah realisasi alokasi dana desa
{

    if(menu2=="3"){
    if(menu=="3"){

    active_eb_v_2();
    QString id_dis = qbx_id_dis->currentText();
    QString id_kam = qbx_id_kam->currentText();
    QString j = le_jml->text();
    if(j=="Rp 0,00"){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh Nilai Rp 0,00,-"); return;}
    if(j==""){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong,-"); return;}
    QString jj_ = j;
    j.replace("Rp ",""); j.replace(".",""); j.replace(",",".");
    bilang(j);
    QString jcair3 = ui->tableWidget_11->item(0,4)->text();
    jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
    double jc3 = jcair3.toDouble();
    double jj = j.toDouble();
    if((jc3-jj)<0){QMessageBox::information(eb_v,"Info...","Mohon Periksa Ketersediaan Dana"); return;}
    //QString jj_ = QString::number(jj);
    //bilang(jj_);
     jj_.replace("Rp ",""); jj_.replace(".",""); jj_.replace(",",".");
    muat_rek(id_kam);
    muat_bend_kp(id_kam);
    tahap_add();
    no_srt1();
    no_srt2();
    persen();
    sk_bup();
   // sk_menteri();
    kp_dns();
    QDate dt = QDate::fromString(de_tgl_terima->text(),"dd-MM-yyyy");
    QDateEdit *de = new QDateEdit;
    de->setDate(dt);
    de->setDisplayFormat("yyyy-MM-dd");

    QString terbil = terbilang;
    QString no_rek = li_norek.at(0);
    QString nm_rek  = li_nm_rek.at(0);
    QString nm_bank = li_nm_bank.at(0);
    QString j_bend = s_sk_bend;
    QString j_kp = s_sk_kp;
    QString nm_kp = s_nm_kp;
    QString nm_bend = s_nm_bend;

    //QString tahap = qbx_thp_penc->currentText();
    QString tahap = tahap__2;

    QString tahap_l = qbx_thp_l->currentText();
    QString tgl_ter = de->text();

    QString no_srt1 =qbx_no_srt1->currentText();
    QString no_srt2 =qbx_no_srt2->currentText();
    QString persen = qbx_persen->currentText();
    QString sk_bup = li_sk_bup.at(0); //=====================
    QString sk_kam = s_sk_kp;

    QString nm_kp_dns = li_kp_dns.at(0);
    QString pg_kp_dns = li_kp_dns.at(1);
    QString nip_kp_dns = li_kp_dns.at(2);
    QString thn2 = thp_2.right(4);
   //Info() << "Tahap Tahun 2" << thn2;

//    QString sk_pmk = qbx_sk_pmk->currentText();
//    QString sk_keu =qbx_sk_keu->currentText();
    QString j_kk = li_j_kk.at(0); //====================
    QString rp ="rupiah";
    QPixmap logo;
    QIcon icon(":/icon/gbr/kcl.png");
    QMessageBox boxPesan;
    boxPesan.setWindowIcon(icon);
    boxPesan.setWindowTitle("Info...!!!");
    boxPesan.setText("<b>Menambah data Realisasi Kampung "+ui->comboBox_nmKampung->currentText()+" </b>");
    boxPesan.setInformativeText("Pastikan data yang anda input sudah sesuai...");
    QAbstractButton * pBhapus = boxPesan.addButton(tr(" LANJUT "), QMessageBox::YesRole) ;
     boxPesan.addButton(tr(" BATAL "), QMessageBox::NoRole);
     if (logo.load(":/icon/gbr/kcl.png"))
     boxPesan.setIconPixmap(logo);
     boxPesan.exec();
     if(boxPesan.clickedButton() == pBhapus)
       {

         if(open()==false){open();}
         QSqlQuery query;
         begin();
         QString cmd = "INSERT INTO pmk_yhk.t_real_2 (id_kam,nm_dis,nm_kam, no_rek, nm_rek,nm_bank, sk_bend, sk_kp, nm_ben, nm_kp, thp_cair, j_cair, j_terbilang, "
                                  "tgl, no_srt1, no_srt2,persentase,sk_bup,sk_kam,sk_pmk,sk_keu, nm_kpd, j_kpd, nip_kpd, ket, thp_l, jkk, nos) VALUES "
                                  "(:id_kam, :nm_dis, :nm_kam, :no_rek, :nm_rek, :nm_bank, :sk_bend, :sk_kp, :nm_ben, :nm_kp, :thp_cair, :j_cair, :j_terbilang, "
                                   ":tgl, :no_srt1, :no_srt2, :persentase, :sk_bup, :sk_kam, :sk_pmk, :sk_keu, :nm_kpd, :j_kpd, :nip_kpd, :ket, :thp_l, :jkk , '2' )";

         query.prepare(cmd);
         query.bindValue(":id_kam",qbx_id_kam->currentText());
         query.bindValue(":nm_dis", ui->comboBox->currentText());
         query.bindValue(":nm_kam",ui->comboBox_nmKampung->currentText());
         query.bindValue(":no_rek",no_rek);
         query.bindValue(":nm_rek",nm_rek);
         query.bindValue(":nm_bank",nm_bank);
         query.bindValue(":sk_bend",j_bend);
         query.bindValue(":sk_kp",j_kp);
         query.bindValue(":nm_ben",nm_bend);
         query.bindValue(":nm_kp",nm_kp);
         query.bindValue(":thp_cair", tahap );
         query.bindValue(":j_cair",jj_);
         query.bindValue(":j_terbilang",terbilang+rp);
         query.bindValue(":tgl",tgl_ter);
         no_srt1.replace("****",nosurat());
         query.bindValue(":no_srt1",no_srt1);
         no_srt2.replace("####",nosurat_2());
         query.bindValue(":no_srt2",no_srt2);
         query.bindValue(":persentase",persen);
         query.bindValue(":sk_bup",sk_bup);
         query.bindValue(":sk_kam",sk_kam);
//     query.bindValue(":sk_pmk",sk_pmk);
//     query.bindValue(":sk_keu",sk_keu);
         query.bindValue(":nm_kpd",nm_kp_dns);
         query.bindValue(":j_kpd",pg_kp_dns);
         query.bindValue(":nip_kpd",nip_kp_dns);
         query.bindValue(":ket",thn2);
         query.bindValue(":thp_l",tahap_l);
         query.bindValue(":jkk",j_kk);

         bool ok = exec(query);
         if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Menambah realisasi , "+query.lastError().text()+"");}
         commit();
            eb_v->close();
        }else { return; }
     if(menu=="3"){
     QString id = qbx_id_kam->currentText();
     muat_real_2(id);} }
}

  // ============== Partr 222 Save Edit realisasi dds
    if(menu2=="4"){
           if(menu=="3"){
            active_eb_v_2();
            int noBrs = ui->tableWidget_9->currentRow();

            QString id_dis = qbx_id_dis->currentText();
            QString id_kam = qbx_id_kam->currentText();
            QString j = le_jml->text();

            QString jcair = le_jml->text();
            jcair.replace("Rp ",""); jcair.replace(".",""); jcair.replace(",",".");
            double jc = jcair.toDouble();

            QString jcair2 = ui->tableWidget_9->item(noBrs,12)->text();
            jcair2.replace("Rp ",""); jcair2.replace(".",""); jcair2.replace(",",".");
            double jc2 = jcair2.toDouble();

            QString jcair3 = ui->tableWidget_11->item(0,4)->text();
            jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
            double jc3 = jcair3.toDouble();
           //Info() << "Doubleklik tw 2";
           //Info() << jc2+jc3;
           //Info() << jc;
            if(jc<jc2){ if(((jc3+jc2)-jc)<0){QMessageBox::information(eb_v,"Info....","Nilai yang anda masukkan melebihi pagu .. "); return;}}
            if(jc>jc2){ if(0>((jc3+jc2)-jc)){QMessageBox::information(eb_v,"Info....","Nilai yang anda masukkan melebihi pagu .. "); return;}}


           //Info() << "j Cair Save Edit"<< j;
            if(j=="Rp 0,00"){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh Nilai Rp 0,00,-"); return;}
            if(j==""){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong,-"); return;}

            j.replace("Rp ",""); j.replace(".",""); j.replace(",",".");
            QString jj_ = j;
            bilang(j);

            // QString jcair3 = ui->tableWidget_11->item(0,4)->text();
            jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
           // double jc3 = jcair3.toDouble();
            //double jj = jcair.toDouble();
            //nfo() << "jc3" << jc3 <<"jj" << jj;
           //if((jc3-jj)<0){QMessageBox::information(eb_v,"Info...","Mohon Periksa Ketersediaan Dana 4444444444"); return;}
            //QString jj_ = QString::number(jj);
            //bilang(jj_);

            QString id=ui->tableWidget_9->item(noBrs,27)->text();

            QString terbil = terbilang;
            QString thp_cair = qbx_thp_penc->currentText();
            QString j_cair  = jj_;

            QDate dtt = QDate::fromString(de_tgl_terima->text(),"dd-MM-yyyy");
            QString tgl = dtt.toString("yyyy-MM-dd");

            QString no_srt1 = qbx_no_srt1->currentText();
            qbx_no_srt1->setEditable(true);
            QString no_srt2 = qbx_no_srt2->currentText();
            qbx_no_srt2->setEditable(true);
            QString persen = qbx_persen->currentText();
            qbx_persen->setEditable(true);
            QString thp_l = qbx_thp_l->currentText();
            qbx_thp_l->setEditable(true);

            QString rp ="rupiah";
            QPixmap logo;
            QIcon icon(":/icon/gbr/kcl.png");
            QMessageBox boxPesan;
            boxPesan.setWindowIcon(icon);
            boxPesan.setWindowTitle("Info...!!!");
            boxPesan.setText("<b>Mengubah data Realisasi Kampung "+ui->comboBox_nmKampung->currentText()+" </b>");
            boxPesan.setInformativeText("Pastikan data yang anda input sudah sesuai...");
            QAbstractButton * pBhapus = boxPesan.addButton(tr(" UBAH "), QMessageBox::YesRole) ;
             boxPesan.addButton(tr(" BATAL "), QMessageBox::NoRole);
             if (logo.load(":/icon/gbr/kcl.png"))
             boxPesan.setIconPixmap(logo);
             boxPesan.exec();
             if(boxPesan.clickedButton() == pBhapus)
               {

                 if(open()==false){open();}
                 QSqlQuery query;
                 begin();
                 QString cmd = "UPDATE pmk_yhk.t_real_2 SET thp_cair = :thp_cair,  j_cair = :j_cair, tgl = :tgl , no_srt1 = :no_srt1 , no_srt2= :no_srt2 , persentase= :persen , thp_l = :thp_l, j_terbilang= :terbil WHERE id = :id";

                 query.prepare(cmd);
                 query.bindValue(":id", id);
                 query.bindValue(":thp_cair", thp_cair);
                 query.bindValue(":j_cair",jj_);
                 query.bindValue(":tgl",tgl);
                 query.bindValue(":no_srt1",no_srt1);
                 query.bindValue(":no_srt2",no_srt2);
                 query.bindValue(":persen",persen);
                 query.bindValue(":thp_l",thp_l);
                 query.bindValue(":terbil", terbilang+" rupiah");

                 bool ok = exec(query);
                 if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Mengubah data realisasi , "+query.lastError().text()+""); return;}

                 commit();
                 QMessageBox::information(eb_v,"Info....","Berhasil Mengubah data realisasi");  eb_v->close();

                }else { return; }
             //menu="2";
             if(menu=="3"){
             QString id = qbx_id_kam->currentText();
             muat_real_2(id);} }
}
on_toolButton_refResh_clicked();
}


void Form::muat_real(QString &s_id_kamp) //
{
    while(ui->tableWidget_2->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_2->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.t_real WHERE id_kam = :id ORDER BY id_real ";
    //if(s_id_kamp==""){cmd ="SELECT * FROM pmk_yhk.t_real ORDER BY id_real ";}
     if(s_id_kamp==""){cmd =" SELECT	* FROM	pmk_yhk.t_real ORDER BY	id_real ASC, pmk_yhk.t_real.id_real LIMIT 40 ";}
    query.prepare(cmd);
    query.bindValue(":id",s_id_kamp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data realisasi"); return;}
    int i=0;
    while (query.next()) {

        //int i = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(i);
        QPushButton *pb = new QPushButton;
        QRadioButton *rb = new QRadioButton;
        rb->setContentsMargins(0,0,0,0);
        pb->setToolTip("klik untuk cetak");

        QTableWidgetItem *nm_dis_ = new QTableWidgetItem;
        QTableWidgetItem *nm_kam_ = new QTableWidgetItem;
        QTableWidgetItem *id_dis_ = new QTableWidgetItem;
        QTableWidgetItem *id_kam_ = new QTableWidgetItem;
        QTableWidgetItem *no_rek_ = new QTableWidgetItem;
        QTableWidgetItem *nm_rek_ = new QTableWidgetItem;
        QTableWidgetItem *nm_bank_ = new QTableWidgetItem;
        QTableWidgetItem *j_sek_ = new QTableWidgetItem;
        QTableWidgetItem *j_kp_ = new QTableWidgetItem;
        QTableWidgetItem *nm_ben_ = new QTableWidgetItem;
        QTableWidgetItem *nm_kp_ = new QTableWidgetItem;
        QTableWidgetItem *thp_cair_ = new QTableWidgetItem;
        QTableWidgetItem *j_cair_ = new QTableWidgetItem;
        QTableWidgetItem *j_terbil_ = new QTableWidgetItem;
        QTableWidgetItem *tgl_ = new QTableWidgetItem;
        QTableWidgetItem *no_srt1_ = new QTableWidgetItem;
        QTableWidgetItem *no_srt2_ = new QTableWidgetItem;
        QTableWidgetItem *persen_ = new QTableWidgetItem;
        QTableWidgetItem *sk_bup_ = new QTableWidgetItem;
        QTableWidgetItem *sk_kam_ = new QTableWidgetItem;
        QTableWidgetItem *sk_men_ = new QTableWidgetItem;
        QTableWidgetItem *sk_keu_ = new QTableWidgetItem;
        QTableWidgetItem *ket_ = new QTableWidgetItem;
        QTableWidgetItem *nm_kpd_ = new QTableWidgetItem;
        QTableWidgetItem *j_kpd_ = new QTableWidgetItem;
        QTableWidgetItem *nip_kpd_ = new QTableWidgetItem;
        QTableWidgetItem *thp_cair2_ = new QTableWidgetItem;
        QTableWidgetItem *id_ = new QTableWidgetItem;
        QTableWidgetItem *jkk_ = new QTableWidgetItem;

        nm_dis_->setText(query.value(0).toString());
        nm_kam_->setText(query.value(1).toString());
        id_dis_->setText(query.value(2).toString());
        id_kam_->setText(query.value(3).toString());
        no_rek_->setText(query.value(4).toString());
        nm_rek_->setText(query.value(5).toString());
        nm_bank_->setText(query.value(6).toString());
        j_sek_->setText(query.value(7).toString());
        j_kp_->setText(query.value(8).toString());
        //qInfo()<< "J Kpppppppp" <<  query.value(8).toString();
        nm_ben_->setText(query.value(9).toString());
        nm_kp_->setText(query.value(10).toString());
        thp_cair_->setText(query.value(11).toString());
        QLocale indo = QLocale(QLocale::Indonesian, QLocale::Indonesia);
        double jj = (query.value(12).toDouble());
        QString jjj = indo.toCurrencyString(jj,"Rp ");
        j_cair_->setText(jjj);
        j_terbil_->setText(query.value(13).toString());
        QDate dt = QDate::fromString(query.value(14).toString(),"yyyy-MM-dd");
        QString dt_ = dt.toString("dd-MM-yyyy");
        tgl_->setText(dt_);
        no_srt1_->setText(query.value(15).toString());
        no_srt2_->setText(query.value(16).toString());
        persen_->setText(query.value(17).toString());
        sk_bup_->setText(query.value(18).toString());
        sk_kam_->setText(query.value(19).toString());
        sk_men_->setText(query.value(20).toString());
        sk_keu_->setText(query.value(21).toString());
        ket_->setText(query.value(22).toString());
        nm_kpd_->setText(query.value(25).toString());
        j_kpd_->setText(query.value(23).toString());
        nip_kpd_->setText(query.value(24).toString());
        thp_cair2_->setText(query.value(26).toString());
        id_->setText(query.value(27).toString());
        jkk_->setText(query.value(28).toString());

        nm_dis_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_kam_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        id_dis_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        id_kam_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        no_rek_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_rek_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_bank_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_sek_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_kp_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_ben_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_kp_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        thp_cair_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_cair_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_terbil_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        tgl_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        no_srt1_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        no_srt2_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        persen_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_bup_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_kam_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_men_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_keu_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ket_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_kpd_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_kpd_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nip_kpd_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        thp_cair2_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        jkk_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidget_2->setCellWidget(i,0,rb);
        ui->tableWidget_2->setItem(i,1,nm_dis_);
        ui->tableWidget_2->setItem(i,2,nm_kam_);
        ui->tableWidget_2->setItem(i,3,id_dis_);
        ui->tableWidget_2->setItem(i,4,id_kam_);
        ui->tableWidget_2->setItem(i,5,no_rek_);
        ui->tableWidget_2->setItem(i,6,nm_rek_);
        ui->tableWidget_2->setItem(i,7,nm_bank_);
        ui->tableWidget_2->setItem(i,8,j_sek_);
        ui->tableWidget_2->setItem(i,9,nm_kp_);
        ui->tableWidget_2->setItem(i,10,nm_ben_);
        ui->tableWidget_2->setItem(i,11,thp_cair_);
        ui->tableWidget_2->setItem(i,12,j_cair_);
        ui->tableWidget_2->setItem(i,13,j_terbil_);
        ui->tableWidget_2->setItem(i,14,tgl_);
        ui->tableWidget_2->setItem(i,15,no_srt1_);
        ui->tableWidget_2->setItem(i,16,no_srt2_);
        ui->tableWidget_2->setItem(i,17,persen_);
        ui->tableWidget_2->setItem(i,18,sk_bup_);
        ui->tableWidget_2->setItem(i,19,sk_kam_);
        ui->tableWidget_2->setItem(i,20,sk_men_);
        ui->tableWidget_2->setItem(i,21,sk_keu_);
        ui->tableWidget_2->setItem(i,22,ket_);
        ui->tableWidget_2->setItem(i,23,nm_kpd_);
        ui->tableWidget_2->setItem(i,24,j_kpd_);
        ui->tableWidget_2->setItem(i,25,nip_kpd_);
        ui->tableWidget_2->setItem(i,26,thp_cair2_);
        ui->tableWidget_2->setItem(i,27,id_);
        ui->tableWidget_2->setItem(i,28,jkk_);
       i++;
    }
}

void Form::muat_real_2(QString &s_id_kamp) //
{
    while(ui->tableWidget_9->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_9->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.t_real_2 WHERE id_kam = :id ORDER BY id_real";
//    if(s_id_kamp==""){cmd ="SELECT * FROM pmk_yhk.t_real_2 ORDER BY id_real ";}
    if(s_id_kamp==""){cmd =" SELECT	* FROM	pmk_yhk.t_real_2 ORDER BY	id_real ASC, pmk_yhk.t_real_2.id_real LIMIT 40 ";}
    query.prepare(cmd);
    query.bindValue(":id",s_id_kamp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data realisasi"); return;}
    int i=0;
    while (query.next()) {

        //int i = ui->tableWidget_2->rowCount();
        ui->tableWidget_9->insertRow(i);
        QPushButton *pb = new QPushButton;
        QRadioButton *rb = new QRadioButton;
        rb->setContentsMargins(0,0,0,0);
        pb->setToolTip("klik untuk cetak");

        QTableWidgetItem *nm_dis_ = new QTableWidgetItem;
        QTableWidgetItem *nm_kam_ = new QTableWidgetItem;
        QTableWidgetItem *id_dis_ = new QTableWidgetItem;
        QTableWidgetItem *id_kam_ = new QTableWidgetItem;
        QTableWidgetItem *no_rek_ = new QTableWidgetItem;
        QTableWidgetItem *nm_rek_ = new QTableWidgetItem;
        QTableWidgetItem *nm_bank_ = new QTableWidgetItem;
        QTableWidgetItem *j_sek_ = new QTableWidgetItem;
        QTableWidgetItem *j_kp_ = new QTableWidgetItem;
        QTableWidgetItem *nm_ben_ = new QTableWidgetItem;
        QTableWidgetItem *nm_kp_ = new QTableWidgetItem;
        QTableWidgetItem *thp_cair_ = new QTableWidgetItem;
        QTableWidgetItem *j_cair_ = new QTableWidgetItem;
        QTableWidgetItem *j_terbil_ = new QTableWidgetItem;
        QTableWidgetItem *tgl_ = new QTableWidgetItem;
        QTableWidgetItem *no_srt1_ = new QTableWidgetItem;
        QTableWidgetItem *no_srt2_ = new QTableWidgetItem;
        QTableWidgetItem *persen_ = new QTableWidgetItem;
        QTableWidgetItem *sk_bup_ = new QTableWidgetItem;
        QTableWidgetItem *sk_kam_ = new QTableWidgetItem;
        QTableWidgetItem *sk_men_ = new QTableWidgetItem;
        QTableWidgetItem *sk_keu_ = new QTableWidgetItem;
        QTableWidgetItem *ket_ = new QTableWidgetItem;
        QTableWidgetItem *nm_kpd_ = new QTableWidgetItem;
        QTableWidgetItem *j_kpd_ = new QTableWidgetItem;
        QTableWidgetItem *nip_kpd_ = new QTableWidgetItem;
        QTableWidgetItem *thp_cair2_ = new QTableWidgetItem;        
        QTableWidgetItem *id_ = new QTableWidgetItem;
        QTableWidgetItem *jkk_ = new QTableWidgetItem;

        nm_dis_->setText(query.value(0).toString());
        nm_kam_->setText(query.value(1).toString());
        id_dis_->setText(query.value(2).toString());
        id_kam_->setText(query.value(3).toString());
        no_rek_->setText(query.value(4).toString());
        nm_rek_->setText(query.value(5).toString());
        nm_bank_->setText(query.value(6).toString());
        j_sek_->setText(query.value(7).toString());
        QLocale indo = QLocale(QLocale::Indonesian, QLocale::Indonesia);
        double jj = (query.value(12).toDouble());
        QString jjj = indo.toCurrencyString(jj,"Rp ");
        j_kp_->setText(query.value(8).toString());
        nm_ben_->setText(query.value(9).toString());
        nm_kp_->setText(query.value(10).toString());
        thp_cair_->setText(query.value(11).toString());
        j_cair_->setText(jjj);
        j_terbil_->setText(query.value(13).toString());
        QDate dt = QDate::fromString(query.value(14).toString(),"yyyy-MM-dd");
        QString dt_ = dt.toString("dd-MM-yyyy");
        tgl_->setText(dt_);
        no_srt1_->setText(query.value(15).toString());
        no_srt2_->setText(query.value(16).toString());
        persen_->setText(query.value(17).toString());
        sk_bup_->setText(query.value(18).toString());
        sk_kam_->setText(query.value(19).toString());
        sk_men_->setText(query.value(20).toString());
        sk_keu_->setText(query.value(21).toString());
        ket_->setText(query.value(22).toString());
        nm_kpd_->setText(query.value(25).toString());
        j_kpd_->setText(query.value(24).toString());
        nip_kpd_->setText(query.value(23).toString());
        thp_cair2_->setText(query.value(26).toString());
        id_->setText(query.value(27).toString());
        jkk_->setText(query.value(28).toString());

        nm_dis_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_kam_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        id_dis_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        id_kam_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        no_rek_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_rek_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_bank_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_sek_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_kp_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_ben_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_kp_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        thp_cair_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_cair_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_terbil_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        tgl_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        no_srt1_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        no_srt2_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        persen_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_bup_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_kam_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_men_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sk_keu_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ket_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_kpd_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        j_kpd_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nip_kpd_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        thp_cair2_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        jkk_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidget_9->setCellWidget(i,0,rb);
        ui->tableWidget_9->setItem(i,1,nm_dis_);
        ui->tableWidget_9->setItem(i,2,nm_kam_);
        ui->tableWidget_9->setItem(i,3,id_dis_);
        ui->tableWidget_9->setItem(i,4,id_kam_);
        ui->tableWidget_9->setItem(i,5,no_rek_);
        ui->tableWidget_9->setItem(i,6,nm_rek_);
        ui->tableWidget_9->setItem(i,7,nm_bank_);
        ui->tableWidget_9->setItem(i,8,j_sek_);
        ui->tableWidget_9->setItem(i,9,nm_kp_);
        ui->tableWidget_9->setItem(i,10,nm_ben_);
        ui->tableWidget_9->setItem(i,11,thp_cair_);
        ui->tableWidget_9->setItem(i,12,j_cair_);
        ui->tableWidget_9->setItem(i,13,j_terbil_);
        ui->tableWidget_9->setItem(i,14,tgl_);
        ui->tableWidget_9->setItem(i,15,no_srt1_);
        ui->tableWidget_9->setItem(i,16,no_srt2_);
        ui->tableWidget_9->setItem(i,17,persen_);
        ui->tableWidget_9->setItem(i,18,sk_bup_);
        ui->tableWidget_9->setItem(i,19,sk_kam_);
        ui->tableWidget_9->setItem(i,20,sk_men_);
        ui->tableWidget_9->setItem(i,21,sk_keu_);
        ui->tableWidget_9->setItem(i,22,ket_);
        ui->tableWidget_9->setItem(i,23,nm_kpd_);
        ui->tableWidget_9->setItem(i,24,j_kpd_);
        ui->tableWidget_9->setItem(i,25,nip_kpd_);
        ui->tableWidget_9->setItem(i,26,thp_cair2_);
        ui->tableWidget_9->setItem(i,27,id_);
        ui->tableWidget_9->setItem(i,28,jkk_);
       i++;
    }
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
    }
    lvl_type.close();
}

void Form::event_klik_tw_6() // Tw 6
{
   // qInfo()<< "Even Klik tw_6 ";
    if(menu=="2"){
    QClipboard *clipboard = QApplication::clipboard();
    int tbl_lines = ui->tableWidget_6->rowCount();
    QString str =  " No \t Nama Distrik  \t Nama Kampung  \t  Pagu Anggaran  \t  Sisa Anggaran  \t Realisasi \n ";
    for (int i=0; i<tbl_lines; i++)
    {
    QString mydata0 = ui->tableWidget_6->item(i, 0)->text();
    QString mydata1 = ui->tableWidget_6->item(i, 1)->text();
    QString mydata2 = ui->tableWidget_6->item(i, 2)->text();
    QString mydata3 = ui->tableWidget_6->item(i, 3)->text();
    mydata3.replace("Rp ",""); mydata3.replace(".",""); mydata3.replace(",",".");
    double p = mydata3.toDouble();
    QString p_ = QString::number(p);

    QString mydata4 = ui->tableWidget_6->item(i, 4)->text();
    mydata4.replace("Rp ",""); mydata4.replace(".",""); mydata4.replace(",",".");
    double r = mydata4.toDouble();
    QString r_ = QString::number(r);
    QString mydata5 = ui->tableWidget_6->item(i, 5)->text();
    mydata5.replace("Rp ",""); mydata5.replace(".",""); mydata5.replace(",",".");
    double s = mydata5.toDouble();
    QString s_ = QString::number(s);

    QTextStream(&str) << mydata0 << "\t" << mydata1 << "\t"<< mydata2 << "\t" << p_ <<"\t"<< r_ << "\t" << s_ << Qt::endl;
    }
    clipboard->setText(str);
}}


void Form::event_klik_tw_11() // Tw 11
{
    //qInfo()<< "Even Klik tw_11 ";
    if(menu=="3"){
    QClipboard *clipboard = QApplication::clipboard();
    int tbl_lines = ui->tableWidget_11->rowCount();
    QString str =  " No \t Nama Distrik  \t Nama Kampung  \t  Pagu Anggaran  \t  Sisa Anggaran  \t Realisasi \n ";
    for (int i=0; i<tbl_lines; i++)
    {
    QString mydata0 = ui->tableWidget_11->item(i, 0)->text();
    QString mydata1 = ui->tableWidget_11->item(i, 1)->text();
    QString mydata2 = ui->tableWidget_11->item(i, 2)->text();
    QString mydata3 = ui->tableWidget_11->item(i, 3)->text();
    mydata3.replace("Rp ",""); mydata3.replace(".",""); mydata3.replace(",",".");
    double p = mydata3.toDouble();
    QString p_ = QString::number(p);

    QString mydata4 = ui->tableWidget_11->item(i, 4)->text();
    mydata4.replace("Rp ",""); mydata4.replace(".",""); mydata4.replace(",",".");
    double r = mydata4.toDouble();
    QString r_ = QString::number(r);
    QString mydata5 = ui->tableWidget_11->item(i, 5)->text();
    mydata5.replace("Rp ",""); mydata5.replace(".",""); mydata5.replace(",",".");
    double s = mydata5.toDouble();
    QString s_ = QString::number(s);

    QTextStream(&str) << mydata0 << "\t" << mydata1 << "\t"<< mydata2 << "\t" << p_ <<"\t"<< r_ << "\t" << s_ << Qt::endl;
    }
    clipboard->setText(str);
}}

void Form::event_klik_tw_13() // Tw 13
{
    //qInfo()<< "Even Klik tw_13 ";
//    if(menu=="4"){
//    QClipboard *clipboard = QApplication::clipboard();
//    int tbl_lines = ui->tableWidget_13->rowCount();
//    QString str =  " No \t Nama Distrik  \t Nama Kampung  \t  Data I  \t  Data II  \t Data II \t  Data III  \t  Data IV  \t Data V \t  Data VI  \t  Data VII  \t Sub Total \t Total \n  ";
//    for (int i=0; i<tbl_lines; i++)
//    {
//    QString mydata0 = ui->tableWidget_13->item(i, 0)->text();
//    QString mydata1 = ui->tableWidget_13->item(i, 1)->text();
//    QString mydata2 = ui->tableWidget_13->item(i, 2)->text();
//    QString mydata3 = ui->tableWidget_13->item(i, 3)->text();
//    mydata3.replace("Rp ",""); mydata3.replace(".",""); mydata3.replace(",",".");
//    double p = mydata3.toDouble();
//    QString p_ = QString::number(p);

//    QString mydata4 = ui->tableWidget_11->item(i, 4)->text();
//    mydata4.replace("Rp ",""); mydata4.replace(".",""); mydata4.replace(",",".");
//    double r = mydata4.toDouble();
//    QString r_ = QString::number(r);
//    QString mydata5 = ui->tableWidget_11->item(i, 5)->text();
//    mydata5.replace("Rp ",""); mydata5.replace(".",""); mydata5.replace(",",".");
//    double s = mydata5.toDouble();
//    QString s_ = QString::number(s);

//    QTextStream(&str) << mydata0 << "\t" << mydata1 << "\t"<< mydata2 << "\t" << p_ <<"\t"<< r_ << "\t" << s_ << Qt::endl;
//    }
//    clipboard->setText(str);
//}
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



void Form::event_klik_tw() // tw
{
    //qInfo()<< "Even Klik tw ";
    if(menu=="1"){
    QClipboard *clipboard = QApplication::clipboard();

    int tbl_lines = ui->tableWidget_Bamuskam->rowCount();

    QString str = " Distrik \t Kampung \t Nama \t Jabatan \t No SK \t Tgl SK \n ";

    for (int i=0; i<tbl_lines; i++)
    {

    QString mydata0 = ui->tableWidget_Bamuskam->item(i, 4)->text();
    QString mydata1 = ui->tableWidget_Bamuskam->item(i, 5)->text();
    QString mydata2 = ui->tableWidget_Bamuskam->item(i, 6)->text();
    QString mydata3 = ui->tableWidget_Bamuskam->item(i, 7)->text();
    QString mydata4 = ui->tableWidget_Bamuskam->item(i, 8)->text();
    QString mydata5 = ui->tableWidget_Bamuskam->item(i, 9)->text();

    QTextStream(&str) << mydata0 << "\t" << mydata1 << "\t"<< mydata2 << "\t" << mydata3 <<"\t"<< mydata4 << "\t" << mydata5 << Qt::endl;
    }
    clipboard->setText(str);
}}

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


void Form::click_btn()
{
    //qInfo() << "Clik Button save di Edit Bamuskam";
    QString de_ = de->text();
    QDate dee = QDate::fromString(de_,"dd-MM-yyyy");
    QString dee_ = dee.toString("yyyy-MM-dd");
    QString nm_ =Le_nm->text();
    QString jbtn_ = Le_jbtn->text();
    QString no_sk_ = Le_no_sk->text();

    //qInfo() << id <<"=="<< de_ <<"==" << nm_<< "== "<< jbtn_ << "=="<< no_sk_ << "==" ;

    QPixmap logo;
    QIcon icon(":/icon/gbr/kcl.png");
    QMessageBox boxPesan;
    boxPesan.setWindowIcon(icon);
    boxPesan.setWindowTitle("Info...!!!");
    boxPesan.setText("<b>Mengubah data </b>");
    boxPesan.setInformativeText("Pastikan data yang anda rubah sudah sesuai...");
    QAbstractButton * pBhapus = boxPesan.addButton(tr(" UBAH "), QMessageBox::YesRole) ;
     boxPesan.addButton(tr(" BATAL "), QMessageBox::NoRole);
     if (logo.load(":/icon/gbr/kcl.png"))
     boxPesan.setIconPixmap(logo);
     boxPesan.exec();
     if(boxPesan.clickedButton() == pBhapus)
       {
         if(open()==false){open();}
         begin();
         QSqlQuery query;
         QString cmd = " UPDATE pmk_yhk.m_bam SET  nama = :nama , no_sk = :no_sk , tgl_sk = :tgl_sk WHERE id = :id ";
         query.prepare(cmd);
         query.bindValue(":id",id);
         query.bindValue(":nama",nm_);
         query.bindValue(":no_sk",no_sk_);
         query.bindValue(":tgl_sk",dee_);
         bool ok = exec(query);
         if(ok)
         {  commit();
             QPixmap logo;
             QIcon icon(":/icon/gbr/kcl.png");
             QMessageBox boxPesan;
             boxPesan.setWindowTitle("Info... ");
             boxPesan.setWindowIcon(icon);
             boxPesan.setText(tr("Berhasil di ubah"));
             boxPesan.addButton(tr("Ok"), QMessageBox::NoRole);
             boxPesan.setIconPixmap(logo);
             boxPesan.exec();

         }else{ rollback();
                    QString err = query.lastError().text();
                     QPixmap logo;
                     QIcon icon(":/icon/gbr/kcl.png");
                     QMessageBox boxPesan;
                     boxPesan.setWindowTitle("Errorr...!!! ");
                     boxPesan.setWindowIcon(icon);
                     boxPesan.setText(tr("Gagal mengubah <b> </b>"));
                     boxPesan.addButton(tr("Ok"), QMessageBox::NoRole);
                     boxPesan.setIconPixmap(logo);
                     boxPesan.exec();
                    }
    }
}


void Form::on_toolButton_4_clicked() // Klik Tambah Pencairan dana desa
{
      if(menu=="2"){
      if(ui->comboBox->currentText()==""&&ui->comboBox_nmKampung->currentText()==""){ QMessageBox::information(this,"Info...!!!","Pilih distrik dan kampung...");     return;}
                            act(); }
}

void Form::on_toolButton_10_clicked() // Klik Tambah Pencairan Alokasi dana desa
{
    if(menu=="3"){
    if(ui->comboBox->currentText()==""&&ui->comboBox_nmKampung->currentText()==""){ QMessageBox::information(this,"Info...!!!","Pilih distrik dan kampung...");     return;}
                            act_2();}
}

void Form::on_toolButton_5_clicked() // cetak menu
{
    //ui->stackedWidget_2->setCurrentIndex(0);
    if(menu=="2")
    {
        if(ui->comboBox->currentText()!="" && ui->comboBox_nmKampung->currentText()!=""){
        int jBrs = ui->tableWidget_2->rowCount();
        if(jBrs>= 1){
            int nRow = ui->tableWidget_2->currentRow();
            if(nRow==-1){QMessageBox::information(this,"Info...!!!","Pilih Realisasi yang ingin di cetak...");return;}
            //qInfo()<< "Baris Sekarna" <<nRow << jBrs;
    }}}
}

// Button pada cetak PDF dana Desa
void Form::on_toolButton_6_clicked()
{
    int noBrs = ui->tableWidget_2->currentRow();
    if(noBrs<0){QMessageBox::information(this,"Info...!!!","Pilih Realisasi yang ingin di cetak...");return;}
    if(noBrs>=0){
       datapdf1();
       qInfo()<<"Ini adalah info pada button klik"<< getLspdf1();
       on_saveFileButton_pressed(qcode());
       Widget1 *a = new Widget1;
       QIcon logo(":/gbr/html/gbr/yhk.png");
       a->setWindowIcon(logo);
       a->show();

}}

QString Form::qcode()
{
    int noBrs = ui->tableWidget_2->currentRow();
    QString a = ui->tableWidget_2->item(noBrs,11)->text();
    QString b = ui->tableWidget_2->item(noBrs,14)->text();
    QString b_ =" "+ b.right(4);
    QString c = " Distrik " + ui->tableWidget_2->item(noBrs,3)->text();
    QString d = " Kampung " + ui->tableWidget_2->item(noBrs,4)->text();
    QString e = "@" + ui->tableWidget_2->item(noBrs,10)->text();
    QString f = "/" +ui->tableWidget_2->item(noBrs,9)->text();
    QString g = "/" +ui->tableWidget_2->item(noBrs,5)->text();
    QString h = "/" +ui->tableWidget_2->item(noBrs,12)->text();
     return a+b_+c+d+e+f+g+h;
}


void Form::on_toolButton_refResh_clicked() // refresh
{

  if(menu=="1"){
      if(qbx_id_kam->currentText()!="")
      {
          QString id = qbx_id_kam->currentText();
        muat_bamuskam(id);
      }}

  if(menu=="2"){
      if(qbx_id_kam->currentText()!="")
                        { QString id = qbx_id_kam->currentText();
                            muat_v_bam(id);
                        }}

  if(menu=="3"){
      if(qbx_id_kam->currentText()!="")
                        { QString id = qbx_id_kam->currentText();
                            muat_v_bam_2(id);
                    }}
  if(menu=="4"){
                        if (ui->comboBox_realisasi->currentIndex()==0){muatrealdds(); loadsppd(); }
                        if (ui->comboBox_realisasi->currentIndex()==1){muatrealadd(); loadsppd_2();}
                         }
}

void Form::on_toolButton_logOut_clicked() //logout Button
{
    this->close();
    MainWindow *a = new MainWindow;
    QIcon logo(":/gbr/html/gbr/yhk.png");
    a->setWindowIcon(logo);
    a->show();
}

void Form::on_toolButton_pdf_main_clicked() // Button Cetak pdf untuk pengantar
{
  if(ui->comboBox_realisasi->currentIndex()==0){    Widget3 *a = new Widget3;
      a->show();}
  if(ui->comboBox_realisasi->currentIndex()==1){    QMessageBox::information(this,"Info","Menu ini Belum tersedia");}

}

void Form::on_toolButton_16_clicked() // Button Tambah Realisasi
{
    if (menu=="4")
    {
        if(ui->comboBox_realisasi->currentIndex()==0){ sppd();}
        if(ui->comboBox_realisasi->currentIndex()==1){ sppd_2();}
    }
}

void Form::on_toolButton_17_clicked() // Button Clik Generate pdf di sppd =====================================
{

    int j = ui->tableWidget_13->currentRow();
    if(j==-1){QMessageBox::information(this,"Info","Pilih Surat yang akan di cetak... "); return;}

    QString nosrt = ui->tableWidget_13->item(j,1)->text();
    QString hal = ui->tableWidget_13->item(j,2)->text();
    QString tgl = ui->tableWidget_13->item(j,3)->text();
    QDate dt = QDate::fromString(tgl,"dd-MM-yyyy");
    QString tgl_ = dt.toString("yyyy-MM-dd");
    QString tgl_1 = dt.toString("dd MMMM yyyy");
    QString tgl_2 = dt.toString("dd MMMM yyyy");
    QString thn = tgl_1.right(4);

    if (menu=="4" && ui->comboBox_realisasi->currentIndex()==0)
    {
        kp_dns();
        ds_sppd_.clear();
        //qInfo() << "generate pdf 000 ............................................................";
        QSqlQuery query;
        QString cmd = " SELECT nm_kam, nm_dis, nm_kp, nm_ben, nm_rek, no_rek, j_cair, thp_cair  FROM pmk_yhk.t_real WHERE tgl = :tgl ORDER BY id_real ";
        query.prepare(cmd);
        query.bindValue(":tgl",tgl_);
        bool ok = exec(query);
        if(!ok){QMessageBox::information(this,"Error....","Gagal memuat data list pencairan pada tanggal "+tgl_2+" Error "+query.lastError().text()+"");}
        int no = 1;
        double jcr=0;
        while(query.next())
            {
                QString no_ =QString::number(no);
                double c = query.value(6).toDouble();
                QString jc = indo.toCurrencyString(c,"Rp ");
                ds_sppd_.append("<tr><td>"+no_+"</td><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td>"+query.value(2).toString()+"</td>"
                "<td>"+query.value(3).toString()+"</td><td>"+query.value(4).toString()+"</td><td>"+query.value(5).toString()+"</td><td>"+jc+"</td><td>"+query.value(7).toString()+"</td></tr>" );
                qInfo() << "generate pdf 000"<< c ;
                jcr +=c;
                no++;
                qInfo() << "generate pdf jcr"<< jcr ;
            }

        tot_sppd = indo.toCurrencyString(jcr,"Rp ");
        qInfo() << "generate pdf 000"<< tot_sppd ;
        QString tot_terbil = tot_sppd;
        tot_terbil.replace("Rp ",""); tot_terbil.replace(".",""); tot_terbil.replace(",",".");
        bilang(tot_terbil);

        ds_sppd_.append("#/#"+tot_sppd);
        ds_sppd_.append("#/#"+tgl);
        ds_sppd_.append("#/#"+tgl_1);
        ds_sppd_.append("#/#"+thn);
        ds_sppd_.append("#/#"+nosrt);
        ds_sppd_.append("#/#"+hal);
        ds_sppd_.append("#/#"+li_kp_dns.at(0));
        ds_sppd_.append("#/#"+li_kp_dns.at(1));
        ds_sppd_.append("#/#"+li_kp_dns.at(2));
        ds_sppd_.append("#/#"+terbilang);

        //setTot_sppd(tot_sppd);
        save_sppd();
       Widget3 *pdf3 = new Widget3;
       QIcon logo(":/gbr/html/gbr/yhk.png");
       pdf3->setWindowIcon(logo);
       pdf3->show();
       qInfo() << "generate pdf 000111"<< tot_sppd ;
    }

    if (menu=="4" && ui->comboBox_realisasi->currentIndex()==1)
    {   ds_sppd_.clear();
        kp_dns();
        QSqlQuery query;
        QString cmd = " SELECT nm_kam, nm_dis, nm_kp, nm_ben, nm_rek, no_rek, j_cair, thp_cair  FROM pmk_yhk.t_real_2 WHERE tgl = :tgl ORDER BY id_real ";
        query.prepare(cmd);
        query.bindValue(":tgl", tgl_);
        bool ok = exec(query);
        if(!ok){QMessageBox::information(this,"Error....","Gagal memuat data list pencairan pada tanggal "+tgl_2+" Error "+query.lastError().text()+"");}
        ds_sppd_.clear();
        int no = 1;
        double jcr=0;
        while(query.next())
            {
                QString no_ =QString::number(no);
                double c = query.value(6).toDouble();
                QString jc = indo.toCurrencyString(c,"Rp ");
                ds_sppd_.append( "<tr> <td>"+no_+"</td><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td>"+query.value(2).toString()+"</td>"
                "<td>"+query.value(3).toString()+"</td><td>"+query.value(4).toString()+"</td><td>"+query.value(5).toString()+"</td><td>"+query.value(6).toString()+""
                                                                                                                                                                   "</td><td>"+query.value(7).toString()+"</td> </tr>");
                qInfo() << "generate pdf 1111"<< c ;
                jcr +=c;
                no++;
                qInfo() << "generate pdf jcr"<< jcr ;
            }
        tot_sppd = indo.toCurrencyString(jcr,"Rp ");
        qInfo() << "generate pdf 000"<< tot_sppd ;
        QString tot_terbil = tot_sppd;
        tot_terbil.replace("Rp ",""); tot_terbil.replace(".",""); tot_terbil.replace(",",".");
        bilang(tot_terbil);
        ds_sppd_.append("#/#"+tot_sppd);
        ds_sppd_.append("#/#"+tgl);
        ds_sppd_.append("#/#"+tgl_1);
        ds_sppd_.append("#/#"+thn);
        ds_sppd_.append("#/#"+nosrt);
        ds_sppd_.append("#/#"+hal);
        ds_sppd_.append("#/#"+li_kp_dns.at(0));
        ds_sppd_.append("#/#"+li_kp_dns.at(1));
        ds_sppd_.append("#/#"+li_kp_dns.at(2));
        ds_sppd_.append("#/#"+terbilang);
        //setTot_sppd(tot_sppd);
         save_sppd();
            Widget4 *pdf4 = new Widget4;
            pdf4->show();
    }
}

void Form::save_sppd(){
QString path("doc/temp/");
QFile fOut(path+"sppd_temp.txt");
if(!fOut.exists()){ QMessageBox::information(this,"Error..","Gagal Creat Data sspd_temp.txt"); return; }
fOut.open(QFile::WriteOnly | QFile::Text);
if(fOut.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"Gagal Menyimpan sppd_temp.txt"; return;} else
{
    QTextStream stream(&fOut);
    stream << ds_sppd_;
    fOut.flush();
    fOut.close();
}}


// ============ From Filter data Bamusmkam =================
void Form::muatListFilter()
{
    li_dftBams.clear();
    qInfo() << "Baca File Daftar Jabatan Bamuskam" ;
     QString path("data/");
    QFile f_bams(path+"bamuskam.txt");
    if(!f_bams.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat List Daftar Jabatan Bamuskam");return;}
     f_bams.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_bams);
     while (!str.atEnd()) {
        QString line = str.readAll();
        li_dftBams = line.split("\n");
    }
    f_bams.close();
    ui->comboBox_4->addItems(li_dftBams);
}



void Form::on_toolButton_18_clicked()
{

    ui->comboBox->setCurrentText("");
    ui->comboBox_nmKampung->setCurrentText("");

    QString jbt = ui->comboBox_4->currentText();
    QString cmd;
    if(ui->comboBox_4->currentIndex() == 0)
    {        cmd  = " SELECT id,id_kam,id_dis,id_j,distrik,kampung, nama, jabatan,no_sk, tgl_sk,ttl,alamat FROM pmk_yhk.v_bam_ ORDER BY id " ;    }
    if(ui->comboBox_4->currentIndex() > 0)
    {        cmd = " SELECT id,id_kam,id_dis,id_j,distrik,kampung, nama, jabatan,no_sk, tgl_sk,ttl,alamat FROM pmk_yhk.v_bam_ WHERE jabatan = :jbt ORDER BY id " ; }


    while(ui->tableWidget_Bamuskam->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_Bamuskam->removeRow(0);}

    QSqlQuery query;
    //QString cmd= "  SELECT id,id_kam,id_dis,id_j,distrik,kampung, nama, jabatan,no_sk, tgl_sk,ttl,alamat FROM pmk_yhk.v_bam_ WHERE id_kam = :id ORDER BY id_j ";
    query.prepare(cmd);
    query.bindValue(":jbt",jbt);
    int noBrs=0;
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data Bamuskam...""...error... "+query.lastError().text()+"...!!!"); return;}
    while (query.next()) {

            ui->tableWidget_Bamuskam->insertRow(noBrs);

            QTableWidgetItem *id_ = new QTableWidgetItem;
            QTableWidgetItem *id_kam_ = new QTableWidgetItem;
            QTableWidgetItem *id_dis_ = new QTableWidgetItem;
            QTableWidgetItem *id_j_ = new QTableWidgetItem;
            QTableWidgetItem *nmDis_ = new QTableWidgetItem;
            QTableWidgetItem *nmKam_ = new QTableWidgetItem;
            QTableWidgetItem *nama_ = new QTableWidgetItem;
            QTableWidgetItem *jabatan_ = new QTableWidgetItem;
            QTableWidgetItem *no_sk_ = new QTableWidgetItem;
            QTableWidgetItem *tgl_sk_ = new QTableWidgetItem;
            QTableWidgetItem *ttl_ = new QTableWidgetItem;
            QTableWidgetItem *almt_ = new QTableWidgetItem;

            id_->setText(query.value(0).toString());
            id_kam_->setText(query.value(1).toString());
            id_dis_->setText(query.value(2).toString());
            id_j_->setText(query.value(3).toString());
            nmDis_->setText(query.value(4).toString());
            nmKam_->setText(query.value(5).toString());
            nama_->setText(query.value(6).toString());
            jabatan_->setText(query.value(7).toString());
            no_sk_->setText(query.value(8).toString());
            QDate d = QDate::fromString(query.value(9).toString(),"yyyy-MM-dd");
            QString dd = d.toString("dd-MM-yyyy");

            tgl_sk_->setText(dd);
            ttl_->setText(query.value(10).toString());
            almt_->setText(query.value(11).toString());

            id_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            id_kam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            id_dis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            id_j_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            nmDis_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            nmKam_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            nama_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            jabatan_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            no_sk_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            tgl_sk_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ttl_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            almt_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableWidget_Bamuskam->setItem(noBrs,0,id_);
            ui->tableWidget_Bamuskam->setItem(noBrs,1,id_kam_);
            ui->tableWidget_Bamuskam->setItem(noBrs,2,id_dis_);
            ui->tableWidget_Bamuskam->setItem(noBrs,3,id_j_);
            ui->tableWidget_Bamuskam->setItem(noBrs,4,nmDis_);
            ui->tableWidget_Bamuskam->setItem(noBrs,5,nmKam_);
            ui->tableWidget_Bamuskam->setItem(noBrs,6,nama_);
            ui->tableWidget_Bamuskam->setItem(noBrs,7,jabatan_);
            ui->tableWidget_Bamuskam->setItem(noBrs,8,no_sk_);
            ui->tableWidget_Bamuskam->setItem(noBrs,9,tgl_sk_);
            ui->tableWidget_Bamuskam->setItem(noBrs,10,ttl_);
            ui->tableWidget_Bamuskam->setItem(noBrs,11,almt_);

            noBrs++;
    }

    if(noBrs==0){QMessageBox::information(this,"Info...!!!","Data Bamuskam pada kampung ini belum tersedia..."); return;}

}

void Form::updateTampilan(Mode mode)
{
    ModeSekarang = mode;

    switch (ModeSekarang) {

    case SuperMode:
        qInfo()<<"Ini adalah Mode Super USER";
        break;

    case AdminMode_1:
        qInfo()<<"Ini adalah Mode Admin_Mode_1";
        break;

    case AdminMode_2:
        qInfo()<<"Ini adalah Mode Admin Mode_2";
        break;

    case UserMode_1:
        qInfo()<<"Ini adalah Mode User MODE 1";
        ui->toolButton->setVisible(false);
        ui->label_Bamuskam->setVisible(false);
        break;

    case UserMode_2:
        qInfo()<<"Ini adalah Mode User MODE 2";

        break;
    }
}

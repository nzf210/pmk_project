#include "form.h"
#include "ui_form.h"
#include "QPdfWriter"
#include "pdf_dok.h"
#include "qrencode.h"
#include "pdf_dds.h"
#include "pdf_add.h"
#include "pdf_sppd_add.h"
#include "psd_sppd_dds.h"
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

void Form::on_toolButton_cetakpdfSppd_clicked() // Button Clik Generate pdf di sppd =====================================
{

    int j = ui->tableWidget_cetak_sppd->currentRow();
    if(j==-1){QMessageBox::information(this,"Info","Pilih Surat yang akan di cetak... "); return;}

    QString nosrt = ui->tableWidget_cetak_sppd->item(j,1)->text();
    QString hal = ui->tableWidget_cetak_sppd->item(j,2)->text();
    QString tgl = ui->tableWidget_cetak_sppd->item(j,3)->text();
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

// Button Cetak pdf untuk pengantar di main
void Form::on_toolButton_pdf_main_clicked()
{
 if(ui->comboBox_realisasi->currentIndex()==0){    Widget3 *a = new Widget3;
     a->show();}
 if(ui->comboBox_realisasi->currentIndex()==1){    QMessageBox::information(this,"Info","Menu ini Belum tersedia");}

}

void Form::on_toolButton_tmbSppd_clicked() // Button Tambah Realisasi
{
   if (menu=="4")
   {
       if(ui->comboBox_realisasi->currentIndex()==0){ sppd();}
       if(ui->comboBox_realisasi->currentIndex()==1){ sppd_2();}
   }
}


void Form::on_toolButton_Sppd_clicked() // ToolButton menu SPPD
{
    //Tambahan Filter
    boderToolbar(6);
    ui->comboBox_filter->setVisible(false);
    ui->label_filter->setVisible(false);
    ui->toolButton_filter->setVisible(false);

    menu="4";
    ui->comboBox->setVisible(false);
    ui->comboBox_nmKampung->setVisible(false);
    ui->label->setVisible(false);
    ui->label_namaKampung->setVisible(false);

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
    headsppd();
    loadsppd();
    loadsppd_2();

}


void Form::updatesppd() // Update sppd
{
    if(ui->comboBox_realisasi->currentIndex()==0)
    {
       int b = ui->tableWidget_cetak_sppd->currentRow();
       QString id = ui->tableWidget_cetak_sppd->item(b,0)->text();
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
        int b = ui->tableWidget_cetak_sppd->currentRow();
        QString id = ui->tableWidget_cetak_sppd->item(b,0)->text();
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


void Form::even_dklik_tw13() // Double Klik Event ...................................... ubah sppd
{
//disconnect(btnsppd_dk, SIGNAL(pressed()), this, SLOT(updatesppd()));
muat_sppd();
muat_sppd_2();
  if(ui->comboBox_realisasi->currentIndex()==0)
  {
    //qInfo() << "qbx  double klik 11....................... ";
      int b = ui->tableWidget_cetak_sppd->currentRow();
      QString id = ui->tableWidget_cetak_sppd->item(b,0)->text();
      QString nosrt = ui->tableWidget_cetak_sppd->item(b,1)->text();
      QString hal = ui->tableWidget_cetak_sppd->item(b,2)->text();
      QString tgl = ui->tableWidget_cetak_sppd->item(b,3)->text();

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
      int b = ui->tableWidget_cetak_sppd->currentRow();
      QString id = ui->tableWidget_cetak_sppd->item(b,0)->text();
      QString nosrt = ui->tableWidget_cetak_sppd->item(b,1)->text();
      QString hal = ui->tableWidget_cetak_sppd->item(b,2)->text();
      QString tgl = ui->tableWidget_cetak_sppd->item(b,3)->text();

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


void Form::headsppd()
{
    QStringList headerWidget;
    ui->tableWidget_cetak_sppd->setColumnCount(4);
    headerWidget <<"id" <<"No Surat"<<"Perihal" <<"Tanggal ";
    ui->tableWidget_cetak_sppd->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_cetak_sppd->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_cetak_sppd->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget_cetak_sppd->setColumnHidden(0,true);
    ui->tableWidget_cetak_sppd->setColumnWidth(1,230);
    ui->tableWidget_cetak_sppd->setColumnWidth(2,330);
    ui->tableWidget_cetak_sppd->setColumnWidth(3,70);
}


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
    while(ui->tableWidget_cetak_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_cetak_sppd->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.sppdd ORDER BY id";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error","Gagal Memuat data nosppd dds");return;}
    int i=0;
    while(query.next())
    {
                ui->tableWidget_cetak_sppd->insertRow(i);
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

                ui->tableWidget_cetak_sppd->setItem(i,0,no_);
                ui->tableWidget_cetak_sppd->setItem(i,1,nosrt_);
                ui->tableWidget_cetak_sppd->setItem(i,2,hal_);
                ui->tableWidget_cetak_sppd->setItem(i,3,tgl_);
                i++;
    }}
}

void Form::loadsppd_2()
{
    if(ui->comboBox_realisasi->currentIndex()==1){
    while(ui->tableWidget_cetak_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_cetak_sppd->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.sppdd_2 ORDER BY id";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error","Gagal Memuat data nosppd dds");return;}
    int i=0;
    while(query.next())
    {
                ui->tableWidget_cetak_sppd->insertRow(i);
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

                ui->tableWidget_cetak_sppd->setItem(i,0,no_);
                ui->tableWidget_cetak_sppd->setItem(i,1,nosrt_);
                ui->tableWidget_cetak_sppd->setItem(i,2,hal_);
                ui->tableWidget_cetak_sppd->setItem(i,3,tgl_);
                i++;
    }}
}


void Form::muatrealadd__()
{

    QDate tg = QDate::fromString(ui->dateEdit->text(),"dd-MMMM-yyyy");
    QDate tg2 = QDate::fromString(ui->dateEdit_2->text(),"dd-MMMM-yyyy");

    QString tgl = tg.toString("yyyy-MM-dd");
    QString tgl2 = tg2.toString("yyyy-MM-dd");

    int j = ui->tableWidget_realisasi_sppd->rowCount();
    int k = ui->tableWidget_realisasi_sppd->columnCount();
    for(int i=0; i<j ; i++)
    {
        double sbt=0;
        for(int ii=4; ii<k-2; ii++)
        {
            QString id = ui->tableWidget_realisasi_sppd->item(i,0)->text();
            QString a = ui->tableWidget_realisasi_sppd->item(i,3)->text();
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
           ui->tableWidget_realisasi_sppd->setItem(i,ii,jc_);

           QTableWidgetItem *sb_ = new QTableWidgetItem;
           QString sb = indo.toCurrencyString(sbt,"Rp ");
           sb_->setText(sb);
           ui->tableWidget_realisasi_sppd->setItem(i, ui->tableWidget_realisasi_sppd->columnCount()-2, sb_);


           double dd_ = aa-sbt;
           QString  dd = indo.toCurrencyString(dd_,"Rp ");
           QTableWidgetItem *sb__ = new QTableWidgetItem;
           sb__->setText(dd);
           ui->tableWidget_realisasi_sppd->setItem(i, ui->tableWidget_realisasi_sppd->columnCount()-1, sb__);

           /**/ }

        }
        sbt=0;
    }
}


void Form::muatrealadd_() // Muat Data Kampung Untuk Realisasi
{
    while(ui->tableWidget_realisasi_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_realisasi_sppd->removeRow(0);}

    QSqlQuery query;
    QString cmd = "SELECT no,distrik,kampung,pagu_add FROM pmk_yhk.m_kampung ORDER BY no";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Nama Distrik dan Kampung di realisasi Dana Desa .."); return;}
    int no=0;
    while(query.next())
    {
        ui->tableWidget_realisasi_sppd->insertRow(no);
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

       ui->tableWidget_realisasi_sppd->setItem(no,0,id_);
       ui->tableWidget_realisasi_sppd->setItem(no,1,nmdis_);
       ui->tableWidget_realisasi_sppd->setItem(no,2,nmkam_);
       ui->tableWidget_realisasi_sppd->setItem(no,3,pagu_);

        no++;
}}


void Form::muatheadertw12_2()
{
    int j = li_realadd.count();
    ui->tableWidget_realisasi_sppd->setColumnCount(j);
    ui->tableWidget_realisasi_sppd->setHorizontalHeaderLabels(li_realadd );
    ui->tableWidget_realisasi_sppd->setColumnHidden(0,true);

}


void Form::muatrealdds__()
{

    QDate tg = QDate::fromString(ui->dateEdit->text(),"dd-MMMM-yyyy");
    QDate tg2 = QDate::fromString(ui->dateEdit_2->text(),"dd-MMMM-yyyy");

    QString tgl = tg.toString("yyyy-MM-dd");
    QString tgl2 = tg2.toString("yyyy-MM-dd");

    int j = ui->tableWidget_realisasi_sppd->rowCount();
    int k = ui->tableWidget_realisasi_sppd->columnCount();

    for(int i=0; i<j ; i++)
    {
        double sbt=0;
        for(int ii=4; ii<k-2; ii++)
        {
                qInfo()<<"iiiii i" << i;
                qInfo()<<"iiiii Ii" << ii;
                qInfo()<<"iiiii k3" << k-2;

            QString id = ui->tableWidget_realisasi_sppd->item(i,0)->text();
            QString a = ui->tableWidget_realisasi_sppd->item(i,3)->text();
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
           ui->tableWidget_realisasi_sppd->setItem(i,ii,jc_);

           QTableWidgetItem *sb_ = new QTableWidgetItem;
           QString sb = indo.toCurrencyString(sbt,"Rp ");
           sb_->setText(sb);
           ui->tableWidget_realisasi_sppd->setItem(i, ui->tableWidget_realisasi_sppd->columnCount()-2, sb_);


           double dd_ = aa-sbt;
           QString  dd = indo.toCurrencyString(dd_,"Rp ");
           QTableWidgetItem *sb__ = new QTableWidgetItem;
           sb__->setText(dd);
           ui->tableWidget_realisasi_sppd->setItem(i, ui->tableWidget_realisasi_sppd->columnCount()-1, sb__);

           /**/ }

        }
        sbt=0;
    }
}


void Form::muatrealdds_() // Muat Data Kampung Untuk Realisasi
{
    while(ui->tableWidget_realisasi_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_realisasi_sppd->removeRow(0);}

    QSqlQuery query;
    QString cmd = "SELECT no,distrik,kampung,pagu FROM pmk_yhk.m_kampung ORDER BY no";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Nama Distrik dan Kampung di realisasi Dana Desa .."); return;}
    int no=0;
    while(query.next())
    {
        ui->tableWidget_realisasi_sppd->insertRow(no);
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

       ui->tableWidget_realisasi_sppd->setItem(no,0,id_);
       ui->tableWidget_realisasi_sppd->setItem(no,1,nmdis_);
       ui->tableWidget_realisasi_sppd->setItem(no,2,nmkam_);
       ui->tableWidget_realisasi_sppd->setItem(no,3,pagu_);

        no++;
}}


void Form::muatheadertw12()
{
    int j = li_realdds.count();
    ui->tableWidget_realisasi_sppd->setColumnCount(j);
    ui->tableWidget_realisasi_sppd->setHorizontalHeaderLabels(li_realdds );
    ui->tableWidget_realisasi_sppd->setColumnHidden(0,true);
}



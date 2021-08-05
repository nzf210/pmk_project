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



// Simpan Halam Untuk Semua Data
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

    int j = ui->tableWidget_realisasi_sppd->currentRow();
    int jBrs = ui->tableWidget_cetak_sppd->rowCount() ;
    if(j==-1){QMessageBox::information(this,"Info","Pilih Surat yang akan di cetak... "); return;}

    QString nosrt = ui->tableWidget_realisasi_sppd->item(j,1)->text();
    QString hal = ui->tableWidget_realisasi_sppd->item(j,2)->text();
    QString tgl = ui->tableWidget_realisasi_sppd->item(j,3)->text();
    QDate dt = QDate::fromString(tgl,"dd-MM-yyyy");
    QString tgl_ = dt.toString("yyyy-MM-dd");
//    QString tgl_1 = dt.toString("dd MMMM yyyy");
   QString tgl_1 = QLocale{QLocale::Indonesian}.toString(dt, "dd MMMM yyyy");
    QString tgl_2 = QLocale{QLocale::Indonesian}.toString(dt, "dd MMMM yyyy");
    QString thn = tgl_1.right(4);
    double jcr=0;
    kp_dns();
    ds_sppd_.clear();
qInfo() << "cek sppd ==============" ;
    for (int i =0  ; i< jBrs ; i++ ) {
        QString no_ =QString::number(i+1);
        QString nm_kampung = ui->tableWidget_cetak_sppd->item(i,1)->text();
        QString nm_distrik = ui->tableWidget_cetak_sppd->item(i,2)->text();
        QString nm_kepalak = ui->tableWidget_cetak_sppd->item(i,7)->text();
        QString nm_benk = ui->tableWidget_cetak_sppd->item(i,6)->text();
        QString nm_rek = ui->tableWidget_cetak_sppd->item(i,4)->text();
        QString no_rek = ui->tableWidget_cetak_sppd->item(i,3)->text();
        QString j_cair = ui->tableWidget_cetak_sppd->item(i,8)->text();
        QString thp = ui->tableWidget_cetak_sppd->item(i,0)->text();
        //QString wnorek = " width=140px; ";

        ds_sppd_.append("<tr><td>"+no_+"</td><td>"+nm_kampung+"</td><td>"+nm_distrik+"</td><td>"+nm_kepalak+"</td>"
        "<td>"+nm_benk+"</td><td>"+nm_rek+"</td><td>"+no_rek+"</td><td>"+j_cair+"</td><td>"+thp+"</td></tr>" );

         j_cair.replace(",00",""); j_cair.replace("Rp ",""); j_cair.replace(".","");
         double jc = j_cair.toDouble();
         jcr += jc;
         qInfo() << "cek sppd ==============" << i;
    }

     qInfo() << "cek sppd ============== luar"  ;
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
    ds_sppd_.append("#/#"+li_kp_dns.at(1));
    ds_sppd_.append("#/#"+li_kp_dns.at(2));
    ds_sppd_.append("#/#"+li_kp_dns.at(3));
    ds_sppd_.append("#/#"+terbilang);
    ds_sppd_.append("#/#"+li_kp_dns.at(0));
    //setTot_sppd(tot_sppd);
    save_sppd();
   Widget3 *pdf3 = new Widget3;
   QIcon logo(":/gbr/html/gbr/yhk.png");
   pdf3->setWindowIcon(logo);
   pdf3->show();

}

// Button Cetak pdf untuk pengantar di main
void Form::on_toolButton_pdf_main_clicked()
{/*
 if(ui->comboBox_realisasi->currentIndex()==0){    Widget3 *a = new Widget3;
     a->show();}
 if(ui->comboBox_realisasi->currentIndex()==1){    QMessageBox::information(this,"Info","Menu ini Belum tersedia");}*/
}

void Form::on_toolButton_tmbSppd_clicked() // Button Tambah Realisasi
{
   if (menu=="4")
   {
       if(ui->comboBox_realisasi->currentIndex()==0){QMessageBox::information(this,"Info...","Pilih menu Pencairan..."); return;}
       int a =0;
       int b =0;
       QString aa=ui->comboBox_realisasi->currentText();

       if(aa=="Realisasi Dana Desa") {  a=8; b=9; }
       if(aa=="Realisasi Alokasi Dana Desa") { a=10; b=11; }
       if(aa=="Realisasi Dana BLT") { a=12; b=13; }
       if(aa=="Realisasi Dana Covid") { a=14; b=15; }

       tambah_sppd(a,b);

   }
}


void Form::on_toolButton_Sppd_clicked() // ToolButton menu SPPD
{
    menu="4";
    boderToolbar(6);
    ui->comboBox_filter->setVisible(false);
    ui->label_filter->setVisible(false);
    ui->toolButton_filter->setVisible(false);
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


    //QDate d = QDate::fromString(tgl,"dd-MM-yyyy");

    QDate d =QDate::currentDate();
    QString de = d.toString("yyyy");
    QString de_ = "01-01-"+de;
    QDate dd = QLocale{QLocale::Indonesian}.toDate(de_,"dd MMMM yyyy");

    ui->dateEdit->setDate(dd);
    ui->dateEdit->setLocale(QLocale::Indonesian);
    ui->dateEdit->setDisplayFormat("dd MMMM yyyy");
    ui->dateEdit->setCalendarPopup(true);

    ui->dateEdit_2->setLocale(QLocale::Indonesian);
    ui->dateEdit_2->setDisplayFormat("dd MMMM yyyy");
    ui->dateEdit_2->setCalendarPopup(true);
    ui->dateEdit_2->setDate(d);

    ui->label_18->setText("Realisasi Anggaran Desa Tahun Anggaran "+de);

    while(ui->tableWidget_realisasi_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
             {ui->tableWidget_realisasi_sppd->removeRow(0);}
     while(ui->tableWidget_cetak_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
              {ui->tableWidget_cetak_sppd->removeRow(0);}
     QStringList qb3;
     qb3 <<"Pilih ... " << "Realisasi Dana Desa" << "Realisasi Dana BLT" << "Realisasi Dana Covid" << "Realisasi Alokasi Dana Desa" ;
     ui->comboBox_realisasi->clear();
     ui->comboBox_realisasi->addItems(qb3);
     ui->comboBox_realisasi->setCurrentIndex(0);
}


void Form::updatesppd() // Update sppd
{

}


void Form::even_dklik_tw13() // Double Klik Event ...................................... ubah sppd
{


}


void Form::headsppd()
{
//    QStringList headerWidget;
//    ui->tableWidget_cetak_sppd->setColumnCount(4);
//    headerWidget <<"id" <<"No Surat"<<"Perihal" <<"Tanggal ";
//    ui->tableWidget_cetak_sppd->setHorizontalHeaderLabels(headerWidget);
//    ui->tableWidget_cetak_sppd->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget_cetak_sppd->setSelectionBehavior(QAbstractItemView::SelectRows);

//    ui->tableWidget_cetak_sppd->setColumnHidden(0,true);
//    ui->tableWidget_cetak_sppd->setColumnWidth(1,230);
//   // ui->tableWidget_cetak_sppd->setColumnWidth(2,330);
//    //ui->tableWidget_cetak_sppd->setColumnWidth(3,70);
}


void Form::tambah_sppd(int noSrt1,int noSrt2) // Form Cetak Surat Pengantar Pencairan
{


muat_nosurat();
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
QString noSrt = li_no_surat.at(noSrt1);
noSrt.replace("****", nosurat());
noSrt.replace("no_sppd_dds=> ","");
noSrt.replace("no_sppd_add=> ","");
noSrt.replace("no_sppd_blt=> ","");
noSrt.replace("no_sppd_covid=> ","");
qbnosppd->addItem(noSrt);

QString ha = li_no_surat.at(noSrt2);
ha.replace("Perihal_dds=> ","");
ha.replace("Perihal_add=> ","");
ha.replace("Perihal_blt=> ","");
ha.replace("Perihal_covid=> ","");
qbhal->addItem(ha);

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

    QString a;
    QString aa=ui->comboBox_realisasi->currentText();

    if(aa=="Realisasi Dana Desa") {  a="dds"; }
    if(aa=="Realisasi Alokasi Dana Desa") { a="add"; }
    if(aa=="Realisasi Dana BLT") { a="blt"; }
    if(aa=="Realisasi Dana Covid") { a="covid"; }
    insert_sppd(a,nosrt, hal, date);

    disconnect(btnsppd, SIGNAL(pressed()), this, SLOT(sppdbtc()));
}



void Form::insert_sppd(QString type ,QString nosrt, QString hal,QString date)
{

QString a = nosrt;
//a.replace("****",nosurat());
QString b = hal;
QString c = date;
QDate dt = QDate::fromString(c,"dd-MM-yyyy");
QString d = dt.toString("yyyy-MM-dd");

//nfo() << "sppd btc 2 2 ..................."<< a <<"==" << b << "==" << c;
QSqlQuery query;
QString cmd = "INSERT INTO pmk_yhk.sppdd_"+type+" (no_srt,perihal,tgl,nos) VALUES (:nosrt, :hal, :tgl, '1') ";
query.prepare(cmd);
query.bindValue(":nosrt", a);
query.bindValue(":hal",b);
query.bindValue(":tgl",d);
bool ok = exec(query);
if(!ok){QMessageBox::information(this,"Error","Gagal Menyimpan data nosppd add");return;}
wg->close();
muat_data_realisasi_sppdd("");

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
}


void Form::muatrealadd__()
{
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


void Form::muatheadertw_realisasi_sppd()
{
    QStringList list;
    list << "id" << "no_surat" << "perihal" << "tanggal" << "nos";
    int j = list.count();
    ui->tableWidget_realisasi_sppd->setColumnCount(j);
    ui->tableWidget_realisasi_sppd->setHorizontalHeaderLabels(list);
    ui->tableWidget_realisasi_sppd->setColumnHidden(0,true);
    ui->tableWidget_realisasi_sppd->setColumnHidden(4,true);
}


void Form::muatheadertw_cetak_sppd()
{
    QStringList head;
    head << "Tahap" << "Kampung" << "Distrik" << "No Rek" << "Nama Rek" << "Nama Bank" << "Bendahara" << "Kepala Kampung" << "Jml Cair" << "Terbilang" << "Tgl" << "Thp II";
    ui->tableWidget_cetak_sppd->setColumnCount(head.count());
    ui->tableWidget_cetak_sppd->setHorizontalHeaderLabels(head);
    ui->tableWidget_cetak_sppd->setColumnHidden(11,true);

    ui->tableWidget_cetak_sppd->setColumnWidth(0,70);
    ui->tableWidget_cetak_sppd->setColumnWidth(1,120);
    ui->tableWidget_cetak_sppd->setColumnWidth(2,120);
    ui->tableWidget_cetak_sppd->setColumnWidth(3,130);
    ui->tableWidget_cetak_sppd->setColumnWidth(4,230);
    ui->tableWidget_cetak_sppd->setColumnWidth(5,230);
    ui->tableWidget_cetak_sppd->setColumnWidth(6,150);
    ui->tableWidget_cetak_sppd->setColumnWidth(7,150);
    ui->tableWidget_cetak_sppd->setColumnWidth(8,100);
    ui->tableWidget_cetak_sppd->setColumnWidth(9,250);
    ui->tableWidget_cetak_sppd->setColumnWidth(10,100);
   // ui->tableWidget_cetak_sppd->setColumnWidth(11,100);
}

void Form::on_comboBox_realisasi_currentIndexChanged(const QString &arg1)
{
    qInfo() << "qCombo realisai " << arg1;
    QString a = arg1;
    QString b="";

    if(a=="Realisasi Dana BLT") { qInfo() << "dana BLT"; b="blt"; }
    if(a=="Realisasi Dana Covid") { qInfo() << "dana Covid"; b="covid"; }
    if(a=="Realisasi Dana Desa") { qInfo() << "dana Desa" ; b="dds"; }
    if(a=="Realisasi Alokasi Dana Desa") { qInfo() << "dana Alokasi Dana Desa" ; b="add"; }

    if(ui->comboBox_realisasi->currentIndex() == 0) {
        while( ui->tableWidget_cetak_sppd->rowCount()>0 )// untuk Hilangkan Tambahan jika button di klik ulang
        { ui->tableWidget_cetak_sppd->removeRow(0); }
        while( ui->tableWidget_realisasi_sppd->rowCount()>0 )// untuk Hilangkan Tambahan jika button di klik ulang
        { ui->tableWidget_realisasi_sppd->removeRow(0); }
    }
    if(ui->comboBox_realisasi->currentIndex() > 0) { select_tgl_sppd("");  muat_data_realisasi_sppdd("");}
}

void Form::muat_data_realisasi_sppdd(QString ppdd)
{

if(ui->comboBox_realisasi->currentIndex()>0){
    qInfo() << ppdd;
    QString a;
    QString aa=ui->comboBox_realisasi->currentText();

    if(aa=="Realisasi Dana Desa") {  a="dds"; }
    if(aa=="Realisasi Alokasi Dana Desa") { a="add"; }
    if(aa=="Realisasi Dana BLT") { a="blt"; }
    if(aa=="Realisasi Dana Covid") { a="covid"; }

   muatheadertw_realisasi_sppd();
   while(ui->tableWidget_realisasi_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_realisasi_sppd->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.sppdd_"+a+" ORDER BY sppdd_"+a+".tgl DESC";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Data tw Realisasi ... ");return;}
    int i=0;
    while(query.next())
    {
                ui->tableWidget_realisasi_sppd->insertRow(i);
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

                ui->tableWidget_realisasi_sppd->setItem(i,0,no_);
                ui->tableWidget_realisasi_sppd->setItem(i,1,nosrt_);
                ui->tableWidget_realisasi_sppd->setItem(i,2,hal_);
                ui->tableWidget_realisasi_sppd->setItem(i,3,tgl_);
                i++;
    }
}}


void Form::on_tableWidget_realisasi_sppd_cellClicked(int row, int column)
{
 if(ui->comboBox_realisasi->currentIndex()==0){ QMessageBox::information(this,"Info...","Pilih data realisasi"); return; }
 qInfo () << "Even cell klik tw realisasi" << row << "===" << column;
 QDate tg = QDate::fromString(ui->tableWidget_realisasi_sppd->item(row,3)->text(),"dd-MM-yyyy");
 //QString tgl = tg.toString("yyyy-MM-dd");
 QString tgl_ = tg.toString("yyyy-MM-dd");
 select_tgl_sppd(tgl_);
}

void Form::select_tgl_sppd(QString tgl)
{
    if(ui->comboBox_realisasi->currentIndex()>0){

    muatheadertw_cetak_sppd();
    while( ui->tableWidget_cetak_sppd->rowCount()>0 )// untuk Hilangkan Tambahan jika button di klik ulang
    { ui->tableWidget_cetak_sppd->removeRow(0); }

    QString dds, dds_,add, add_,blt, blt_,covid, covid_;
    QSqlQuery query;
    QString cmd;

    dds= "SELECT "
             "pmk_yhk.dds_cair.thp_cair, "
             "pmk_yhk.dds_cair.nm_kam, "
             "pmk_yhk.dds_cair.nm_dis, "
             "pmk_yhk.dds_cair.no_rek, "
             "pmk_yhk.dds_cair.nm_rek, "
             "pmk_yhk.dds_cair.nm_bank, "
             "pmk_yhk.dds_cair.nm_ben, "
             "pmk_yhk.dds_cair.nm_kp, "
             "pmk_yhk.dds_cair.j_cair, "
             "pmk_yhk.dds_cair.j_terbilang, "
             "pmk_yhk.dds_cair.tgl, "
             "pmk_yhk.dds_cair.thp_l "
        " FROM "
             "pmk_yhk.sppdd_dds"
            " RIGHT JOIN "
             " pmk_yhk.dds_cair "
            " ON "
                 " pmk_yhk.sppdd_dds.tgl = pmk_yhk.dds_cair.tgl "
         "WHERE "
             " pmk_yhk.dds_cair.j_cair > 0 AND "
             " pmk_yhk.sppdd_dds.tgl = :tgl ";

    dds_= "SELECT "
             "pmk_yhk.dds_cair.thp_cair, "
             "pmk_yhk.dds_cair.nm_kam, "
             "pmk_yhk.dds_cair.nm_dis, "
             "pmk_yhk.dds_cair.no_rek, "
             "pmk_yhk.dds_cair.nm_rek, "
             "pmk_yhk.dds_cair.nm_bank, "
             "pmk_yhk.dds_cair.nm_ben, "
             "pmk_yhk.dds_cair.nm_kp, "
             "pmk_yhk.dds_cair.j_cair, "
             "pmk_yhk.dds_cair.j_terbilang, "
             "pmk_yhk.dds_cair.tgl, "
             "pmk_yhk.dds_cair.thp_l "
        " FROM "
             "pmk_yhk.sppdd_dds"
            " RIGHT JOIN "
             " pmk_yhk.dds_cair "
            " ON "
                 " pmk_yhk.sppdd_dds.tgl = pmk_yhk.dds_cair.tgl "
         "WHERE "
             " pmk_yhk.dds_cair.j_cair > 0 LIMIT 300";

    add = " SELECT pmk_yhk.add_cair.thp_cair, pmk_yhk.add_cair.nm_kam, pmk_yhk.add_cair.nm_dis, pmk_yhk.add_cair.no_rek, pmk_yhk.add_cair.nm_rek, pmk_yhk.add_cair.nm_bank, pmk_yhk.add_cair.nm_ben, "
          " pmk_yhk.add_cair.nm_kp, pmk_yhk.add_cair.jml_cair_add, pmk_yhk.add_cair.j_terbilang, pmk_yhk.add_cair.tgl, pmk_yhk.add_cair.thp_l  FROM pmk_yhk.sppdd_add RIGHT JOIN  pmk_yhk.add_cair  "
          " ON  pmk_yhk.sppdd_add.tgl = pmk_yhk.add_cair.tgl WHERE  pmk_yhk.add_cair.jml_cair_add > 0 AND  pmk_yhk.sppdd_add.tgl = :tgl ";

    add_ = "SELECT pmk_yhk.add_cair.thp_cair, pmk_yhk.add_cair.nm_kam, pmk_yhk.add_cair.nm_dis, pmk_yhk.add_cair.no_rek, pmk_yhk.add_cair.nm_rek, pmk_yhk.add_cair.nm_bank, pmk_yhk.add_cair.nm_ben, "
          " pmk_yhk.add_cair.nm_kp, pmk_yhk.add_cair.jml_cair_add, pmk_yhk.add_cair.j_terbilang, pmk_yhk.add_cair.tgl, pmk_yhk.add_cair.thp_l  FROM pmk_yhk.sppdd_add RIGHT JOIN  pmk_yhk.add_cair  "
          " ON  pmk_yhk.sppdd_add.tgl = pmk_yhk.add_cair.tgl WHERE  pmk_yhk.add_cair.jml_cair_add > 0  LIMIT 300";

    covid = " SELECT pmk_yhk.covid_cair.thp_cair, pmk_yhk.covid_cair.nm_kam, pmk_yhk.covid_cair.nm_dis, pmk_yhk.covid_cair.no_rek, pmk_yhk.covid_cair.nm_rek, pmk_yhk.covid_cair.nm_bank, "
            " pmk_yhk.covid_cair.nm_ben, pmk_yhk.covid_cair.nm_kp, pmk_yhk.covid_cair.jml_cair_covid, pmk_yhk.covid_cair.j_terbilang, pmk_yhk.covid_cair.tgl, pmk_yhk.covid_cair.thp_l  "
            " FROM pmk_yhk.sppdd_covid RIGHT JOIN  pmk_yhk.covid_cair  ON  pmk_yhk.sppdd_covid.tgl = pmk_yhk.covid_cair.tgl WHERE  pmk_yhk.covid_cair.jml_cair_covid > 0 AND  pmk_yhk.sppdd_covid.tgl = :tgl " ;

    covid_ = " SELECT pmk_yhk.covid_cair.thp_cair, pmk_yhk.covid_cair.nm_kam, pmk_yhk.covid_cair.nm_dis, pmk_yhk.covid_cair.no_rek, pmk_yhk.covid_cair.nm_rek, pmk_yhk.covid_cair.nm_bank, "
            " pmk_yhk.covid_cair.nm_ben, pmk_yhk.covid_cair.nm_kp, pmk_yhk.covid_cair.jml_cair_covid, pmk_yhk.covid_cair.j_terbilang, pmk_yhk.covid_cair.tgl, pmk_yhk.covid_cair.thp_l  "
            " FROM pmk_yhk.sppdd_covid RIGHT JOIN  pmk_yhk.covid_cair  ON  pmk_yhk.sppdd_covid.tgl = pmk_yhk.covid_cair.tgl WHERE  pmk_yhk.covid_cair.jml_cair_covid > 0 LIMIT 300" ;

    blt = " SELECT pmk_yhk.blt_cair.thp_cair, pmk_yhk.blt_cair.nm_kam, pmk_yhk.blt_cair.nm_dis, pmk_yhk.blt_cair.no_rek, pmk_yhk.blt_cair.nm_rek, pmk_yhk.blt_cair.nm_bank, pmk_yhk.blt_cair.nm_ben, "
          " pmk_yhk.blt_cair.nm_kp, pmk_yhk.blt_cair.jml_cair_blt, pmk_yhk.blt_cair.j_terbilang, pmk_yhk.blt_cair.tgl, pmk_yhk.blt_cair.thp_l  FROM pmk_yhk.sppdd_blt RIGHT JOIN  pmk_yhk.blt_cair  ON  "
          " pmk_yhk.sppdd_blt.tgl = pmk_yhk.blt_cair.tgl WHERE  pmk_yhk.blt_cair.jml_cair_blt > 0 AND  pmk_yhk.sppdd_blt.tgl =  :tgl ";

    blt_ = " SELECT pmk_yhk.blt_cair.thp_cair, pmk_yhk.blt_cair.nm_kam, pmk_yhk.blt_cair.nm_dis, pmk_yhk.blt_cair.no_rek, pmk_yhk.blt_cair.nm_rek, pmk_yhk.blt_cair.nm_bank, pmk_yhk.blt_cair.nm_ben, "
          " pmk_yhk.blt_cair.nm_kp, pmk_yhk.blt_cair.jml_cair_blt, pmk_yhk.blt_cair.j_terbilang, pmk_yhk.blt_cair.tgl, pmk_yhk.blt_cair.thp_l  FROM pmk_yhk.sppdd_blt RIGHT JOIN  pmk_yhk.blt_cair  ON  "
          " pmk_yhk.sppdd_blt.tgl = pmk_yhk.blt_cair.tgl WHERE  pmk_yhk.blt_cair.jml_cair_blt > 0  LIMIT 300";

     QString aa=ui->comboBox_realisasi->currentText();
    if(tgl == "")
    {
        //if(aa!=""){return;}
        if(aa=="Realisasi Dana Desa") {  cmd=dds_; }
        if(aa=="Realisasi Alokasi Dana Desa") { cmd=add_; }
        if(aa=="Realisasi Dana BLT") { cmd=blt_; }
        if(aa=="Realisasi Dana Covid") { cmd=covid_; }

    }

    if(tgl != "")
    {
        //if(aa==""){return;}
        if(aa=="Realisasi Dana Desa") {  cmd=dds; }
        if(aa=="Realisasi Alokasi Dana Desa") { cmd=add; }
        if(aa=="Realisasi Dana BLT") { cmd=blt; }
        if(aa=="Realisasi Dana Covid") { cmd=covid; }
    }

     query.prepare(cmd);
     query.bindValue(":tgl", tgl);
     bool ok = exec(query);
     if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Daftar ... "); return;}
     int i=0;
     while(query.next())
     {
                 ui->tableWidget_cetak_sppd->insertRow(i);
                 QTableWidgetItem *tahap = new QTableWidgetItem;
                 QTableWidgetItem *kmp = new QTableWidgetItem;
                 QTableWidgetItem *dis = new QTableWidgetItem;
                 QTableWidgetItem *norek = new QTableWidgetItem;
                 QTableWidgetItem *nmrek = new QTableWidgetItem;
                 QTableWidgetItem *nmbank = new QTableWidgetItem;
                 QTableWidgetItem *bend = new QTableWidgetItem;
                 QTableWidgetItem *kkap = new QTableWidgetItem;
                 QTableWidgetItem *jml = new QTableWidgetItem;
                 QTableWidgetItem *terbilang = new QTableWidgetItem;
                 QTableWidgetItem *tgl = new QTableWidgetItem;
                 QTableWidgetItem *thpII = new QTableWidgetItem;

                 tahap->setText(query.value(0).toString());
                 kmp->setText(query.value(1).toString());
                 dis->setText(query.value(2).toString());
                 norek->setText(query.value(3).toString());
                 nmrek->setText(query.value(4).toString());
                 nmbank->setText(query.value(5).toString());
                 bend->setText(query.value(6).toString());
                 kkap->setText(query.value(7).toString());
                 double jj = (query.value(8).toDouble());
                 QString jjj = indo.toCurrencyString(jj,"Rp ");
                 jml->setText(jjj);
                 terbilang->setText(query.value(9).toString());
                 QDate dt = QDate::fromString(query.value(10).toString(),"yyyy-MM-dd");
                 tgl->setText(dt.toString("dd-MM-yyyy"));
                 thpII->setText(query.value(11).toString());

                 tahap->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 kmp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 dis->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 norek->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 nmrek->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 nmbank->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 bend->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 kkap->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 jml->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 terbilang->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 tgl->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
                 thpII->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                 ui->tableWidget_cetak_sppd->setItem(i,0,tahap);
                 ui->tableWidget_cetak_sppd->setItem(i,1,kmp);
                 ui->tableWidget_cetak_sppd->setItem(i,2,dis);
                 ui->tableWidget_cetak_sppd->setItem(i,3,norek);
                 ui->tableWidget_cetak_sppd->setItem(i,4,nmrek);
                 ui->tableWidget_cetak_sppd->setItem(i,5,nmbank);
                 ui->tableWidget_cetak_sppd->setItem(i,6,bend);
                 ui->tableWidget_cetak_sppd->setItem(i,7,kkap);
                 ui->tableWidget_cetak_sppd->setItem(i,8,jml);
                 ui->tableWidget_cetak_sppd->setItem(i,9,terbilang);
                 ui->tableWidget_cetak_sppd->setItem(i,10,tgl);
                 ui->tableWidget_cetak_sppd->setItem(i,11,thpII);

                 i++;
     }
    }
}


void Form::on_tableWidget_realisasi_sppd_cellDoubleClicked(int row, int column)
{
   qInfo() << "Column" << column;
    QString id = ui->tableWidget_realisasi_sppd->item(row,0)->text();
    QString noSrt = ui->tableWidget_realisasi_sppd->item(row,1)->text();
    QString perihal = ui->tableWidget_realisasi_sppd->item(row,2)->text();
    QString tanggal = ui->tableWidget_realisasi_sppd->item(row,3)->text();
    update_data_sppd(id,tanggal, noSrt, perihal);
}


void Form::update_data_sppd(QString id, QString date, QString no_Srt, QString perihal){

    id_update_sppd = id;
    QDate dt = QDate::fromString(date,"dd-MM-yyyy");
    desppd = new QDateEdit;
    desppd->setDate(dt);
    desppd->setDisplayFormat("dd-MM-yyyy");
    desppd->setCalendarPopup(true);

    QString tgl = desppd->text();
    QString thn = tgl.right(4);
    qbnosppd = new QComboBox;
    qbhal = new QComboBox;
    qbnosppd->setEditable(true);
    qbhal->setEditable(true);
    QString noSrt = no_Srt;
    qbnosppd->addItem(noSrt);

    QString ha = perihal;
    qbhal->addItem(ha);

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
    wg->setWindowTitle(" Updata Data Surat Pengantar Pencairan Dana Desa "+thn+"");
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

    btnsppd->setText("UPDATE");
    btnsppd->setMaximumWidth(140);
    HL4->addWidget(btnsppd);
    wg->show();
    connect(btnsppd, SIGNAL(pressed()), this, SLOT(update_sppd_toDB()));
}

void Form::update_sppd_toDB()
{
    qInfo() << "Insert Update ";

    QString a;
    QString aa=ui->comboBox_realisasi->currentText();

    if(aa=="Realisasi Dana Desa") {  a="dds"; }
    if(aa=="Realisasi Alokasi Dana Desa") { a="add"; }
    if(aa=="Realisasi Dana BLT") { a="blt"; }
    if(aa=="Realisasi Dana Covid") { a="covid"; }

    if(ui->comboBox_realisasi->currentIndex()>0){

    QString nosrt = qbnosppd->currentText();
    QString hal = qbhal->currentText();
    QString tgl =  desppd->text();
    QDate dt = QDate::fromString(tgl,"dd-MM-yyyy");
    QString tgl_ = dt.toString("yyyy-MM-dd");
    //nfo()<< " info 000" << nosrt << "==" << hal << "==" << tgl;
    QSqlQuery  query;
    QString cmd = " UPDATE pmk_yhk.sppdd_"+a+" SET no_srt = :no_srt, perihal= :hal, tgl= :tgl  WHERE id= :id " ;
    query.prepare(cmd);
    query.bindValue(":no_srt",nosrt);
    query.bindValue(":hal",hal);
    query.bindValue(":tgl",tgl_);
    query.bindValue(":hal",hal);
    query.bindValue(":id",id_update_sppd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error","Gagal Update data Surat Pencairan "+query.lastError().text()+""); return;}
    if(ok){QMessageBox::information(this,"Info ... ","Berhasil Update data Surat Pencairan");
    disconnect(btnsppd, SIGNAL(pressed()), this, SLOT(update_sppd_toDB()));}
     }
    wg->close();
    muat_data_realisasi_sppdd("");
    select_tgl_sppd("");
}



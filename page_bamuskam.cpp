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



void Form::on_toolButton_clicked() //tb 1
{
    boderToolbar(1);
    ui->comboBox->setVisible(true);
    ui->comboBox_nmKampung->setVisible(true);
    ui->label->setVisible(true);
    ui->label_namaKampung->setVisible(true);
    ui->toolButton_refResh->setVisible(true);

    ui->label_realisasi->setVisible(false);
    ui->label_s_d->setVisible(false);
    ui->comboBox_realisasi->setVisible(false);
    ui->toolButton_pdf_main->setVisible(false);

    ui->dateEdit->setVisible(false);
    ui->dateEdit_2->setVisible(false);

    //Tambahan Filter
    ui->comboBox_filter->setVisible(true);
    ui->label_filter->setVisible(true);
    ui->toolButton_filter->setVisible(true);
    ui->comboBox_filter->clear();
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
    ui->comboBox_filter->addItems(li_dftBams);
}



void Form::on_toolButton_filter_clicked()
{

    ui->comboBox->setCurrentText("");
    ui->comboBox_nmKampung->setCurrentText("");
    QString jbt = ui->comboBox_filter->currentText();
    QString cmd;
    if(ui->comboBox_filter->currentIndex() == 0)
    {        cmd  = " SELECT id,id_kam,id_dis,id_j,distrik,kampung, nama, jabatan,no_sk, tgl_sk,ttl,alamat FROM pmk_yhk.v_bam_ ORDER BY id " ;    }
    if(ui->comboBox_filter->currentIndex() > 0)
    {        cmd = " SELECT id,id_kam,id_dis,id_j,distrik,kampung, nama, jabatan,no_sk, tgl_sk,ttl,alamat FROM pmk_yhk.v_bam_ WHERE jabatan = :jbt ORDER BY id " ; }

    while(ui->tableWidget_Bamuskam->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_Bamuskam->removeRow(0);}

    QSqlQuery query;
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

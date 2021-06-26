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


void Form::on_toolButton_blt_clicked()
{
    boderToolbar(5);
    ui->stackedWidget->setCurrentIndex(5);
    sembunyi_subMenu();
    memuatData_twRealisasiblt();

}

void Form::memuatData_twRealisasiblt(){
    header_twRealisasiblt();
   while(ui->tableWidget_realisasi_blt->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_realisasi_blt->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT pmk_yhk.blt.nama_kampung,pmk_yhk.blt.nama_distrik,pmk_yhk.blt.id_dis, pmk_yhk.blt.id_k,pmk_yhk.blt.pagu,"
                  "SUM ( pmk_yhk.blt_cair.jml_cair_blt ) AS total_pencairan, pmk_yhk.blt.pagu - SUM ( pmk_yhk.blt_cair.jml_cair_blt ) AS sisa_dana "
       " FROM"
           " pmk_yhk.blt"
           " LEFT JOIN pmk_yhk.blt_cair ON pmk_yhk.blt.id_k = pmk_yhk.blt_cair.id_kam "
        "GROUP BY"
           " pmk_yhk.blt.nama_kampung,"
            "pmk_yhk.blt.nama_distrik,"
            "pmk_yhk.blt.id_dis,"
            "pmk_yhk.blt.id_k,"
           " pmk_yhk.blt_cair.jml_cair_blt "
       " ORDER BY "
           "pmk_yhk.blt.id_k";
    //if(s_id_kamp==""){cmd ="SELECT * FROM pmk_yhk.t_real ORDER BY id_real ";}
     //if(s_id_kamp==""){cmd =" SELECT	* FROM	pmk_yhk.t_real ORDER BY	id_real ASC, pmk_yhk.t_real.id_real LIMIT 40 ";}
    query.prepare(cmd);
    //query.bindValue(":id",s_id_kamp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data realisasi"); return;}
    int i=0;
    while (query.next()) {

        //int i = ui->tableWidget_realisasi_blt->rowCount();
        ui->tableWidget_realisasi_blt->insertRow(i);

        QTableWidgetItem *nm_kampung_ = new QTableWidgetItem;
        QTableWidgetItem *nm_distrik_ = new QTableWidgetItem;
        QTableWidgetItem *id_dis_ = new QTableWidgetItem;
        QTableWidgetItem *id_kam_ = new QTableWidgetItem;
        QTableWidgetItem *pagu_ = new QTableWidgetItem;
        QTableWidgetItem *tot_cair_ = new QTableWidgetItem;
        QTableWidgetItem *sisa_dana_ = new QTableWidgetItem;

        nm_kampung_->setText(query.value(0).toString()); //nama_kampung
        nm_distrik_->setText(query.value(1).toString()); //Nama Distrik
        id_dis_->setText(query.value(2).toString()); //Id Distrik
        id_kam_->setText(query.value(3).toString()); //id Kampung

        QString pagu_s = query.value(4).toString();
        double pagu_d = pagu_s.toDouble();
        QString pagu_ss = indo.toCurrencyString(pagu_d, "Rp ");

        QString tot_cair_s = query.value(5).toString();
        double tot_cair_d = tot_cair_s.toDouble();
        QString tot_cair_ss = indo.toCurrencyString(tot_cair_d, "Rp ");

        double realisasi_blt = pagu_d - tot_cair_d;

        QString sisa_dana_s = query.value(6).toString();
        //double sisa_dana_d = sisa_dana_s.toDouble();
        QString sisa_dana_ss = indo.toCurrencyString(realisasi_blt, "Rp ");


        pagu_->setText(pagu_ss);
        tot_cair_->setText(tot_cair_ss); //total Pencairan
        sisa_dana_->setText(sisa_dana_ss); //Sisa Dana

        nm_kampung_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        nm_distrik_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        id_dis_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        id_kam_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        pagu_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        tot_cair_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        sisa_dana_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        //ui->tableWidget_realisasi_blt->setCellWidget(i,0,rb);
        ui->tableWidget_realisasi_blt->setItem(i,2,nm_kampung_);
        ui->tableWidget_realisasi_blt->setItem(i,1,nm_distrik_);
        ui->tableWidget_realisasi_blt->setItem(i,6,id_dis_);
        ui->tableWidget_realisasi_blt->setItem(i,0,id_kam_);
        ui->tableWidget_realisasi_blt->setItem(i,3,pagu_);
        ui->tableWidget_realisasi_blt->setItem(i,5,tot_cair_);
        ui->tableWidget_realisasi_blt->setItem(i,4,sisa_dana_);
       i++;
    }
}


void Form::header_twRealisasiblt() // Header table widget 6
{
    QStringList headerWidget;
    ui->tableWidget_realisasi_blt->setColumnCount(7);
    headerWidget <<"id" <<"Nama Distrik"<<"Nama Kampung" <<"Pagu Anggaran " <<"Sisa Anggaran "<<"Realisasi " <<"id Distrik";
    ui->tableWidget_realisasi_blt->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_realisasi_blt->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_realisasi_blt->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_realisasi_blt->setColumnWidth(0,8);
    ui->tableWidget_realisasi_blt->setColumnHidden(0,true);
    ui->tableWidget_realisasi_blt->setColumnWidth(1,140);
    ui->tableWidget_realisasi_blt->setColumnWidth(2,150);
    ui->tableWidget_realisasi_blt->setColumnHidden(6,true);
    ui->tableWidget_realisasi_blt->setColumnWidth(3,120);
    ui->tableWidget_realisasi_blt->setColumnWidth(4,120);
    ui->tableWidget_realisasi_blt->setColumnWidth(5,120);
        //ui->tableWidget_realisasi_blt->setColumnWidth(6,120);

}



void Form::on_toolButton_tmbRealblt_clicked()
{

}

void Form::on_toolButton_cetakPdfblt_clicked()
{

}

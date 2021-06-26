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


void Form::on_toolButton_covid_clicked()
{
    boderToolbar(4);
    ui->stackedWidget->setCurrentIndex(4);
    sembunyi_subMenu();
    memuatData_twRealisasicovid("");

    menu="5";
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_nmKampung->setCurrentIndex(0);
    qInfo() << "Nilai Menu untuk ====" << menu;
}



void Form::memuatData_twRealisasicovid(QString id_kamp){
    header_twRealisasicovid();
   while(ui->tableWidget_realisasi_covid->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_realisasi_covid->removeRow(0);}



    QSqlQuery query;
    QString cmd = "SELECT "
            "pmk_yhk.covid.nama_kampung, "
            "pmk_yhk.covid.nama_distrik, "
           " pmk_yhk.covid.id_kam, "
            "pmk_yhk.covid.id_dis, "
            "pmk_yhk.covid.pagu, "
            "SUM(pmk_yhk.covid_cair.jml_cair_covid) AS total_pencairan, pmk_yhk.covid.pagu - SUM(pmk_yhk.covid_cair.jml_cair_covid) AS sisa_dana"
        " FROM "
            "pmk_yhk.covid"
            " LEFT JOIN "
            "pmk_yhk.covid_cair"
            " ON "
                "pmk_yhk.covid.id_kam = pmk_yhk.covid_cair.id_kam "
       " GROUP BY "
            "pmk_yhk.covid.id_kam, "
           " pmk_yhk.covid.id_dis, "
            "pmk_yhk.covid.nama_kampung, "
            "pmk_yhk.covid.nama_distrik, "
            "pmk_yhk.covid.pagu"
       " ORDER BY "
            "pmk_yhk.covid.id_kam";

    QString cmd_ = "SELECT "
            "pmk_yhk.covid.nama_kampung, "
            "pmk_yhk.covid.nama_distrik, "
           " pmk_yhk.covid.id_kam, "
            "pmk_yhk.covid.id_dis, "
            "pmk_yhk.covid.pagu, "
            "SUM(pmk_yhk.covid_cair.jml_cair_covid) AS total_pencairan, pmk_yhk.covid.pagu - SUM(pmk_yhk.covid_cair.jml_cair_covid) AS sisa_dana"
        " FROM "
            "pmk_yhk.covid"
            " LEFT JOIN "
            "pmk_yhk.covid_cair"
            " ON "
                "pmk_yhk.covid.id_kam = pmk_yhk.covid_cair.id_kam "
            " WHERE pmk_yhk.covid.id_kam = :id "
       " GROUP BY "
            "pmk_yhk.covid.id_kam, "
           " pmk_yhk.covid.id_dis, "
            "pmk_yhk.covid.nama_kampung, "
            "pmk_yhk.covid.nama_distrik, "
            "pmk_yhk.covid.pagu"
       " ORDER BY "
            "pmk_yhk.covid.id_kam";

    if(id_kamp!=""){cmd=cmd_;};
    //if(s_id_kamp==""){cmd ="SELECT * FROM pmk_yhk.t_real ORDER BY id_real ";}
     //if(s_id_kamp==""){cmd =" SELECT	* FROM	pmk_yhk.t_real ORDER BY	id_real ASC, pmk_yhk.t_real.id_real LIMIT 40 ";}
    query.prepare(cmd);
    query.bindValue(":id",id_kamp);
    //query.bindValue(":id",s_id_kamp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data realisasi COVID"); return;}
    int i=0;
    double pagu_covid=0;
    double tot_cair_covid=0;
    double sisa_covid=0;
    while (query.next()) {

        //int i = ui->tableWidget_realisasi_covid->rowCount();
        ui->tableWidget_realisasi_covid->insertRow(i);

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
        //qInfo()<<"total Cair =========" << tot_cair_d;
        QString tot_cair_ss = indo.toCurrencyString(tot_cair_d, "Rp ");

        double realisasi_covid = pagu_d - tot_cair_d;

        QString sisa_dana_s = query.value(6).toString();
        //double sisa_dana_d = sisa_dana_s.toDouble();
        QString sisa_dana_ss = indo.toCurrencyString(realisasi_covid, "Rp ");

     //qInfo()<<"Sisa Dana =========" << sisa_dana_d;
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

        //ui->tableWidget_realisasi_covid->setCellWidget(i,0,rb);
        ui->tableWidget_realisasi_covid->setItem(i,2,nm_kampung_);
        ui->tableWidget_realisasi_covid->setItem(i,1,nm_distrik_);
        ui->tableWidget_realisasi_covid->setItem(i,6,id_dis_);
        ui->tableWidget_realisasi_covid->setItem(i,0,id_kam_);
        ui->tableWidget_realisasi_covid->setItem(i,3,pagu_);
        ui->tableWidget_realisasi_covid->setItem(i,5,tot_cair_);
        ui->tableWidget_realisasi_covid->setItem(i,4,sisa_dana_);

        i++;
       pagu_covid+= pagu_d;
       tot_cair_covid+= tot_cair_d;

    }

     //sisa_covid = pagu_covid - realisasi_covid;
     //qInfo() << "== "<<i<<"===" <<  sisa_covid << "====" << pagu_covid << "===" << realisasi_covid;

     sisa_covid = pagu_covid - tot_cair_covid ;

    QString pagu_covid_ss = indo.toCurrencyString(pagu_covid, "Rp ");
    QString tot_cair_d_ss = indo.toCurrencyString(tot_cair_covid, "Rp ");
    QString realisasi_covid_ss = indo.toCurrencyString(sisa_covid, "Rp ");

    //qInfo() << "=====" << pagu_covid_ss << "====" << tot_cair_d_ss << "===" << realisasi_covid_ss;

    ui->lineEdit_pagu_covid->setText(pagu_covid_ss);
    ui->lineEdit_realisasi_covid->setText(tot_cair_d_ss);
    ui->lineEdit_sisa_covid->setText(realisasi_covid_ss);

}


void Form::header_twRealisasicovid() // Header table widget 6
{
    QStringList headerWidget;
    ui->tableWidget_realisasi_covid->setColumnCount(7);
    headerWidget <<"id" <<"Nama Distrik"<<"Nama Kampung" <<"Pagu Anggaran " <<"Sisa Anggaran "<<"Realisasi " <<"id Distrik";
    ui->tableWidget_realisasi_covid->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_realisasi_covid->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_realisasi_covid->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_realisasi_covid->setColumnWidth(0,8);
    ui->tableWidget_realisasi_covid->setColumnHidden(0,true);
    ui->tableWidget_realisasi_covid->setColumnWidth(1,140);
    ui->tableWidget_realisasi_covid->setColumnWidth(2,150);
    ui->tableWidget_realisasi_covid->setColumnHidden(6,true);
    ui->tableWidget_realisasi_covid->setColumnWidth(3,120);
    ui->tableWidget_realisasi_covid->setColumnWidth(4,120);
    ui->tableWidget_realisasi_covid->setColumnWidth(5,120);
        //ui->tableWidget_realisasi_covid->setColumnWidth(6,120);

}


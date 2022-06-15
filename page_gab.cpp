#include "form.h"
#include "ui_form.h"
#include "QPdfWriter"
#include "qrencode.h"
#include "pdf_dds.h"
#include "mainwindow.h"
#include "pdf_sppd_all_cair.h"
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


void Form::on_toolButton_gab_clicked()
{
    boderToolbar(8);
    ui->stackedWidget->setCurrentIndex(7);
    sembunyi_subMenu();

    //memuatData_twRealisasigab("");
    menu="8";
    ui->label_barCode->setVisible(false);
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_nmKampung->setCurrentIndex(0);
    ui->toolButton_logOut->setVisible(true);
    //muat_data_realisasi_gab("");
    qInfo() << "Nilai Menu untuk ====" << menu;
}


void Form::on_toolButton_tmbRealgab_clicked()
{
    if(menu=="8"){
    check_qbx_kam_dis();
    tambah_sppd(18,19);


    /* act();*/ }
}

void Form::muatheadertw_realisasi_gab()
{
    QStringList list;
    list << "id" << "no_surat" << "perihal" << "tanggal" << "id_kp";
    int j = list.count();
    ui->tableWidget_realisasi_gab->setColumnCount(j);
    ui->tableWidget_realisasi_gab->setHorizontalHeaderLabels(list);
    ui->tableWidget_realisasi_gab->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_realisasi_gab->setColumnHidden(0, true);
    ui->tableWidget_realisasi_gab->setColumnHidden(4, true);

    ui->tableWidget_realisasi_gab->setColumnWidth(1, 240);
    ui->tableWidget_realisasi_gab->setColumnWidth(2, 240);
    ui->tableWidget_realisasi_gab->setColumnWidth(3, 130);
}


void Form::muatheadertw_cetak_gab()
{
    QStringList head;
    head << "Tgl" << "Tahap" << "Kampung" << "Distrik" << "No Rek" << "Nama Rek" << "Nama Bank" << "Bendahara" << "Kepala Kampung" << "Jml Cair" << "Terbilang"  << "Thp II" << "no_srt" << "id_kamp";
    ui->tableWidget_cetak_gab->setColumnCount(head.count());
    ui->tableWidget_cetak_gab->setHorizontalHeaderLabels(head);
    ui->tableWidget_cetak_gab->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_cetak_gab->setColumnHidden(11, true);
    ui->tableWidget_cetak_gab->setColumnHidden(12, true);
    ui->tableWidget_cetak_gab->setColumnHidden(13, true);


    ui->tableWidget_cetak_gab->setColumnWidth(0, 70);
    ui->tableWidget_cetak_gab->setColumnWidth(1, 70);
    ui->tableWidget_cetak_gab->setColumnWidth(2, 100);
    ui->tableWidget_cetak_gab->setColumnWidth(3, 100);
    ui->tableWidget_cetak_gab->setColumnWidth(4, 115);
    ui->tableWidget_cetak_gab->setColumnWidth(5, 220);
    ui->tableWidget_cetak_gab->setColumnWidth(6, 250);
    ui->tableWidget_cetak_gab->setColumnWidth(7, 150);
    ui->tableWidget_cetak_gab->setColumnWidth(8, 150);
    ui->tableWidget_cetak_gab->setColumnWidth(9, 125);
    ui->tableWidget_cetak_gab->setColumnWidth(10, 300);
    // ui->tableWidget_cetak_gab->setColumnWidth(11,100);
}

void Form::muatData_realisasi_gab(QString &id_kp) //Table Widget Cetak
{

    muatheadertw_cetak_gab();
    muat_data_realisasi_gab(id_kp);
    QString cmd;
    cmd  = " SELECT * FROM pmk_yhk.v_sppdd_all WHERE id_kam=:id ORDER BY pmk_yhk.v_sppdd_all.tgl DESC " ;

    while(ui->tableWidget_cetak_gab->rowCount()>0) // untuk Hilangkan Tambahan jika button di klik ulang
    {ui->tableWidget_cetak_gab->removeRow(0);}
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id", id_kp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Daftar ... "); return;}
    int i=0;
    while(query.next())
    {
                ui->tableWidget_cetak_gab->insertRow(i);
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
                QTableWidgetItem *no_srt = new QTableWidgetItem;


                tahap->setText(query.value(1).toString());
                kmp->setText(query.value(2).toString());
                dis->setText(query.value(3).toString());
                norek->setText(query.value(4).toString());
                nmrek->setText(query.value(5).toString());
                nmbank->setText(query.value(6).toString());
                bend->setText(query.value(7).toString());
                kkap->setText(query.value(8).toString());
                double jj = (query.value(9).toDouble());
                QString jjj = indo.toCurrencyString(jj,"Rp ");
                jml->setText(jjj);
                terbilang->setText(query.value(10).toString());
                QDate dt = QDate::fromString(query.value(11).toString(),"yyyy-MM-dd");
                tgl->setText(dt.toString("dd-MM-yyyy"));
                thpII->setText(query.value(12).toString());
                no_srt->setText(query.value(0).toString());

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

                ui->tableWidget_cetak_gab->setItem(i,0,tgl);
                ui->tableWidget_cetak_gab->setItem(i,1,tahap);
                ui->tableWidget_cetak_gab->setItem(i,2,kmp);
                ui->tableWidget_cetak_gab->setItem(i,3,dis);
                ui->tableWidget_cetak_gab->setItem(i,4,norek);
                ui->tableWidget_cetak_gab->setItem(i,5,nmrek);
                ui->tableWidget_cetak_gab->setItem(i,6,nmbank);
                ui->tableWidget_cetak_gab->setItem(i,7,bend);
                ui->tableWidget_cetak_gab->setItem(i,8,kkap);
                ui->tableWidget_cetak_gab->setItem(i,9,jml);
                ui->tableWidget_cetak_gab->setItem(i,10,terbilang);

                ui->tableWidget_cetak_gab->setItem(i,11,thpII);
                ui->tableWidget_cetak_gab->setItem(i,12,no_srt);
                i++;
    }
}

void Form::muatData_realisasi_gab2(QString id_kp, int row) //Table Widget Cetak
{

    QDate tg = QDate::fromString(ui->tableWidget_realisasi_gab->item(row,3)->text(),"dd-MM-yyyy");
    QString tgl = tg.toString("yyyy-MM-dd");

    //muatheadertw_cetak_gab();
    //muat_data_realisasi_gab(id_kp);
    QString cmd;
     cmd  = " SELECT * FROM pmk_yhk.v_sppdd_all WHERE tgl=:tgl AND id_kam=:id ORDER BY v_sppdd_all.tgl DESC"  ;

    while(ui->tableWidget_cetak_gab->rowCount()>0) // untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_cetak_gab->removeRow(0);}
    QSqlQuery query;
    query.prepare(cmd);
    query.bindValue(":id", id_kp);
    query.bindValue(":tgl", tgl);

    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Daftar ... "); return;}
    int i=0;
    while(query.next())
    {
                ui->tableWidget_cetak_gab->insertRow(i);
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
                QTableWidgetItem *no_srt = new QTableWidgetItem;


                tahap->setText(query.value(1).toString());
                kmp->setText(query.value(2).toString());
                dis->setText(query.value(3).toString());
                norek->setText(query.value(4).toString());
                nmrek->setText(query.value(5).toString());
                nmbank->setText(query.value(6).toString());
                bend->setText(query.value(7).toString());
                kkap->setText(query.value(8).toString());
                double jj = (query.value(9).toDouble());
                QString jjj = indo.toCurrencyString(jj,"Rp ");
                jml->setText(jjj);
                terbilang->setText(query.value(10).toString());
                QDate dt = QDate::fromString(query.value(11).toString(),"yyyy-MM-dd");
                tgl->setText(dt.toString("dd-MM-yyyy"));
                thpII->setText(query.value(12).toString());
                no_srt->setText(query.value(13).toString());

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

                ui->tableWidget_cetak_gab->setItem(i,0,tgl);
                ui->tableWidget_cetak_gab->setItem(i,1,tahap);
                ui->tableWidget_cetak_gab->setItem(i,2,kmp);
                ui->tableWidget_cetak_gab->setItem(i,3,dis);
                ui->tableWidget_cetak_gab->setItem(i,4,norek);
                ui->tableWidget_cetak_gab->setItem(i,5,nmrek);
                ui->tableWidget_cetak_gab->setItem(i,6,nmbank);
                ui->tableWidget_cetak_gab->setItem(i,7,bend);
                ui->tableWidget_cetak_gab->setItem(i,8,kkap);
                ui->tableWidget_cetak_gab->setItem(i,9,jml);
                ui->tableWidget_cetak_gab->setItem(i,10,terbilang);

                ui->tableWidget_cetak_gab->setItem(i,11,thpII);
                ui->tableWidget_cetak_gab->setItem(i,12,no_srt);
                i++;
    }
}

void Form::muat_data_realisasi_gab(QString &id_kp)
{

    muatheadertw_realisasi_gab();
    while(ui->tableWidget_realisasi_gab->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
    {ui->tableWidget_realisasi_gab->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.all_cair WHERE id_kam=:id ORDER BY all_cair.tgl DESC ";
    query.prepare(cmd);
    query.bindValue(":id", id_kp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Data Gabungan  Realisasi Kampung... ");return;}
    int i=0;
        while(query.next())
        {
                ui->tableWidget_realisasi_gab->insertRow(i);
                QTableWidgetItem *no_ = new QTableWidgetItem;
                QTableWidgetItem *nosrt_ = new QTableWidgetItem;
                QTableWidgetItem *hal_ = new QTableWidgetItem;
                QTableWidgetItem *tgl_ = new QTableWidgetItem;
                QTableWidgetItem *id_kp_ = new QTableWidgetItem;
                no_->setText(query.value(0).toString());
                nosrt_->setText(query.value(1).toString());
                hal_->setText(query.value(2).toString());
                QDate dt = QDate::fromString(query.value(3).toString(),"yyyy-MM-dd");
                tgl_->setText(dt.toString("dd-MM-yyyy"));
                id_kp_->setText(query.value(5).toString());

//                no_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//                nosrt_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//                hal_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
//                tgl_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                ui->tableWidget_realisasi_gab->setItem(i,0,no_);
                ui->tableWidget_realisasi_gab->setItem(i,1,nosrt_);
                ui->tableWidget_realisasi_gab->setItem(i,2,hal_);
                ui->tableWidget_realisasi_gab->setItem(i,3,tgl_);
                ui->tableWidget_realisasi_gab->setItem(i,4,id_kp_);
                i++;
        }
}



void Form::on_tableWidget_realisasi_gab_cellClicked(int row, int column)
{
    qInfo() << column;
    QString id_kp = ui->tableWidget_realisasi_gab->item(row,4)->text();
    muatData_realisasi_gab2(id_kp,row); //Table Widget Cetak
}



void Form::on_tableWidget_realisasi_gab_cellDoubleClicked(int row, int column)
{
    qInfo() << "Column" << column;
    QString id = ui->tableWidget_realisasi_gab->item(row,0)->text();
    QString noSrt = ui->tableWidget_realisasi_gab->item(row,1)->text();
    QString perihal = ui->tableWidget_realisasi_gab->item(row,2)->text();
    QString tanggal = ui->tableWidget_realisasi_gab->item(row,3)->text();
    update_data_sppd(id,tanggal, noSrt, perihal);
}



void Form::on_toolButton_cetakPdfgab_clicked()
{
    int j = ui->tableWidget_realisasi_gab->currentRow();
    int jBrs = ui->tableWidget_cetak_gab->rowCount();
    if (jBrs == 0) { QMessageBox::information(this, "Info", "Belum Ada kegiatan pada surat ini... "); return; }
    if (j == -1) { QMessageBox::information(this, "Info", "Pilih Surat yang akan di cetak... "); return; }

    //if (brs > 20) { QMessageBox::information(this, "Info ... ", "Maksimal angka 20"); return; }

    QString nosrt = ui->tableWidget_realisasi_gab->item(j, 1)->text();
    QString hal = ui->tableWidget_realisasi_gab->item(j, 2)->text();
    QString tgl = ui->tableWidget_realisasi_gab->item(j, 3)->text();
    QDate dt = QDate::fromString(tgl, "dd-MM-yyyy");
    QString tgl_ = dt.toString("yyyy-MM-dd");
    //    QString tgl_1 = dt.toString("dd MMMM yyyy");
    QString tgl_1 = QLocale{ QLocale::Indonesian }.toString(dt, "dd MMMM yyyy");
    QString tgl_2 = QLocale{ QLocale::Indonesian }.toString(dt, "dd MMMM yyyy");
    QString thn = tgl_1.right(4);
    double jcr = 0;

    kp_dns();
    ds_sppd_.clear();

    qInfo() << "cek sppd ==============";
    //QString aa = ui->comboBox_realisasi->currentText();
    QString thp_jm = ui->tableWidget_cetak_gab->item(0, 1)->text();
    QString nm_kam_ = ui->tableWidget_cetak_gab->item(0, 2)->text();
    QString nm_dis_ = ui->tableWidget_cetak_gab->item(0, 3)->text();

    for (int i = 0; i < jBrs; i++) {
        QString no_ = QString::number(i + 1);
        QString thp = ui->tableWidget_cetak_gab->item(i, 1)->text();
        QString nm_kampung = ui->tableWidget_cetak_gab->item(i, 2)->text();
        QString nm_distrik = ui->tableWidget_cetak_gab->item(i, 3)->text();
        QString no_rek = ui->tableWidget_cetak_gab->item(i, 4)->text();
        QString nm_rek = ui->tableWidget_cetak_gab->item(i, 5)->text();

        QString nm_benk = ui->tableWidget_cetak_gab->item(i, 7)->text();
        QString nm_kepalak = ui->tableWidget_cetak_gab->item(i, 8)->text();
        QString j_cair = ui->tableWidget_cetak_gab->item(i, 9)->text();
        QString terbilang = ui->tableWidget_cetak_gab->item(i, 10)->text();
        QString tgl = ui->tableWidget_cetak_gab->item(i, 0)->text();
        QString thp_advis = ui->tableWidget_cetak_gab->item(i, 11)->text();
        QString no_srt = ui->tableWidget_cetak_gab->item(i, 12)->text();

        ds_sppd_.append("<tr><td style='text-align:center'>" + no_ + "</td><td>" + nm_kampung + "</td><td>" + nm_distrik + "</td><td>" + nm_kepalak + "</td>"
                "<td>" + nm_benk + "</td><td style='font-size: 13px; text-align: center;'>" + thp_advis + "</td><td style='text-align:right; font-size: 13px;'>" + j_cair + "</td><td style='font-size: 13px'><i>" + toCamelCase(terbilang) + "</i></td><td>" + no_rek +
                "</td><td style='font-size: 13px;'>" + nm_rek + "</td><td style='font-size: 13px;'>" + no_srt + ", " + tgl_1 + "</td></tr>");

        j_cair.replace(",00", ""); j_cair.replace("Rp ", ""); j_cair.replace(".", "");  j_cair.replace(",", ".");
        double jc = j_cair.toDouble();
        jcr += jc;
        qInfo() << "cek sppd ==============" << i;
    }

    qInfo() << "cek sppd ============== luar";
    tot_sppd = indo.toCurrencyString(jcr, "Rp ");
    qInfo() << "generate pdf 000" << tot_sppd;
    QString tot_terbil = tot_sppd;
    bila(tot_terbil.right(2));
    tot_terbil.replace("Rp ", ""); tot_terbil.replace(".", ""); tot_terbil.replace(",", ".");
    bilang(tot_terbil);

    ds_sppd_.append("#/#" + tot_sppd);
    ds_sppd_.append("#/#" + thp_jm);
    ds_sppd_.append("#/#" + tgl_1);
    ds_sppd_.append("#/#" + thn);
    ds_sppd_.append("#/#" + nosrt);
    QString th = " I ";
    bool ok = nosrt.contains(th);
    qInfo() << "Jumlah Cair dari form --------SPP----------- " << ok;
    ds_sppd_.append("#/#" + hal);
    ds_sppd_.append("#/#" + li_kp_dns.at(1));
    ds_sppd_.append("#/#" + li_kp_dns.at(2));
    ds_sppd_.append("#/#" + li_kp_dns.at(3));
    ds_sppd_.append("#/#" + terbilang + terbila);
    ds_sppd_.append("#/#" + li_kp_dns.at(0));
    ds_sppd_.append("#/#" + spasi_br(ui->lineEdit_br->text()));
    ds_sppd_.append("#/#" + nm_kam_);
    ds_sppd_.append("#/#" + nm_dis_);
    //setTot_sppd(tot_sppd);
        save_gab();
        Widget7* pdf7 = new Widget7;
        pdf7->hide();
}


void Form::save_gab() {
    QString path("doc/temp/");
    QFile fOut(path + "gab_temp.txt");
    //if(!fOut.exists()){ QMessageBox::information(this,"Error..","Gagal Creat Data spp_temp.txt"); return; }
    fOut.open(QFile::WriteOnly | QFile::Text);
    if (fOut.open(QFile::WriteOnly | QFile::Text)) { qInfo() << "Gagal Menyimpan gab_temp.txt"; return; }
    else
    {
        QTextStream stream(&fOut);
        stream << ds_sppd_;
        fOut.flush();
        fOut.close();
    }
}

#include "form.h"
#include "ui_form.h"
//#include "QPdfWriter"
//#include "pdf_dok.h"
#include "qrencode.h"
#include "pdf_covid.h"
//#include "pdf_add.h"
//#include "pdf_sppd_add.h"
//#include "psd_sppd_covid.h"
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

    ui->label_barCode->setVisible(true);

    menu="5";
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_nmKampung->setCurrentIndex(0);
    qInfo() << "Nilai Menu untuk ====" << menu;

    memuatData_twRealisasicovid("");
    muat_data_realisasi_covid("");
}


void Form::header_cetak_covid()
{
    QStringList headerWidget;
    ui->tableWidget_cetak_covid->setColumnCount(30);
    headerWidget <<"cetak"<<"Id Dis"<<"Id Kam"<<" Nama Distrik"<<"Nama Kampung"<<"No Rekening"<<"Nama Rekening"<<"Nama Bank"<<"SK Bendahara"
                <<"Nama Kepala Kampung"<<" Nama Bendahara "
                 <<"Tahap Pencairan"<<"Jumlah Pencairan"<<"Terbilang"<<"Tanggal Terima"<<"No Srt 1"<<"No Srt 2"<<"%"<<"SK Bupati"<<"SK Kep.Kampung"
                <<"SK Men PMK"<<"SK Men Keu"<<"Kepala Dinas" << " Ket " << "Pangkat" << "NIP" <<"Laporan Realisasi"<<"id"<<"jkk"<<"jbt kp dns" << "id_realisasi";
    ui->tableWidget_cetak_covid->setHorizontalHeaderLabels(headerWidget);

    ui->tableWidget_cetak_covid->setColumnHidden(0,true);
    ui->tableWidget_cetak_covid->setColumnHidden(1,true);
    ui->tableWidget_cetak_covid->setColumnHidden(2,true);
    //ui->tableWidget_cetak_covid->setColumnHidden(3,true);
    //ui->tableWidget_cetak_covid->setColumnHidden(4,true);
    ui->tableWidget_cetak_covid->setColumnHidden(5,true);
    ui->tableWidget_cetak_covid->setColumnHidden(6,true);
    ui->tableWidget_cetak_covid->setColumnHidden(7,true);
    ui->tableWidget_cetak_covid->setColumnHidden(8,true);
    ui->tableWidget_cetak_covid->setColumnHidden(9,true);
    ui->tableWidget_cetak_covid->setColumnHidden(10,true);
    ui->tableWidget_cetak_covid->setColumnHidden(18,true);
    ui->tableWidget_cetak_covid->setColumnHidden(19,true);
    ui->tableWidget_cetak_covid->setColumnHidden(20,true);
    ui->tableWidget_cetak_covid->setColumnHidden(21,true);
    ui->tableWidget_cetak_covid->setColumnHidden(22,true);
    ui->tableWidget_cetak_covid->setColumnHidden(23,true);
    ui->tableWidget_cetak_covid->setColumnHidden(24,true);
    ui->tableWidget_cetak_covid->setColumnHidden(25,true);
    ui->tableWidget_cetak_covid->setColumnHidden(27,true);
    ui->tableWidget_cetak_covid->setColumnHidden(28,true);
    ui->tableWidget_cetak_covid->setColumnHidden(29,true);
    ui->tableWidget_cetak_covid->setColumnHidden(30,true);

    ui->tableWidget_cetak_covid->setColumnWidth(3,120);
    ui->tableWidget_cetak_covid->setColumnWidth(4,120);
   ui->tableWidget_cetak_covid->setColumnWidth(11,150);
   ui->tableWidget_cetak_covid->setColumnWidth(12,120);
   ui->tableWidget_cetak_covid->setColumnWidth(13,340);
   ui->tableWidget_cetak_covid->setColumnWidth(14,90);
   ui->tableWidget_cetak_covid->setColumnWidth(15,230);
   ui->tableWidget_cetak_covid->setColumnWidth(16,230);
   ui->tableWidget_cetak_covid->setColumnWidth(17,50);
   ui->tableWidget_cetak_covid->setColumnWidth(26,120);

}


void Form::header_realisasi_covid() // Header table widget 6
{
    QStringList headerWidget;
    ui->tableWidget_realisasi_covid->setColumnCount(6);
    headerWidget <<"id" <<"Nama Distrik"<<"Nama Kampung" <<"Pagu Anggaran " <<"Sisa Anggaran "<<"Realisasi ";
    ui->tableWidget_realisasi_covid->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_realisasi_covid->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_realisasi_covid->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_realisasi_covid->setColumnWidth(0,8);
    ui->tableWidget_realisasi_covid->setColumnHidden(0,true);
    ui->tableWidget_realisasi_covid->setColumnWidth(1,140);
    ui->tableWidget_realisasi_covid->setColumnWidth(2,150);
    //ui->tableWidget->setColumnHidden(2,true);
    ui->tableWidget_realisasi_covid->setColumnWidth(3,120);
    ui->tableWidget_realisasi_covid->setColumnWidth(4,120);
    ui->tableWidget_realisasi_covid->setColumnWidth(5,120);

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
    query.prepare(cmd);
    query.bindValue(":id",id_kamp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data realisasi COVID"); return;}
    int i=0;
    double pagu_covid=0;
    double tot_cair_covid=0;
    double sisa_covid=0;
    while (query.next()) {

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
    muat_data_realisasi_covid(id_kamp);

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


void Form::on_toolButton_tmbRealCovid_clicked() // toolButton tambah realisasi dana covid
{
tambah_real_covid();
             qInfo() << "Cari Error covid ===================  03<<" ;
}

void Form::tambah_real_covid()
{
    qInfo() << "tambah Real Covid";
    if(menu=="5"){ menu2="5";

        QString id_dis = qbx_id_dis->currentText();
        QString id_kam = qbx_id_kam->currentText();
        if(id_kam == "" && id_dis ==""){QMessageBox::information(this,"Peringatan...!!!","Pilih Distrik dan Kampung");return;}
        if(id_kam!="" && id_dis!=""){

            tahap(3);
            muat_bend_kp(id_kam);
            muat_k_kp(id_kam);
            tambah_realisasi(li_tahap,"Rp 0,00", no_srt_covid_1().at(0),no_srt_covid_2().at(0),"TAMBAH",s_nm_kp, s_nm_bend,"Tambah Realisasi Dana Covid" );
        } }
}


void Form::btnAdd_covid()
{

    if(menu2=="5"){
         if(menu=="5"){;

         if(qbx_thp_penc->currentIndex() == 0 ){QMessageBox::information(eb_v,"Info...!!"," Pilih Tahap Pencairan... "); return;};
        QString id_dis = qbx_id_dis->currentText();
        QString id_kam = qbx_id_kam->currentText();
        QString j = le_jml->text();
        if(j=="Rp 0,00"){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh Nilai Rp 0,00,-"); return;}
        if(j==""){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong,-"); return;}
        QString jj_ = j;
        j.replace("Rp ",""); j.replace(".",""); j.replace(",",".");
        bilang(j);
        QString jcair3 = ui->tableWidget_realisasi_covid->item(0,4)->text();
        jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
        double jc3 = jcair3.toDouble();
        double jj = j.toDouble();

        if((jc3-jj)<0){QMessageBox::information(eb_v,"Info...","Mohon Periksa Ketersediaan Dana"); return;}
        jj_.replace("Rp ",""); jj_.replace(".",""); jj_.replace(",",".");
        muat_rek(id_kam);
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
        QString tahap = qbx_thp_penc->currentText();

       //String tahap_l = qbx_thp_l->currentText();
        QString tgl_ter = de->text();
        QString no_srt1 =qbx_no_srt1->currentText();
        QString no_srt2 =qbx_no_srt2->currentText();
        QString persen = "100%";
        QString sk_bup = "li_sk_bup.at(0)"; //=====================
        QString sk_kam = s_sk_kp;
        QString nm_kp_dns = li_kp_dns.at(1);
        QString pg_kp_dns = li_kp_dns.at(2);
        QString nip_kp_dns = li_kp_dns.at(3);
        QString jbt_kpl_dns = li_kp_dns.at(0);
        QString thn2 = "thp.right(4)";

        QString j_kk = li_j_kk.at(0); //====================
        QString rp ="rupiah";
        QPixmap logo;
        QIcon icon(":/icon/gbr/kcl.png");
        QMessageBox boxPesan;
        boxPesan.setWindowIcon(icon);
        boxPesan.setWindowTitle("Info...!!!");
        boxPesan.setText("<b>Menambah data Realisasi covid "+ui->comboBox_nmKampung->currentText()+" </b>");
        boxPesan.setInformativeText("Pastikan data yang anda input sudah sesuai...");
        QAbstractButton * pBlanjut = boxPesan.addButton(tr(" LANJUT "), QMessageBox::YesRole) ;
         boxPesan.addButton(tr(" BATAL "), QMessageBox::NoRole);
         if (logo.load(":/icon/gbr/kcl.png"))
         boxPesan.setIconPixmap(logo);
         boxPesan.exec();
         if(boxPesan.clickedButton() == pBlanjut)
           {

             QSqlQuery query;
             begin();
             QString cmd = "INSERT INTO pmk_yhk.covid_cair (id_kam,nm_dis,nm_kam, no_rek, nm_rek,nm_bank, sk_bend, sk_kp, nm_ben, nm_kp, thp_cair, jml_cair_covid, j_terbilang, "
                                       "tgl, no_srt1, no_srt2,persentase,sk_bup,sk_kam,sk_pmk,sk_keu, nm_kpd, j_kpd, nip_kpd, ket, thp_l, jkk, nos, jbt_kpl_dns) VALUES "
                                        "(:id_kam, :nm_dis, :nm_kam, :no_rek, :nm_rek, :nm_bank, :sk_bend, :sk_kp, :nm_ben, :nm_kp, :thp_cair, :j_cair, :j_terbilang, "
                                        ":tgl, :no_srt1, :no_srt2, :persentase, :sk_bup, :sk_kam, :sk_pmk, :sk_keu, :nm_kpd, :j_kpd, :nip_kpd, :ket, :thp_l, :jkk, '2' , :jbt_kpl_dns)";

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
             query.bindValue(":persentase","100%");
             query.bindValue(":sk_bup",sk_bup);
             query.bindValue(":sk_kam",sk_kam);

             query.bindValue(":nm_kpd",nm_kp_dns);
             query.bindValue(":j_kpd",pg_kp_dns);
             query.bindValue(":nip_kpd",nip_kp_dns);
             query.bindValue(":ket",thn2);
             query.bindValue(":thp_l","tahap_l");
             query.bindValue(":jkk",j_kk);
             query.bindValue(":jbt_kpl_dns", jbt_kpl_dns);

             bool ok = exec(query);
             if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Menambah realisasi Covid, "+query.lastError().text()+"");}
             commit();
                QMessageBox::information( eb_v,"Info...","Berhasil Menambah realisasi Covid... ");
                eb_v->close();
            }else { return; }
         menu="5";

         if(menu=="5"){
         QString id = qbx_id_kam->currentText();
         memuatData_twRealisasicovid(id);}
                  } }

        if(menu2=="52"){data_update_sementara();}
}



QStringList Form::no_srt_covid_1()
{
    QStringList li_srt_covid1;
    li_srt_covid1.clear();
    qInfo() << "Baca File covid_no_srt_1.txt" ;
     QString path("data/");
    QFile f_srt1(path+"covid_no_srt_1.txt");
    if(!f_srt1.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat no Surat Covid 1");}
     f_srt1.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_srt1);
     while (!str.atEnd()) {
        QString line = str.readAll();
        li_srt_covid1 = line.split("\n");
    }
     f_srt1.close();
     return  li_srt_covid1;
}

QStringList Form::no_srt_covid_2()
{
    QStringList li_srt_covid2;
    li_srt_covid2.clear();
    qInfo() << "Baca File covid_no_srt_2.txt" ;
     QString path("data/");
    QFile f_srt2(path+"covid_no_srt_2.txt");
    if(!f_srt2.exists()) {QMessageBox::information(this,"Error...!!!","Gagal Memuat no Surat Covid 2");}
     f_srt2.open(QIODevice::ReadOnly|QIODevice::Text);
     QTextStream str(&f_srt2);
     while (!str.atEnd()) {
        QString line = str.readAll();
        li_srt_covid2 = line.split("\n");
    }
     f_srt2.close();
     return  li_srt_covid2;
}




void Form::muat_data_realisasi_covid(QString s_id_kamp)
{
    header_cetak_covid();
    while(ui->tableWidget_cetak_covid->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_cetak_covid->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.covid_cair WHERE id_kam = :id ORDER BY id_real ";
    //if(s_id_kamp==""){cmd ="SELECT * FROM pmk_yhk.t_real ORDER BY id_real ";}
     if(s_id_kamp==""){cmd =" SELECT	* FROM	pmk_yhk.covid_cair ORDER BY	id_real ASC, pmk_yhk.covid_cair.id_real LIMIT 40 ";}
    query.prepare(cmd);
    query.bindValue(":id",s_id_kamp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data realisasi Covid"); return;}
    int i=0;
    while (query.next()) {

        //int i = ui->tableWidget_cetak_covid->rowCount();
        ui->tableWidget_cetak_covid->insertRow(i);
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
        QTableWidgetItem *jbt_kpl_dns_ = new QTableWidgetItem;

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
         jbt_kpl_dns_->setText(query.value(30).toString());

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
        jbt_kpl_dns_->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidget_cetak_covid->setCellWidget(i,0,rb);
        ui->tableWidget_cetak_covid->setItem(i,1,nm_dis_);
        ui->tableWidget_cetak_covid->setItem(i,2,nm_kam_);
        ui->tableWidget_cetak_covid->setItem(i,3,id_dis_);
        ui->tableWidget_cetak_covid->setItem(i,4,id_kam_);
        ui->tableWidget_cetak_covid->setItem(i,5,no_rek_);
        ui->tableWidget_cetak_covid->setItem(i,6,nm_rek_);
        ui->tableWidget_cetak_covid->setItem(i,7,nm_bank_);
        ui->tableWidget_cetak_covid->setItem(i,8,j_sek_);
        ui->tableWidget_cetak_covid->setItem(i,9,nm_kp_);
        ui->tableWidget_cetak_covid->setItem(i,10,nm_ben_);
        ui->tableWidget_cetak_covid->setItem(i,11,thp_cair_);
        ui->tableWidget_cetak_covid->setItem(i,12,j_cair_);
        ui->tableWidget_cetak_covid->setItem(i,13,j_terbil_);
        ui->tableWidget_cetak_covid->setItem(i,14,tgl_);
        ui->tableWidget_cetak_covid->setItem(i,15,no_srt1_);
        ui->tableWidget_cetak_covid->setItem(i,16,no_srt2_);
        ui->tableWidget_cetak_covid->setItem(i,17,persen_);
        ui->tableWidget_cetak_covid->setItem(i,18,sk_bup_);
        ui->tableWidget_cetak_covid->setItem(i,19,sk_kam_);
        ui->tableWidget_cetak_covid->setItem(i,20,sk_men_);
        ui->tableWidget_cetak_covid->setItem(i,21,sk_keu_);
        ui->tableWidget_cetak_covid->setItem(i,22,ket_);
        ui->tableWidget_cetak_covid->setItem(i,23,nm_kpd_);
        ui->tableWidget_cetak_covid->setItem(i,24,j_kpd_);
        ui->tableWidget_cetak_covid->setItem(i,25,nip_kpd_);
        ui->tableWidget_cetak_covid->setItem(i,26,thp_cair2_);
        ui->tableWidget_cetak_covid->setItem(i,27,id_);
        ui->tableWidget_cetak_covid->setItem(i,28,jkk_);
        ui->tableWidget_cetak_covid->setItem(i,29,jbt_kpl_dns_);
       i++;
    }

}


void Form::on_tableWidget_cetak_covid_cellDoubleClicked(int i, int column)
{
    qInfo() << "ini adalah even change tw Covid ============== " << i <<" === " <<column;
    if(qbx_id_dis->currentText() == "" && qbx_id_kam->currentText() == "" ){ QMessageBox::information(this, "Info...","Pilih Kampung dan Distrik ..."); return; }
    if(qbx_id_dis->currentText() != "" && qbx_id_kam->currentText() == "" ){ QMessageBox::information(this, "Info...","Pilih Kampung dan Distrik ..."); return; }
    if(qbx_id_dis->currentText() == "" && qbx_id_kam->currentText() != "" ){ QMessageBox::information(this, "Info...","Pilih Kampung dan Distrik ..."); return; }
    li_data_covid.clear();
    QString list;
    for (int ii=1; ii<30; ii++) {
        li_data_covid << ui->tableWidget_cetak_covid->item(i,ii)->text();
    }
    if(i>=0) {even_dbKlik_tw_covid();}
}


void Form::even_dbKlik_tw_covid(){
    qInfo() << "Double Klik data covid";

    if(menu=="5"){ menu2="52";
    qInfo() << "Dalam Double Klik data covid";

     update_data_realisasi(li_data_covid.at(13),li_data_covid.at(10),li_data_covid.at(11),li_data_covid.at(14),li_data_covid.at(15),li_data_covid.at(8), li_data_covid.at(9),"Update Realisasi dana Covid");
        }
}

void Form::data_update_sementara() // semua
{

    QString tahap = qbx_thp_penc->currentText();
    QString j = le_jml->text();
    j.replace("Rp ",""); j.replace(".",""); j.replace(",",".");
    QString jj_ = j;
    bilang(j);
    QString tgl_ = de_tgl_terima->text();
    QDate date = QDate::fromString(tgl_,"dd-MM-yyyy");
    QString tgl = date.toString("yyyy-MM-dd");
    QString noSrt1 = qbx_no_srt1->currentText();
    QString noSrt2 = qbx_no_srt2->currentText();
    QString jml_2 = ui->tableWidget_realisasi_covid->item(0,4)->text();
    QString jml_4 = li_data_covid.at(11);
    jml_2.replace("Rp ",""); jml_2.replace(".",""); jml_2.replace(",",".");
    jml_4.replace("Rp ",""); jml_4.replace(".",""); jml_4.replace(",",".");
    double j_ = j.toDouble();
    double jml2 = jml_2.toDouble();
    double jml4 = jml_4.toDouble();
    if(jml4 < j_) {  double c;
         c =  jml2 + jml4 - j_;
         if( c<0 ) { QMessageBox::information(this,"Info...","Periksa Ketersediaan Dana...");  return;  }
    }
    update_data_covid(li_data_covid.at(0), tahap, tgl, jj_, terbilang, noSrt1, noSrt2);
}


void Form::update_data_covid(QString id, QString tahap, QString date, QString jml, QString terbilang, QString noSrt1, QString noSrt2)
{

    QSqlQuery query;
    begin();
    QString cmd = "UPDATE pmk_yhk.covid_cair SET thp_cair = :thp_cair,  jml_cair_covid = :j_cair, tgl = :tgl , no_srt1 = :no_srt1 , no_srt2= :no_srt2 , j_terbilang= :terbil WHERE id_real = :id";
    query.prepare(cmd);
    query.bindValue(":id", id);
    query.bindValue(":thp_cair", tahap);
    query.bindValue(":j_cair",jml);
    query.bindValue(":tgl",date);
    query.bindValue(":no_srt1",noSrt1);
    query.bindValue(":no_srt2",noSrt2);
    query.bindValue(":terbil", terbilang+" rupiah");

    bool ok = exec(query);
    if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Mengubah data realisasi Covid, "+query.lastError().text()+""); return;}
    if(ok) { commit();
                QMessageBox::information(eb_v,"Info....","Berhasil Mengubah data realisasi covid");
                eb_v->close();
                memuatData_twRealisasicovid(qbx_id_kam->currentText());
                }
}


void Form::on_toolButton_cetakPdfcovid_clicked()
{
    int noBrs = ui->tableWidget_cetak_covid->currentRow();
    if(noBrs<0){QMessageBox::information(this,"Info...!!!","Pilih Realisasi yang ingin di cetak...");return;}
    if(noBrs>=0){
       dataPdf_covid(noBrs);
       //qInfo()<<"Ini adalah info pada button klik"<< getLspdf1();
       on_saveFileButton_pressed(qcode_covid());
       //qInfo() << "Qinfo Cari Bug===================" <<  on_saveFileButton_pressed(qcode_covid();
       Widget5 *a = new Widget5;
       QIcon logo(":/gbr/html/gbr/yhk.png");
       a->setWindowIcon(logo);
       a->setWindowTitle("Cetak pdf Realisasi Covid");
       a->show();
                        }
}


void Form::dataPdf_covid(int row)
{
    QString lspdf;
    if(menu=="5"){
        for ( int i=1; i<30 ; i++ ) {
            if(i==1){ lspdf.append(ui->tableWidget_cetak_covid->item(row,i)->text());}else {
                lspdf.append("/n"+ui->tableWidget_cetak_covid->item(row,i)->text());
            }
        }

    QString path = "doc/temp/";
    QString l= lspdf;
    QFile fOut(path+"dtpdf_covid1.txt");
    if(fOut.exists()){fOut.remove();}
    if(!fOut.open(QFile::WriteOnly | QFile::Text))
        { qInfo()<<"tdk Bisa Buka file dtpdf_covid1.txt ";}
    else { QTextStream stream(&fOut);
               stream << l;
               fOut.flush();
                fOut.close(); }
    }
}

QString Form::qcode_covid()
{
    int noBrs = ui->tableWidget_cetak_covid->currentRow();
    QString a = ui->tableWidget_cetak_covid->item(noBrs,11)->text();
    QString b = ui->tableWidget_cetak_covid->item(noBrs,14)->text();
    QString b_ =" "+ b.right(4);
    QString c = " Distrik " + ui->tableWidget_cetak_covid->item(noBrs,3)->text();
    QString d = " Kampung " + ui->tableWidget_cetak_covid->item(noBrs,4)->text();
    QString e = "@" + ui->tableWidget_cetak_covid->item(noBrs,10)->text();
    QString f = "/" +ui->tableWidget_cetak_covid->item(noBrs,9)->text();
    QString g = "/" +ui->tableWidget_cetak_covid->item(noBrs,5)->text();
    QString h = "/" +ui->tableWidget_cetak_covid->item(noBrs,12)->text();
    qInfo() << "Barcode Text" << a+b_+c+d+e+f+g+h;
    return a+b_+c+d+e+f+g+h;
}

void Form::update_data_realisasi(QString date, QString tahap ,QString jml,QString noSrt1,QString noSrt2,QString nmKp, QString nmBk, QString title )
{

    QDate dt = QDate::fromString(date,"dd-MM-yyyy");
    c =new QPushButton;
    c->setText("C");
    c->setMaximumWidth(22);

    qbx_thp_penc = new QComboBox;
    qbx_thp_penc->setEditable(true);
    qbx_thp_penc->addItem(tahap);
    //qbx_thp_l->addItems(li_tahap);
    de_tgl_terima = new QDateEdit;
    de_tgl_terima->setDate(dt);
    de_tgl_terima->setDisplayFormat("dd-MM-yyyy");
    de_tgl_terima->setCalendarPopup(true);
    de_tgl_terima->setMinimumWidth(80);

    le_jml = new QLineEdit;
    le_jml->setValidator(new QDoubleValidator);
    le_jml->setClearButtonEnabled(true);
    le_jml->setText(jml);

    qbx_no_srt1 = new QComboBox;
    qbx_no_srt1->addItem(noSrt1);
    qbx_no_srt1->setEditable(true);
    qbx_no_srt1->setMinimumWidth(140);
    qbx_no_srt2 = new QComboBox;
    qbx_no_srt2->addItem(noSrt2);
    qbx_no_srt2->setEditable(true);
    qbx_no_srt2->setMinimumWidth(140);
    //qbx_persen= new QComboBox;

    btn1= new QPushButton;
    btn1->setText("UBAH");
    btn1->setStyleSheet("QPushButton{font-weight: bold; }");
    btn1->setMaximumWidth(145);
    QLabel *ksg = new QLabel;
    ksg->setText("");
    ksg->setMaximumWidth(145);
    QLabel *thp= new QLabel; thp->setText("Tahap Pencairan :"); thp->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    thp->setMaximumWidth(145);
    thp->setMinimumWidth(145);
    QLabel *tgl= new QLabel; tgl->setText("Tanggal :"); tgl->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    tgl->setMaximumWidth(145);
    tgl->setMinimumWidth(145);
    QLabel *Jumlah= new QLabel; Jumlah->setText("Jumlah Pencairan :"); Jumlah->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    Jumlah->setMaximumWidth(145);
    Jumlah->setMinimumWidth(145);
    QLabel *No_1= new QLabel; No_1->setText("NO SPPD :"); No_1->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    No_1->setMaximumWidth(145);
    No_1->setMinimumWidth(145);
    QLabel *No_2= new QLabel; No_2->setText("NO SPPS :"); No_2->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    No_2->setMaximumWidth(145);
    No_2->setMinimumWidth(145);
    QLabel *per= new QLabel; per->setText("Persentase Pencairan :"); per->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    per->setMaximumWidth(145);
    per->setMinimumWidth(145);
    QLabel *nmKkam = new QLabel;nmKkam->setText("Kepala Kampung :"); nmKkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    nmKkam->setMaximumWidth(145);
    nmKkam->setMinimumWidth(145);
    QLabel *nmBkam = new QLabel;nmBkam->setText("Bendahara :"); nmBkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    nmBkam->setMaximumWidth(145);
    nmBkam->setMinimumWidth(145);
    QLabel *nmKkam_ = new QLabel; nmKkam_->setStyleSheet(" QLabel { font-weight: bold;}");
    QLabel *nmBkam_ = new QLabel; nmBkam_->setStyleSheet(" QLabel { font-weight: bold;}");
    nmKkam_->setText(nmKp);
    nmBkam_->setText(nmBk);

    eb_v = new QWidget;
    QIcon logo(":/gbr/html/gbr/yhk.png");
    eb_v->setWindowIcon(logo);
    eb_v->setWindowTitle(title);
    eb_v->setMinimumWidth(500);
    eb_v->setMaximumWidth(500);
    eb_v->setMaximumHeight(220);

    QVBoxLayout *VL = new QVBoxLayout(eb_v);
    QHBoxLayout *HL1 = new QHBoxLayout;
    QHBoxLayout *HL2 = new QHBoxLayout;
    QHBoxLayout *HL3 = new QHBoxLayout;
    QHBoxLayout *HL4 = new QHBoxLayout;
    QHBoxLayout *HL5 = new QHBoxLayout;

    QHBoxLayout *HL8 = new QHBoxLayout;
    QHBoxLayout *HL9 = new QHBoxLayout;
    QHBoxLayout *HL10 = new QHBoxLayout;

    VL->addLayout(HL1);
    VL->addLayout(HL2);
    VL->addLayout(HL3);
    VL->addLayout(HL4);
    VL->addLayout(HL5);

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

    HL8->addWidget(nmKkam);
    HL8->addWidget(nmKkam_);
    HL9->addWidget(nmBkam);
    HL9->addWidget(nmBkam_);

    HL10->addWidget(btn1);
    eb_v->show();

   connect(btn1, SIGNAL(pressed()), this, SLOT(click_btn1()));
   connect(c, SIGNAL(pressed()), this, SLOT(active_eb_v()));

   //Tambahan untuk auto lap dan persentase nilai pencairan
   connect(qbx_thp_penc, SIGNAL (currentIndexChanged(int)), this, SLOT(eventQbxadd()));
}

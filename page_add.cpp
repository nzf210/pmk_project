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



void Form::on_toolButton_add_clicked()
{
    boderToolbar(3);
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
    ui->comboBox_filter->setVisible(false);
    ui->label_filter->setVisible(false);
    ui->toolButton_filter->setVisible(false);

    ui->stackedWidget->setCurrentIndex(2);
    menu="3";
    muat_kampung();
    muat_dis();
    ui->comboBox->setCurrentIndex(0);
    QString n="";
    muat_v_bam_2(n);
}


// Klik Tambah Pencairan Alokasi dana desa
void Form::on_toolButton_tmbRealadd_clicked()
{
    if(menu=="3"){
    if(ui->comboBox->currentText()==""&&ui->comboBox_nmKampung->currentText()==""){ QMessageBox::information(this,"Info...!!!","Pilih distrik dan kampung...");     return;}
                            act_2();}
}

void Form::on_toolButton_cetakPdfadd_clicked()
{
    int noBrs = ui->tableWidget_cetak_add->currentRow();
    if(noBrs<0){QMessageBox::information(this,"Info...!!!","Pilih Realisasi yang ingin di cetak...");return;}
    if(noBrs>=0){
        datapdf2();
        QString id = ui->tableWidget_cetak_add->item(noBrs,2)->text();
        databam(id);

        Widget2 *a = new Widget2;
        QIcon logo(":/gbr/html/gbr/yhk.png");
        a->setWindowIcon(logo);
        a->show();
    }
}


void Form::event_klik_tw_11() // Tw 11
{
    //qInfo()<< "Even Klik tw_11 ";
    if(menu=="3"){
    QClipboard *clipboard = QApplication::clipboard();
    int tbl_lines = ui->tableWidget_rAnggaran_add->rowCount();
    QString str =  " No \t Nama Distrik  \t Nama Kampung  \t  Pagu Anggaran  \t  Sisa Anggaran  \t Realisasi \n ";
    for (int i=0; i<tbl_lines; i++)
    {
    QString mydata0 = ui->tableWidget_rAnggaran_add->item(i, 0)->text();
    QString mydata1 = ui->tableWidget_rAnggaran_add->item(i, 1)->text();
    QString mydata2 = ui->tableWidget_rAnggaran_add->item(i, 2)->text();
    QString mydata3 = ui->tableWidget_rAnggaran_add->item(i, 3)->text();
    mydata3.replace("Rp ",""); mydata3.replace(".",""); mydata3.replace(",",".");
    double p = mydata3.toDouble();
    QString p_ = QString::number(p);

    QString mydata4 = ui->tableWidget_rAnggaran_add->item(i, 4)->text();
    mydata4.replace("Rp ",""); mydata4.replace(".",""); mydata4.replace(",",".");
    double r = mydata4.toDouble();
    QString r_ = QString::number(r);
    QString mydata5 = ui->tableWidget_rAnggaran_add->item(i, 5)->text();
    mydata5.replace("Rp ",""); mydata5.replace(".",""); mydata5.replace(",",".");
    double s = mydata5.toDouble();
    QString s_ = QString::number(s);

    QTextStream(&str) << mydata0 << "\t" << mydata1 << "\t"<< mydata2 << "\t" << p_ <<"\t"<< r_ << "\t" << s_ << Qt::endl;
    }
    clipboard->setText(str);
}}


void Form::muat_real_2(QString &s_id_kamp) //
{
    while(ui->tableWidget_cetak_add->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_cetak_add->removeRow(0);}
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

        //int i = ui->tableWidget_cetak_dds->rowCount();
        ui->tableWidget_cetak_add->insertRow(i);
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

        ui->tableWidget_cetak_add->setCellWidget(i,0,rb);
        ui->tableWidget_cetak_add->setItem(i,1,nm_dis_);
        ui->tableWidget_cetak_add->setItem(i,2,nm_kam_);
        ui->tableWidget_cetak_add->setItem(i,3,id_dis_);
        ui->tableWidget_cetak_add->setItem(i,4,id_kam_);
        ui->tableWidget_cetak_add->setItem(i,5,no_rek_);
        ui->tableWidget_cetak_add->setItem(i,6,nm_rek_);
        ui->tableWidget_cetak_add->setItem(i,7,nm_bank_);
        ui->tableWidget_cetak_add->setItem(i,8,j_sek_);
        ui->tableWidget_cetak_add->setItem(i,9,nm_kp_);
        ui->tableWidget_cetak_add->setItem(i,10,nm_ben_);
        ui->tableWidget_cetak_add->setItem(i,11,thp_cair_);
        ui->tableWidget_cetak_add->setItem(i,12,j_cair_);
        ui->tableWidget_cetak_add->setItem(i,13,j_terbil_);
        ui->tableWidget_cetak_add->setItem(i,14,tgl_);
        ui->tableWidget_cetak_add->setItem(i,15,no_srt1_);
        ui->tableWidget_cetak_add->setItem(i,16,no_srt2_);
        ui->tableWidget_cetak_add->setItem(i,17,persen_);
        ui->tableWidget_cetak_add->setItem(i,18,sk_bup_);
        ui->tableWidget_cetak_add->setItem(i,19,sk_kam_);
        ui->tableWidget_cetak_add->setItem(i,20,sk_men_);
        ui->tableWidget_cetak_add->setItem(i,21,sk_keu_);
        ui->tableWidget_cetak_add->setItem(i,22,ket_);
        ui->tableWidget_cetak_add->setItem(i,23,nm_kpd_);
        ui->tableWidget_cetak_add->setItem(i,24,j_kpd_);
        ui->tableWidget_cetak_add->setItem(i,25,nip_kpd_);
        ui->tableWidget_cetak_add->setItem(i,26,thp_cair2_);
        ui->tableWidget_cetak_add->setItem(i,27,id_);
        ui->tableWidget_cetak_add->setItem(i,28,jkk_);
       i++;
    }
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
    QString jcair3 = ui->tableWidget_rAnggaran_add->item(0,4)->text();
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
            int noBrs = ui->tableWidget_cetak_add->currentRow();

            QString id_dis = qbx_id_dis->currentText();
            QString id_kam = qbx_id_kam->currentText();
            QString j = le_jml->text();

            QString jcair = le_jml->text();
            jcair.replace("Rp ",""); jcair.replace(".",""); jcair.replace(",",".");
            double jc = jcair.toDouble();

            QString jcair2 = ui->tableWidget_cetak_add->item(noBrs,12)->text();
            jcair2.replace("Rp ",""); jcair2.replace(".",""); jcair2.replace(",",".");
            double jc2 = jcair2.toDouble();

            QString jcair3 = ui->tableWidget_rAnggaran_add->item(0,4)->text();
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

            // QString jcair3 = ui->tableWidget_rAnggaran_add->item(0,4)->text();
            jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
           // double jc3 = jcair3.toDouble();
            //double jj = jcair.toDouble();
            //nfo() << "jc3" << jc3 <<"jj" << jj;
           //if((jc3-jj)<0){QMessageBox::information(eb_v,"Info...","Mohon Periksa Ketersediaan Dana 4444444444"); return;}
            //QString jj_ = QString::number(jj);
            //bilang(jj_);

            QString id=ui->tableWidget_cetak_add->item(noBrs,27)->text();

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



void Form::datapdf2()
{
    int noBrs = ui->tableWidget_cetak_add->currentRow();

if(noBrs!=-1 && menu=="3"){
    QString id_kam =  ui->tableWidget_cetak_add->item(noBrs,2)->text();
    QString nmdis = ui->tableWidget_cetak_add->item(noBrs,3)->text();
    QString nmkamp = ui->tableWidget_cetak_add->item(noBrs,4)->text();
    QString terbilang= ui->tableWidget_cetak_add->item(noBrs,13)->text();
    QString norek = ui->tableWidget_cetak_add->item(noBrs,5)->text();
    QString nmrek = ui->tableWidget_cetak_add->item(noBrs,6)->text();
    QString nmbank = ui->tableWidget_cetak_add->item(noBrs,7)->text();
    QString nmkkp = ui->tableWidget_cetak_add->item(noBrs,9)->text();
    QString nmbenk = ui->tableWidget_cetak_add->item(noBrs,10)->text();
    QString jkk = ui->tableWidget_cetak_add->item(noBrs,28)->text();
    QString tahap = ui->tableWidget_cetak_add->item(noBrs,11)->text();
    QString jml = ui->tableWidget_cetak_add->item(noBrs,12)->text();
    QString persen = ui->tableWidget_cetak_add->item(noBrs,17)->text();
    QString nmkpdns = ui->tableWidget_cetak_add->item(noBrs,22)->text();
    QString tahap2 = ui->tableWidget_cetak_add->item(noBrs,26)->text();
    QString pkpldns = ui->tableWidget_cetak_add->item(noBrs,24)->text();
    QString nip = ui->tableWidget_cetak_add->item(noBrs,25)->text();
    QString nosrt1 = ui->tableWidget_cetak_add->item(noBrs,15)->text();
    QString nosrt2 = ui->tableWidget_cetak_add->item(noBrs,16)->text();
    QString tgl = ui->tableWidget_cetak_add->item(noBrs,14)->text();
    QString skben= ui->tableWidget_cetak_add->item(noBrs,8)->text();
    QString skkkam= ui->tableWidget_cetak_add->item(noBrs,19)->text();

    QString unmdis = nmdis.toUpper();
    QString unmkam = nmkamp.toUpper();

    //Tambahan thn 2
    QString thn2 = ui->tableWidget_cetak_add->item(noBrs,23)->text();

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
     QString pagu = ui->tableWidget_rAnggaran_add->item(0,3)->text();
     pagu.replace("Rp ",""); pagu.replace(".",""); pagu.replace(",",".");

     double pg = pagu.toDouble();
     double pe = per.toDouble();
     double hs1 = pe/100;
     double hs = hs1 * pg;
     //nfo() << pg;
     QString j = indo.toCurrencyString(hs, "Rp ");
     le_jml->setText(j);
}



void Form::event_doubleklik_tw_cetak_add()  //Double Klik Edit data realisasi add double klik tw 9
{

//    disconnect(btn1, SIGNAL(pressed()), this, SLOT(click_btn2()));
//    disconnect(c, SIGNAL(pressed()), this, SLOT(active_eb_v_2()));

   //Info() << "Doubleklik tw 9";
    if(menu=="3"){ menu2="4";
        QString id_dis = qbx_id_dis->currentText();
        QString id_kam = qbx_id_kam->currentText();
        if(id_dis==""&& id_kam==""){QMessageBox::information(this,"Info","Pilih Distrik dan Kampung..."); return;}
        int noBrs = ui->tableWidget_cetak_add->currentRow();

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
            le_jml->setText( ui->tableWidget_cetak_add->item(noBrs,12)->text());
            le_jml->setMaximumWidth(190);


            //========== Data dari table widget ================
            QString thp_tw = ui->tableWidget_cetak_add->item(noBrs,11)->text();

//            QString jcair = le_jml->text();
//            jcair.replace("Rp ",""); jcair.replace(".",""); jcair.replace(",",".");
//            double jc = jcair.toDouble();

//            QString jcair2 = ui->tableWidget_cetak_add->item(noBrs,12)->text();
//            jcair2.replace("Rp ",""); jcair2.replace(".",""); jcair2.replace(",",".");
//            double jc2 = jcair2.toDouble();

//            QString jcair3 = ui->tableWidget_rAnggaran_add->item(0,4)->text();
//            jcair3.replace("Rp ",""); jcair3.replace(".",""); jcair3.replace(",",".");
//            double jc3 = jcair3.toDouble();
//            qInfo() << "Doubleklik tw 2";
//            if((jc3+jc2)-(jc3+jc)<0){QMessageBox::information(eb_v,"Info....","Nilai yang anda masukkan melebihi pagu"); return;}

            QString tl = ui->tableWidget_cetak_add->item(noBrs, 14)->text();
            QDate dt = QDate::fromString(tl,"dd-MM-yyyy");
//        QDateEdit *de = new QDateEdit;
//        de->setDate(dt);
//        de->setDisplayFormat("dd-MM-yyyy");

            QString nosr1 = ui->tableWidget_cetak_add->item(noBrs,15)->text();
            QString nosr2 = ui->tableWidget_cetak_add->item(noBrs,16)->text();
            QString persent = ui->tableWidget_cetak_add->item(noBrs,17)->text();
            QString lap = ui->tableWidget_cetak_add->item(noBrs,26)->text();

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


void Form::header_wt9()
{
    QStringList headerWidget;
    ui->tableWidget_cetak_add->setColumnCount(29);
    headerWidget <<"cetak"<<"Id Dis"<<"Id Kam"<<" Nama Distrik"<<"Nama Kampung"<<"No Rekening"<<"Nama Rekening"<<"Nama Bank"<<"SK Bendahara"<<"Nama Kepala Kampung"<<" Nama Bendahara "
                 <<"Tahap Pencairan"<<"Jumlah Pencairan"<<"Terbilang"<<"Tanggal Terima"<<"No Srt 1"<<"No Srt 2"<<"%"<<"SK Bupati"<<"SK Kep.Kampung"
                <<"SK Men PMK"<<"SK Men Keu"<<"Kepala Dinas" << " Ket " << "Pangkat" << "NIP" <<"Laporan Realisasi"<<"id"<<"jkk";
    ui->tableWidget_cetak_add->setHorizontalHeaderLabels(headerWidget);

    ui->tableWidget_cetak_add->setColumnHidden(0,true);
    ui->tableWidget_cetak_add->setColumnHidden(1,true);
    ui->tableWidget_cetak_add->setColumnHidden(2,true);
    //ui->tableWidget_cetak_add->setColumnHidden(3,true);
    //ui->tableWidget_cetak_add->setColumnHidden(4,true);
    ui->tableWidget_cetak_add->setColumnHidden(5,true);
    ui->tableWidget_cetak_add->setColumnHidden(6,true);
    ui->tableWidget_cetak_add->setColumnHidden(7,true);
    ui->tableWidget_cetak_add->setColumnHidden(8,true);
    ui->tableWidget_cetak_add->setColumnHidden(9,true);
    ui->tableWidget_cetak_add->setColumnHidden(10,true);
    //ui->tableWidget_cetak_add->setColumnHidden(11,true);
    //ui->tableWidget_cetak_add->setColumnHidden(12,true);
    //ui->tableWidget_cetak_add->setColumnHidden(13,true);
    //ui->tableWidget_cetak_add->setColumnHidden(14,true);
    //ui->tableWidget_cetak_add->setColumnHidden(15,true);
    //ui->tableWidget_cetak_add->setColumnHidden(16,true);
    //ui->tableWidget_cetak_add->setColumnHidden(17,true);
    ui->tableWidget_cetak_add->setColumnHidden(18,true);
    ui->tableWidget_cetak_add->setColumnHidden(19,true);
    ui->tableWidget_cetak_add->setColumnHidden(20,true);
    ui->tableWidget_cetak_add->setColumnHidden(21,true);
    ui->tableWidget_cetak_add->setColumnHidden(22,true);
    ui->tableWidget_cetak_add->setColumnHidden(23,true);
    ui->tableWidget_cetak_add->setColumnHidden(24,true);
    ui->tableWidget_cetak_add->setColumnHidden(25,true);
    //ui->tableWidget_cetak_add->setColumnHidden(26,true);
    ui->tableWidget_cetak_add->setColumnHidden(27,true);
    ui->tableWidget_cetak_add->setColumnHidden(28,true);

   ui->tableWidget_cetak_add->setColumnWidth(3,120);
   ui->tableWidget_cetak_add->setColumnWidth(4,120);
   ui->tableWidget_cetak_add->setColumnWidth(11,150);
   ui->tableWidget_cetak_add->setColumnWidth(12,120);
   ui->tableWidget_cetak_add->setColumnWidth(13,340);
   ui->tableWidget_cetak_add->setColumnWidth(14,90);
   ui->tableWidget_cetak_add->setColumnWidth(15,230);
   ui->tableWidget_cetak_add->setColumnWidth(16,230);
   ui->tableWidget_cetak_add->setColumnWidth(17,50);
   ui->tableWidget_cetak_add->setColumnWidth(26,120);

}


void Form::header_wt11() // Header table widget 6
{
    QStringList headerWidget;
    ui->tableWidget_rAnggaran_add->setColumnCount(6);
    headerWidget <<"id" <<"Nama Distrik"<<"Nama Kampung" <<"Pagu Anggaran " <<"Sisa Anggaran "<<"Realisasi ";
    ui->tableWidget_rAnggaran_add->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_rAnggaran_add->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_rAnggaran_add->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_rAnggaran_add->setColumnWidth(0,8);
    ui->tableWidget_rAnggaran_add->setColumnHidden(0,true);
    ui->tableWidget_rAnggaran_add->setColumnWidth(1,140);
    ui->tableWidget_rAnggaran_add->setColumnWidth(2,150);
    //ui->tableWidget->setColumnHidden(2,true);
    ui->tableWidget_rAnggaran_add->setColumnWidth(3,120);
    ui->tableWidget_rAnggaran_add->setColumnWidth(4,120);
    ui->tableWidget_rAnggaran_add->setColumnWidth(5,120);

}


void Form::muat_v_bam_2(QString &id_kam_s) // Data realisasi kampung
{
    while(ui->tableWidget_rAnggaran_add->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_rAnggaran_add->removeRow(0);}
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

            ui->tableWidget_rAnggaran_add->insertRow(noBrs);

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
            ui->tableWidget_rAnggaran_add->setItem(noBrs,0,no_);
            ui->tableWidget_rAnggaran_add->setItem(noBrs,1,dis_);
            ui->tableWidget_rAnggaran_add->setItem(noBrs,2,kam_);
            ui->tableWidget_rAnggaran_add->setItem(noBrs,3,pagu_);
            ui->tableWidget_rAnggaran_add->setItem(noBrs,4,real_);
            ui->tableWidget_rAnggaran_add->setItem(noBrs,5,sisa_);
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



void Form::muatrealadd()
{
    while(ui->tableWidget_realisasi_sppd->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_realisasi_sppd->removeRow(0);}

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

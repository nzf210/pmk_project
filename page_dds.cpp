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


void Form::on_toolButton_cetakPdfdds_clicked()
{
    int noBrs = ui->tableWidget_cetak_dds->currentRow();
    if(noBrs<0){QMessageBox::information(this,"Info...!!!","Pilih Realisasi yang ingin di cetak...");return;}
    if(noBrs>=0){
       datapdf1();
       //qInfo()<<"Ini adalah info pada button klik"<< getLspdf1();
       on_saveFileButton_pressed(qcode());
       //qInfo() << "Qinfo Cari Bug===========================================================================================";
       Widget1 *a = new Widget1;
       QIcon logo(":/gbr/html/gbr/yhk.png");
       a->setWindowIcon(logo);
       a->show();
                        }
}


// Klik Tambah Pencairan dana desa
void Form::on_toolButton_tmbRealdds_clicked()
{
      if(menu=="2"){
      if(ui->comboBox->currentText()==""&&ui->comboBox_nmKampung->currentText()==""){ QMessageBox::information(this,"Info...!!!","Pilih distrik dan kampung...");     return;}
                            act(); }
}


void Form::event_copy_twRealisasi_dds()
{
   // qInfo()<< "Even Klik tw_6 ";
    if(menu=="2"){
    QClipboard *clipboard = QApplication::clipboard();
    int tbl_lines = ui->tableWidget_realisasi_dds->rowCount();
    QString str =  " No \t Nama Distrik  \t Nama Kampung  \t  Pagu Anggaran  \t  Sisa Anggaran  \t Realisasi \n ";
    for (int i=0; i<tbl_lines; i++)
    {
    QString mydata0 = ui->tableWidget_realisasi_dds->item(i, 0)->text();
    QString mydata1 = ui->tableWidget_realisasi_dds->item(i, 1)->text();
    QString mydata2 = ui->tableWidget_realisasi_dds->item(i, 2)->text();

    QString mydata3 = ui->tableWidget_realisasi_dds->item(i, 3)->text();
    mydata3.replace(",00","");
    mydata3.replace("Rp ",""); mydata3.replace(".","");
//    double p = mydata3.toDouble();
//    QString p_ = QString::number(p);

    QString mydata4 = ui->tableWidget_realisasi_dds->item(i, 4)->text();
    mydata4.replace("Rp ",""); mydata4.replace(".",""); mydata4.replace(",00","");
//    double r = mydata4.toDouble();
//    QString r_ = QString::number(r);

    QString mydata5 = ui->tableWidget_realisasi_dds->item(i, 5)->text();
    mydata5.replace("Rp ",""); mydata5.replace(".",""); mydata5.replace(",00","");
//    double s = mydata5.toDouble();

//    QString s_ = QString::number(s);

    QTextStream(&str) << mydata0 << "\t" << mydata1 << "\t"<< mydata2 << "\t" << mydata3 <<"\t"<< mydata4 << "\t" << mydata5 << Qt::endl;
    }
    clipboard->setText(str);
}}

void Form::on_tableWidget_realisasi_dds_cellClicked(int row, int column)
{
    qInfo()<< row << column;
    event_copy_twRealisasi_dds();
}


void Form::muat_real(QString &s_id_kamp) //
{
    while(ui->tableWidget_cetak_dds->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_cetak_dds->removeRow(0);}
    QSqlQuery query;
    QString cmd = "SELECT * FROM pmk_yhk.dds_cair WHERE id_kam = :id ORDER BY id_real ";
    //if(s_id_kamp==""){cmd ="SELECT * FROM pmk_yhk.t_real ORDER BY id_real ";}
     if(s_id_kamp==""){cmd =" SELECT	* FROM	pmk_yhk.dds_cair ORDER BY	id_real ASC, pmk_yhk.dds_cair.id_real LIMIT 40 ";}
    query.prepare(cmd);
    query.bindValue(":id",s_id_kamp);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat data realisasi"); return;}
    int i=0;
    while (query.next()) {

        //int i = ui->tableWidget_cetak_dds->rowCount();
        ui->tableWidget_cetak_dds->insertRow(i);
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

        ui->tableWidget_cetak_dds->setCellWidget(i,0,rb);
        ui->tableWidget_cetak_dds->setItem(i,1,nm_dis_);
        ui->tableWidget_cetak_dds->setItem(i,2,nm_kam_);
        ui->tableWidget_cetak_dds->setItem(i,3,id_dis_);
        ui->tableWidget_cetak_dds->setItem(i,4,id_kam_);
        ui->tableWidget_cetak_dds->setItem(i,5,no_rek_);
        ui->tableWidget_cetak_dds->setItem(i,6,nm_rek_);
        ui->tableWidget_cetak_dds->setItem(i,7,nm_bank_);
        ui->tableWidget_cetak_dds->setItem(i,8,j_sek_);
        ui->tableWidget_cetak_dds->setItem(i,9,nm_kp_);
        ui->tableWidget_cetak_dds->setItem(i,10,nm_ben_);
        ui->tableWidget_cetak_dds->setItem(i,11,thp_cair_);
        ui->tableWidget_cetak_dds->setItem(i,12,j_cair_);
        ui->tableWidget_cetak_dds->setItem(i,13,j_terbil_);
        ui->tableWidget_cetak_dds->setItem(i,14,tgl_);
        ui->tableWidget_cetak_dds->setItem(i,15,no_srt1_);
        ui->tableWidget_cetak_dds->setItem(i,16,no_srt2_);
        ui->tableWidget_cetak_dds->setItem(i,17,persen_);
        ui->tableWidget_cetak_dds->setItem(i,18,sk_bup_);
        ui->tableWidget_cetak_dds->setItem(i,19,sk_kam_);
        ui->tableWidget_cetak_dds->setItem(i,20,sk_men_);
        ui->tableWidget_cetak_dds->setItem(i,21,sk_keu_);
        ui->tableWidget_cetak_dds->setItem(i,22,ket_);
        ui->tableWidget_cetak_dds->setItem(i,23,nm_kpd_);
        ui->tableWidget_cetak_dds->setItem(i,24,j_kpd_);
        ui->tableWidget_cetak_dds->setItem(i,25,nip_kpd_);
        ui->tableWidget_cetak_dds->setItem(i,26,thp_cair2_);
        ui->tableWidget_cetak_dds->setItem(i,27,id_);
        ui->tableWidget_cetak_dds->setItem(i,28,jkk_);
        ui->tableWidget_cetak_dds->setItem(i,29,jbt_kpl_dns_);
       i++;
    }
}


void Form::datapdf1()
{

    QStringList lspdf;
    int noBrs = ui->tableWidget_cetak_dds->currentRow();
    if(noBrs !=-1 && menu=="2"){
    QString id_kam =  ui->tableWidget_cetak_dds->item(noBrs,2)->text();
    QString nmdis = ui->tableWidget_cetak_dds->item(noBrs,3)->text();
    QString nmkamp = ui->tableWidget_cetak_dds->item(noBrs,4)->text();
    QString terbilang= ui->tableWidget_cetak_dds->item(noBrs,13)->text();
    QString norek = ui->tableWidget_cetak_dds->item(noBrs,5)->text();
    QString nmrek = ui->tableWidget_cetak_dds->item(noBrs,6)->text();
    QString nmbank = ui->tableWidget_cetak_dds->item(noBrs,7)->text();
    QString nmkkp = ui->tableWidget_cetak_dds->item(noBrs,9)->text();
    QString nmbenk = ui->tableWidget_cetak_dds->item(noBrs,10)->text();
    QString jkk = ui->tableWidget_cetak_dds->item(noBrs,28)->text();
    QString tahap = ui->tableWidget_cetak_dds->item(noBrs,11)->text();
    QString jml = ui->tableWidget_cetak_dds->item(noBrs,12)->text();
    QString persen = ui->tableWidget_cetak_dds->item(noBrs,17)->text();
    QString nmkpdns = ui->tableWidget_cetak_dds->item(noBrs,22)->text();
    QString tahap2 = ui->tableWidget_cetak_dds->item(noBrs,26)->text();
    QString pkpldns = ui->tableWidget_cetak_dds->item(noBrs,24)->text();
    QString nip = ui->tableWidget_cetak_dds->item(noBrs,25)->text();
    QString nosrt1 = ui->tableWidget_cetak_dds->item(noBrs,15)->text();
    QString nosrt2 = ui->tableWidget_cetak_dds->item(noBrs,16)->text();
    QString tgl = ui->tableWidget_cetak_dds->item(noBrs,14)->text();
    QString jbt_kp_dns= ui->tableWidget_cetak_dds->item(noBrs,29)->text();

    QString unmdis = nmdis.toUpper();
    QString unmkam = nmkamp.toUpper();\

    QString skben= ui->tableWidget_cetak_dds->item(noBrs,8)->text();
    QString skkkam= ui->tableWidget_cetak_dds->item(noBrs,19)->text();

    //Tambahan thn 2
    QString thn2 = ui->tableWidget_cetak_dds->item(noBrs,23)->text();
    QString id_real_dds = ui->tableWidget_cetak_dds->item(noBrs,1)->text();


    lspdf << nmdis << nmkamp << terbilang << norek << nmrek << nmbank <<nmkkp << nmbenk << jkk << tahap << jml << persen
          << nmkpdns << tahap2 << pkpldns << nip << nosrt1 << nosrt2 << tgl << unmdis << unmkam <<id_real_dds;
    lspdf1 =lspdf;
    qInfo() << "Data List Dari PDF 1 at 2" << getLspdf1() << " ============== Data id  ========="+id_real_dds;


    pdfdt1 = nmdis +"/n "+ nmkamp+ "/n" +terbilang +"/n"+norek+"/n"+ nmrek +"/n"+nmbank+"/n"+nmkkp+"/n"+nmbenk+"/n"
                     ""+jkk+"/n"+ tahap+ "/n"+ jml+"/n"+ persen+ "/n" +nmkpdns+ "/n"+ tahap2+ "/n"+ pkpldns +"/n"+ nip +"/n"+ nosrt1 +"/n"
                    ""+ nosrt2 +"/n"+ tgl +"/n"+ unmdis+"/n"+ unmkam +"/n"+ skben +"/n"+ skkkam +"/n"+thn2+ "/n"+jbt_kp_dns;

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
}
}



// ==== Button Menu Dana Desa ==========
void Form::on_toolButton_danaDesa_clicked()
{
    boderToolbar(2);
    ui->label_barCode->setVisible(true);
    sembunyi_subMenu();

    ui->stackedWidget->setCurrentIndex(1);

   menu ="2";
   muat_kampung();
   muat_dis();
   ui->comboBox->setCurrentIndex(0);
   QString n="";
   memuatData_twRealisasidds(n);
}


void Form::eventQbxadd() // QCombox even pada tahap pencairan dana desa
{
      int indextahap = qbx_thp_penc->currentIndex();
      if(indextahap == 0){ le_jml->setText("");
       return;
      }
     qInfo() << "Tahapan index dana Desa" << indextahap;
    QString   thp = li_tahap_select.at(indextahap);
      qInfo() << "Tahapan pada dana Desa" << thp;

    muatTahap(thp);
    //qbx_thp_l->clear();
    //qbx_persen->clear();
    le_jml->clear();
    QString pagu =  pagu_pencairan;
    double pg = pagu.toDouble();
    QString j = indo.toCurrencyString(pg, "Rp ");
    le_jml->setText(j);
}



void Form::header_wt2()
{
    QStringList headerWidget;
    ui->tableWidget_cetak_dds->setColumnCount(30);
    headerWidget <<"cetak"<<"Id Dis"<<"Id Kam"<<" Nama Distrik"<<"Nama Kampung"<<"No Rekening"<<"Nama Rekening"<<"Nama Bank"<<"SK Bendahara"<<"Nama Kepala Kampung"<<" Nama Bendahara "
                 <<"Tahap Pencairan"<<"Jumlah Pencairan"<<"Terbilang"<<"Tanggal Terima"<<"No Srt 1"<<"No Srt 2"<<"%"<<"SK Bupati"<<"SK Kep.Kampung"
                <<"SK Men PMK"<<"SK Men Keu"<<"Kepala Dinas" << " Ket " << "Pangkat" << "NIP" <<"Laporan Realisasi"<<"id"<<"jkk"<<"jbt kp dns" << "id_realisasi";
    ui->tableWidget_cetak_dds->setHorizontalHeaderLabels(headerWidget);

    ui->tableWidget_cetak_dds->setColumnHidden(0,true);
    ui->tableWidget_cetak_dds->setColumnHidden(1,true);
    ui->tableWidget_cetak_dds->setColumnHidden(2,true);
    //ui->tableWidget_cetak_dds->setColumnHidden(3,true);
    //ui->tableWidget_cetak_dds->setColumnHidden(4,true);
    ui->tableWidget_cetak_dds->setColumnHidden(5,true);
    ui->tableWidget_cetak_dds->setColumnHidden(6,true);
    ui->tableWidget_cetak_dds->setColumnHidden(7,true);
    ui->tableWidget_cetak_dds->setColumnHidden(8,true);
    ui->tableWidget_cetak_dds->setColumnHidden(9,true);
    ui->tableWidget_cetak_dds->setColumnHidden(10,true);
    ui->tableWidget_cetak_dds->setColumnHidden(18,true);
    ui->tableWidget_cetak_dds->setColumnHidden(19,true);
    ui->tableWidget_cetak_dds->setColumnHidden(20,true);
    ui->tableWidget_cetak_dds->setColumnHidden(21,true);
    ui->tableWidget_cetak_dds->setColumnHidden(22,true);
    ui->tableWidget_cetak_dds->setColumnHidden(23,true);
    ui->tableWidget_cetak_dds->setColumnHidden(24,true);
    ui->tableWidget_cetak_dds->setColumnHidden(25,true);
    ui->tableWidget_cetak_dds->setColumnHidden(27,true);
    ui->tableWidget_cetak_dds->setColumnHidden(28,true);
    ui->tableWidget_cetak_dds->setColumnHidden(29,true);
    ui->tableWidget_cetak_dds->setColumnHidden(30,true);
    ui->tableWidget_cetak_dds->setColumnHidden(26,true);
    ui->tableWidget_cetak_dds->setColumnHidden(17,true);

    ui->tableWidget_cetak_dds->setColumnWidth(3,120);
    ui->tableWidget_cetak_dds->setColumnWidth(4,120);
   ui->tableWidget_cetak_dds->setColumnWidth(11,150);
   ui->tableWidget_cetak_dds->setColumnWidth(12,120);
   ui->tableWidget_cetak_dds->setColumnWidth(13,340);
   ui->tableWidget_cetak_dds->setColumnWidth(14,90);
   ui->tableWidget_cetak_dds->setColumnWidth(15,230);
   ui->tableWidget_cetak_dds->setColumnWidth(16,230);
   //ui->tableWidget_cetak_dds->setColumnWidth(17,50);
   //ui->tableWidget_cetak_dds->setColumnWidth(26,120);

}


void Form::header_realisasi_dds() // Header table widget 6
{
    QStringList headerWidget;
    ui->tableWidget_realisasi_dds->setColumnCount(6);
    headerWidget <<"id" <<"Nama Distrik"<<"Nama Kampung" <<"Pagu Anggaran " <<"Sisa Anggaran "<<"Realisasi ";
    ui->tableWidget_realisasi_dds->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_realisasi_dds->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_realisasi_dds->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_realisasi_dds->setColumnWidth(0,8);
    ui->tableWidget_realisasi_dds->setColumnHidden(0,true);
    ui->tableWidget_realisasi_dds->setColumnWidth(1,140);
    ui->tableWidget_realisasi_dds->setColumnWidth(2,150);
    //ui->tableWidget->setColumnHidden(2,true);
    ui->tableWidget_realisasi_dds->setColumnWidth(3,120);
    ui->tableWidget_realisasi_dds->setColumnWidth(4,120);
    ui->tableWidget_realisasi_dds->setColumnWidth(5,120);

}



void Form::memuatData_twRealisasidds(QString id_kam_s) // Data realisasi kampung
{
    while(ui->tableWidget_realisasi_dds->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_realisasi_dds->removeRow(0);}
    //QLocale indo = QLocale(QLocale::Indonesian, QLocale::Indonesia);

    double pagu_blt=0;
    double tot_cair_blt=0;
    double sisa_blt=0;

    QSqlQuery query;
    QString cmd= " SELECT pmk_yhk.m_dds_2021.nama_kampung, pmk_yhk.m_dds_2021.nama_distrik, pmk_yhk.m_dds_2021.id_dis, pmk_yhk.m_dds_2021.id, pmk_yhk.m_dds_2021.pagu_dana_desa, SUM ( pmk_yhk.dds_cair.j_cair ) "
                 "AS total_pencairan, pmk_yhk.m_dds_2021.pagu_dana_desa - SUM ( pmk_yhk.dds_cair.j_cair ) AS sisa_dana "
           " FROM "
                "pmk_yhk.m_dds_2021 "
              " LEFT JOIN pmk_yhk.dds_cair ON pmk_yhk.m_dds_2021.id = pmk_yhk.dds_cair.id_kam "
            " GROUP BY "
                "pmk_yhk.m_dds_2021.nama_kampung,"
             "   pmk_yhk.m_dds_2021.nama_distrik,"
              "  pmk_yhk.m_dds_2021.id_dis,"
              "  pmk_yhk.m_dds_2021.id"
          "  ORDER BY "
            "  pmk_yhk.m_dds_2021.id ";

    QString cmd_ = " SELECT pmk_yhk.m_dds_2021.nama_kampung, pmk_yhk.m_dds_2021.nama_distrik, pmk_yhk.m_dds_2021.id_dis, pmk_yhk.m_dds_2021.id, pmk_yhk.m_dds_2021.pagu_dana_desa, SUM ( pmk_yhk.dds_cair.j_cair ) "
                   "AS total_pencairan, pmk_yhk.m_dds_2021.pagu_dana_desa - SUM ( pmk_yhk.dds_cair.j_cair ) AS sisa_dana "
             " FROM "
                  "pmk_yhk.m_dds_2021 "
                " LEFT JOIN pmk_yhk.dds_cair ON pmk_yhk.m_dds_2021.id = pmk_yhk.dds_cair.id_kam "
             " WHERE pmk_yhk.m_dds_2021.id = :id "
              " GROUP BY "
                  "pmk_yhk.m_dds_2021.nama_kampung,"
               "   pmk_yhk.m_dds_2021.nama_distrik,"
                "  pmk_yhk.m_dds_2021.id_dis,"
                "  pmk_yhk.m_dds_2021.id"
            "  ORDER BY "
              "  pmk_yhk.m_dds_2021.id " ;

    if (id_kam_s != ""){cmd=cmd_;}

    query.prepare(cmd);
    query.bindValue(":id",id_kam_s);
   //int noBrs=0;
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...!!!","Gagal Memuat realisasi Dana Desa ... "); return;}
    int i=0;
    while (query.next()) {

        //int i = ui->tableWidget_realisasi_blt->rowCount();
        ui->tableWidget_realisasi_dds->insertRow(i);

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

        ui->tableWidget_realisasi_dds->setItem(i,2,nm_kampung_);
        ui->tableWidget_realisasi_dds->setItem(i,1,nm_distrik_);
        ui->tableWidget_realisasi_dds->setItem(i,6,id_dis_);
        ui->tableWidget_realisasi_dds->setItem(i,0,id_kam_);
        ui->tableWidget_realisasi_dds->setItem(i,3,pagu_);
        ui->tableWidget_realisasi_dds->setItem(i,5,tot_cair_);
        ui->tableWidget_realisasi_dds->setItem(i,4,sisa_dana_);
        i++;
       pagu_blt += pagu_d;
       tot_cair_blt += tot_cair_d;

    }

    sisa_blt = pagu_blt - tot_cair_blt;

    QString pagu_blt_ss = indo.toCurrencyString(pagu_blt, "Rp ");
    QString tot_cair_d_ss = indo.toCurrencyString(tot_cair_blt, "Rp ");
    QString realisasi_blt_ss = indo.toCurrencyString(sisa_blt, "Rp ");

    ui->lineEdit_pagu_dds->setText(pagu_blt_ss);
    ui->lineEdit_realisasi_dds->setText(tot_cair_d_ss);
    ui->lineEdit_sisa_dds->setText(realisasi_blt_ss);
    muat_real(id_kam_s);
}





void Form::act()  // form event pada saat add tealisasi dana desa reguler
{

if(menu=="2"){ menu2="2";

    QString id_dis = qbx_id_dis->currentText();
    QString id_kam = qbx_id_kam->currentText();
    if(id_kam!="" && id_dis!=""){
        tahap(1);
        muat_bend_kp(id_kam);
        muat_k_kp(id_kam);
        muat_nosurat();
        qInfo() << "Cari Error +++++++++++++++++++++";
        QString no_srt1 = li_no_surat.at(0);
        QString no_srt2 = li_no_surat.at(1);
        no_srt1.replace("****", nosurat());
        no_srt1.replace("Reg1=> ", "");
        no_srt2.replace("####", nosurat_2());
        no_srt2.replace("Reg2=> ", "");
        tambah_realisasi(li_tahap,"Rp 0,00",  no_srt1  , no_srt2,"TAMBAH",s_nm_kp, s_nm_bend,"Tambah Realisasi Dana Desa Reguler" );

    } }
}

void Form::btnAdd_dds()
{

    if(menu2=="2"){
        if(menu=="2"){
            if(qbx_thp_penc->currentIndex() == 0 ){QMessageBox::information(eb_v,"Info...!!"," Pilih Tahap Pencairan... "); return;};
           QString id_dis = qbx_id_dis->currentText();
           QString id_kam = qbx_id_kam->currentText();
           QString j = le_jml->text();
           if(j=="Rp 0,00"){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh Nilai Rp 0,00,-"); return;}
           if(j==""){le_jml->setText("");QMessageBox::information(eb_v,"Info...!!","Jumlah Pencairan tdk boleh kosong,-"); return;}
           QString jj_ = j;
           j.replace("Rp ",""); j.replace(".",""); j.replace(",",".");
           bilang(j);
           QString jcair3 = ui->tableWidget_realisasi_dds->item(0,4)->text();
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
             QString cmd = "INSERT INTO pmk_yhk.dds_cair (id_kam,nm_dis,nm_kam, no_rek, nm_rek,nm_bank, sk_bend, sk_kp, nm_ben, nm_kp, thp_cair, j_cair, j_terbilang, "
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
             query.bindValue(":persentase","persen");
             query.bindValue(":sk_bup","sk_bup");
             query.bindValue(":sk_kam",sk_kam);
    //         query.bindValue(":sk_pmk",sk_pmk);
    //         query.bindValue(":sk_keu",sk_keu);
             query.bindValue(":nm_kpd",nm_kp_dns);
             query.bindValue(":j_kpd",pg_kp_dns);
             query.bindValue(":nip_kpd",nip_kp_dns);
             query.bindValue(":ket",thn2);
             query.bindValue(":thp_l","tahap_l");
             query.bindValue(":jkk",j_kk);
             query.bindValue(":jbt_kpl_dns", jbt_kpl_dns);

             bool ok = exec(query);
             if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Menambah realisasi Dana Desa , "+query.lastError().text()+""); return;}
             commit();
                eb_v->close();
            }
                  }
    }

        if(menu2=="22"){data_update_sementara_dds();}

}


void Form::on_tableWidget_cetak_dds_cellDoubleClicked(int i, int column)
{
    qInfo() << "ini adalah even change tw dds ============== " << i <<" === " <<column;
    if(qbx_id_dis->currentText() == "" && qbx_id_kam->currentText() == "" ){ QMessageBox::information(this, "Info...","Pilih Kampung dan Distrik ..."); return; }
    if(qbx_id_dis->currentText() != "" && qbx_id_kam->currentText() == "" ){ QMessageBox::information(this, "Info...","Pilih Kampung dan Distrik ..."); return; }
    if(qbx_id_dis->currentText() == "" && qbx_id_kam->currentText() != "" ){ QMessageBox::information(this, "Info...","Pilih Kampung dan Distrik ..."); return; }
    li_data_dds.clear();
    //QString list;
    for (int ii=1; ii<30; ii++) {
        li_data_dds << ui->tableWidget_cetak_dds->item(i,ii)->text();
    }
    if(i>=0) {even_dbKlik_tw_dds();}
}

void Form::even_dbKlik_tw_dds(){
    qInfo() << "Double Klik data dds";

    if(menu=="2"){ menu2="22";
    qInfo() << "Dalam Double Klik data dds";

     update_data_realisasi(li_data_dds.at(13),li_data_dds.at(10),li_data_dds.at(11),li_data_dds.at(14),li_data_dds.at(15),li_data_dds.at(8), li_data_dds.at(9),"Update Realisasi Dana Desa ...");
    }
}

void Form::data_update_sementara_dds()
{
    qInfo() << "upate dds ================ 1111111111";
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
    QString jml_2 = ui->tableWidget_realisasi_dds->item(0,4)->text(); // ===============
    QString jml_4 = li_data_dds.at(11); // ==================
    jml_2.replace("Rp ",""); jml_2.replace(".",""); jml_2.replace(",",".");
    jml_4.replace("Rp ",""); jml_4.replace(".",""); jml_4.replace(",",".");
    double j_ = j.toDouble();
    double jml2 = jml_2.toDouble();
    double jml4 = jml_4.toDouble();
    if(jml4 < j_) {  double c;
         c =  jml2 + jml4 - j_;
         if( c<0 ) { QMessageBox::information(this,"Info...","Periksa Ketersediaan Dana...");  return;  }
    }
    qInfo() << "upate dds ================ 1111111111";
    update_data_dds(li_data_dds.at(0), tahap, tgl, jj_, terbilang, noSrt1, noSrt2);
    qInfo() << "upate dds ================ 22222222222222";
}


void Form::update_data_dds(QString id, QString tahap, QString date, QString jml, QString terbilang, QString noSrt1, QString noSrt2)
{
    qInfo() << "upate dds ================";
    QSqlQuery query;
    begin();
    QString cmd = "UPDATE pmk_yhk.dds_cair SET thp_cair = :thp_cair,  j_cair = :j_cair, tgl = :tgl , no_srt1 = :no_srt1 , no_srt2= :no_srt2 , j_terbilang= :terbil WHERE id_real = :id";
    query.prepare(cmd);
    query.bindValue(":id", id);
    query.bindValue(":thp_cair", tahap);
    query.bindValue(":j_cair",jml);
    query.bindValue(":tgl",date);
    query.bindValue(":no_srt1",noSrt1);
    query.bindValue(":no_srt2",noSrt2);
    query.bindValue(":terbil", terbilang+" rupiah");

    bool ok = exec(query);
    if(!ok){rollback(); QMessageBox::information( eb_v,"Error...!!!","Gagal Mengubah data realisasi Dana Desa, "+query.lastError().text()+""); return;}
    if(ok) { commit();
                QMessageBox::information(eb_v,"Info....","Berhasil Mengubah data realisasi Dana Desa");
                eb_v->close();
                memuatData_twRealisasidds(qbx_id_kam->currentText());
                }
}

#include "form.h"
#include "ui_form.h"
#include "mainwindow.h"
//#include "pdf_dds.h"
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
// =======================================================================================


// === Border ToolButton selected ==========
void Form::boderToolbar(int a)
{
    if(a==1){ui->toolButton->setStyleSheet("QToolButton#toolButton{ border: 2px solid #FFD700; }" );
                ui->toolButton_danaDesa->setStyleSheet("QToolButton#toolButton_danaDesa{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_add->setStyleSheet("QToolButton#toolButton_add{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_covid->setStyleSheet("QToolButton#toolButton_covid{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_blt->setStyleSheet("QToolButton#toolButton_blt{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_Sppd->setStyleSheet("QToolButton#toolButton_Sppd{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_user->setStyleSheet("QToolButton#toolButton_user{ border: 2px solid rgba(255,255,255,.5); }" );
    }

    if(a==2){ui->toolButton->setStyleSheet("QToolButton#toolButton{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_danaDesa->setStyleSheet("QToolButton#toolButton_danaDesa{ border: 2px solid #FFD700; }" );
                ui->toolButton_add->setStyleSheet("QToolButton#toolButton_add{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_covid->setStyleSheet("QToolButton#toolButton_covid{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_blt->setStyleSheet("QToolButton#toolButton_blt{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_Sppd->setStyleSheet("QToolButton#toolButton_Sppd{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_user->setStyleSheet("QToolButton#toolButton_user{ border: 2px solid rgba(255,255,255,.5); }" );
    }

    if(a==3){ui->toolButton->setStyleSheet("QToolButton#toolButton{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_danaDesa->setStyleSheet("QToolButton#toolButton_danaDesa{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_add->setStyleSheet("QToolButton#toolButton_add{ border: 2px solid #FFD700; }" );
                ui->toolButton_covid->setStyleSheet("QToolButton#toolButton_covid{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_blt->setStyleSheet("QToolButton#toolButton_blt{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_Sppd->setStyleSheet("QToolButton#toolButton_Sppd{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_user->setStyleSheet("QToolButton#toolButton_user{ border: 2px solid rgba(255,255,255,.5); }" );
    }

    if(a==4){ui->toolButton->setStyleSheet("QToolButton#toolButton{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_danaDesa->setStyleSheet("QToolButton#toolButton_danaDesa{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_add->setStyleSheet("QToolButton#toolButton_add{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_covid->setStyleSheet("QToolButton#toolButton_covid{ border: 2px solid #FFD700; }" );
                ui->toolButton_blt->setStyleSheet("QToolButton#toolButton_blt{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_Sppd->setStyleSheet("QToolButton#toolButton_Sppd{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_user->setStyleSheet("QToolButton#toolButton_user{ border: 2px solid rgba(255,255,255,.5); }" );
    }

    if(a==5){ui->toolButton->setStyleSheet("QToolButton#toolButton{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_danaDesa->setStyleSheet("QToolButton#toolButton_danaDesa{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_add->setStyleSheet("QToolButton#toolButton_add{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_covid->setStyleSheet("QToolButton#toolButton_covid{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_blt->setStyleSheet("QToolButton#toolButton_blt{ border: 2px solid #FFD700; }" );
                ui->toolButton_Sppd->setStyleSheet("QToolButton#toolButton_Sppd{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_user->setStyleSheet("QToolButton#toolButton_user{ border: 2px solid rgba(255,255,255,.5); }" );
    }

    if(a==6){ui->toolButton->setStyleSheet("QToolButton#toolButton{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_danaDesa->setStyleSheet("QToolButton#toolButton_danaDesa{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_add->setStyleSheet("QToolButton#toolButton_add{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_covid->setStyleSheet("QToolButton#toolButton_covid{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_blt->setStyleSheet("QToolButton#toolButton_blt{ border: 2px solid rgba(255,255,255,.5);}" );
                ui->toolButton_Sppd->setStyleSheet("QToolButton#toolButton_Sppd{ border: 2px solid #FFD700; }" );
                ui->toolButton_user->setStyleSheet("QToolButton#toolButton_user{ border: 2px solid rgba(255,255,255,.5); }" );
    }

    if(a==7){ui->toolButton->setStyleSheet("QToolButton#toolButton{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_danaDesa->setStyleSheet("QToolButton#toolButton_danaDesa{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_add->setStyleSheet("QToolButton#toolButton_add{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_covid->setStyleSheet("QToolButton#toolButton_covid{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_blt->setStyleSheet("QToolButton#toolButton_blt{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_Sppd->setStyleSheet("QToolButton#toolButton_Sppd{ border: 2px solid rgba(255,255,255,.5); }" );
                ui->toolButton_user->setStyleSheet("QToolButton#toolButton_user{ border: 2px solid #FFD700; }" );
    }
}
// === Border ToolButton selected ==========


QString Form::qcode()
{
    int noBrs = ui->tableWidget_cetak_dds->currentRow();
    QString a = ui->tableWidget_cetak_dds->item(noBrs,11)->text();
    QString b = ui->tableWidget_cetak_dds->item(noBrs,14)->text();
    QString b_ =" "+ b.right(4);
    QString c = " Distrik " + ui->tableWidget_cetak_dds->item(noBrs,3)->text();
    QString d = " Kampung " + ui->tableWidget_cetak_dds->item(noBrs,4)->text();
    QString e = "@" + ui->tableWidget_cetak_dds->item(noBrs,10)->text();
    QString f = "/" +ui->tableWidget_cetak_dds->item(noBrs,9)->text();
    QString g = "/" +ui->tableWidget_cetak_dds->item(noBrs,5)->text();
    QString h = "/" +ui->tableWidget_cetak_dds->item(noBrs,12)->text();
     return a+b_+c+d+e+f+g+h;
}

// refresh
void Form::on_toolButton_refResh_clicked()
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
                           memuatData_twRealisasidds(id);
                       }}

 if(menu=="3"){
     if(qbx_id_kam->currentText()!="")
                       { QString id = qbx_id_kam->currentText();
                           //muat_v_bam_2(id);
                   }}
 if(menu=="4"){ int qbxreal = ui->comboBox_realisasi->currentIndex();
                       if ( qbxreal <= 0){
                            QMessageBox::information(this,"Info ..." , "Pilih data Realisasi ...");
                       }
                       if ( qbxreal > 0){
                            qInfo() << " ini adalah menu refres 4 ";
                            refresh_data_sppd();
                        }
}
}

//logout Button
void Form::on_toolButton_logOut_clicked()
{
   this->close();
   MainWindow *a = new MainWindow;
   QIcon logo(":/gbr/html/gbr/yhk.png");
   a->setWindowIcon(logo);
   a->show();
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
       ui->toolButton_add->setVisible(false);
       ui->label_Add->setVisible(false);
       break;

   case AdminMode_2:
       qInfo()<<"Ini adalah Mode Admin Mode_2";
       ui->toolButton_danaDesa->setVisible(false);
       ui->label_danaDesa->setVisible(false);

       ui->label_Blt->setVisible(false);
       ui->label_Covid->setVisible(false);
       ui->toolButton_blt->setVisible(false);
       ui->toolButton_covid->setVisible(false);
       break;

   case UserMode_1:
       qInfo()<<"Ini adalah Mode User MODE 1";
//       ui->toolButton_user->setVisible(false);
//       ui->label_User->setVisible(false);
      ui->toolButton_add->setVisible(false);
      ui->label_Add->setVisible(false);
       break;

   case UserMode_2:
       qInfo()<<"Ini adalah Mode User MODE 2";
//        ui->toolButton_user->setVisible(false);
//        ui->label_User->setVisible(false);
       ui->toolButton_danaDesa->setVisible(false);
       ui->label_danaDesa->setVisible(false);
       ui->toolButton_blt->setVisible(false);
       ui->toolButton_covid->setVisible(false);
       ui->label_Blt->setVisible(false);
       ui->label_Covid->setVisible(false);
       break;
   }
}

void Form::sembunyi_subMenu()
{

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

    ui->comboBox_filter->setVisible(false);
    ui->label_filter->setVisible(false);
    ui->toolButton_filter->setVisible(false);

}

// === Menentukan Tampilan User Login ===
void Form::modeTampilan_usr()
{
    muat_lvl_type();
    if(lvl=="su" && type=="su")
    {
        updateTampilan(SuperMode);
    }
       else if( lvl=="admin" && type=="dds" )
                 {updateTampilan(AdminMode_1);}
       else if(lvl=="admin" && type=="add")
                {updateTampilan(AdminMode_2);}
       else if(lvl=="usr" && type=="dds")
                 {updateTampilan(UserMode_1);}
       else if(lvl=="usr" && type=="add")
                {updateTampilan(UserMode_2);}
}

void Form::conect_slot()
{




    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(qbx_id_dis_conn()));
    connect(ui->comboBox_nmKampung, SIGNAL (currentIndexChanged(int)), this, SLOT(qbx_id_kam_conn()));
    connect(ui->comboBox_realisasi, SIGNAL (currentIndexChanged(int)), this, SLOT(qbx3event())); // even qComboBox realisasi pada SPPD
    connect(ui->tableWidget_Bamuskam, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_bamuskam);
    connect(ui->tableWidget_Bamuskam, &QTableWidget::cellClicked , this, &Form::event_klik_tw);
    //connect(ui->tableWidget_realisasi_dds, &QTableWidget::cellClicked , this, &Form::event_klik_tw_6);
    //connect(ui->tableWidget_realisasi_add, &QTableWidget::cellClicked , this, &Form::event_klik_tw_11);
    connect(ui->tableWidget_daftarUser, &QTableWidget::cellClicked , this, &Form::even_klik_tw_user);
    //connect(ui->tableWidget_cetak_dds, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_cetak_dds);
    //connect(ui->tableWidget_cetak_add, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_cetak_add);
    connect(ui->tableWidget_cetak_sppd, &QTableWidget::doubleClicked , this, &Form::even_dklik_tw13);
    connect(this, SIGNAL( currentChanged(int)), this, SLOT(onTabChanged(int)));

}



void Form::tambah_realisasi(QStringList tahap,QString jml,QString noSrt1,QString noSrt2,QString btnText,QString nmKkap,QString nmBend,QString title)
{

    c =new QPushButton;
    c->setText("C");
    c->setMaximumWidth(22);

    qbx_thp_penc = new QComboBox;
    qbx_thp_penc->setEditable(true);
    qbx_thp_penc->addItems(tahap); //par 1

    QDate dt = QDate::currentDate();
    QDateEdit *de = new QDateEdit;
    de->setDate(dt);
    de->setDisplayFormat("dd-MM-yyyy");
    de_tgl_terima = new QDateEdit;
    de_tgl_terima->setDate(dt);
    de_tgl_terima->setDisplayFormat("dd-MM-yyyy");
    de_tgl_terima->setCalendarPopup(true);
    de_tgl_terima->setMinimumWidth(80);

    le_jml = new QLineEdit;
    le_jml->setValidator(new QDoubleValidator);

    le_jml->setClearButtonEnabled(true);
    le_jml->setText(jml); // par 2


    qbx_no_srt1 = new QComboBox;
    qbx_no_srt1->addItem(noSrt1); //par 3
    qbx_no_srt1->setEditable(true);
    qbx_no_srt1->setMinimumWidth(140);
    qbx_no_srt2 = new QComboBox;
    qbx_no_srt2->addItem(noSrt2); // 4
    qbx_no_srt2->setEditable(true);
    qbx_no_srt2->setMinimumWidth(140);
    qbx_persen= new QComboBox;


    btn1= new QPushButton;
    btn1->setText(btnText); // par 5
    btn1->setStyleSheet("QPushButton{font-weight: bold; }");
    btn1->setMaximumWidth(145);
    QLabel *ksg = new QLabel;
    ksg->setText("");
    ksg->setMaximumWidth(145);
    QLabel *thp= new QLabel; thp->setText("Tahap Pencairan :"); thp->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    thp->setMaximumWidth(145);
    thp->setMinimumWidth(145);
    QLabel *thp_l= new QLabel; thp_l->setText("Laporan Realisasi :"); thp_l->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    thp_l->setMaximumWidth(145);
    thp_l->setMinimumWidth(145);
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

    //Tambahan Nama Kepala Kampung dan Bendahara
    QLabel *nmKkam = new QLabel;nmKkam->setText("Kepala Kampung :"); nmKkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    nmKkam->setMaximumWidth(145);
    nmKkam->setMinimumWidth(145);
    QLabel *nmBkam = new QLabel;nmBkam->setText("Bendahara :"); nmBkam->setStyleSheet(" QLabel { font-weight: bold; qproperty-alignment: AlignRight; }");
    nmBkam->setMaximumWidth(145);
    nmBkam->setMinimumWidth(145);
    QLabel *nmKkam_ = new QLabel; nmKkam_->setStyleSheet(" QLabel { font-weight: bold;}");
    QLabel *nmBkam_ = new QLabel; nmBkam_->setStyleSheet(" QLabel { font-weight: bold;}");
    nmKkam_->setText(nmKkap); //par 6
    nmBkam_->setText(nmBend); //par 7

    eb_v = new QWidget;
    QIcon logo(":/gbr/html/gbr/yhk.png");
    eb_v->setWindowIcon(logo);
    eb_v->setWindowTitle(title); //par 8
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
}

void Form::click_btn1() // Even Klik Save di tambah semua realisasi
{
    btnAdd_dds();
    btnAdd_covid();
    btnAdd_blt();
    btnAdd_add();

on_toolButton_refResh_clicked();
}



void Form::muat_nama_header(QString nama)  // Muat Data hedaer  Realisasi Dana Desa
{
    qInfo() << id;
    li_realdds.clear();
    li_realdds.append("ID");
    li_realdds.append("Nama Distrik");
    li_realdds.append("Nama Kampung");
    li_realdds.append("Pagu Anggaran");
    QSqlQuery query;
    QString cmd = "SELECT thp_cair FROM pmk_yhk."+nama+"_cair GROUP BY thp_cair ORDER BY thp_cair ";
    query.prepare(cmd);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat data nama Header ."); return;}
    while(query.next())
    {
        li_realdds.append(query.value(0).toString());
    }
    li_realdds.append("Sub Total");
    li_realdds.append("Sisa Pagu");

    muatheadertw_realisasi_sppd();
    //muatrealdds_();
    //muatrealdds__();
}

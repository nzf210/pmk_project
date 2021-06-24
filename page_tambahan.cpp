#include "form.h"
#include "ui_form.h"
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
                           muat_v_bam(id);
                       }}

 if(menu=="3"){
     if(qbx_id_kam->currentText()!="")
                       { QString id = qbx_id_kam->currentText();
                           muat_v_bam_2(id);
                   }}
 if(menu=="4"){
                       if (ui->comboBox_realisasi->currentIndex()==0){muatrealdds(); loadsppd(); }
                       if (ui->comboBox_realisasi->currentIndex()==1){muatrealadd(); loadsppd_2();}
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
    connect(ui->comboBox_realisasi, SIGNAL (currentIndexChanged(int)), this, SLOT(qbx3event()));
    connect(ui->tableWidget_Bamuskam, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_bamuskam);
    connect(ui->tableWidget_Bamuskam, &QTableWidget::cellClicked , this, &Form::event_klik_tw);
    connect(ui->tableWidget_rAnggaran_dds, &QTableWidget::cellClicked , this, &Form::event_klik_tw_6);
    connect(ui->tableWidget_rAnggaran_add, &QTableWidget::cellClicked , this, &Form::event_klik_tw_11);
    connect(ui->tableWidget_daftarUser, &QTableWidget::cellClicked , this, &Form::even_klik_tw_user);
    connect(ui->tableWidget_cetak_dds, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_cetak_dds);
    connect(ui->tableWidget_cetak_add, &QTableWidget::doubleClicked , this, &Form::event_doubleklik_tw_cetak_add);
    connect(ui->tableWidget_cetak_sppd, &QTableWidget::doubleClicked , this, &Form::even_dklik_tw13);
    connect(this, SIGNAL( currentChanged(int)), this, SLOT(onTabChanged(int)));
}

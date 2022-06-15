#include "form.h"
#include "ui_form.h"
#include "QPdfWriter"
#include "pdf_dok.h"
#include "qrencode.h"

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


void Form::header_tw_daftarUser() // Header table widget Daftar User
{
    QStringList headerWidget;
    ui->tableWidget_daftarUser->setColumnCount(7);
    headerWidget <<" id " <<" Nama "<<" User Name "<<" Level User " << "Pass" << "Id_lvl" << "id_type";
    ui->tableWidget_daftarUser->setHorizontalHeaderLabels(headerWidget);
    ui->tableWidget_daftarUser->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_daftarUser->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_daftarUser->setColumnWidth(0,8);
    ui->tableWidget_daftarUser->setColumnHidden(0,true);
    ui->tableWidget_daftarUser->setColumnHidden(4,true);
    ui->tableWidget_daftarUser->setColumnHidden(5,true);
    ui->tableWidget_daftarUser->setColumnHidden(6,true);

    ui->tableWidget_daftarUser->setColumnWidth(1,120);
    ui->tableWidget_daftarUser->setColumnWidth(2,80);
    ui->tableWidget_daftarUser->setColumnWidth(3,40);
    ui->tableWidget_daftarUser->setColumnWidth(4,40);
    ui->tableWidget_daftarUser->setColumnWidth(5,40);
    ui->tableWidget_daftarUser->setColumnWidth(6,40);

    ui->pushButton_buat_usr->setEnabled(false);
    ui->lineEdit_id_usr->setVisible(false);
}


void Form::on_toolButton_user_clicked()
{
    boderToolbar(7);
    ui->stackedWidget->setCurrentIndex(6);
    header_tw_daftarUser();
    activitiUser();
    sembunyi_subMenu();

    menu="7";
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_nmKampung->setCurrentIndex(0);
    qInfo() << "Nilai Menu untuk ====" << menu;


    //Tambahan Sembunyi Line Edit Label
    ui->label->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->label_namaKampung->setVisible(false);
    ui->comboBox_nmKampung->setVisible(false);
    ui->toolButton_refResh->setVisible(false);
    ui->toolButton_logOut->setVisible(false);
    //Tambahan Sembunyi Line Edit Label


}

void Form::on_pushButton_buat_usr_clicked()
{
if(ui->lineEdit_id_usr->text() !=nullptr){QMessageBox::information(this, "Info", "Bersihkan terlebih dahulu"); return;}
buat_user();
}

void Form::on_pushButton_update_usr_clicked()
{
    if(ui->lineEdit_id_usr->text() ==nullptr){QMessageBox::information(this, "Info", "Pilih lebih dahulu User yang akan di update"); return;}
    update_user();
    activitiUser();
}

void Form::on_pushButton_hapus_usr_clicked()
{
    if(ui->lineEdit_id_usr->text() ==nullptr){QMessageBox::information(this,"Info", "Pilih lebih dahulu User yang akan di hapus"); return;}
    hapus_user();
}

void Form::on_pushButton_bersihkan_usr_clicked()
{
ui->pushButton_buat_usr->setEnabled(true);
activitiUser();

}

void Form::hapus_user()
{
   if(lvl=="usr"){QMessageBox::information(this,"Info...","Level user anda bukan Admin"); return;}
   QString id_b = ui->lineEdit_id_usr->text();
   QSqlQuery query;
   QString cmd = " DELETE FROM pmk_yhk.usr WHERE id= :id ";
    query.prepare(cmd);
    query.bindValue(":id",id_b);
    bool ok = exec(query);
    if(ok){QMessageBox::information(this,"Info","Berhasil Menghapus Data user .."); memuat_data_user_();}
    if(!ok){QMessageBox::information(this,"Info","Gagal Menghapus Data user .."); return;}
    ui->pushButton_buat_usr->setEnabled(false);
    activitiUser();
}


void Form::update_user()
{
    //if(lvl=="usr"){QMessageBox::information(this,"Info...","Level user anda bukan Admin"); return;}
    QString id_b = ui->lineEdit_id_usr->text();
    QString nama_b = ui->lineEdit_namaUser->text();
    if(nama_b==nullptr){QMessageBox::information(this,"Info...","Isi nama..."); return;}
    QString usrName_b = ui->lineEdit_userName->text();
    if(usrName_b==nullptr){QMessageBox::information(this,"Info...","Isi username..."); return;}
    QString pass_b = ui->lineEdit_password->text();
    //if(pass_b==nullptr){QMessageBox::information(this,"Info...","Isi password..."); return;}
    QString pass_b2 =ui->lineEdit_password_2->text();
    //if(pass_b2==nullptr){QMessageBox::information(this,"Info...","Isi password sekali lagi..."); return;}
    if(pass_b != pass_b2){QMessageBox::information(this,"Info...","Password tidak sama..."); return;}
   QByteArray hash = QCryptographicHash::hash(pass_b.toLocal8Bit(),QCryptographicHash::Sha256).toHex();
   QString pass_B = QString::fromLocal8Bit(hash);
   QString textCombo = ui->comboBox_level_user->currentText();

   QSqlQuery query;
   QString cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass, level = :level ";

    if(lvl=="usr"){cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass  WHERE id= :id_usr "; }

    if(type=="su"){ if(textCombo=="admin"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass   WHERE id= :id ";}
                            if(textCombo=="usr"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass WHERE id= :id " ;}
    }

    if(lvl=="admin" && type=="dds"){
        if(pass_b==nullptr){
            if(textCombo=="admin"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan ,  level= 'admin' ,id_lvl='2'  WHERE id= :id " ;}
            if(textCombo=="usr"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan ,  level= 'usr' ,id_lvl='3'  WHERE id= :id ";}
        }else{
                                                              if(textCombo=="admin"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass, level= 'admin' ,id_lvl='2'  WHERE id= :id " ;}
                                                              if(textCombo=="usr"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass, level= 'usr' ,id_lvl='3'  WHERE id= :id ";} }
                }

    if(lvl=="admin" && type=="add"){
        if(pass_b==nullptr){
            if(textCombo=="admin"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , level= 'admin' ,id_lvl='2'  WHERE id= :id ";}
            if(textCombo=="usr"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , level= 'usr' ,id_lvl='3'  WHERE id= :id ";}
                                        }else{
                                                                    if(textCombo=="admin"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass, level= 'admin' ,id_lvl='2'  WHERE id= :id ";}
                                                                    if(textCombo=="usr"){ cmd = " UPDATE pmk_yhk.usr SET nama =:nama , jabatan = :jabatan , pass = :pass, level= 'usr' ,id_lvl='3'  WHERE id= :id ";}
        }
    }

    query.prepare(cmd);
    query.bindValue(":jabatan", nama_b);
    query.bindValue(":nama",usrName_b);
    query.bindValue(":pass",pass_B);
    query.bindValue(":id",id_b);
    query.bindValue(":id_usr",id_usr);
    bool ok = exec(query);
    if(ok){QMessageBox::information(this,"Info","Berhasil Memperbaharui Data user .."); memuat_data_user_();}
    if(!ok){QMessageBox::information(this,"Info","Gagal Memperbaharui Data user .."); return;}
    ui->pushButton_buat_usr->setEnabled(false);
    activitiUser();
}


void Form::buat_user()
{
    if(lvl=="usr"){QMessageBox::information(this,"Info...","Level user anda bukan Admin"); return;}
    QString nama_b = ui->lineEdit_namaUser->text();
    if(nama_b==nullptr){QMessageBox::information(this,"Info...","Isi nama..."); return;}
    QString usrName_b = ui->lineEdit_userName->text();
    if(usrName_b==nullptr){QMessageBox::information(this,"Info...","Isi username..."); return;}
    QString pass_b = ui->lineEdit_password->text();
    if(pass_b==nullptr){QMessageBox::information(this,"Info...","Isi password..."); return;}
    QString pass_b2 =ui->lineEdit_password_2->text();
    if(pass_b2==nullptr){QMessageBox::information(this,"Info...","Isi password sekali lagi..."); return;}
    if(pass_b != pass_b2){QMessageBox::information(this,"Info...","Password tidak sama..."); return;}
   QByteArray hash = QCryptographicHash::hash(pass_b.toLocal8Bit(),QCryptographicHash::Sha256).toHex();
   QString pass_B = QString::fromLocal8Bit(hash);
   QString textCombo = ui->comboBox_level_user->currentText();
   QSqlQuery query;
   QString cmd = "SELECT id,nama,jabatan,pass,level,id_lvl,id_type FROM pmk_yhk.usr";

    if(type=="su"){ if(textCombo=="admin"){ cmd = "INSERT INTO pmk_yhk.usr (jabatan,nama,pass,level,type, id_lvl, id_type) VALUES (:jabatan, :nama, :pass, 'admin', 'dds', '2', '1') ";}
                            if(textCombo=="usr"){ cmd = "INSERT INTO pmk_yhk.usr (jabatan,nama,pass,level,type, id_lvl, id_type) VALUES (:jabatan, :nama, :pass, 'admin', 'add', '2', '2') ";}
    }

    if(lvl=="admin" && type=="dds"){ if(textCombo=="admin"){ cmd = "INSERT INTO pmk_yhk.usr (jabatan,nama,pass,level,type, id_lvl, id_type) VALUES (:jabatan, :nama, :pass, 'admin', 'dds', '2', '1') ";}
                                                              if(textCombo=="usr"){ cmd = "INSERT INTO pmk_yhk.usr (jabatan,nama,pass,level,type, id_lvl, id_type) VALUES (:jabatan, :nama, :pass, 'usr', 'dds', '3', '1') ";}}

    if(lvl=="admin" && type=="add"){ if(textCombo=="admin"){ cmd = "INSERT INTO pmk_yhk.usr (jabatan,nama,pass,level,type, id_lvl, id_type) VALUES (:jabatan, :nama, :pass, 'admin', 'add', '2', '2') ";}
                                                                if(textCombo=="usr"){ cmd = "INSERT INTO pmk_yhk.usr (jabatan,nama,pass,level,type, id_lvl, id_type) VALUES (:jabatan, :nama, :pass, 'usr', 'add', '3', '2') ";}}
    if(lvl=="usr"){QMessageBox::information(this,"Info...","Level user anda bukan Admin"); return;}

    query.prepare(cmd);
    query.bindValue(":jabatan", nama_b);
    query.bindValue(":nama",usrName_b);
    query.bindValue(":pass",pass_B);
    bool ok = exec(query);
    if(ok){QMessageBox::information(this,"Info","Berhasil Menambahkan Data user .."); memuat_data_user_();}
    if(!ok){QMessageBox::information(this,"Info","Gagal menambah Data user .."); return;}
    ui->pushButton_buat_usr->setEnabled(false);
    activitiUser();
}

void Form::memuat_data_user_()
{

    while(ui->tableWidget_daftarUser->rowCount()>0)// untuk Hilangkan Tambahan jika button di klik ulang
   {ui->tableWidget_daftarUser->removeRow(0);}

    QSqlQuery query;
    QString cmd = "SELECT id,nama,jabatan,pass,level,id_lvl,id_type FROM pmk_yhk.usr";

    if(lvl=="usr"){cmd = "SELECT id,nama,jabatan,pass,level,id_lvl,id_type FROM pmk_yhk.usr WHERE  id= :id ";}
    if(lvl=="admin" && type=="dds"){cmd = "SELECT id,nama,jabatan,pass,level,id_lvl,id_type FROM pmk_yhk.usr WHERE id_type=1";}

    if(lvl=="admin" && type=="add"){cmd = "SELECT id,nama,jabatan,pass,level,id_lvl,id_type FROM pmk_yhk.usr WHERE id_type=2";}

    query.prepare(cmd);
    query.bindValue(":id", id_usr);
    bool ok = exec(query);
    if(!ok){QMessageBox::information(this,"Error...","Gagal Memuat Data user .."); return;}
    int no=0;
    while(query.next())
    {
        ui->tableWidget_daftarUser->insertRow(no);
        QTableWidgetItem *id_ = new QTableWidgetItem;
        QTableWidgetItem *nama_ = new QTableWidgetItem;
        QTableWidgetItem *userName_ = new QTableWidgetItem;
        QTableWidgetItem *level_ = new QTableWidgetItem;
        QTableWidgetItem *pass_ = new QTableWidgetItem;
        QTableWidgetItem *id_lvl_ = new QTableWidgetItem;
        QTableWidgetItem *id_type_ = new QTableWidgetItem;

        id_->setText(query.value(0).toString());
        nama_->setText(query.value(2).toString());
        userName_->setText(query.value(1).toString());
        pass_->setText(query.value(3).toString());
        level_->setText(query.value(4).toString());
        id_lvl_->setText(query.value(5).toString());
        id_type_->setText(query.value(6).toString());

        id_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        nama_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        userName_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        level_->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

       ui->tableWidget_daftarUser->setItem(no,0,id_);
       ui->tableWidget_daftarUser->setItem(no,1,nama_);
       ui->tableWidget_daftarUser->setItem(no,2,userName_);
       ui->tableWidget_daftarUser->setItem(no,4,pass_);
       ui->tableWidget_daftarUser->setItem(no,3,level_);
       ui->tableWidget_daftarUser->setItem(no,5,id_lvl_);
       ui->tableWidget_daftarUser->setItem(no,6,id_type_);
        no++;
}
}


void Form::even_klik_tw_user()
{
    //qInfo()<<"Ini Even Klik tw User";
    int noBrs = ui->tableWidget_daftarUser->currentRow();
    QString id_u = ui->tableWidget_daftarUser->item(noBrs,0)->text();
    QString nama_u = ui->tableWidget_daftarUser->item(noBrs,1)->text();
    QString usrName_u = ui->tableWidget_daftarUser->item(noBrs,2)->text();
    QString level_u = ui->tableWidget_daftarUser->item(noBrs,3)->text();
    QString pass_u = ui->tableWidget_daftarUser->item(noBrs,4)->text();
    QString id_lvl_u =ui->tableWidget_daftarUser->item(noBrs,5)->text();
    QString id_type_u =ui->tableWidget_daftarUser->item(noBrs,6)->text();

    qInfo() << id_u <<" " << nama_u << " " << usrName_u << " " << level_u << " " << pass_u << " " << id_lvl_u << " " << id_type_u;
    ui->lineEdit_id_usr->setText(id_u);
    ui->lineEdit_namaUser->setText(nama_u);
    ui->lineEdit_userName->setText(usrName_u);
}


void Form::activitiUser()
{
    QStringList lvl_usr;
    lvl_usr << "admin" << "usr" ;
//ui->pushButton_buat_usr->setEnabled(true);
ui->lineEdit_namaUser->clear();
ui->lineEdit_password->clear();
ui->lineEdit_userName->clear();
ui->lineEdit_namaUser->setPlaceholderText("Nama lengkap");
ui->lineEdit_userName->setPlaceholderText("username");
ui->lineEdit_password->setPlaceholderText("masukan password");
ui->lineEdit_password_2->setPlaceholderText("masukan password ulang");
ui->lineEdit_password_2->clear();
ui->lineEdit_id_usr->clear();
ui->comboBox_level_user->clear();
ui->comboBox_level_user->addItems(lvl_usr);

memuat_data_user_();
}


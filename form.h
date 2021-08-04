#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QCompleter>
#include <QDateEdit>
#include <QList>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMoveEvent>
#include <QCursor>
#include <QTimer>
#include <QEvent>
#include <QLabel>
#include <QPainter>
#include <QVector>
#include <string>

#ifdef Q_OS_WINDOWS
#include <QLibrary>
#endif
#include "qrencode.h"
#include "mainwindow.h"

namespace Ui {
class Form;

}

class Form : public QWidget

{
    Q_OBJECT
public:

    explicit Form(QWidget *parent = nullptr);
    ~Form();
    QLocale indo = QLocale(QLocale::Indonesian,QLocale::Indonesia);
    bool open();
    void begin();
    void commit();
    void rollback();
    QWidget *eb_v;
    //bool eventFilter(QObject *object, QEvent *event);
    static bool GeneratePixmapFromText(QString &text, QPixmap &pixmap_,int width, int height);
    const QPixmap *QRCodePixmap;

    QString jumlahcair();
    enum Mode { SuperMode, AdminMode_1, AdminMode_2, UserMode_1, UserMode_2 };

//#Q_SLOT void setMyText(const QString &);

    #ifdef Q_OS_WINDOWS
        static void Initialize();
        static void Deinitialize();
    #endif

        QString getTot_sppd() const;
        void setTot_sppd(const QString &value);
        QStringList getLspdf1() const;
        void  rundatapdf1();
        QString getPdfdt1() const;
        QString id_update_sppd;

private slots:

 // ==== Qr_Code ====
    void on_saveFileButton_pressed(QString);
    void on_generateButton_pressed(QString text);
// ==== Qr_Code ====
    void muat_lvl_type(); // memuat data kepala dinas
    void on_toolButton_clicked(); // tool Button Bamuskam
    void on_toolButton_danaDesa_clicked(); // TB dana Desa
    //void on_comboBox_currentIndexChanged(int index); //
    void muat_dis();
    void muat_kampung();
    void muat_rek(QString &id_kam_s);
    void qbx_id_kam_conn(); // dengan SIGNAL SLOT Connect qbx ke ke even id_kampung
    void even_distrik_combo(QString &id_kam_s); // clear data qCombo Dis dan kamp
    void qbx_id_dis_conn(); // Connect qbx ke ke even id_distri
    //void on_toolButton_3_clicked();
    void on_toolButton_tmbRealdds_clicked();
    //void on_toolButton_5_clicked();
    void on_toolButton_cetakPdfdds_clicked();

    void sembunyi_subMenu();

    void modeTampilan_usr();

    void conect_slot();

      //private slots:

      void even_dklik_tw13();
      void event_doubleklik_tw_bamuskam();

      void event_copy_twRealisasi_dds(); // dds copy data realisasi ke excel
      void event_copy_twRealisasi_add();
      void event_copy_twRealisasi_blt();
      void event_copy_twRealisasi_covid();

      void event_klik_tw();
      void click_btn(); //Button di edit data bamuskam;
      void click_btn1(); //Tambah kegiatan ke tw widget
      void active_eb_v();
      void on_toolButton_refResh_clicked();
      void on_toolButton_add_clicked();
      void on_toolButton_tmbRealadd_clicked();

      void on_toolButton_cetakPdfadd_clicked();
      void muat_nama_header(QString);

      void on_toolButton_Sppd_clicked();
      void on_toolButton_logOut_clicked();
      void on_toolButton_pdf_main_clicked();
      void on_toolButton_tmbSppd_clicked();
      void on_toolButton_cetakpdfSppd_clicked();

      void sppdbtc();
      void sppdbtc_2();

      //void qbx3event(); // even qComboBox realisasi pada SPPD
      void updatesppd();
      void update_data_sppd(QString id,QString date, QString noSrt, QString perihal);
      void update_sppd_toDB();

      //Tambahan untuk Otomatisasi Jumlah Caor, Persen dan Tahap Laporan
      void muatTahap(QString); // memuat tahap pencairan

      void muat_data_realisasi_sppdd(QString); // memuat data realisasi untuk tw realisasi di sppd

      void eventQbxadd(); // semua

      void on_toolButton_filter_clicked();

      void on_toolButton_covid_clicked();

      void on_toolButton_blt_clicked();

      void on_toolButton_user_clicked();

      void on_pushButton_buat_usr_clicked();

      void on_pushButton_update_usr_clicked();

      void on_pushButton_hapus_usr_clicked();

      void on_pushButton_bersihkan_usr_clicked();

      void header_tw_daftarUser();

      void activitiUser();

      void memuat_data_user_();

      void even_klik_tw_user();

      void on_toolButton_tmbRealblt_clicked();

      void on_toolButton_cetakPdfblt_clicked();

      void on_toolButton_cetakPdfcovid_clicked();
      // ====== dana covid =======
      void on_toolButton_tmbRealCovid_clicked();



       // ====== dana covid =======
      void on_tableWidget_realisasi_covid_cellDoubleClicked(int row, int column);

     void tambah_real_add();

      void tambah_real_blt();

      void tambah_real_covid();


      void on_tableWidget_cetak_covid_cellDoubleClicked(int row, int column);

      void on_tableWidget_cetak_blt_cellDoubleClicked(int row, int column);

      void on_tableWidget_cetak_add_cellDoubleClicked(int row, int column);

      void on_tableWidget_cetak_dds_cellDoubleClicked(int row, int column);

      void on_tableWidget_realisasi_dds_cellClicked(int row, int column);

      void on_tableWidget_realisasi_covid_cellClicked(int row, int column);

      void on_tableWidget_realisasi_blt_cellClicked(int row, int column);

      //void on_tableWidget_realisasi_add_cellChanged(int row, int column);

      void on_tableWidget_realisasi_add_cellClicked(int row, int column);

      void on_comboBox_realisasi_currentIndexChanged(const QString &arg1);

      void on_tableWidget_realisasi_sppd_cellClicked(int row, int column);

      void on_tableWidget_realisasi_sppd_cellDoubleClicked(int row, int column);

signals:
     // void myLabelClicked();      // Signal to emit
      void sendData(QString data);

      //ini percobaan Slot signal


public slots:
      // void slotLabelClicked();    // Slot which will consume signal
    void onTabChanged(int tabIndex);



private:
    //ini percobaan Slot signal

    //=========== Data Page BLT ==========
    void memuatData_twRealisasiblt(QString);
    void header_twRealisasiblt();
    //=========== Data Page BLT ==========

    //=========== Data Page Covid =========
    void memuatData_twRealisasicovid(QString);
    void header_twRealisasicovid();

    //=========== Data Page Covid =========
    void memuatData_twRealisasiadd(QString);
    void header_twRealisasiadd();

    void boderToolbar(int);
    void updateTampilan(Mode mode);
    Mode ModeSekarang;
    // === Tambahan Fungsi Buat Getter Ke Cetak PDF1 ===S===
    QStringList lspdf1;
    QString pdfdt1;
    // === Tambahan Fungsi Buat Getter Ke Cetak PDF1 ===E===

    void select_tgl_sppd(QString tgl);

    QString lvl;
    QString type;
    QString id_usr;
    QString nama_l;
    //Tambahan untuk Otomatisasi Jumlah Caor, Persen dan Tahap Laporan
    QString persen_2;
    QString laporan;
    QString thp;
    QString tahap_;
    QString persen_2_2;
    QString laporan_2;
    QString thp_2;
    QString tahap__2;

    QString  pagu_pencairan; // nilai pagu dari database

    void  datapdf1(); // fungsi load data dari twidget buat cetak pdf
    void  datapdf2(); //
    //void  datapdf3();
    //void  databam(QString skam ); //write data bamuskam ke txt
    void  muatheadertw_realisasi_sppd();
    void  muatheadertw_cetak_sppd();
    void  muatrealdds_();
    void  muatrealdds__();

    void  muatrealadd_();
    void  muatrealadd__();

    //Tambah Filter
    QStringList li_lvl_type;
    QStringList li_dftBams;
    void muatListFilter();

    QString muatreal();
    QString qcode();
    void isiCode();
    QString data;
    QCursor cursor;

    QStringList li_realdds; // daftar nama header di sppd
    QStringList li_realadd;
    QStringList li_bam;

    QStringList Distrik;
    QStringList id_dis;
    QStringList Kampung;
    QStringList id_kam;

    QStringList li_norek;
    QStringList li_kas_kam;
    QStringList li_nm_rek;
    QStringList li_nm_bank;
    QStringList li_j_kk;

    QStringList li_tahap;
    QStringList li_tahap_add;
    QStringList li_tahap_select;
//    QStringList li_srt1;
//    QStringList li_srt2;

    QStringList li_srt4;
    QStringList li_persen;

    QStringList li_sk_bup;
    QStringList li_sk_kampung;
    QStringList li_menteri;
    QStringList li_kp_dns;

    QString s_j_bend;
    QString s_nm_bend;
    QString s_sk_bend;
    QString s_j_kp;
    QString s_nm_kp;
    QString s_sk_kp;

    bool exec(QSqlQuery &query);
    QComboBox *qbx_id_dis;
    QComboBox *qbx_id_kam;
    QComboBox *qbx_jml_kk;

    QComboBox *qbx_thp_penc;
    QComboBox *qbx_thp_l;
    QDateEdit *de_tgl_terima;
    QLineEdit *le_jml;
    QComboBox *qbx_no_srt1;
    QComboBox *qbx_no_srt2;
    QComboBox *qbx_persen;
    QComboBox *qbx_sk_pmk;
    QComboBox *qbx_sk_keu;
    QPushButton *btn1;
    QPushButton *c;

    void tahap(int id); // memuat tahap pencairan pada dana desa, covid, blt
    //void no_srt1();
    //void no_srt2();
    void btnAdd_dds();

    //Covid data
    QString qcode_covid();
    QStringList li_data_covid;
    void dataPdf_covid(int row);
    void btnAdd_covid();
    void header_realisasi_covid();
    void header_cetak_covid();
    void muat_data_realisasi_covid(QString);
    void even_dbKlik_tw_covid();
    void update_data_covid(QString id, QString tahap, QString date, QString jml, QString terbilang, QString noSrt1, QString noSrt2);
    void data_update_sementara_covid();

    void tambah_realisasi(QStringList tahap,QString jml,QString noSrt1,QString noSrt2,QString btnText,QString nmKkap,QString nmBen,QString title);
    void update_data_realisasi(QString date, QString tahap ,QString jml,QString noSrt1,QString noSrt2,QString nmKp, QString nmBk, QString title );

    //blt data
    QString qcode_blt();
    QStringList li_data_blt;
    void dataPdf_blt(int row);
    void btnAdd_blt();
    void header_realisasi_blt();
    void header_cetak_blt();
    void muat_data_realisasi_blt(QString);
    void even_dbKlik_tw_blt();
    void update_data_blt(QString id, QString tahap, QString date, QString jml, QString terbilang, QString noSrt1, QString noSrt2);
     void data_update_sementara_blt();

     //ad data
     QStringList li_data_add;
     void dataPdf_add(int row);
     void btnAdd_add();
     void header_realisasi_add();
     void header_cetak_add();
     void muat_data_realisasi_add(QString);
     void even_dbKlik_tw_add();
     void update_data_add(QString id, QString tahap, QString date, QString jml, QString terbilang, QString noSrt1, QString noSrt2);
     void data_update_sementara_add();

//data dds
     QStringList li_data_dds;
     void even_dbKlik_tw_dds();
     void data_update_sementara_dds();
     void update_data_dds(QString id, QString tahap, QString date, QString jml, QString terbilang, QString noSrt1, QString noSrt2);
     void memuatData_twRealisasidds( QString);


    //void tahap_add();
    //void no_srt4();
    //void persen();
    //void sk_bup();
    void sk_kampung();
    void sk_menteri();
    void kp_dns();

    void muat_bm(QString skam); // Memuat data nama Bamuskam untuk add dan tulis ke txt
    void muat_bamuskam(QString &id_kam_s);

    //void muat_v_bam_2( QString &id_kamp_s);
    void muat_bend_kp( QString &id_kamp_s);
    void muat_k_kp(QString &id_kam_s);
    void muat_real(QString &id_kam_s);
    //void muat_real_2(QString &id_kam_s);
    void header_wt1();
    void header_realisasi_dds();
    //void header_wt11();

    void header_wt2();
    //void header_wt9();
    void bilang(QString nilai);
    void bil(QString nilai);
    QPushButton *btn;
    QString id; //Bamuskam
    QDateEdit *de; //Bamuskam
    QLineEdit *Le_nm;//Bamuskam
    QLineEdit *Le_jbtn;//Bamuskam
    QLineEdit *Le_no_sk;//Bamuskam
    QString menu;
    QString menu2;
    QString terbilang;
    void act();
    //void act_2();

    void buat_user();
    void update_user();
    void hapus_user();
//================= sppd ================
void instsppd(QString nosrt, QString hal,QString date);
void insert_sppd(QString type ,QString nosrt, QString hal,QString date);

void muat_nosurat();
//void muat_sppd_2();

//void muat_sppd_();
//void muat_sppd_2_();

void loadsppd();
void loadsppd_2();

void tambah_sppd(int noSrt1,int noSrt);
void sppd_2();
void headsppd();

QDateEdit *desppd;
QComboBox *qbnosppd;
QComboBox *qbhal;
QWidget *wg;
QPushButton *btnsppd;

QDateEdit *desppd_2;
QComboBox *qbnosppd_2;
QComboBox *qbhal_2;
QWidget *wg_2;
QPushButton *btnsppd_2;

QPushButton *btnsppd_dk;

QStringList li_no_surat;
QStringList li_sppd_2;

QStringList li_sppd_;
QStringList li_sppd_2_;

QString nosurat();
QString nosurat_2();
QString nosurat_3();

QDataStream ds_sppd;
QString ds_sppd_;
QString tot_sppd;
QString tot_sppd_2;

void save_sppd();
//QString j1_; QString j2_; QString j3_; QString j4_;

//================= sppd ================
    Ui::Form *ui;

    #ifdef Q_OS_WINDOWS
        static QLibrary *m_qrencodelib;
    #endif


protected:
 void keyvent(QEvent * e);
 void klik(QKeyEvent *k);
 //static QLibrary *m_qrencodelib;
 bool saveDocument(QString filePath);
 QString html1;



};
#endif // FORM_H



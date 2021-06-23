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
//#include "mainwindow.h"

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
    bool eventFilter(QObject *object, QEvent *event);
    static bool GeneratePixmapFromText(QString &text, QPixmap &pixmap,
                                       int width, int height);

    QString jumlahcair();
    enum Mode { SuperMode, AdminMode_1, AdminMode_2, UserMode_1, UserMode_2 };

    #ifdef Q_OS_WINDOWS
        static void Initialize();
        static void Deinitialize();
    #endif

        QString getTot_sppd() const;
        void setTot_sppd(const QString &value);
        QStringList getLspdf1() const;
        void  rundatapdf1();
        QString getPdfdt1() const;


private slots:
// ==== Qr_Code ====
    void on_saveFileButton_pressed(QString);
    void on_generateButton_pressed(QString text);
// ==== Qr_Code ====
    void muat_lvl_type();
    void on_toolButton_clicked();
    void on_toolButton_danaDesa_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void muat_dis();
    void muat_kampung();
    void muat_rek(QString &id_kam_s);
    void qbx_id_kam_conn();
    void even_distrik_combo(QString &id_kam_s);
    void qbx_id_dis_conn();
    //void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_6_clicked();

      //private slots:
      void even_klik_tw13();
      void even_dklik_tw13();
      void headsppd_2(); // Double Klik tw13
      void event_doubleklik_tw_2();
      void event_doubleklik_tw_9();
      void event_doubleklik_tw();
      void event_klik_tw_6();
      void event_klik_tw_11();
      void event_klik_tw_13();
      void event_klik_tw();
      void click_btn(); //Button di edit data bamuskam;
      void click_btn1(); //Tambah kegiatan ke tw widget
      void click_btn2();  //Tambah kegiatan ke tw widget add
      void active_eb_v();
      void active_eb_v_2();
     // void on_pushButton_clicked();
      void on_toolButton_7_clicked();
      void on_toolButton_8_clicked();
      void on_toolButton_10_clicked();

      void on_toolButton_12_clicked();
      void muatrealdds();
      void muatrealadd();

      void on_toolButton_13_clicked();
      void on_toolButton_15_clicked();
      void on_toolButton_14_clicked();
      void on_toolButton_16_clicked();
      void on_toolButton_17_clicked();

      void sppdbtc();
      void sppdbtc_2();
      void qbx3event();
      void updatesppd();

      //Tambahan untuk Otomatisasi Jumlah Caor, Persen dan Tahap Laporan
      void muatTahap(QString);
      void eventQbxadd();
      void muatTahap_2(QString);
      void eventQbxadd_2();
      void on_toolButton_18_clicked();

signals:
     // void myLabelClicked();      // Signal to emit
      void sendData(QString data);

public slots:
      // void slotLabelClicked();    // Slot which will consume signal
    void onTabChanged(int tabIndex);

private:
    void updateTampilan(Mode mode);
    Mode ModeSekarang;
    // === Tambahan Fungsi Buat Getter Ke Cetak PDF1 ===S===
    QStringList lspdf1;
    QString pdfdt1;
    // === Tambahan Fungsi Buat Getter Ke Cetak PDF1 ===E===

    QString lvl;
    QString type;
    //Tambahan untuk Otomatisasi Jumlah Caor, Persen dan Tahap Laporan
    QString persen_2;
    QString laporan;
    QString thp;
    QString tahap_;
    QString persen_2_2;
    QString laporan_2;
    QString thp_2;
    QString tahap__2;

    void  datapdf1();
    void  datapdf2();
    void  datapdf3();
    void  databam(QString skam );
    void  muatheadertw12();
    void  muatheadertw12_2();
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

    QStringList li_realdds;
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
    QStringList li_srt1;
    QStringList li_srt2;

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

    void tahap();
    void no_srt1();
    void no_srt2();
    void tahap_add();
    void no_srt4();
    void persen();
    void sk_bup();
    void sk_kampung();
    void sk_menteri();
    void kp_dns();

    void muat_bm(QString skam);
    void muat_bamuskam(QString &id_kam_s);
    void muat_v_bam( QString &id_kamp_s);
    void muat_v_bam_2( QString &id_kamp_s);
    void muat_bend_kp( QString &id_kamp_s);
    void muat_k_kp(QString &id_kam_s);
    void muat_real(QString &id_kam_s);
    void muat_real_2(QString &id_kam_s);
    void header_wt1();
    void header_wt6();
    void header_wt11();

    void header_wt2();
    void header_wt9();
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
    void act_2();

//================= sppd ================
void instsppd(QString nosrt, QString hal,QString date);
void instsppd_2(QString nosrt, QString hal,QString date);

void muat_sppd();
void muat_sppd_2();

void muat_sppd_();
void muat_sppd_2_();

void loadsppd();
void loadsppd_2();

void sppd();
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

QStringList li_sppd;
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
QString j1_; QString j2_; QString j3_; QString j4_;

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

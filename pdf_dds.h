#ifndef PDF_DDS_H
#define PDF_DDS_H

#include <QtWebEngineWidgets>
#include <QThread>
#include "form.h"

class Widget1 : public QWidget
{
public slots:

public:
        QStringList list;
        QTimer* timer;
        QMessageBox *msgBox;
        explicit Widget1(QWidget *parent = nullptr):
        QWidget(parent),

        button2(new QPushButton),
        button(new QPushButton),
        progressbar(new QProgressBar),
        progressbar2(new QProgressBar),
        view(new QWebEngineView),
        view2(new QWebEngineView)


  {

        button->setText(tr("generate pdf dds"));
        button->setEnabled(false);

        connect(button, &QPushButton::clicked, this, &Widget1::onClicked);
        connect(view, &QWebEngineView::loadFinished, this, &Widget1::onLoadFinished);
        connect(view->page(), &QWebEnginePage::pdfPrintingFinished, this, &Widget1::onPdfPrintingFinished);

        //connect(button, &QPushButton::clicked, this, &Widget::onClicked2);
        connect(view2, &QWebEngineView::loadFinished, this, &Widget1::onLoadFinished2);
        connect(view2->page(), &QWebEnginePage::pdfPrintingFinished, this, &Widget1::onPdfPrintingFinished2);
        //connect(progressbar, &QProgressBar::valueChanged, this, &Widget1::openFile);

//!=================================================================================
       loadlspdf1();
       loadpdf1();
//!=================================================================================

        QString path2("doc/temp/");
        QString path("doc/html/");

        QDir dir(path);
        QFile  htmlFile (path+ "dana_desa_srt_1.html");

        if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           qInfo() << "Html File tdk terbuka"; return;
        }
        QString htmlContent;
        htmlContent = htmlFile.readAll();

       htmlContent.replace("#DSTR#", nmdis );
       htmlContent.replace("#KMP#", nmkamp );
       htmlContent.replace("#TERBILANG#", terbilang);
       htmlContent.replace("#NOREK#", norek);
       htmlContent.replace("#NMREK#", nmrek);
       htmlContent.replace("#NMBANK#", nmbank);
       htmlContent.replace("#NKKP#", nmkkp);
       htmlContent.replace("#NBENK#", nmbenk);
       htmlContent.replace("#JKK#", jkk);
       htmlContent.replace("#tahap#", tahap);
       htmlContent.replace("#JML#", jml);
       htmlContent.replace("#%#" , persen);
       htmlContent.replace("#KPDNS#", nmkpdns);
       htmlContent.replace("#tahap2#", tahap2);
       htmlContent.replace("#PKPLD#", pkpldns);
       //QString nmkdns;
       htmlContent.replace("#NIPKDNS#", nip);
       htmlContent.replace("****", nosrt1);
       htmlContent.replace("####", nosrt2);
       QDate d = QDate::fromString(tgl,"dd-MM-yyyy");
       QString tgl_ = QLocale{QLocale::Indonesian}.toString(d, "dd MMMM yyyy");
       htmlContent.replace("#TGL#", tgl_);
       htmlContent.replace("#thn#", thn);
       htmlContent.replace("#thn2#",thn2);
       htmlContent.replace("#UDSTR#", unmdis);
       htmlContent.replace("#UKMP#", unmkam );
       htmlContent.replace("#SKKP#", skkam);
       htmlContent.replace("#sk#", skkam);
       htmlContent.replace("#skben#", skben );
       htmlContent.replace("#JBTKPDNS#", jbt_klp_dns);

        QString html = htmlContent;
        htmlFile.close();

        QString l= html;
        QFile fOut(path2+"dds.html");
        fOut.open(QFile::WriteOnly | QFile::Text);
        if(fOut.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"tdk Bisa Buka file";} else {
            QTextStream stream(&fOut);
            stream << l;
            fOut.flush();
            fOut.close();
           // QMessageBox::information(this,tr("Info"),tr("dds tersimpan"));
         }

        QFile  htmlFile2 (path+ "dana_desa_srt_2.html");
        if (!htmlFile2.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           qInfo() << "Html File 2 tdk terbuka"; return;
        }
         QString htmlContent2;
         htmlContent2 = htmlFile2.readAll();
         htmlFile2.close();

        htmlContent2.replace("#DSTR#", nmdis );
        htmlContent2.replace("#KMP#", nmkamp );
        htmlContent2.replace("#TERBILANG#", terbilang);
        htmlContent2.replace("#NOREK#", norek);
        htmlContent2.replace("#NMREK#", nmrek);
        htmlContent2.replace("#NMBANK#", nmbank);
        htmlContent2.replace("#NKKP#", nmkkp);
        htmlContent2.replace("#NBENK#", nmbenk);
        htmlContent2.replace("#JKK#", jkk);
        htmlContent2.replace("#tahap#", tahap);
        htmlContent2.replace("#JML#", jml);
        htmlContent2.replace("#%#" , persen);
        htmlContent2.replace("#KPDNS#", nmkpdns);
        htmlContent2.replace("#tahap2#", tahap2);
        htmlContent2.replace("#PKPLD#", pkpldns);
        htmlContent2.replace("#NIPKDNS#", nip);
        htmlContent2.replace("****", nosrt1);
        htmlContent2.replace("####", nosrt2);
        htmlContent2.replace("#JBTKPDNS#", jbt_klp_dns);

        htmlContent2.replace("#TGL#", tgl_);
        htmlContent2.replace("#thn#", thn);
        htmlContent2.replace("#thn2#",thn2);
        htmlContent2.replace("#UDSTR#", unmdis);
        htmlContent2.replace("#UKMP#", unmkam );
        htmlContent2.replace("#SKKP#", skkam);
        htmlContent2.replace("#sk#", skkam);
        htmlContent2.replace("#skben#", skben );

        QString html2 = htmlContent2;
         htmlFile2.close();
         fOut.close();

         QString l2= html2;
         QFile fOut2(path2+"dds2.html");
         fOut2.open(QFile::WriteOnly | QFile::Text);
         if(fOut2.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"tdk Bisa Buka file";} else {
             QTextStream stream2(&fOut2);
             stream2 << l2;
             fOut2.flush();
             fOut2.close();
            // QMessageBox::information(this,tr("Info"),tr("dds 2 tersimpan"));
          }
         fOut2.close();
         htmlFile2.close();

        QUrl url = QUrl::fromLocalFile("/doc/temp/dds.html");
        view->setHtml(html);
        view->load(url);

        QUrl url2 = QUrl::fromLocalFile("/doc/temp/dds2.html");
        view2->setHtml(html2);
        view2->load(url2);

        auto lay = new QVBoxLayout(this);
        lay->addWidget(button);
        //lay->addWidget(progressbar);
        //lay->addWidget(view);
        //lay->addWidget(view2);
        //resize(630, 780);
        resize(330, 280);
        //run();
    }

// QStringList pdfForm()
// {
//     Form *f = new Form;
//     f->rundatapdf1();
//     return  f->getLspdf1();
// }

//private:
bool ok_()
{
    qInfo() << "Run bool";
    msleep(4300);
     qInfo() << "Run bool 222";
    return true;
}



    void onLoadFinished(bool ok)
    {
        button->setEnabled(ok); run();
     }

    void onLoadFinished2(bool ok)
    {
        button2->setEnabled(ok);
     }

    void onClicked()
    {

        msgBox = new QMessageBox;
        msgBox->setWindowTitle("Info ... ");
        QIcon logo(":/gbr/html/gbr/yhk.png");
        msgBox->setWindowIcon(logo);
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText("Open File pdf DDS ... ");


        progressbar->setRange(0, 0);
        QString path("laporan/dds/");
        QDir dir(path);

        //QString Nama Auto
        QString nmpdf = "Reg "+tahap +" "+nmkamp+" Dis "+nmdis;
        QFile  pdfFile ( path + nmpdf+ " D.pdf");
        fn = path +  nmpdf +" D.pdf";

        QFile  pdfFile2 (path+nmpdf+" K.pdf");
        fn2 = path +nmpdf+" K.pdf";

        if(pdfFile.exists()){ pdfFile.remove(); }
        if(pdfFile.exists()) {QMessageBox::information(this,"Info...","Tutup File pdf yg terbuka dan Generate Kembali"); qInfo()<<"Silahkan tutup pdf file"; return;}
        if(pdfFile2.exists()){ pdfFile2.remove(); }
        if(pdfFile2.exists()) {QMessageBox::information(this,"Info...","Tutup File pdf yg terbuka dan Generate Kembali"); qInfo()<<"Silahkan tutup pdf file"; return;}
        view2->page()->printToPdf(fn2,QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMargins(0,0,0,0)));
        view->page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize(QSize(780,1154))), QPageLayout::Landscape, QMargins(50,0,15,15)) ); // fix old

        //view->page()->pdfPrintingFinished(fn, QMessageBox::information(this,"Info...","Menyiapkan file, <b>Tekan OK setelah loading selesai</b>")); // old fix
        //QEventLoop loop;
///         connect(ui->tableWidget_cetak_sppd, &QTableWidget::doubleClicked , this, &Form::even_dklik_tw13);
///         connect(this, SIGNAL( currentChanged(int)), this, SLOT(onTabChanged(int)));
        ///QObject::connect(progressbar, SIGNAL(valueChanged(int)), this, &Widget1::openFile);
       // bool ok = loop.exec();
        //sleep(3);
        qInfo() << "runnnn runnnnnnnnnnn exec";
        //sleep(20);
        msgBox->exec();
        view->page()->pdfPrintingFinished(fn,ok_());
        //sleep(2);
        view2->page()->pdfPrintingFinished(fn2, true);
        //sleep(20);
        qInfo() << "runnnn runnnnnnnnnnn exec sebelum slepp";
        //while (chech_finish()) { continue;}

        view->close();
        view2->close();
        this->close();
        //sleep(2);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fn));  msleep(500);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fn2));

    }


    void run() {
            qInfo() << "runnnn runnnnnnnnnnn";
            timer = new QTimer(this);
            timer->setInterval(3500);
            //timer->connect(timer, SIGNAL(timeout()), this, SLOT(klik_buton()));
            timer->connect(timer, &QTimer::timeout, this, &Widget1::klik_buton);
            timer->start();
            qInfo() << "runnnn runnnnnnnnnnn start";
        }

    void loadlspdf1()
    {
        QString pt("doc/temp/");
        QFile  dtpdf1 (pt+ "dtpdf1.txt");
       if (!dtpdf1.open(QIODevice::ReadOnly | QIODevice::Text))
       {
          qInfo() << "dtpdf1.txt File tdk terbuka";
          return;
       }

       QTextStream stream(&dtpdf1);
       while (!stream.atEnd()) {
           QString line = stream.readAll();
            list = line.split("/n");
            list << "aaa";
           }
    }

    void loadpdf1()
    {
        nmdis = list.at(0);
        nmkamp = list.at(1);

        terbilang= toCamelCase(list.at(2).toCaseFolded());
        norek = list.at(3);
        nmrek = list.at(4);
        nmbank = list.at(5);;
        nmkkp = list.at(6);;
        nmbenk = list.at(7);;
        jkk = list.at(8);;
        tahap = list.at(9);
        jml = list.at(10);
        persen = list.at(11);
        nmkpdns = list.at(12);
        tahap2 = list.at(13);
        pkpldns = list.at(14);
        //QString nmkdns;
        nip = list.at(15);
        nosrt1 = list.at(16);
        nosrt2 = list.at(17);
        tgl = list.at(18);
        thn=tgl.right(4);
        unmdis = list.at(19);
        unmkam =list.at(20);

        skben=list.at(21);
        skkam=list.at(22);
        thn2=list.at(23);
        jbt_klp_dns = list.at(24);
    }

    QString toCamelCase(const QString& s)
    {
        QStringList parts = s.split(' ', QString::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
            parts[i].replace(0, 1, parts[i][0].toUpper());
        return parts.join(" ");
    }


    void onPdfPrintingFinished(const QString & filename, bool ok)
    {
        qDebug() <<"pdf Finish"<< filename << ok;
        progressbar->setRange(0, 100);
    }

    void onPdfPrintingFinished2(const QString & filename, bool ok)
    {
        qDebug() <<"pdf Finish 2"<< filename << ok;
        progressbar2->setRange(0, 100);
    }

     static void usleep(unsigned long usecs){QThread::usleep(usecs);}
     static void msleep(unsigned long msecs){QThread::msleep(msecs);}
     static void sleep(unsigned long secs){QThread::sleep(secs);}

public slots:

 void openFile()
     {
        qInfo() << "Jalan Kah tdk ini" ;
         view->page()->pdfPrintingFinished(fn, true);
         view2->page()->pdfPrintingFinished(fn2, true);
         //sleep(5);
         view->close();
         view2->close();
         this->close();
         QDesktopServices::openUrl(QUrl::fromLocalFile(fn));  //msleep(1);
         QDesktopServices::openUrl(QUrl::fromLocalFile(fn2));
     }

void klik_buton()
{
    qInfo() << "dtpdf1.txt File tdk terbuka runnnnnnnnnnn button clik";
    //button = new QPushButton;
    //button->click();
    timer->stop();
    onClicked();
}

   //  bool ok_();

      QString fn;
       QString fn2;
     //===================================================
    QString jbt_klp_dns;
     QString nmdis ;
     QString nmkamp ;
     QString terbilang;
     QString norek ;
     QString nmrek ;
     QString nmbank ;
     QString nmkkp ;
     QString nmbenk;
     QString jkk ;
     QString tahap ;
     QString jml ;
     QString persen ;
     QString nmkpdns ;
     QString tahap2 ;
     QString pkpldns ;
     //QString nmkdns;
     QString nip ;
     QString nosrt1 ;
     QString nosrt2 ;
     QString tgl ;
     QString thn ;

     QString skben;
     QString skkam;

     QString unmdis ;
     QString unmkam ;

     QString thn2;

     //===================================================

    QPushButton *button2;
    QPushButton *button;
    QProgressBar *progressbar;
    QProgressBar *progressbar2;
    QWebEngineView *view;
    QWebEngineView *view2;

};


#endif // PDF_DDS_H



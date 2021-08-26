#ifndef PDF_SPPD_ADD_H
#define PDF_SPPD_ADD_H

#include <QtWebEngineWidgets>
#include <QThread>
#include "form.h"

class Widget3 : public QWidget
{
public:

    QTimer* timer;
    QMessageBox *msgBox;
    QStringList list; QStringList li_bm;
    explicit Widget3(QWidget *parent = nullptr):
        QWidget(parent),  button2(new QPushButton),button(new QPushButton),progressbar(new QProgressBar),  progressbar2(new QProgressBar), view(new QWebEngineView), view2(new QWebEngineView)
    {

        button->setText(tr("generate pdf sppd dds"));
        button->setEnabled(false);

        connect(button, &QPushButton::clicked, this, &Widget3::onClicked);
        connect(view, &QWebEngineView::loadFinished, this, &Widget3::onLoadFinished);
        connect(view->page(), &QWebEnginePage::pdfPrintingFinished, this, &Widget3::onPdfPrintingFinished);

        //connect(button, &QPushButton::clicked, this, &Widget::onClicked2);
        connect(view2, &QWebEngineView::loadFinished, this, &Widget3::onLoadFinished2);
        connect(view2->page(), &QWebEnginePage::pdfPrintingFinished, this, &Widget3::onPdfPrintingFinished2);

//!=================================================================================
       loadlspdf1();
       loadpdf1();
//!=================================================================================

//        Form *fr = new Form;
//        qInfo() <<"Jumlah Cair dari form" <<  fr->getTot_sppd();

        QString path2("doc/temp/");
        QString path("doc/html/");

        QDir dir(path);
        QFile  htmlFile (path+ "SPPD.html");

        if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           qInfo() << "Html File SPPD tdk terbuka"; return;
        }
        QString htmlContent;
        htmlContent = htmlFile.readAll();

        htmlContent.replace("****",nosrt);
        htmlContent.replace("#dddd#",htm);
        htmlContent.replace("#total#", tot);
        //htmlContent.replace("#tgl#", tgl);
        htmlContent.replace("#tgl_#", tgl_);
        htmlContent.replace("#tgl#", tgl);
        htmlContent.replace("#thn#", thn);
        htmlContent.replace("#KPDNS#", kpdns);
        htmlContent.replace("#PKPLD#", jkpdns);
        htmlContent.replace("#NIPKDNS#", nipkpdns);
        htmlContent.replace("#hal#", hal);
        htmlContent.replace("#JBTKPDNS#",jbt_klp_dns);

        QString html = htmlContent;
        htmlFile.close();

        QString l= html;
        QFile fOut(path2+"sppd.html");
        fOut.open(QFile::WriteOnly | QFile::Text);
        if(fOut.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"tdk Bisa Buka file sppd.html";} else {
            QTextStream stream(&fOut);
            stream << l;
            fOut.flush();
            fOut.close();
         }

        QFile  htmlFile2 (path+ "SPPD_Lampiran.html");
        if (!htmlFile2.open(QIODevice::ReadOnly | QIODevice::Text))
        {
           qInfo() << "Html File SPPD L tdk terbuka"; return;
        }
         QString htmlContent2;
         htmlContent2 = htmlFile2.readAll();
         htmlFile2.close();

         htmlContent2.replace("****",nosrt);
         htmlContent2.replace("#dddd#",htm);
         htmlContent2.replace("#total#", tot);
         //htmlContent2.replace("#tgl#", tgl);
         htmlContent2.replace("#tgl_#", tgl_);
         htmlContent2.replace("#tgl#", tgl);
         htmlContent2.replace("#thn#", thn);
         htmlContent2.replace("#KPDNS#", kpdns);
         htmlContent2.replace("#PKPLD#", jkpdns);
         htmlContent2.replace("#NIPKDNS#", nipkpdns);
         htmlContent2.replace("#terbilang#", toCamelCase(terbilang));
        htmlContent2.replace("#hal#", hal);
        htmlContent2.replace("#JBTKPDNS#",jbt_klp_dns);
        htmlContent2.replace("#SPASI#",spasi_br);
         qInfo() << "Terbilang pdf 3" << terbilang;

        QString html2 = htmlContent2;
         htmlFile2.close();
         fOut.close();

         QString l2= html2;
         QFile fOut2(path2+"sppdl.html");
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



        QUrl url = QUrl::fromLocalFile("/doc/temp/sppd.html");
        view->setHtml(html);
        view->load(url);

        QUrl url2 = QUrl::fromLocalFile("/doc/temp/sppdl.html");
        view2->setHtml(html2);
        view2->load(url2);

        auto lay = new QVBoxLayout(this);
        lay->addWidget(button);
        //lay->addWidget(progressbar);
        //lay->addWidget(view);
        //lay->addWidget(view2);
        //resize(630, 780);
       //resize(330, 280);
        //run();
    }

//private:
    bool ok_()
    {
        qInfo() << "Run bool";
        sleep(4);
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
        msgBox->setText("Open File pdf Surat Pengantar ... ");
        //progressbar->setRange(0, 0);
        QString path("laporan/sppd/");
        QDir dir(path);

        QFile  pdfFile (path+ tgl_+ "SPPD.pdf");
        QString fn = path+ tgl_+"SPPD.pdf";

        QFile  pdfFile2 (path+ tgl_+ "SPPD_L.pdf");
        QString fn2 = path+ tgl_+"SPPD_L.pdf";

        if(pdfFile.exists()){ pdfFile.remove(); }
        if(pdfFile.exists()) {QMessageBox::information(this,"Info...","Tutup File pdf yg terbuka"); qInfo()<<"Silahkan tutup pdf file"; return;}
        if(pdfFile2.exists()){ pdfFile2.remove(); }
        if(pdfFile2.exists()) {QMessageBox::information(this,"Info...","Tutup File pdf yg terbuka"); qInfo()<<"Silahkan tutup pdf file"; return;}

       // view->page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize(QSize(743,987))), QPageLayout::Portrait, QMargins(55,25,15,25)) );
       // view2->page()->printToPdf(fn2,QPageLayout(QPageSize(QPageSize(QSize(780,1154))), QPageLayout::Landscape, QMargins(40,15,15,20)) );

        view->page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMargins(0,0,0,0))); // fix sppd
        view2->page()->printToPdf(fn2,QPageLayout(QPageSize(QPageSize(QSize(780,1154))), QPageLayout::Landscape, QMargins(50,30,15,30))); // fix sppd

        //sleep(2);//view->page()->pdfPrintingFinished(fn, QMessageBox::information(this,"Info...","<b>Generating pdf file...</b>"));
        msgBox->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        qInfo() << " === msgBox === " <<msgBox->exec();
        view->page()->pdfPrintingFinished(fn,ok_());

        view2->page()->pdfPrintingFinished(fn2, true);
       // if(ok==false){QMessageBox::information(this,"Info...","Generating pdf file Gagal, Coba Kembali"); return; }

        QDesktopServices::openUrl(QUrl::fromLocalFile(fn));  msleep(1);   QDesktopServices::openUrl(QUrl::fromLocalFile(fn2));
        view->close();
        view2->close();
        this->close();
    }

    void run() {
            qInfo() << "runnnn runnnnnnnnnnn";
            timer = new QTimer(this);
            timer->setInterval(50);
            //timer->connect(timer, SIGNAL(timeout()), this, SLOT(klik_buton()));
            timer->connect(timer, &QTimer::timeout, this, &Widget3::klik_buton);
            timer->start();
        }
    void klik_buton()
    {
        qInfo() << "dtpdf1.txt File tdk terbuka runnnnnnnnnnn button clik";
        //button = new QPushButton;
        //button->click();
        timer->stop();
        onClicked();
    }

    void loadlspdf1()
    {
        QString pt("doc/temp/");
        QFile  dtpdf1 (pt+ "sppd_temp.txt");
       if (!dtpdf1.open(QIODevice::ReadOnly | QIODevice::Text))
       {
          qInfo() << "sppd_temp File tdk terbuka"; return;
       }

       QTextStream stream(&dtpdf1);
       while (!stream.atEnd()) {
           QString line = stream.readAll();
            list = line.split("#/#");
            list << "aaa";
           }
    }

    void loadpdf1()
    {
        htm = list.at(0);
        tot = list.at(1);
        tgl = list.at(2);
        tgl_= list.at(3);
        thn = list.at(4);
        nosrt = list.at(5);
        hal = list.at(6);
        kpdns = list.at(7);
        jkpdns= list.at(8);
        nipkpdns = list.at(9);
        terbilang = list.at(10);
         jbt_klp_dns = list.at(11);
         spasi_br = list.at(12);

    }

    QString toCamelCase(const QString &s)
    {
        QStringList parts = s.split(' ',QString::SkipEmptyParts );
        for (int i = 0; i < parts.size(); ++i)
            parts[i].replace(0, 1, parts[i][0].toUpper());
        return parts.join(" ");
    }


    void onPdfPrintingFinished(const QString & filename, bool ok)
    {
        qDebug() << filename << ok;
        progressbar->setRange(0, 100);
    }

    void onPdfPrintingFinished2(const QString & filename, bool ok)
    {
        qDebug() << filename << ok;
        progressbar2->setRange(0, 100);
    }

     static void usleep(unsigned long usecs){QThread::usleep(usecs);}
     static void msleep(unsigned long msecs){QThread::msleep(msecs);}
     static void sleep(unsigned long secs){QThread::sleep(secs);}

public slots:

     //===================================================
     QString htm;
     QString tot;
     QString tgl;
     QString tgl_;
     QString thn;
     QString nosrt;
     QString hal;
     QString kpdns;
     QString jkpdns;
     QString nipkpdns;
     QString terbilang;
     QString jbt_klp_dns;
     QString spasi_br;
     //===================================================
    QPushButton *button2;
    QPushButton *button;
    QProgressBar *progressbar;
    QProgressBar *progressbar2;
    QWebEngineView *view;
    QWebEngineView *view2;

};



#endif // PDF_SPPD_ADD_H

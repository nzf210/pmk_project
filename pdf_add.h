#ifndef PDF_ADD_H
#define PDF_ADD_H

#include <QtWebEngineWidgets>
#include <QThread>

class Widget2 : public QWidget
{
public:
    QStringList list; QStringList li_bm;
    explicit Widget2(QWidget *parent = nullptr):
        QWidget(parent),  button2(new QPushButton),button(new QPushButton),progressbar(new QProgressBar),  progressbar2(new QProgressBar), view(new QWebEngineView), view2(new QWebEngineView)
    {

        button->setText(tr("generate pdf add"));
        button->setEnabled(false);

        connect(button, &QPushButton::clicked, this, &Widget2::onClicked);
        connect(view, &QWebEngineView::loadFinished, this, &Widget2::onLoadFinished);
        connect(view->page(), &QWebEnginePage::pdfPrintingFinished, this, &Widget2::onPdfPrintingFinished);

        //!=================================================================================
         loadmbam();
         loadlspdf1();
         loadpdf1();
        //!=================================================================================

        QString path2("doc/temp/");
        QString path("doc/html/");

        QDir dir(path);
        QFile  htmlFile (path+ "srt-add.html");

        if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {qInfo() << "Html File tdk terbuka"; return;}
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
        QString tgl_ = d.toString("dd MMMM yyyy");
        htmlContent.replace("#TGL#", tgl_);
        htmlContent.replace("#thn#",thn);
        htmlContent.replace("#thn2#",thn2);
        htmlContent.replace("#UDSTR#", unmdis);
        htmlContent.replace("#UKMP#", unmkam );

        htmlContent.replace("#SKKP#", skkam);
        htmlContent.replace("#skben#", skben );

         htmlContent.replace("#NM1#", nama1 );
         htmlContent.replace("#NM2#", nama2 );
         htmlContent.replace("#NM3#", nama3 );
         htmlContent.replace("#NM4#", nama4 );
         htmlContent.replace("#NM5#", nama5 );
         htmlContent.replace("#NM6#", nama6 );
         htmlContent.replace("#NM7#", nama7 );
         htmlContent.replace("#NM8#", nama8 );
         htmlContent.replace("#NM9#", nama9 );
         htmlContent.replace("#NM10#", nama10 );
         htmlContent.replace("#NM11#", nama11 );
         htmlContent.replace("#NM12#", nama12);
         htmlContent.replace("#NM13#", nama13 );

        QString html = htmlContent;
        htmlFile.close();

        QString l= html;
        QFile fOut(path2+"add.html");
        fOut.open(QFile::WriteOnly | QFile::Text);
        if(fOut.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"tdk Bisa Buka file";} else {
            QTextStream stream(&fOut);
            stream << l;
            fOut.flush();
            fOut.close();
           // QMessageBox::information(this,tr("Info"),tr("dds tersimpan"));
         }

//        QFile aa(path2+"add.html");
//        aa.close();
        htmlFile.close();
        QUrl url = QUrl::fromLocalFile("/doc/temp/add.html");

       // view->load(url);
        view->setHtml(html);
        view->load(url);

        auto lay = new QVBoxLayout(this);
        lay->addWidget(button);
        lay->addWidget(progressbar);
        //lay->addWidget(view);
         resize(330, 280);
    }

    void loadlspdf1()
    {
        QString pt("doc/temp/");
        QFile  dtpdf1 (pt+ "dtpdf2.txt");
       if (!dtpdf1.open(QIODevice::ReadOnly | QIODevice::Text))
       {
          qInfo() << "dtpdf1.txt File tdk terbuka"; return;
       }

       QTextStream stream(&dtpdf1);
       while (!stream.atEnd()) {
           QString line = stream.readAll();
            list = line.split("/n");
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
        thn = tgl.right(4);
        unmdis = list.at(19);
        unmkam =list.at(20);

        skben=list.at(21);
        skkam=list.at(22);
        thn2=list.at(23);

        nama1 = li_bm.at(0);
        nama2= li_bm.at(1);
        nama3= li_bm.at(2);
        nama4= li_bm.at(3);
        nama5= li_bm.at(4);
        nama6= li_bm.at(5);
        nama7= li_bm.at(6);
        nama8= li_bm.at(7);
        nama9= li_bm.at(8);
        nama10= li_bm.at(9);
        nama11= li_bm.at(10);
        nama12= li_bm.at(11);
        nama13= li_bm.at(12);

    }

    void loadmbam()
    {
        QString pt("doc/temp/");
        QFile  bam (pt+ "nmbam.txt");
       if (!bam.open(QIODevice::ReadOnly | QIODevice::Text))
       {
          qInfo() << "nmbam.txt File tdk terbuka"; return;
       }

       QTextStream stream(&bam);
       while (!stream.atEnd()) {
           QString line = stream.readAll();
            li_bm = line.split(";");
           }
    }


//private:
    QString toCamelCase(const QString &s)
    {
        QStringList parts = s.split(' ', QString::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
            parts[i].replace(0, 1, parts[i][0].toUpper());
        return parts.join(" ");
    }

    void onLoadFinished(bool ok) {button->setEnabled(ok);}
    void onLoadFinished2(bool ok){button2->setEnabled(ok);}

    void onClicked()
    {
        progressbar->setRange(0, 0);
        QString path("laporan/add/");

        QDir dir(path);
//        Nama Lama
//        QFile  pdfFile (path+ "ADD_SRT.pdf");
//        QString fn = path+"ADD_SRT.pdf";
//        Nama Auto
        QString nmpdf =tahap+ " dis "+nmdis+" kam "+nmkamp+".pdf";
        QFile  pdfFile (path + nmpdf);
        QString fn = path + nmpdf;

        if(pdfFile.exists()){ pdfFile.remove(); }
        if(pdfFile.exists()) {QMessageBox::information(this,"Info...","Tutup File pdf yg terbuka dan Generate Kembali"); qInfo()<<"Silahkan tutup pdf file"; return;}

        view->page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize(QSize(743,987))), QPageLayout::Portrait, QMargins(65,0,15,15)) );
  //   view->page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMargins(25,15,15,15)) );
        view->page()->pdfPrintingFinished(fn, QMessageBox::information(this,"Info...","Menyiapkan file, <b>Tekan ok setelah loading selesai</b>"));
        sleep(2);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fn));
        view->close();
        this->close();
    }

    void run() {
            QTimer* timer = new QTimer(this);
            timer->setInterval(500);
            timer->connect(timer, SIGNAL(timeout()), this, SLOT(doIt()));
            timer->start();
                     }

//    void onClicked2()
//    {
//        progressbar2->setRange(0, 0);
//        QString path("html/");
//        QDir dir(path);
//        //QFile  htmlFile (path+ "srt1.html");
//        QFile  pdfFile2 (path+ "DD SRT KAMPUNG.pdf");
//        QString fn2 = path+"DD SRT KAMPUNG.pdf";
//        if(pdfFile2.exists()){ pdfFile2.remove(); }
//        if(pdfFile2.exists()) {QMessageBox::information(this,"Info...","Tutup File pdf yg terbuka"); qInfo()<<"Silahkan tutup pdf file"; return;}
//        view2->page()->printToPdf(fn2,QPageLayout(QPageSize(QPageSize(QSize(780,1154))), QPageLayout::Portrait, QMargins(15,15,15,50)) );
//        QDesktopServices::openUrl(QUrl::fromLocalFile(fn2));
//    }

    void onPdfPrintingFinished(const QString & filename, bool ok)
    {
        qDebug() <<"pdf Finish"<< filename << ok;
        progressbar->setRange(0, 1);
    }

    void onPdfPrintingFinished2(const QString & filename, bool ok)
    {
        qDebug() <<"Pdf fnsh 2"<< filename << ok;
        progressbar2->setRange(0, 1);
    }

     static void usleep(unsigned long usecs){QThread::usleep(usecs);}
     static void msleep(unsigned long msecs){QThread::msleep(msecs);}
     static void sleep(unsigned long secs){QThread::sleep(secs);}

     //===================================================

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

     QString nama1;
     QString nama2;
     QString nama3;
     QString nama4;
     QString nama5;
     QString nama6;
     QString nama7;
     QString nama8;
     QString nama9;
     QString nama10;
     QString nama11;
     QString nama12;
     QString nama13;

     //QString Nama Auto

     //===================================================

public slots:
     QPushButton *button2;
    QPushButton *button;
    QProgressBar *progressbar;
     QProgressBar *progressbar2;
    QWebEngineView *view;
    QWebEngineView *view2;

};


#endif // PDF_ADD_H


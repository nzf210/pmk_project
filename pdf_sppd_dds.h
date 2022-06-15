#ifndef PDF_SPPD_DDS_H
#define PDF_SPPD_DDS_H

#include <QtWebEngineWidgets>
#include <QThread>
#include "form.h"

class Widget4 : public QWidget
{
public:
    QTimer* timer;
    QStringList list; QStringList li_bm;
    QMessageBox *msgBox;
    explicit Widget4(QWidget *parent = nullptr):
        QWidget(parent),  button2(new QPushButton),button(new QPushButton),progressbar(new QProgressBar),  progressbar2(new QProgressBar), view(new QWebEngineView), view2(new QWebEngineView)
    {

        button->setText(tr("generate pdf sppd dds"));
        button->setEnabled(false);

        connect(button, &QPushButton::clicked, this, &Widget4::onClicked);
        connect(view, &QWebEngineView::loadFinished, this, &Widget4::onLoadFinished);
        connect(view->page(), &QWebEnginePage::pdfPrintingFinished, this, &Widget4::onPdfPrintingFinished);

        //connect(button, &QPushButton::clicked, this, &Widget::onClicked2);
       // connect(view2, &QWebEngineView::loadFinished, this, &Widget4::onLoadFinished2);
        //connect(view2->page(), &QWebEnginePage::pdfPrintingFinished, this, &Widget4::onPdfPrintingFinished2);


//        Form *fr = new Form;
//         qInfo() <<"Jumlah Cair dari form" <<  fr->jumlahcair();
//!=================================================================================
       loadlspdf1();
       loadpdf1();
       //!=================================================================================

//               Form *frx = new Form;
//               qInfo() <<"Jumlah Cair dari form" <<  fr->getTot_sppd();

                QString path("doc/html/");
                QString path2("doc/temp/");


               QDir dir(path);
               QFile  htmlFile (path+ "SPP.html");

               if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
               {
                  qInfo() << "Html File SPP tdk terbuka"; return;
               }
               QString htmlContent;
               htmlContent = htmlFile.readAll();

               htmlContent.replace("****",nosrt);
               QString th="I";
               bool ok = nosrt.contains(th);
               qInfo() <<"Jumlah Cair dari form ------------------- " <<  ok;

               htmlContent.replace("#dddd#",htm);
               htmlContent.replace("#total#", tot);
               htmlContent.replace("#tgl_#", tgl_);
               htmlContent.replace("#tgl#", tgl_);
               htmlContent.replace("#thn#", thn);
               htmlContent.replace("#KPDNS#", kpdns);
               htmlContent.replace("#PKPLD#", jkpdns);
               htmlContent.replace("#NIPKDNS#", nipkpdns);
               htmlContent.replace("#hal#", hal);
               htmlContent.replace("#JBTKPDNS#",jbt_klp_dns);
               htmlContent.replace("#terbilang#", toCamelCase(terbilang));
               htmlContent.replace("#SPASI#",spasi_br);

               QString html = htmlContent;
               htmlFile.close();

               QString l= html;
               QFile fOut(path2+"spp_temp.html");
               fOut.open(QFile::WriteOnly | QFile::Text);
               if(fOut.open(QFile::WriteOnly | QFile::Text)){qInfo()<<"tdk Bisa Buka file sppd.html";} else {
                   QTextStream stream(&fOut);
                   stream << l;
                   fOut.flush();
                   fOut.close();
                }

               QUrl url = QUrl::fromLocalFile("/doc/temp/spp_temp.html");
               view->setHtml(html);
               view->load(url);

               auto lay = new QVBoxLayout(this);
               lay->addWidget(button);
               lay->addWidget(progressbar);
               //lay->addWidget(view);
               //lay->addWidget(view2);
               //resize(630, 780);
               resize(330, 280);

           }

           void onLoadFinished(bool ok)
           {
               button->setEnabled(ok); run();
            }

//           void onLoadFinished2(bool ok)
//           {
//              // button2->setEnabled(ok);
//            }

           void onClicked()
           {
               msgBox = new QMessageBox;
               msgBox->setWindowTitle("Info ... ");
               QIcon logo(":/gbr/html/gbr/yhk.png");
               msgBox->setWindowIcon(logo);
               msgBox->setIcon(QMessageBox::Information);
               msgBox->setText("Open File pdf Surat Pengantar ... ");

               QString path("laporan/sppd/");
               QDir dir(path);
               QFile  pdfFile (path+ tgl_+ " SPP_all.pdf");
               QString fn = path+ tgl_+" SPP_all.pdf";

               if(pdfFile.exists()){ pdfFile.remove(); }
               if(pdfFile.exists()) {QMessageBox::information(this,"Info...","Tutup File pdf yg terbuka"); qInfo()<<"Silahkan tutup pdf file"; return;}
               view->page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize(QSize(780,1154))), QPageLayout::Landscape, QMargins(50,20,25,15))); // fix sppd

               // QMessageBox::information(this,"Info..."," Menyiapkan file, <b>Tekan OK setelah loading selesai...</b>" ));
               //bool ok;
               msgBox->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
               qInfo() << " === msgBox === " <<msgBox->exec();
               //sleep(2);
               view->page()->pdfPrintingFinished(fn,ok_());
               //sleep(2);
               QDesktopServices::openUrl(QUrl::fromLocalFile(fn));  //msleep(1);   QDesktopServices::openUrl(QUrl::fromLocalFile(fn2));
               view->close();
               this->close();
           }

           void run() {
                   qInfo() << "runnnn runnnnnnnnnnn";
                   timer = new QTimer(this);
                   timer->setInterval(50);
                   //timer->connect(timer, SIGNAL(timeout()), this, SLOT(klik_buton()));
                   timer->connect(timer, &QTimer::timeout, this, &Widget4::klik_buton);
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

           bool ok_()
           {
               qInfo() << "Run bool";
               sleep(3);
                qInfo() << "Run bool 222";
               return true;
           }

           void loadlspdf1()
           {
               QString pt("doc/temp/");
               QFile  dtpdf1 (pt+ "spp_temp.txt");
              if (!dtpdf1.open(QIODevice::ReadOnly | QIODevice::Text))
              {
                 qInfo() << "spp_temp File tdk terbuka"; return;
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

           QString toCamelCase(const QString& s)
           {
               QStringList parts = s.split(' ', QString::SkipEmptyParts);
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

#endif // PDF_SPPD_DDS_H

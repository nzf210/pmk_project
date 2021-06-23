#include "pdf_dok.h"
#include "ui_pdf_dok.h"

#include "pdf.h"
#include "pdf2.h"

#include <QApplication>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextStream>
#include <QFile>
#include <QPrinter>
#include <QDir>
#include <QDebug>
#include <QtWidgets>
#include <QPixmap>
#include <QPdfWriter>
#include <QPainter>
#include <QProgressDialog>
#include <QTextTableCell>
#include <QTextCursor>
#include <QTransform>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QApplication>
#include <QtWebEngine/QtWebEngine>

#include <functional>

 using namespace std;
 using namespace std::placeholders;



void writePdf()
{
    const QString fileName("output.pdf");
    const QImage image("html/gbr/yhk.jpg");

    const QPoint imageCoordinates(600,511);

    QPdfWriter pdfWriter(fileName);

    //pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    //pdfWriter.setPageSize(QPageSize(QPageSize::A4Extra));
    QPainter painter(&pdfWriter);

    painter.drawImage(imageCoordinates, image);

//    for (int i=0; i<1; ++i) {
//        painter.drawImage(imageCoordinates, image);
//        pdfWriter.newPage();
//    }

//    const QString fileName("C:/Qt");
//const QImage image("C:/Users/georg/Desktop/testPdf/Untitled.jpg");
//const QPoint imageCoordinates(0,0);

//QPdfWriter pdfWriter(fileName);
//pdfWriter.setPageSize(QPagedPaintDevice::A1);
//QPainter painter(&pdfWriter);

//for (int i=0; i<3; ++i)
//{
//painter.drawImage(imageCoordinates, image);
//pdfWriter.newPage();
//qDebug()<<2;
//}

}

int print(){
   // const int highQualityDPI = 300;
    QString path("html/");
    QDir dir(path);
    //QDir::setCurrent(QCoreApplication::applicationDirPath());
    QPixmap gambar(":/gbr/html/gbr/yhk.jpg");
    QFile  htmlFile (path+ "dok_2.html");
    if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
       qInfo() << "Html File tdk terbuka";
        return -1;
    }
    QDir dirr(path , "ouput.pdf");

    if(dirr.exists()){for(const QString & filename: dirr.entryList()){
            dirr.remove(filename);
        }}
    QTextEdit *document = new QTextEdit();

    QString htmlContent;
    QTextStream in(&htmlFile);


    htmlContent = in.readAll();
    htmlContent.replace("****","0033");
    htmlContent.replace("Sumohai, 19 Agustus 2020","Dekai 20 November 2020");

    QPainter p;
    QPrinter printer(QPrinter::HighResolution);
   // QTextDocument *document = new QTextDocument();
    printer.setOutputFormat(QPrinter::PdfFormat);

   printer.setPaperSize(QSizeF(210,330), QPrinter::Millimeter);
   printer.setPageMargins(QMarginsF(1, 1, 1, 1));
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOrientation(QPrinter::Landscape);
    document->setHtml(htmlContent);

 //p.setBrush(Qt::transparent);
    printer.setOutputFileName(path+"output.pdf");
//p.drawPixmap(0,0,100,100, QPixmap(":/gbr/html/gbr/yhk.jpg"));
//p.drawPixmap(150,150,300,300, QPixmap(":/gbr/html/gbr/yhk.jpg"));
//p.end();

    document->print(&printer);

    delete document;
    return 0;
}



//pdf_dok::pdf_dok(QString inputPath, QString outputPath)
//    : m_inputPath(move(inputPath))
//    , m_outputPath(move(outputPath))
//    , m_page(new QWebEnginePage)
//{
//    connect(m_page.data(), &QWebEnginePage::loadFinished,
//            this, &pdf_dok::loadFinished);
//    connect(m_page.data(), &QWebEnginePage::pdfPrintingFinished,
//            this, &pdf_dok::pdfPrintingFinished);
//}




pdf_dok::pdf_dok(QWidget *parent) : QWidget(parent), ui(new Ui::pdf_dok)
{
    ui->setupUi(this);
    m_printer = new QPrinter(QPrinter::ScreenResolution);
    m_printer->setPaperSize(QSizeF(210,330),QPrinter::Millimeter);
    m_printer->setPageMargins(27,11.2,1,1.2,QPrinter::Millimeter);
    m_printer->setOrientation(QPrinter::Landscape);
    m_printer->setFullPage(true);

    m_inputPath = "html/srt1.html";
    m_outputPath = "html/sss.pdf";


//       html= "<!DOCTYPE html><html>  <head>    <meta http-equiv='content-type' content='text/html;charset=UTF-8' />    "
//             "<title>Paginated HTML</title>    <style type='text/css' media='print'>      div.page      {        page-break-after: always;       "
//             " page-break-inside: avoid;      }    </style>  </head>  <body>    <div class='page'>      <h1>This is Page 1</h1>    "
//             "</div>    <div class='page'>      <h1>This is Page 2</h1>    </div>    <div class='page'>      <h1>This is Page 3</h1>    </div>  </body></html>";

       webView->setHtml(html);
      //printHTML();
      //pdf();
      //handlePagePrint("","");

}

pdf_dok::~pdf_dok()
{
    delete ui;
}



void pdf_dok::printPreview(QPrinter *printer)
{
//    bool ok;

    QString path("html/");
    QDir dir(path);
    QFile  htmlFile (path+ "srt1.html");
    QString fn = path+"tess.pdf";
    QUrl url = QUrl::fromLocalFile(path+"srt1.html");
    //QWebEngineView *view = new QWebEngineView;
   webView->load(url);
   // webView.page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize::A4Extra), QPageLayout::Landscape, QMarginsF()) );
    //view->page()->printToPdf(fn,QPageLayout(QPageSize(QPageSize(QSize(450,670))), QPageLayout::Landscape, QMarginsF()) );
    //view->page()->pdfPrintingFinished(fn, QMessageBox::information(this,"Info...","Berhasil Menyimpan"));

    webView->page()->print(printer, [this] (bool result) { qInfo() << result;
        // ignore the result for now. Decide what to do with it later on
        delete m_printer;
        m_printer = nullptr;
    });
    webView->deleteLater();

//    QDesktopServices::openUrl(QUrl::fromLocalFile(qApp->applicationDirPath() + "/docs/" + "nameof.pdf"));

//     webView.setHtml(html, QUrl(QString("file:///%1/").arg(QFileInfo("filename").absolutePath())));
}

void pdf_dok::printHTML()
{
    QPrinter printer;
    QPrintPreviewDialog preview(&printer);
    preview.setWindowTitle("DPMK YAHUKIMO");
    //preview.setWindowModified(true);
    preview.setModal(true);
    preview.setMinimumSize(650,970);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSizeF(210,330),QPrinter::Millimeter);
    printer.setPageMargins(0.,0.,0.,0., QPrinter::Millimeter);
    printer.setFullPage(false);

    connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printPreview(QPrinter*)));
    preview.exec();
   // this->close();
}


void pdf_dok::printToPDF()
{

  QString h = "html/";
    QDesktopServices::openUrl(QUrl::fromLocalFile( h+"tess.pdf"));

    //QDesktopServices::openUrl(QUrl::fromLocalFile(qApp->applicationDirPath() + "/html/" + "tess.pdf"));

    //=================================================================
//    m_page.data();
//    connect(m_page.data(), &QWebEnginePage::loadFinished, this, &pdf_dok::loadFinished);
//   connect(m_page.data(), &QWebEnginePage::pdfPrintingFinished, this, &pdf_dok::pdfPrintingFinished);
//==================================================================

//    // Set location of resulting PDF
//    QString saveLocation = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + filename + ".pdf";

//    // Initialize printer and set save location
//    QPrinter printer(QPrinter::HighResolution);
//    printer.setOutputFileName(saveLocation);

//    // Create webview and load html source
//    //QScopedPointer<QWebEnginePage> webview;
//    QWebEngineView webview;
//    webview.setHtml(htmlinput);

//    // Create PDF
//    //webview.printToPdf(m_outputPath);
//
}

int pdf_dok::run()
 {
     m_page->load(QUrl::fromUserInput(m_inputPath));
     return QApplication::exec();
 }

void pdf_dok::loadFinished(bool ok)
 {
     if (!ok) {
         QTextStream(stderr)
             << tr("failed to load URL '%1'").arg(m_inputPath) << "\n";
         QCoreApplication::exit(1);
         return;
     }
     m_page->printToPdf(m_outputPath);
 }

void pdf_dok::pdfPrintingFinished(const QString &filePath, bool success)
{
    if (!success) {
        QTextStream(stderr)
            << tr("failed to print to output file '%1'").arg(filePath) << "\n";
        QCoreApplication::exit(1);
    } else {
        QCoreApplication::quit();
    }
}

//! ============================================================================


void pdf_dok::on_toolButton_clicked()  //Button in PDF View
{

//printToPDF();
//Widget *a = new Widget;
//a->hide();
//a->button->click();


}

//! ============================================================================


void pdf_dok::pdf()
{

    QString path("html/");
    QDir dir(path);
    QFile  htmlFile (path+ "srt1.html");
    if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
       qInfo() << "Html File tdk terbuka"; return;
    }
     QString htmlContent;
     QTextStream in(&htmlFile);
     htmlContent = in.readAll();

     html = htmlContent;
      QUrl url =  QUrl::fromLocalFile("/html/srt1-.html");

       //QTextStream *d = new QTextStream;

      m_webEnginePage = new QWebEngineView(this);
      m_webEnginePage->load(url);
      QString pt = "html/cobb.pdf";
      m_webEnginePage->page()->printToPdf(pt);
       handlePagePrint(htmlContent, "html/srt1-.html");


}

void pdf_dok::handlePagePrint(const QString& html, const QString& fileName)
{
    qInfo() << fileName;

    QTextEdit *editor = new QTextEdit;
    editor->setHtml(html);
    editor->show();
    QUrl url =  QUrl::fromLocalFile("/html/srt1.html");
   QWebEngineView *document = new QWebEngineView;
     //document->setHtml(html);
       document->load(url);

       QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
       printer->setPaperSize(QSizeF(210,330), QPrinter::Millimeter);
       printer->setPageMargins(QMarginsF(1, 1, 1, 1));
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOrientation(QPrinter::Landscape);
        printer->setFullPage(true);


       QPrintDialog *dlg = new QPrintDialog(printer, this);
       if (dlg->exec() != QDialog::Accepted)
       {return;}
       QString pdf="html/mmco.pdf";
       document->page()->printToPdf(pdf);
       document->page()->print(printer, [this] (bool result) { qInfo() << result;
            // ignore the result for now. Decide what to do with it later on
            delete m_printer;
            m_printer = nullptr;
        });
}


void pdf_dok::vpdf()
{
    QString p = ":/html/srt1.html";
    QDir html(p);
    QWebEngineView* myWebView = new QWebEngineView;
    myWebView->load(p);
    ui->graphicsView->setToolTip("untuk Menampilkan");
    // when loaded:
    QPixmap pix( myWebView->size());
    myWebView->render( &pix );
    //print pixmap to pdf
}

int pdf_dok::cetak()
{
    qInfo() << "Cetakkk" ;
       if(print()<0){}
       return -1;
}

void pdf_dok::pdf2()
{
    QTextDocument *doc = new QTextDocument();
    const char* s1 = "<html><head><style>body { font-family: sans-serif;}.note-atomic { background: rgb(242,242,242); width: 1000px; margin: 5px auto; border: 1px solid grey; border-radius: 8px;padding: 5px;}"
                                    "</style></head><body><div class = 'note-atomic'>Hi</div><img src = '/html/gbr/yhk.png' width='400' height='300' /></body></html>";
    doc->setHtml(QString::fromStdString(s1));
    QString out = "/html/coba.pdf";
    QPdfWriter pdf(QString::fromStdString(s1));
    int logicalDPIX=pdf.logicalDpiX();
    const int PointsPerInch=72;
    QPainter painter(&pdf);
    painter.scale(20.0,20.0);
    doc->drawContents(&painter);
    QTransform t;
    float scaling=(float)logicalDPIX/PointsPerInch; //16.6
    t.scale(scaling,scaling);

    // do drawing with painter
    painter.end();
    painter.setTransform(t);

//   QPrinter *printer = new QPrinter();

//    printer->setOutputFileName("/html/fool.pdf");
//    printer->setOutputFormat(QPrinter::PdfFormat);

//    QPainter *painter = new QPainter();
//    painter->begin( printer );
//    doc->drawContents(painter, printer->pageRect().translated( -printer->pageRect().x(), -    printer->pageRect().y() ));
//    doc->print(printer);

//    const char* s2 = "<html><body><div>Bye</div></body></html>";
//    doc->setHtml(QString::fromStdString(s2));

//    printer->newPage();
//    doc->drawContents(painter, printer->pageRect().translated( -printer->pageRect().x(), - printer->pageRect().y() ));

//    doc->print(printer);
//    painter->end();

}

void pdf_dok::tess()
{
    QString description = ui->textEdit->toPlainText ();
    qInfo() << "tesss " << description;
}

void pdf_dok::qtwehtmltopdf()
{


//    QtWebEngine::initialize();
//    // Delete WebEngine cache
//    QDir cache_dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
//    cache_dir.removeRecursively();

//    QCommandLineParser parser;
//    parser.setApplicationDescription("Qt WebEngine HTML to PDF");
//    parser.addHelpOption();
//    parser.addOptions({
//        {"printer", "Send output to printer instead of PDF file"},
//        {"list-printers", "Show list of available printers and exit"},
//        {"post", "Send HTTP POST request with data from stdin"},
//    });
//    parser.addPositionalArgument("url", "URL to HTML document");
//    parser.addPositionalArgument("output", "PDF filename, printer name or '-' for stdout");
//    parser.process(app);

//    // If list-printers is set, show list and exit
//    if (parser.isSet("list-printers")) {
//        const QStringList printerNames = QPrinterInfo::availablePrinterNames();
//        for (const auto& name : printerNames) { qInfo().quote() << name; }
//        return EXIT_SUCCESS;
//    }

//    // Check number of positional args
//    const QStringList args_url_output = parser.positionalArguments();
//    if (args_url_output.size() != 2) {
//        parser.showHelp(1);
//    }
//    const QUrl documentUrl(args_url_output[0]);
//    const QString outputName(args_url_output[1]);

//    // Create HTTP request for given program arguments
//    const QWebEngineHttpRequest request = [&]() {
//        QWebEngineHttpRequest request { documentUrl };
//        if (parser.isSet("post")) {
//            request.setMethod(QWebEngineHttpRequest::Post);
//            request.setHeader("Content-Type", "application/json");  // Fixed type for now
//            request.setPostData(readStdin());
//        }
//        return request;
//    }();

//    // Create web page and PDF creation handlers
//    QWebEnginePage page;
//    QObject::connect(&page, &QWebEnginePage::loadFinished, &page, [&](const bool ok) {
//        if (!ok) {
//            qCritical() << "Error loading page";
//            QCoreApplication::exit(EXIT_FAILURE);
//            return;
//        }
//        // Queue print request after page finished loading
//        // TODO: How to ensure that page finished js processing? -> Add some time as workaround
//        QTimer::singleShot(100, &page, [&]() {
//            getCssPageLayout(page, [&](const QPageLayout& layout) {
//                if (!parser.isSet("printer")) {
//                    printToPDF(page, layout, outputName);
//                } else {
//                    printToPrinter(page, layout, outputName);
//                }
//            });
//        });
//    });

//    // Init process by loading the web page
//    page.load(request);

}


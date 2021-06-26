#ifndef PDF_DOK_H
#define PDF_DOK_H

#include <QWidget>
#include <QObject>
#include <QtWebEngineWidgets>
#include <QPrinter>
#include <QApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>
#include <functional>
using namespace std;
 using namespace std::placeholders;

namespace Ui {
class pdf_dok;
}

class pdf_dok : public QWidget
{
    Q_OBJECT

public:
    explicit pdf_dok(QWidget *parent = nullptr);
    ~pdf_dok();
    void handlePagePrint(const QString& html, const QString& fileName);
    void printToPDF();
    int run();

private slots:
    void on_toolButton_clicked();
    void loadFinished(bool ok);
    void pdfPrintingFinished(const QString &filePath, bool success);


public slots:
    void printHTML();
    void printPreview(QPrinter *printer);

private:

    QString m_inputPath;
    QString m_outputPath;
    QScopedPointer<QWebEnginePage> m_page;

    void pdf2();
    int cetak();
    void tess();
    void pdf();
    void vpdf();
    void qtwehtmltopdf();
    QPrinter *m_printer;
    QWebEngineView  *m_webEnginePage;
    QWidget *w;
    QWebEngineView *webView;
    QString html;
    Ui::pdf_dok *ui;
};

#endif // PDF_DOK_H

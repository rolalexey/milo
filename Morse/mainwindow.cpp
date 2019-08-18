#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mtranslator.h"

#include <QFileDialog>
#include <QDebug>



#define     MAX_TEXWIDGET_LENGTH    1000000


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    m_translator = new MTranslator(this);

    ui->setupUi(this);
    setSource("");
}

MainWindow::~MainWindow()
{
    delete m_translator;
    delete ui;
}

/*!
 * \brief MainWindow::setSource - set new source text to translator
 * \param text - string to  translate
 */
void MainWindow::setSource(const QString &text)
{
    m_translator->setSource(text);


    /*!
      Widgets have some limitation on text size, so this is a barrier.
     */
    if(text.length() > MAX_TEXWIDGET_LENGTH)
    {

        QSignalBlocker block(ui->inputField);
        ui->inputField->setPlainText(tr("Source text too big to display."
                                      " But will be translated."));
        return;
    }
    ui->inputField->setPlainText(text);
}

/*!
 * \brief MainWindow::result - get translated text
 * \return
 */
const QString &MainWindow::result()
{
    return m_translator->result();
}


/*!
 * \brief MainWindow::on_MTranslator_morseMode - display format of source upon signal from
 *        translator. Connected automatically.
 * \param mode
 */
void MainWindow::on_MTranslator_morseMode(bool mode)
{

    if(mode)
        ui->label->setText(tr("MORSE"));
    else {
        ui->label->setText(tr("TEXT"));
    }

}


/*!
 * \brief MainWindow::on_inputField_textChanged - autoconnected slot updating translator source
 *        on UI input
 * \return
 */
void MainWindow::on_inputField_textChanged()
{

    m_translator->setSource( ui->inputField->toPlainText() );
    m_translator->translate();
}


/*!
 * \brief MainWindow::on_MTranslator_translated - display result of translation. Barrier on big text
 *      applied. Slot is autoconnected.
 */
void MainWindow::on_MTranslator_translated()
{
    if(result().length() > MAX_TEXWIDGET_LENGTH)
    {
        ui->resultBrowser->setText(tr("Text translated, but is too big to display."
                                      " Choose File->Save... to save translation to file."));
        return;
    }
    ui->resultBrowser->setText(result());
}


/*!
 * \brief MainWindow::on_actionOpen_file_triggered -
 */
void MainWindow::on_actionOpen_file_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open File"), "",  "*.txt");
    if(fileName.isEmpty())  return;
    QFile rfile(fileName);
    if(rfile.open(QIODevice::ReadOnly))
    {
        ui->label->setText(tr("Loading..."));
        qApp->processEvents();
        auto src = rfile.readAll();
        setSource(src);
        m_translator->translate();
    }
}

void MainWindow::on_actionSave_file_triggered()
{
    auto fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save File..."), "",  "*.txt");
    if(fileName.isEmpty())  return;
    QFile wfile(fileName);
    if(wfile.open(QIODevice::WriteOnly))
    {
        wfile.write( result().toLocal8Bit() );
    }
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

/*!
 * \brief MainWindow::on_actionSwap_Result_triggered - ui slot to feed translation result to source.
 */
void MainWindow::on_actionSwap_Result_triggered()
{
    // We need to pull result into s because translator returns result as reference
    QString s = result();
    setSource( s );
}

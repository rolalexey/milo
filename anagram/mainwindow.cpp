#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tick(0)
{


    m_dict = new AnagramFinder(this);
    ui->setupUi(this);

    ui->actionStop_Search->setVisible(false);

    connect(m_dict, SIGNAL(progress(int)), this, SLOT(showPercent(int)));
    connect(m_dict, SIGNAL(longestChain(const QString&)), this, SLOT(showLongestChain(const QString&)));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(showTick()));
}

MainWindow::~MainWindow()
{
    delete m_dict;
    delete ui;
}

void MainWindow::on_actionLoad_Dictionary_triggered()
{
    auto fname = QFileDialog::getOpenFileName(Q_NULLPTR, "Open Dict...", "", "Text Files (*.txt)");
    if(fname.isEmpty())
       return;

    m_dict->loadFile(fname);

    auto status = QString(tr("%1 words loaded")).arg( m_dict->wordCount() );
    showStatusText(status);

}

void MainWindow::on_actionSearch_Anagrams_triggered()
{
    auto    anList = m_dict->searchAnagram(ui->lineEdit->text());
    auto    result = QString(tr("%1 anagrams found\n%2"))
                    .arg(   anList.count() )
                    .arg(   anList.join("\n") );

    showResultText( result );
}

void MainWindow::on_actionFind_Longest_triggered()
{
    m_dict->resetResult();
    m_dict->startSearchLongest(ui->lineEdit->text());
}


void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionStop_Search_triggered()
{
    if(m_dict->isRunning())     m_dict->terminate();
}

void MainWindow::on_Dict_started()
{
    ui->actionFind_Longest->setEnabled(false);
    ui->actionLoad_Dictionary->setEnabled(false);
    ui->actionStop_Search->setVisible(true);
    showResultText(tr("Searching..."));

    m_timer.start(100);
    m_start.start();
}

void MainWindow::on_Dict_finished()
{
    ui->actionFind_Longest->setEnabled(true);
    ui->actionLoad_Dictionary->setEnabled(true);
    ui->actionStop_Search->setVisible(false);

    m_timer.stop();

    auto elapsed = QString(tr("Finished in %1 seconds. Dictionary: %2 words"))
            .arg( m_start.elapsed()/1000 )
            .arg( m_dict->wordCount() );

    ui->statusBar->showMessage(elapsed);
}

void MainWindow::showResultText(const QString &text)
{
    ui->textBrowser->setText(text);
}

void MainWindow::showStatusText(const QString &text)
{
    ui->statusBar->showMessage(text);
}

void MainWindow::showPercent(int percent)
{
    ui->progressBar->setValue(percent);
}

void MainWindow::showLongestChain(const QString &chain)
{

    if( chain.isEmpty() )
    {
        showResultText(tr("Nothing found"));
        return;
    }

    auto res = QString(tr("Longest derived chain is: %1\n")).arg(chain);
    auto src = ui->lineEdit->text();



    foreach(QChar ch, chain)
    {

        auto next = src + ch;
        auto anList = m_dict->searchAnagram(next);
        QString stepBuf = QString(" + %1 = [%2]\n").arg(ch).arg(anList.join(','));

        res += src;
        res += stepBuf;
        src = next;

    }
    showResultText(res);
}

void MainWindow::showTick()
{
    static QString tick("\\-|/-");
    int tickPhase = m_tick++ % tick.length();
    QString s(tick[tickPhase]);
    ui->statusBar->showMessage(s);
    qApp->processEvents();
}

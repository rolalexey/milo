#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTime>
#include "dict.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



protected slots:

    // UI actions handlers
    void        on_actionLoad_Dictionary_triggered();
    void        on_actionSearch_Anagrams_triggered();
    void        on_actionFind_Longest_triggered();
    void        on_actionExit_triggered();
    void        on_actionStop_Search_triggered();

    void        on_Dict_started();
    void        on_Dict_finished();


    void        showResultText(const QString & text);
    void        showStatusText(const QString & text);

    void        showPercent(int percent);
    void        showLongestChain(const QString & chain);

    // Show rotating tick
    void        showTick();


private:
    Ui::MainWindow *ui;

    AnagramFinder            *m_dict;
    int             m_tick;
    QTimer          m_timer;
    QTime           m_start;

};

#endif // MAINWINDOW_H

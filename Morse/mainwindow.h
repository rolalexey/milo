#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMap>


namespace Ui {
class MainWindow;
}

class MTranslator;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void                setSource(const QString &text);
    const QString &     result();



protected slots:


    void            on_inputField_textChanged();

    void            on_MTranslator_morseMode(bool mode);
    void            on_MTranslator_translated();

    void            on_actionOpen_file_triggered();
    void            on_actionSave_file_triggered();
    void            on_actionExit_triggered();
    void            on_actionSwap_Result_triggered();



private:
    Ui::MainWindow *ui;


    MTranslator *    m_translator;

};

#endif // MAINWINDOW_H

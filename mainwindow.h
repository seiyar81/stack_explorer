#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
class StackExplorer;
}
class Worker;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile();
    void addr2lineFinished(QString,QString);

private:

    Ui::StackExplorer *ui;

    Worker *                            m_worker;

    QVector< QPair<QString, QString> >  m_stack;
    QString                             m_program;
};

#endif // MAINWINDOW_H

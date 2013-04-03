#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"

#include <QSplitter>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StackExplorer)
{
    ui->setupUi(this);
    centralWidget()->setLayout( ui->gridLayout );

    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(ui->textEdit);
    splitter->addWidget(ui->tableWidget);

    ui->gridLayout->addWidget(splitter);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_worker = new Worker(this);
    connect(m_worker, SIGNAL(addr2lineParsed(QString,QString)), this, SLOT(addr2lineFinished(QString,QString)), Qt::DirectConnection);
}

void MainWindow::openFile()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->textEdit->clear();
    m_stack.clear();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose an input file with the addresses to read"));
    if(fileName.length())
    {
        QFile file(fileName);
        Q_ASSERT(file.exists());

        if(file.open(QFile::ReadOnly))
        {
            m_program = QFileDialog::getOpenFileName(this, tr("Choose the binary program to read the addresses from"));
            Q_ASSERT(m_program.length());

            m_worker->setProgram(m_program);

            QString fullContent = "";
            while (!file.atEnd()) {
                QString line = file.readLine();
                fullContent += line;
                line.remove("\n");

                QRegExp regexp("^0x.{1,8}");
                regexp.indexIn(line);
                line = regexp.cap(0).trimmed();

                if(line.indexOf(":") -1)
                {
                    line = line.remove(":");
                }

                if(line.length())
                {
                    m_worker->addAddress( line );
                }
            }

            ui->textEdit->setPlainText( fullContent );

            m_worker->run();
        }
        else
        {
            QMessageBox::critical(this, tr("Fatal error"), tr("Error while opening the file !"));
        }
    }

}

void MainWindow::addr2lineFinished(QString address, QString line)
{
    m_stack.push_back( QPair<QString,QString>(address, line) );

    int row = ui->tableWidget->currentRow() + 1;
    ui->tableWidget->insertRow( row );
    QTableWidgetItem *first = new QTableWidgetItem( address );
    QTableWidgetItem *second = new QTableWidgetItem( line );
    ui->tableWidget->setItem( row, 0, first );
    ui->tableWidget->setItem( row, 1, second );
}

MainWindow::~MainWindow()
{
    delete ui;

    m_worker->wait();
    delete m_worker;
}

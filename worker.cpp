#include "worker.h"

#include <QEventLoop>
#include <QProcess>

Worker::Worker(QObject *parent) :
    QThread(parent)
{
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(finished(int)), this, SLOT(addr2lineFinished(int)));
}

Worker::~Worker()
{
    delete m_process;
}

void Worker::setProgram(const QString & program)
{
    m_program = program;
}

void Worker::addAddress(const QString & address)
{
    m_addresses.push_back(address);
}

void Worker::run()
{
    QVectorIterator< QString > it(m_addresses);
    it.toBack();
    while(it.hasPrevious())
    {
        m_currentAddress = it.previous();

        QEventLoop loop;
        connect(this, SIGNAL(parseNextLine()), &loop, SLOT(quit()));

        m_process->start("addr2line", QStringList() << "-e" << m_program << m_currentAddress);
        loop.exec();
    }
}

void Worker::addr2lineFinished(int status)
{
    Q_UNUSED(status);

    QString line = m_process->readAll();
    if(line.length())
    {
        line.remove("\n");
    }

    emit addr2lineParsed(m_currentAddress, line);
    emit parseNextLine();
}

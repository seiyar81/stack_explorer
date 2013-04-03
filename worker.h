#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QVector>

class QProcess;

class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();

    void setProgram(const QString &);
    void addAddress(const QString &);

    void run();

signals:
    void addr2lineParsed(const QString &, const QString &);
    void parseNextLine();

private slots:
    void addr2lineFinished(int);

private:
    QVector< QString >      m_addresses;

    QProcess *                      m_process;
    QString                         m_currentAddress;
    QString                         m_program;

};

#endif // WORKER_H

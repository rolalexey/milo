#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H
#include <QTimer>
#include <QThread>
#include <QSharedPointer>

class QMutex;
typedef QSharedPointer<QMutex>      ForkPtr;


class Philosopher;
typedef QList<Philosopher *>        PhilosList;


class Philosopher : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString statestr READ stateStr NOTIFY stateChanged)

public:

    enum State { Stopped=1 , Thinking, Waiting1, Waiting2, Eating};

    Philosopher(const QString & Name, QObject * parent = nullptr);
    virtual ~Philosopher();

    /*!
     * \brief setName set Philosopher's name
     * \param Name
     */
    void        setName(const QString & Name);
    QString     name();

    State       state();
    void        setState(State state);

    // returns Philo's state as string
    QString     stateStr();


    void        setPhilosList( PhilosList * list );

    // pointer to philosopher's fork
    ForkPtr     fork() { return m_myfork; }

    //! stop pholosopher's thread
    void        reset();


protected:


    void        run();


    void        do_thinking();
    void        do_eating();

    // forks aqcuisition and releasing
    void        get_fork_1();
    void        get_fork_2();
    void        release_forks();

    // define fork ordering based of list position
    void        getForksOrder();

signals:

    void        stateChanged();
private:

    QString     m_name;
    State       m_state;

    ForkPtr     m_myfork;

    ForkPtr     m_left;
    ForkPtr     m_right;


    PhilosList  *m_philosList;

    int         m_running;
};

#endif // PHILOSOPHER_H

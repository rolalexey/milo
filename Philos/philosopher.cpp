#include "philosopher.h"
#include <QSharedPointer>
#include <QDebug>
#include <QMutex>

Philosopher::Philosopher(const QString &Name, QObject * parent) :
    QThread(parent),
    m_name(Name),
    m_state(Stopped),
    m_myfork(new QMutex()),
    m_left(nullptr),
    m_right(nullptr)

{}

Philosopher::~Philosopher()
{
    reset();

    if( m_left->try_lock() )
        m_left->unlock();
    if( m_right->try_lock() )
        m_right->unlock();

}



void Philosopher::setName(const QString &Name)
{
    m_name = Name;
}

QString Philosopher::name()
{
    return m_name;
}

Philosopher::State Philosopher::state()
{
    return  m_state;
}


/*!
 * \brief Philosopher::setState set state of Philosopher
 * \param state new state
 *
 *
 */
void Philosopher::setState(Philosopher::State state)
{
    if(m_state == state)    return;
    m_state = state;
    emit stateChanged();

}


/*!
 * \brief Philosopher::stateStr provides QString representation  of State
 * \return
 */
QString Philosopher::stateStr()
{
    QHash<int, QByteArray> stnames;
    stnames[Stopped]  = "Stopped";
    stnames[Thinking] = "Thinking";
    stnames[Waiting1] = "Waiting 1";
    stnames[Waiting2] = "Waiting 2";
    stnames[Eating]   = "Eating";

    return stnames[m_state];
}

void Philosopher::setPhilosList(PhilosList *list)
{
    m_philosList = list;
}

/*!
 * \brief Philosopher::run dinning simulation entry point. Executes in separate thread.
 */
void Philosopher::run()
{
    getForksOrder();
    while(1)
    {
        do_thinking();
        do_eating();
    }
}

/*!
 * \brief Philosopher::do_thinking - realizing Thinking phase of similation
 *
 * Thinking phase do not require any shared resources
 * and lasts 2 seconds
 */
void Philosopher::do_thinking()
{
    setState(Thinking);
    sleep(2); // time to think
}

/*!
 * \brief Philosopher::do_eating Method provides participating in dinner
 *
 *
 */
void Philosopher::do_eating()
{
    // aqcuire 1st fork
    get_fork_1();

    // aqcuire 2nd fork
    get_fork_2();

    setState (Eating);

    sleep(2);   // time for dinner

    // release 2nd fork
    // release 1st fork
    release_forks();

}

/*!
 * \brief Philosopher::get_fork_1 acquiring the first fork. Execution block
 * until fork is blocked
 */
void Philosopher::get_fork_1()
{
    setState(Waiting1);
    m_left->lock();
}

/*!
 * \brief Philosopher::get_fork_2 acquiring the second fork. Execution block
 * until fork is blocked
 */
void Philosopher::get_fork_2()
{

    setState(Waiting2);
    m_right->lock();
}


/*!
 * \brief Philosopher::release_forks  after dinner action - releasing of used forks
 */
void Philosopher::release_forks()
{
    if(m_right) m_right->unlock();
    if(m_left)   m_left->unlock();
}

void Philosopher::getForksOrder()
{
    if(!m_philosList)    reset();

    auto mypos = m_philosList->indexOf(this);

    if(mypos == m_philosList->count()-1)
    {
        // Last JEDI in list
        m_left = (*m_philosList)[0]->fork();
        m_right = (*m_philosList)[mypos]->fork();
    }
    else {
        m_left = (*m_philosList)[mypos]->fork();
        m_right = (*m_philosList)[mypos+1]->fork();
    }
}


/*!
 * \brief Philosopher::reset stop working thread
 */


void Philosopher::reset()
{
    if(isRunning()) terminate();

    if(m_state == Eating)
    {
        m_right->unlock();
        m_left->unlock();
    }
    if(m_state == Waiting2)
    {
        m_left->unlock();
    }

    setState(Stopped);
}

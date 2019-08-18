#include "dinner.h"

#include "phmodel.h"

#include <QDebug>

Dinner::Dinner() :
    QObject()
{
    m_model = new PHModel();

}

Dinner::~Dinner()
{
    delete m_model;
}

void Dinner::addItem(const QString &Name)
{
    auto ph = new Philosopher(Name);

    m_model->addItem(ph);
    ph->start();

}

void Dinner::delItem(int itemnum)
{
    Q_ASSERT( itemnum >= 0 && itemnum < m_model->rowCount() );

    stopDinner();
    m_model->delItem(itemnum);
    startDinner();
}

void Dinner::stopDinner()
{
    for(int i=0; i<m_model->rowCount(); i++)
        m_model->getItem(i)->reset();

}

void Dinner::startDinner()
{

    for(int i=0; i<m_model->rowCount(); i++)
        m_model->getItem(i)->start();

}


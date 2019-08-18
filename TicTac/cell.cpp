#include "cell.h"

#include <QtDebug>

Cell::Cell(QObject *parent) :
    QObject(parent),
    m_State(Free)

{

}

void Cell::setState(Cell::State newstate)
{
    if(m_State != Free)   return;

    m_State = newstate;
    emit        stateChanged(m_State);
    emit        nextStep();

}

void Cell::resetCell()
{
    m_State = Free;
    emit        stateChanged(m_State);

}

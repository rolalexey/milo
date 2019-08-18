#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QSharedPointer>


class Cell;


typedef     Cell *      CellPtr;


class Cell : public QObject
{
    Q_OBJECT
public:
    explicit Cell(QObject *parent = nullptr);

    enum State {
        Free = 0,
        Xed,
        Oed,
        Draw
    } ;


    State   cellState()     { return m_State; }

signals:

    void        stateChanged(Cell::State);
    void        nextStep();

public slots:

    void        setState(State newstate);
    void        resetCell();


private:

    State   m_State;

};

#endif // CELL_H

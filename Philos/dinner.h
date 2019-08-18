#ifndef DINNER_H
#define DINNER_H

#include <QObject>
#include <QMutex>


class PHModel;

/*!
 * \brief The Dinner class
 */
class Dinner : public QObject
{

    Q_OBJECT
public:

    Dinner();
    virtual ~Dinner();


    PHModel *       model() { return m_model; }

public slots:

    void    addItem( const QString & Name );
    void    delItem( int position );

    void    stopDinner();
    void    startDinner();

protected:




private:

    QList<QMutex *>   m_phForks;

    PHModel *       m_model;
};

#endif // DINNER_H

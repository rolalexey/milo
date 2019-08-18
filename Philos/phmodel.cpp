#include "phmodel.h"


#include <QDebug>
#include <QImage>



PHModel::PHModel(QObject *parent) noexcept
    :  QAbstractListModel(parent)
{}


QHash<int, QByteArray> PHModel::roleNames() const noexcept
{
    QHash<int, QByteArray> roles;
    roles[Name_Role] = "name";
    roles[State_Role] = "state";
    roles[Face_Role] = "face";
    roles[StateUrl_Role] = "stateurl";

    return roles;
}

int PHModel::rowCount(const QModelIndex &parent) const noexcept
{
    if(parent.isValid())
        return 0;

    return m_list.size();
}



QVariant PHModel::data(const QModelIndex &index, int role) const noexcept
{

    if(!index.isValid())
        return QVariant();

    Philosopher *ph = m_list.at(index.row());

    switch(role)
    {
    case Name_Role:
        return QVariant( ph->name() );

    case Face_Role:
        return QVariant( ph->name() );

    case State_Role:

        return QVariant( ph->stateStr() );

    case StateUrl_Role:
        {
        auto url = face_url(ph);
        return QVariant( url );
        }

    }

    return QVariant();

}

bool PHModel::setData(const QModelIndex &index, const QVariant &value, int role) noexcept
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

Qt::ItemFlags PHModel::flags(const QModelIndex &index) const noexcept
{
    Q_UNUSED(index);
        if (!index.isValid())
            return Qt::NoItemFlags;
    return (Qt::ItemIsEditable |
            Qt::ItemIsSelectable);
}

Philosopher *PHModel::getItem(int itemNum)
{
    Q_ASSERT( itemNum >=0 && itemNum < m_list.count() );

    return m_list[itemNum];
}



void PHModel::delItem(int num)
{
    Q_ASSERT( num >=0 && num < m_list.count() );

    beginRemoveRows(QModelIndex(), num, num);
        delete m_list[num];
        m_list.takeAt(num);
    endRemoveRows();
}


void PHModel::addItem(Philosopher *item)
{
    const int start = m_list.size();
    beginInsertRows(QModelIndex(), start, start);
        m_list.append(item);
        connect(item, SIGNAL(stateChanged()), this, SLOT(itemStateChanged()));
        item->setPhilosList(&m_list);
    endInsertRows();

}

void PHModel::itemStateChanged()
{

    Philosopher * ph = static_cast<Philosopher*>(sender());
    int row = m_list.indexOf(ph);
    emit dataChanged( index(row), index(row));
}

QString PHModel::face_url(Philosopher *ph) const
{
    QHash<Philosopher::State, QString>  furls;

    furls[Philosopher::Stopped]  = "qrc://images/think.png";
    furls[Philosopher::Thinking] = "qrc://images/think.png";
    furls[Philosopher::Waiting1]  = "qrc://images/wait1.png";
    furls[Philosopher::Waiting2]  = "qrc://images/wait2.png";
    furls[Philosopher::Eating]   = "qrc://images/eat.png";
    return furls[ ph->state() ];

}




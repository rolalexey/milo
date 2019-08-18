#include "tic_model.h"


#include <QDebug>
#include <QImage>



TicModel::TicModel(QObject *parent) noexcept
    :  QAbstractListModel(parent)
{}


QHash<int, QByteArray> TicModel::roleNames() const noexcept
{
    QHash<int, QByteArray> roles;

    roles[State_Role] = "stateurl";
    roles[Face_Role] = "face";

    return roles;
}

int TicModel::rowCount(const QModelIndex &parent) const noexcept
{
    if(parent.isValid())
        return 0;

    return m_list.size();
}



QVariant TicModel::data(const QModelIndex &index, int role) const noexcept
{

    if(!index.isValid())
        return QVariant();

    CellPtr ph = m_list.at(index.row());

    switch(role)
    {
    case State_Role:
        return QVariant( stateUrl( ph ));
    }

    return QVariant();

}

bool TicModel::setData(const QModelIndex &index, const QVariant &value, int role) noexcept
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

Qt::ItemFlags TicModel::flags(const QModelIndex &index) const noexcept
{
    Q_UNUSED(index);
        if (!index.isValid())
            return Qt::NoItemFlags;
    return (Qt::ItemIsEditable |
            Qt::ItemIsSelectable);
}

CellPtr TicModel::getItem(int itemNum)
{
//    qDebug() << "getItem:itemnum" << itemNum;
    Q_ASSERT( itemNum >=0 && itemNum < m_list.count() );

    return m_list[itemNum];
}



void TicModel::delItem(int num)
{
    Q_ASSERT( num >=0 && num < m_list.count() );

    beginRemoveRows(QModelIndex(), num, num);
        delete m_list.takeAt(num);
    endRemoveRows();
}


void TicModel::addItem(CellPtr item)
{
    const int start = m_list.size();
    beginInsertRows(QModelIndex(), start, start);
        m_list.append(item);

        connect(item, SIGNAL(stateChanged(Cell::State)), this, SLOT(itemStateChanged(Cell::State)));
        endInsertRows();
}

void TicModel::update()
{
    qDebug() << "model update";
    emit dataChanged( index(0), index(8));



}

void TicModel::itemStateChanged(Cell::State s)
{
    Q_UNUSED(s);
    CellPtr ph = static_cast<CellPtr>(sender());


    int row = m_list.indexOf(ph);

    emit dataChanged( index(row), index(row));
}

QString TicModel::stateUrl(CellPtr item) const
{
    QHash<Cell::State, QString>  furls;

    furls[Cell::Free]  = "";
    furls[Cell::Xed] = "qrc://images/x.png";
    furls[Cell::Oed]  = "qrc://images/o.png";
    return furls[ item->cellState() ];

}




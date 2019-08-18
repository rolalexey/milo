#ifndef TIC_MODEL_H
#define TIC_MODEL_H


#include <QAbstractItemModel>
#include <QMutex>
#include <QList>
#include "cell.h"

class TicModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TicModel(QObject * parent = nullptr) noexcept;

    enum {
        State_Role = Qt::UserRole,
        Face_Role
    };

    // QAbstractListModel virtual methods :                                                                                                               virtual QHash<int, QByteArray> roleNames() const noexcept override;                                                                                   int rowCount(const QModelIndex &parent = QModelIndex()) const noexcept override;                                                                      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const noexcept override;                                                          bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) noexcept override;
    virtual QHash<int, QByteArray> roleNames() const noexcept override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const noexcept override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const noexcept override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) noexcept override;
    Qt::ItemFlags flags(const QModelIndex &index) const noexcept override;

    CellPtr    getItem( int num );

    QString  stateUrl(CellPtr item) const;

public slots:

    void    delItem(int num);
    void    addItem(CellPtr item);

    void    update();

protected slots:

    void    itemStateChanged(Cell::State s);

protected:


signals:

    void stateChanged();

private:

    QList<CellPtr>  m_list;
};

#endif // TIC_MODEL_H

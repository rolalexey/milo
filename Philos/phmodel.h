#ifndef PHMODEL_H
#define PHMODEL_H


#include <QAbstractItemModel>
#include <QMutex>
#include <QList>
#include "philosopher.h"

class PHModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PHModel(QObject * parent = nullptr) noexcept;

    enum {
        Name_Role = Qt::UserRole,
        State_Role,
        StateUrl_Role,
        Face_Role
    };

    // QAbstractListModel virtual methods :                                                                                                               virtual QHash<int, QByteArray> roleNames() const noexcept override;                                                                                   int rowCount(const QModelIndex &parent = QModelIndex()) const noexcept override;                                                                      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const noexcept override;                                                          bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) noexcept override;
    virtual QHash<int, QByteArray> roleNames() const noexcept override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const noexcept override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const noexcept override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) noexcept override;
    Qt::ItemFlags flags(const QModelIndex &index) const noexcept override;

    Philosopher *   getItem( int num );

public slots:

    void    delItem(int num);
    void    addItem(Philosopher *philos);



protected slots:

    void    itemStateChanged();

protected:

    QString face_url(Philosopher * ph) const;

signals:

    void stateChanged();

private:
    PhilosList      m_list;

};

#endif // PHMODEL_H

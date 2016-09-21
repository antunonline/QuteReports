#include "bufferedquerymodel.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <algorithm>
#include <QTimer>



BufferedQueryModel::BufferedQueryModel(QObject *parent) : BufferedQueryModel(QSqlDatabase::database(), parent)
{
}

BufferedQueryModel::BufferedQueryModel(const QSqlDatabase &_db, QObject *parent) : QAbstractItemModel(parent)
{
    _query = QSqlQuery(_db);
}

BufferedQueryModel::~BufferedQueryModel()
{
    qDebug() << "Destructing BufferedQueryModel";
}

void BufferedQueryModel::setQuery(const QString &query)
{
    qDebug() << "setQuery " << query;
    _sql = query;
    _query.setForwardOnly(true);
    if(false == _query.prepare(query)){
        qDebug() << _query.lastError();
    }
}

void BufferedQueryModel::bindValue(const QString &key, const QVariant &value)
{
    qDebug() << "bindValue: " << key << ": " << value;
    _query.bindValue(key, value);
}

void BufferedQueryModel::exec()
{

    _headers.clear();
    beginRemoveColumns({}, 0, std::max(0, static_cast<int>(_headers.size()-1)));
    _rows.clear();
    endRemoveColumns();

    qDebug() << "BufferedQueryModel::exec(): beginRemoveRows";
    beginRemoveRows({}, 0, std::max(0, static_cast<int>(_rows.size()-1)));
    _rows.clear();
    endRemoveRows();

    qDebug() <<  "BufferedQueryModel::exec()";
    if(!_query.exec()){
        qDebug() << _query.lastError();
    }

    qDebug() << _query.lastQuery();

    const int colCount = _query.record().count();

    qDebug() << "BufferedQueryModel::exec(): beginInsertRows";
    while(_query.next()){
        vector<QVariant> row;
        for(int i = 0; i < colCount; i++) {
            row.push_back(_query.value(i).toString());
        }
        _rows.push_back(std::move(row));
    };


    qDebug() << "Is active: " << _query.isActive();
    qDebug() << "Query size: " << _query.size();

    beginInsertRows({},0, std::max(static_cast<int>(_rows.size()-1),0));
    endInsertRows();


    qDebug() << "BufferedQueryModel::exec(): beginInsertColumns";
    for(int i = 0; i < colCount; i++){
        auto fieldName = _query.record().fieldName(i);
        qDebug() << "Field " << i << ": " << fieldName;
        _headers.push_back(fieldName);
    }


    qDebug() << "Rows: " << rowCount({}) << " , Columns: " << columnCount({});
    qDebug() << "Rows: " << _rows.size() << " , Columns: " << _headers.size();

    int discardedRows = 0;
    while(_query.nextResult()){
        while(_query.next()) discardedRows++;
    }

    if(discardedRows>0) qDebug() << "Discarding " << discardedRows << " rows.";
    dataLoaded();


}

int BufferedQueryModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return _rows.size();
}

int BufferedQueryModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return _headers.size();
}

QVariant BufferedQueryModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return {};

    if(index.isValid()==false || _rows.size() < index.row() || _headers.size() < index.column())
        return QVariant::Invalid;


    return _rows[index.row()][index.column()];
}

QVariant BufferedQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal || section < static_cast<int>(_headers.size())){
        if(Qt::DisplayRole == role)
            return _headers[section];
        else {
            return QVariant::Invalid;
        }
    } else  {
        return {};
    }
}

QModelIndex BufferedQueryModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.internalId() == 1)
        return {};
    (void)parent;
    return createIndex(row, column, 1);
}

QModelIndex BufferedQueryModel::parent(const QModelIndex &child) const
{
    (void) child;
    return  {};
}

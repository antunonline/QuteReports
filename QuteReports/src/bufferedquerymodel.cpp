#include "bufferedquerymodel.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <algorithm>
#include <QTimer>

std::atomic_int BufferedQueryModel::_conn_count(0);

BufferedQueryModel::BufferedQueryModel(QObject *parent) : QAbstractItemModel(parent)
{
    connect(this, &BufferedQueryModel::execFinished, this, &BufferedQueryModel::onExecFinished);
    auto new_conn_name = QString("BufferedQueryModel") + _conn_count.fetch_add(1);
    _db = QSqlDatabase::cloneDatabase(QSqlDatabase::database(), new_conn_name);
    _db.open();
    _query = QSqlQuery(_db);
    _conn_count.fetch_add(1);
}

BufferedQueryModel::~BufferedQueryModel()
{
    qDebug() << "Closing " << _db.connectionName();
    _db.close();
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

void BufferedQueryModel::bindSubquery(const QString &key, const QString &sql, const QString &subqueryKey)
{

}

void BufferedQueryModel::exec()
{
    if(_refCount.fetch_add(1) == 0) // It's already deleted
        return;

    std::unique_lock<std::mutex> auto_lock(_mtx);

    _thread = std::thread([this](){{
            std::unique_lock<std::mutex> auto_lock(_mtx);

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
        }


        if(_refCount.fetch_sub(1)==1){
            delete this;
            return;
        }
        execFinished();

    });
    _thread.detach();

    qDebug() << "Is active: " << _query.isActive();
    qDebug() << "Query size: " << _query.size();

    qDebug() << "BufferedQueryModel::exec(): beginRemoveColumns";
     _headers.clear();



    qDebug() << "BufferedQueryModel::exec(): beginRemoveRows";
    _rows.clear();

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

void BufferedQueryModel::deleteLater()
{
    if(_refCount.fetch_sub(1)==1){
           delete this;
    }
}

void BufferedQueryModel::onExecFinished()
{
    std::unique_lock<std::mutex> auto_lock(_mtx);
    const int colCount = _query.record().count();

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

#include "qmlsqlquerymodel.h"
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

void QmlSqlQueryModel::generateRoles()
{
    if(_roles.size() > 0)
        return;
    const int colCount = record().count();

    for(int i = 0; i < colCount; i++){
        _role_names[Qt::UserRole+1+i] = record().fieldName(i).toUtf8();
    }
}

QmlSqlQueryModel::QmlSqlQueryModel()
{
    _dbName = QSqlDatabase::defaultConnection;
}

QString QmlSqlQueryModel::sqlQuery() const
{
    if(_parametrizedQuery.type() == QVariant::Invalid)
        return "";
    return _parametrizedQuery.toList().first().toString();
}

void QmlSqlQueryModel::setSqlQuery(const QString &query)
{
    _parametrizedQuery = {
        QList<QVariant>{QVariant(query)}
    };
}

QString QmlSqlQueryModel::dbName() const
{
    return _dbName;
}

void QmlSqlQueryModel::setDbName(const QString &dbName)
{
    _dbName = dbName;
}

QVariant QmlSqlQueryModel::parametrizedQuery() const
{
    return _parametrizedQuery;
}

void QmlSqlQueryModel::setParametrizedQuery(const QVariant &q)
{
    _parametrizedQuery = q;
}

void QmlSqlQueryModel::setColumnNames(const QVariant &sl)
{
    _roles = sl.toStringList();
    for(int i = 0; i < _roles.size(); i++){
        _role_names[Qt::UserRole+i+1] = _roles[i].toUtf8();
    }
}

QVariant QmlSqlQueryModel::columnNames() const
{
    return QVariant(_roles);
}

void QmlSqlQueryModel::exec()
{

    auto queryParams = _parametrizedQuery.toList();
    QSqlQuery query(sqlQuery(), QSqlDatabase::database(dbName()));
    for(auto from = queryParams.begin()+1; from != queryParams.end(); from++){
        auto bindValue = from->toList();
        if(bindValue.size() != 2) continue;
        query.bindValue(bindValue.first().toString(), bindValue.last());
    }

    _sqlQuery = query;

    setQuery(_sqlQuery);
    qDebug() << _sqlQuery.lastError();
    generateRoles();
}

QHash<int, QByteArray> QmlSqlQueryModel::roleNames() const {
    return _role_names;
}

QVariant QmlSqlQueryModel::data(const QModelIndex &index, int role) const{
    if(role < Qt::UserRole){
        return QSqlQueryModel::data(index, role);
    } else {
        const int colId = role - 1 - Qt::UserRole, rowId = index.row();
        return QSqlQueryModel::data(createIndex(rowId, colId), Qt::DisplayRole);
    }
}

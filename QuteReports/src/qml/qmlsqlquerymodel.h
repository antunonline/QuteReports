#ifndef QMLSQLQUERYMODEL_H
#define QMLSQLQUERYMODEL_H
#include <QString>
#include <QSqlQueryModel>
#include <QHash>
#include <QVariant>
#include <QSqlQuery>

class QmlSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
    QString  _dbName;
    QSqlQuery _sqlQuery;
    QHash<int, QByteArray> _role_names;
    QVariant _parametrizedQuery{QVariant::Invalid};
    QStringList _roles;

    Q_PROPERTY(QString dbName READ dbName WRITE setDbName)
    Q_PROPERTY(QString sqlQuery READ sqlQuery WRITE setSqlQuery)
    Q_PROPERTY(QVariant parametrizedQuery READ parametrizedQuery WRITE setParametrizedQuery)
    Q_PROPERTY(QVariant columnNames READ columnNames WRITE setColumnNames)
protected:
    void generateRoles();
public:
    QmlSqlQueryModel();
    QString sqlQuery() const;
    void setSqlQuery(const QString & query);
    QString dbName() const;
    void setDbName(const QString & dbName);
    QVariant parametrizedQuery() const;
    void setParametrizedQuery(QVariant const &q);
    void setColumnNames(QVariant const & sl);
    QVariant columnNames() const;
    Q_INVOKABLE void exec();


    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

};

#endif // QMLSQLQUERYMODEL_H

#ifndef BUFFEREDQUERYMODEL_H
#define BUFFEREDQUERYMODEL_H

#include <QAbstractItemModel>
#include <QSqlQuery>
#include  <vector>
#include <QObject>
#include <thread>
#include <mutex>
#include <QSqlDatabase>
#include <atomic>
using std::vector;

class BufferedQueryModel  : public QAbstractItemModel
{
    Q_OBJECT
    QString _sql;
    QSqlQuery _query;
    vector<vector<QVariant>> _rows;
    vector<QVariant> _headers;
    QSqlDatabase _db;

public:
    BufferedQueryModel(QObject * parent = 0);
    BufferedQueryModel(const QSqlDatabase & _db, QObject * parent = 0);
    virtual ~BufferedQueryModel();

    void setQuery(const QString & query);
    void bindValue(const QString & key, QVariant const & value);
    void bindSubquery(const QString & key, const QString & sql, const QString & subqueryKey);
    void exec();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;


signals:
    void dataLoaded();

private:
signals:


private slots:

};

#endif // BUFFEREDQUERYMODEL_H

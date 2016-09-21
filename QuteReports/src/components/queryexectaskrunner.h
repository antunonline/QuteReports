#ifndef QUERYEXECTASKRUNNER_H
#define QUERYEXECTASKRUNNER_H
#include <thread>
#include <future>
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include <atomic>

#include <QObject>
#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "src/bufferedquerymodel.h"

using std::vector;
using std::packaged_task;
using std::future;
using std::thread;
using std::pair;
using std::map;
using std::tuple;
using std::shared_ptr;

typedef packaged_task<QVariant(QString const&, const QVariant &)> SubqueryPackagedTask;

typedef vector<tuple<QString, QString, QVariant>> Subqueries;

class QueryExecTaskRunner : public QObject
{
    Q_OBJECT
    std::thread _thread;
    Subqueries _subqueries;
    map<QString, QVariant> _bindValues;
    QSqlDatabase _db;
    shared_ptr<BufferedQueryModel> _queryModel;
    QString _query;
    std::atomic_int _ref_count{1};

public:
    QueryExecTaskRunner();
    QueryExecTaskRunner(QSqlDatabase const & _db);
    virtual ~QueryExecTaskRunner();
    void addSubquery(const QString modelQueryKey ,const QString &query, const QVariant &input);
    void bindValue(const QString&k, const QVariant &v);
    void setQuery(const QString &query);
    void exec();
    shared_ptr<BufferedQueryModel> model();
    void deleteLater();

signals:
    void done();
};

#endif // QUERYEXECTASKRUNNER_H

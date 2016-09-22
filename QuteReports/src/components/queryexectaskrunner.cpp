#include "queryexectaskrunner.h"
#include <QDebug>

QueryExecTaskRunner::QueryExecTaskRunner() : QueryExecTaskRunner(QSqlDatabase::database())
{

}

QueryExecTaskRunner::QueryExecTaskRunner(QSqlDatabase const & _db) : _db{_db}
{

}

QueryExecTaskRunner::~QueryExecTaskRunner()
{
    _db.close();
}

void QueryExecTaskRunner::addSubquery(const QString modelBindName ,const QString &query, const QVariant &input)
{
    _subqueries.emplace_back(modelBindName, query, input);
}

void QueryExecTaskRunner::bindValue(const QString &k, const QVariant &v)
{
    _bindValues[k] = v;
}

void QueryExecTaskRunner::setQuery(const QString &query)
{
    _query = query;
}

void QueryExecTaskRunner::exec()
{
    _ref_count.fetch_add(1);

    _queryModel = shared_ptr<BufferedQueryModel>(new BufferedQueryModel(_db));
    _thread = std::thread([this](){

        // Run all subqueries
        QSqlQuery q(_db);
        for(auto const & sq : _subqueries){
            q.prepare(std::get<1>(sq));
            q.bindValue(0, std::get<2>(sq));
            if(q.exec() && q.next()){
                bindValue(std::get<0>(sq), q.value(0));
            }
            qDebug() << q.lastQuery() ;
            qDebug() << q.lastError();
        }

        // Set query
       _queryModel->setQuery(_query);

        // Bind model values
       for(auto const & kv : _bindValues)
           _queryModel->bindValue(kv.first, kv.second);

       // Execute model
       _queryModel->exec();

       if(_ref_count.fetch_sub(1)==1){
           delete this;
           return;
       }

       // Emit signal
       emit done();
    });
    _thread.detach();
}

shared_ptr<BufferedQueryModel> QueryExecTaskRunner::model()
{
    return _queryModel;
}

void QueryExecTaskRunner::deleteLater()
{
    if(_ref_count.fetch_sub(1)==1){
        delete this;
    }
}

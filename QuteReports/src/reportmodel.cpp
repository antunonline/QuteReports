#include "reportmodel.h"
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QSqlError>;


ReportModel::ReportModel()
{

}

Report ReportModel::fetchById(const int id)
{
    QSqlQuery query;
    query.prepare("select id, name, description, type, data from reports where id = ?");
    query.bindValue(0, id);
    query.exec();

    if(query.next()){

        auto id = query.value("id").toInt();
        auto name = query.value("name").toString();
        auto description = query.value("description").toString();
        auto type = query.value("type").toString();
        auto data = query.value("data").toString();

        return Report(id, name, description, type, data);
    } else{
        return Report();
    }
}

bool ReportModel::update(const Report &r)
{
    if(r.id() == 0)
        return false;

    QSqlQuery query;
    query.prepare("update reports set name = :name, description = :description, type = :type, data = :data where id = :id");
    query.bindValue(":name", r.name());
    query.bindValue(":description", r.description());
    query.bindValue(":type", r.type());
    query.bindValue(":data", r.data());
    query.bindValue(":id", r.id());

    query.exec();
    qDebug() << query.lastError();
    return true;
}

int ReportModel::insert(const Report &r)
{
    if(r.id() != 0)
        return 0;

    QSqlQuery query;
    query.prepare("insert into reports (name, description, type, data) values( :name, :description, :type, :data)");
    query.bindValue(":name", r.name());
    query.bindValue(":description", r.description());
    query.bindValue(":type", r.type());
    query.bindValue(":data", r.data());
    query.bindValue(":id", r.id());

    query.exec();
    qDebug() << query.lastError();
    if(query.lastInsertId() != QVariant::Invalid) {
        auto id = query.lastInsertId().toInt();
        return id;
    }
    else
        return 0;
}

void ReportModel::remove(const Report &r)
{
    if(r.id() == 0)
        return ;

    QSqlQuery query;
    query.prepare("delete from  reports where id = :id)");
    query.bindValue(":id", r.id());

    query.exec();
    qDebug() << __FILE__ << __FUNCTION__ << query.lastError();
}

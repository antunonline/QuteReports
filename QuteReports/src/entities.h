#ifndef ENTITIES_H
#define ENTITIES_H
#include <QString>

class Report{
    int _id = 0;
     QString _name, _description, _type, _data;
public:
     Report(){}
     Report(const QString & name, const QString & description, const QString & type, const QString & data) :
         _name{name}, _description{description}, _type{type}, _data{data} {}
     Report(const int id, const QString & name, const QString & description, const QString & type, const QString & data) :
         _id{id}, _name{name}, _description{description}, _type{type}, _data{data} {}
     QString name() const {return _name;}
     QString description() const {return _description;}
     QString type() const {return _type;}
     QString data() const {return _data;}
     int id() const {return _id;}
     Report setId(const int id) const { return {id, _name, _description, _type, _data}; }
};


#endif // ENTITIES_H

#include <QObject>
#include "objecthelper.h"
#include <string>
#include <iostream>


#ifndef PROPERTYHELPER_H
#define PROPERTYHELPER_H

// hash that is is passed in is <QString, QString>
// key the property name:     someProperty
// value the asoperator  :    int


class PropertyHelper : public ObjectHelper {
    Q_OBJECT

public:
    explicit PropertyHelper(const QHash<QString, QString> &hash ) :
        m_hash(hash)
       , m_iter( m_hash )
    { }
    void setHash( const QHash<QString, QString> &hash  )
    {
        m_hash.clear();
        m_hash = hash;
        m_iter = m_hash;
    }

    std::__cxx11::string qPropertyString()
    {
        QString ret;
        m_iter.toFront();
        while( m_iter.hasNext() )
        {
            m_iter.next();
            ret += QString( "\n    Q_PROPERTY( %1 %2 READ %2 WRITE set%3 NOTIFY %2Changed )" )
                    .arg( m_iter.value() )
                    .arg( m_iter.key() )
                    .arg( capFirstLetter( m_iter.key() ) );
        }
        return ret.toStdString();
    }


    std::__cxx11::string getterSetterString()
    {
        QString ret;
        m_iter.toFront();
        while( m_iter.hasNext() )
        {
            m_iter.next();
            ret +=  QString( "\n%1 %2() const "
                             "\n{"
                             "\n    return m_%2;"
                             "\n}\n" )
                    .arg( m_iter.value() )
                    .arg( m_iter.key() );

            ret +=  QString( "\nvoid set%3( const %2 &%1 )"
                             "\n{"
                             "\n   if( m_%1 != %1 )"
                             "\n   {"
                             "\n       m_%1 = %1;"
                             "\n       emit %1Changed();"
                             "\n   }"
                             "\n}\n" )
                    .arg( camelCase( m_iter.key() ) )
                    .arg( m_iter.value() )
                    .arg( capFirstLetter( m_iter.key() ) );
        }
        return ret.toStdString();
    }

    // Fixme make a override here is need be on signals and what is getting
    // passed arround.
    std::__cxx11::string signalsString(  const QString &type = "void" )
    {
        QString ret;
        m_iter.toFront();
        while( m_iter.hasNext() )
        {
            m_iter.next();
            ret += QString( "\n    %2 %1Changed();" )
                    .arg( camelCase( m_iter.key() ) )
                    .arg( type );
        }
        return ret.toStdString();
    }



    std::__cxx11::string privateString()
    {
        QString ret;
        m_iter.toFront();
        while( m_iter.hasNext() )
        {
            m_iter.next();
            ret +=  QString( "\n    %1 m_%2;" )
                    .arg( m_iter.value() )
                    .arg( camelCase( m_iter.key()) );
        }
        return ret.toStdString();
    }
    void test()
    {
        //        QHash<QString, QString> m_prop_hash;
        //        QString type = "QString";
        //        QString name = "buses";
        //        qDebug() << "PROPERTY " << QString::fromStdString( qPropertyString( type, name ).c_str() );
        //        qDebug() << "GETTER " << QString::fromStdString( getterString( type , name ).c_str() ) ;
        //        qDebug() << "SETTER " << QString::fromStdString( setterString( type, name ).c_str() );
        //        qDebug() << "SIGNALS " << QString::fromStdString( signalsString( name ).c_str() ) ;
        //        qDebug() << "PRIVATE " << QString::fromStdString( privateString( type, name ).c_str() );
    }

private:
    QHash<QString, QString> m_hash;
    QHashIterator<QString, QString> m_iter;
};

#endif // PROPERTYHELPER_H

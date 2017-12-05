#include <QObject>
#include "objecthelper.h"
#include <string>
#include <iostream>

#ifndef ENUMGENHELPER_H
#define ENUMGENHELPER_H
class EnumHelper : public ObjectHelper {
    Q_OBJECT
public:
    explicit EnumHelper( const QHash<int, QPair<QString, QStringList >> &hash ) :
        m_hash( hash )
      , m_iter( m_hash )
    { }
    void setHash( const QHash<int, QPair<QString, QStringList >> &hash )
    {
        if( !m_hash.isEmpty() )
        {
            m_hash.clear();
        }
        m_hash = hash;
        m_iter = m_hash;
    }
    std::__cxx11::string enumPropertyString()
    {
        QString ret;
        m_iter.toFront();
        while ( m_iter.hasNext() )
        {
            m_iter.next();
            ret += QString( "\n    Q_ENUMS( %1 )" )
                    .arg( m_iter.value().first ) ;
        }
        return ret.toStdString();
    }

    std::__cxx11::string enumDefentionString()
    {
        QString ret;
        m_iter.toFront();
        while (m_iter.hasNext() )
        {
            m_iter.next();
            int tempEnum = 0;
            QPair<QString, QStringList> enum_pair;
            enum_pair = m_iter.value();
            ret += QString("\n     enum %1{")
                    .arg(enum_pair.first);

            foreach( QString l, enum_pair.second )
            {
                if( tempEnum != ( enum_pair.second.size() - 1 ))
                {

                    ret += QString( "\n        %1 = %2,")
                            .arg( l )
                            .arg( QString::number( tempEnum )
                                  );
                }
                else
                {
                    ret += QString( "\n        %1 = %2")
                    .arg( l )
                    .arg( QString::number( tempEnum ) );
                }
                tempEnum++;
            }
            ret += "\n    };";
        }
        return ret.toStdString();
    }

private:
    QHash<int, QPair<QString, QStringList >> m_hash;
    QHashIterator<int, QPair<QString, QStringList > > m_iter;
};

#endif // ENUMHELPER_H

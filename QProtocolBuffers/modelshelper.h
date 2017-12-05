#include "objecthelper.h"
#ifndef MODELSHELPER_H
#define MODELSHELPER_H

// Hash
// key = counter int
// value = Name of Object to pass in

class ModelsHelper : public ObjectHelper
{
public:
    ModelsHelper( const QHash<int, QString> &hash ) :
        m_hash( hash )
      ,m_iter( m_hash )
    {}
    void setHash( const QHash<int, QString> &hash)
    {
        if( !m_hash.isEmpty() )
        {
            m_hash.clear();
        }
        m_hash = hash;
        m_iter = m_hash;
    }
    std::__cxx11::string modelString()
    {
        QString ret;
        m_iter.toFront();
        while ( m_iter.hasNext() )
        {
            m_iter.next();
            ret +=  QString( "\nclass %1Model : public QQmlObjectListModel<%1> { };" )
                    .arg( m_iter.value() );
        }
        return ret.toStdString();
    }

private:
    QHash<int, QString> m_hash;
    QHashIterator<int, QString> m_iter;
};

#endif // MODELSHELPER_H

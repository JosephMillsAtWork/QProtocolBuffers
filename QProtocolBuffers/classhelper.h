#include "objecthelper.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>



#ifndef CLASSHELPER_H
#define CLASSHELPER_H
using namespace GOOGLE_NAMESPACE::protobuf;
class ClassHelper : public ObjectHelper {
public:
    explicit ClassHelper( const FileDescriptor *file ) :
        m_file( file )
    { }

    std::string includesString()
    {
        std::string ret;
        // FIXME get default headers
        ret = "#include <QObject>\n";
        ret += "#include \"QQmlObjectListModel.h\" \n \n" ;

        for( int i = 0; i < m_file->dependency_count(); i++ )
        {
            std::string d = m_file->dependency( i )->name();
            std::string path = stripProto( d );

            QString camelPath = QString::fromStdString( path.c_str() );
            QString includePath = camelPath.append( ".pb.h" ).toLower() ;
            ret += "\n#include \"";
            ret +=  includePath.toStdString() ;
            ret +=  "\"\n" ;
        }
        return ret;
    }


    std::string namespaceString( const std::string &packageName )
    {
        // namespace
        QString ret = QString::fromStdString( packageName );
        ret.replace(" ","");
        ret.replace(".","::");
        return QString( "\nnamespace %1 {" )
                .arg( ret )
                .toStdString() ;
    }
    std::string ifDefString( const std::string  &ifDefName ) const
    {
        return QString( "\n#ifndef %1\n#define %1" )
                .arg( QString::fromStdString( ifDefName ) ).toStdString();
    }
    std::string classString( const std::string  &classStr ) const
    {
        return QString( "\nclass %1 : public QObject \n{" )
                .arg( QString::fromStdString( classStr ) )
                .toStdString();
    }
    std::string classInfoString()const
    {
        return "\n    Q_OBJECT"
               "\n    Q_CLASSINFO( \"autor\", \"\" )"
               "\n    Q_CLASSINFO( \"url\", \"\" )" ;
    }
    // create the public explicit class
    std::string explicitClassString( const QString &className )
    {
        return QString("\npublic: "
                       "\n    explicit %1(" ).arg(className).toStdString();
    }
    // close the public explicit class
    std::string closeExplicitClassString()
    {
        return "\n    {}" ;
    }

    // This is for properties
    std::string constuctorString( const QHash<QString, QString> m_hash) const
    {
        QString ret;
        int commaHelper = 0;

        QHashIterator<QString, QString> iter(m_hash);
        while( iter.hasNext() )
        {
            iter.next();
            if( commaHelper == 0 )
            {
                ret += QString ("\n        const %1 &%2")
                        .arg( iter.value() )
                        .arg( iter.key() );
            }
            else
            {
                ret += QString ("\n        ,const %1 &%2")
                        .arg( iter.value() )
                        .arg( iter.key() );
            }
            commaHelper++;
        }

        ret += "\n    ):";

        commaHelper = 0;
        QHashIterator< QString, QString > iterTwo( m_hash );
        while( iterTwo.hasNext() )
        {
            iterTwo.next();
            if( commaHelper == 0 )
            {
                ret += QString("\n        m_%1( %1 )" )
                        .arg( iterTwo.key() );
            }
            else
            {
                ret += QString("\n        ,m_%1( %1 )" )
                        .arg( iterTwo.key() );
            }
            commaHelper++;
        }

        return ret.toStdString();
    }

    std::string closeClassString() const
    {
        return "\n};";
    }
    std::string closeIfDefString( const QString &className )
    {
        return QString( "\n#endif // %1" )
                .arg( classToIfDef( className ) )
                .toStdString();
    }
    std::string closeNameSpaceStirng() const
    {
        return "\n}";
    }

private :
    const FileDescriptor *m_file;
};

#endif // CLASSHELPER_H

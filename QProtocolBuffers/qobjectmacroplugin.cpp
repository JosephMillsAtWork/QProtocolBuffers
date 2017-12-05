#include "qobjectmacroplugin.h"


using namespace google::protobuf;
//using namespace std::__cxx11;
using namespace std;
bool QObjectGenerator::Generate( const FileDescriptor *file,
                                      const string &parameter,
                                      compiler::GeneratorContext *generator_context,
                                      string *error ) const
{
    try
    {
        string headerPath = QString::fromStdString(
                    compiler::cpp::StripProto( file->name() ) )
                .toLower()
                .append( ".pb.h" )
                .toStdString();



        //        souresPath.append( ".cpp" );
        QScopedPointer<io::ZeroCopyOutputStream> m_headerStream(
                    generator_context->Open( headerPath ) );
        io::Printer p( m_headerStream.data(), '$' );

        //        QScopedPointer<io::ZeroCopyOutputStream> m_sourcesStream(
        //                    generator_context->Open( sourcesPath ) );
        //        io::Printer sPrinter( m_sourcesStream.data(), '$' );
        //        Q_UNUSED( sPrinter );

        QCodeGenerator qt_codeGen( file );
        qt_codeGen.generateCode( p );
        return true;
    }
    catch( std::exception &ex )
    {
        *error = ex.what();
        qCritical() << __PRETTY_FUNCTION__ << ex.what() ;
        return false;
    }
}

QCodeGenerator::QCodeGenerator( const FileDescriptor *file )
    :
      p_fileDesc( file )
{
    if( !file )
    {
        qCritical() << __PRETTY_FUNCTION__ << "null ptr on FileDescriptor";
    }
    else
    {
        // handle file
        m_classHelper = QSharedPointer<ClassHelper>( new ClassHelper( p_fileDesc ) );
        m_propertyHelper = QSharedPointer<PropertyHelper>( new PropertyHelper( m_propertyHash ) );
        m_enumHelper = QSharedPointer<EnumHelper>( new EnumHelper( m_enumHash ) );
        m_modelsHelper = QSharedPointer<ModelsHelper>( new ModelsHelper( m_modelHash ) );
    }
}
QCodeGenerator::~QCodeGenerator()
{
    if( m_classHelper.data() )
    {
        m_classHelper.clear();
    }
    if ( m_propertyHelper.data() )
    {
        m_propertyHelper.clear();
    }
    if( m_enumHelper.data() )
    {
        m_enumHelper.clear();
    }
    if( m_modelsHelper.data() )
    {
        m_modelsHelper.clear();
    }
}

void QCodeGenerator::generateCode( io::Printer &p )
{

    // fixme Generated String
    p.Print( "/*\n This file was generated by Synexxus Code\n"
             " Generated on $now_time$"
             "\n*/"
             "\n\n",
             "now_time", QDateTime::currentDateTime().toString( Qt::ISODate ).toStdString().c_str()
             );

    //FIXME Get Lic


    p.Print( m_classHelper->includesString().c_str() );
    for( int i = 0 ; i < p_fileDesc->extension_count(); i++ )
    {
        qDebug() << "file_->extension_count()"
                 << p_fileDesc->extension( i )->DebugString().data();
    }

    for( int i = 0; i < p_fileDesc->message_type_count(); i++ )
    {
        for( int q = 0 ; q < p_fileDesc->message_type( i )->nested_type_count() ; q++ )
        {
            setPackageName( QString::fromStdString( p_fileDesc->package() ) );
            setIfDefName( QString::fromStdString( p_fileDesc->message_type( i )->nested_type( q )->name().c_str() ) );
            generateMessage( p,  p_fileDesc->message_type( i )->nested_type( q ) );
        }
        setPackageName( QString::fromStdString( p_fileDesc->package() ) );
        setIfDefName( QString::fromStdString( p_fileDesc->message_type( i )->name().c_str() ) );
        generateMessage( p, p_fileDesc->message_type( i ) );
    }



    // Add our models foreach message that has a repeted field
    if ( m_modelHash.size() > 0 )
    {
        p.Print( m_classHelper.data()->namespaceString( getPackageName() ).c_str() );
        p.Print( m_modelsHelper.data()->modelString().c_str() );
        p.Print("\n}");
    }
}

void QCodeGenerator::generateMessage( io::Printer &p, const Descriptor *t )
{
    if( !t )
    {
        throw std::invalid_argument( "Null descriptor" );
    }


    // clear our models, but we do not clean out our models
    // as they go all the way at the end of the file.
    if( !m_propertyHash.isEmpty() )
    {
        m_propertyHash.clear();
    }
    if( !m_enumHash.isEmpty() )
    {
        m_enumHash.clear();
    }

    
    // enums in message
    for( int i = 0; i < t->enum_type_count() ; i++ )
    {
        QString enum_name = QString::fromStdString( t->enum_type( i )->name().c_str() );
        QStringList enum_values;
        enum_values.clear();
        
        for( int u = 0; u < t->enum_type( i )->value_count(); u++ )
        {
            enum_values << QString::fromStdString(
                               t->enum_type( i )->value( u )->name()
                               );
        }
        QPair<QString, QStringList > enum_pair;
        enum_pair.first = enum_name;
        enum_pair.second = enum_values;
        m_enumHash.insert( i , enum_pair );
    }
    m_enumHelper.data()->setHash( m_enumHash );

    // properties in each message
    for( int i = 0; i < t->field_count(); i++ )
    {
        QString camelName = QString::fromStdString(
                    t->field( i )->camelcase_name() );
        QString qtType = toStringType(
                    t->field( i )->cpp_type(),
                    t->field( i )
                    );


        if( t->field(i)->cpp_type() != FieldDescriptor::CPPTYPE_MESSAGE )
        {
            m_propertyHash.insert( camelName, qtType );
        }
        else
        {
            if( t->field(i)->is_repeated() )
            {
                if( !camelName.isEmpty() )
                {
                    m_modelHash.insert( ( m_modelHash.size() +1 ) , qtType );
                }
            }
            else
            {
                m_propertyHash.insert( camelName, qtType );
            }
        }
        // Fill the properties hash
    }
    m_propertyHelper.data()->setHash( m_propertyHash );
    m_modelsHelper.data()->setHash( m_modelHash );

    //
    if( !m_propertyHash.isEmpty() )
    {
        // namespace
        p.Print( m_classHelper.data()->namespaceString(
                     m_packageName
                     ).c_str() );

        // ifndef class name
        p.Print( m_classHelper.data()->ifDefString( getIfDefName() ).c_str() );

        // create class
        p.Print( m_classHelper.data()->classString( t->name() ).c_str() );
        // Area that is not 100% defined. Will be used for Q_CLASSINFO macros
        p.Print( m_classHelper.data()->classInfoString().c_str() );

        // add the protoced macros Q_PROPERTY( type , name ,READ .....)
        // warning this is a WRITE only So far there is no way for read only..
        p.Print( m_propertyHelper.data()->qPropertyString( ).c_str() );
        p.Print( m_enumHelper->enumPropertyString().c_str() );

        p.Print( m_classHelper.data()->explicitClassString( QString::fromStdString( t->name() )
                                                            ).c_str()
                 );

        p.Print( m_classHelper.data()->constuctorString( m_propertyHash).c_str() );
        // close out the Object class

        p.Print( m_classHelper.data()->closeExplicitClassString().c_str() );


        // PUBLIC
        p.Print( m_enumHelper.data()->enumDefentionString().c_str() );
        p.Print( m_propertyHelper.data()->getterSetterString().c_str() );

        // SIGNALS
        p.Print("\nsignals:");
        p.Print( m_propertyHelper.data()->signalsString( ).c_str() ) ;

        // PROTOCTED SLOTS
        p.Print("\nprotected slots:");

        // PRIVATE
        p.Print("\nprivate:");
        p.Print( m_propertyHelper.data()->privateString( ).c_str() );


        // Close the class
        p.Print( m_classHelper.data()->closeClassString().c_str() );
        p.Print( m_classHelper.data()->closeIfDefString(
                     QString::fromStdString( t->name() )
                     ).c_str()
                 );

        // end namespace
        p.Print( m_classHelper.data()->closeNameSpaceStirng().c_str() );

        // EOF
        p.Print( "\n" );
    }
}

QString QCodeGenerator::toStringType( FieldDescriptor::CppType cppType
                                      , const FieldDescriptor *fDesc )
{
    QString qtType;
    bool isRep = fDesc->is_repeated();
    if( cppType == FieldDescriptor::CPPTYPE_INT32 )
    {
        isRep ? qtType = "QList<int>" : qtType = "int" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_INT64 )
    {
        isRep ? qtType = "QList<int>" : qtType = "int" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_UINT32 )
    {
        isRep ? qtType = "QList<int>" : qtType = "int" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_UINT64 )
    {
        isRep ? qtType = "QList<int>" : qtType = "int" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_DOUBLE )
    {
        isRep ? qtType = "QList<double>" : qtType = "double" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_FLOAT )
    {
        isRep ? qtType = "QList<float>" : qtType = "float" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_BOOL )
    {
        isRep ? qtType = "QList<bool>" : qtType = "bool" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_STRING )
    {
        isRep ? qtType = "QStringList" : qtType = "QString" ;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_ENUM )
    {
        QString enumName = QString::fromStdString( fDesc->enum_type()->name() );
        isRep ? qtType = QString( "QList<%1>" ).arg( enumName ) :  qtType = enumName;
    }
    else if( cppType == FieldDescriptor::CPPTYPE_MESSAGE )
    {
        qtType = QString::fromStdString( fDesc->message_type()->name() );
    }
    return qtType;
}

string QCodeGenerator::getIfDefName()
{
    return  m_ifdefName;
}

void QCodeGenerator::setPackageName( QString name )
{
    if( m_packageName != name.replace( ".", "::" ).toStdString() )
    {
        m_packageName = name.replace( ".", "::" ).toStdString();
    }
}

string QCodeGenerator::getPackageName()
{
    return m_packageName;
}

void QCodeGenerator::setIfDefName( const QString &name )
{
    string tmpIfDef = name.toUpper().toStdString();
    m_ifdefName = tmpIfDef;
}
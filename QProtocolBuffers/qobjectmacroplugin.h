#include <QByteArray>
#include <QStringList>
#include <QHash>
#include <QHashIterator>
#include <QPair>
#include <QString>
#include <QDateTime>
#include <QDebug>


#include "classhelper.h"
#include "propertyhelper.h"
#include "enumgen.h"
#include "modelshelper.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/printer.h>

#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>


// 1 get the file and setup the FileDescriptor Done
// 2 create all the helper classes
// 3 parse the messages ect and insert into hash's
// 4 parse the hash's with the helper clases while using Printer
// 4 close everything up.

#include <memory>
#include <stdexcept>
using namespace std::__cxx11;

namespace google {
namespace protobuf {
class DescriptorPool;
class Descriptor;
class FileDescriptor;
namespace io {
class Printer;
}
}
}

#ifndef QOBJECTGENERATOR_H
#define QOBJECTGENERATOR_H
using namespace google::protobuf;
class QObjectGenerator : public compiler::CodeGenerator {
public:
    virtual bool Generate(
        const FileDescriptor *file,
        const string &parameter,
        compiler::GeneratorContext *generator_context,
        string *error
    ) const override;
private:

};
#endif // QOBJECTGENERATOR_H


class QCodeGenerator {
public:
    QCodeGenerator( const FileDescriptor *file );
    ~QCodeGenerator();
    void generateCode( io::Printer & );
    void generateMessage( io::Printer &,
                          const Descriptor * );

    QString toStringType( FieldDescriptor::CppType cppType
                          , const FieldDescriptor *fDesc );
    
    void setIfDefName( const QString &name );
    string getIfDefName( );
    void setPackageName( QString name );
    string getPackageName( );

private:
    const FileDescriptor *p_fileDesc;
    string m_ifdefName;
    string m_packageName;

    QHash<QString, QString> m_propertyHash;
    QHash<int, QPair<QString, QStringList >> m_enumHash;
    QHash<int, QString> m_modelHash;

    QSharedPointer<ClassHelper> m_classHelper;
    QSharedPointer<PropertyHelper> m_propertyHelper;
    QSharedPointer<EnumHelper> m_enumHelper;
    QSharedPointer<ModelsHelper> m_modelsHelper;
};

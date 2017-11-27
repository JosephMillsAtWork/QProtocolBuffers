#include <QByteArray>
#include <QStringList>
#include <QHash>
#include <QHashIterator>
#include <QPair>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QPair>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>


#include <memory>
#include <stdexcept>
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

#ifndef QOBJECTMACRO_GENERATOR_H
#define QOBJECTMACRO_GENERATOR_H
using namespace google::protobuf;
class QObjectMacroGenerator : public compiler::CodeGenerator {
public:
    virtual bool Generate(
        const FileDescriptor *file,
        const std::string &parameter,
        compiler::GeneratorContext *generator_context,
        std::string *error
    ) const override;




};
#endif // QOBJECTMACRO_GENERATOR_H



class FileGenerator {
public:
    FileGenerator( const FileDescriptor *file );

    void generateObjMacroFile( io::Printer & );
    void generateMessage( io::Printer &,
                          const Descriptor * );
    QString toQtType( FieldDescriptor::CppType cppType );

    void setIfDefName( const QString &name );
    std::string getIfDefName( );
    void setPackageName( QString name );
    std::string getPackageName( );

private:
    const FileDescriptor *file_;
    std::string m_ifdefName;
    std::string m_packageName;
    QHash<QString, QString> m_writableMacros;
    QHash<int, QPair<QString, QStringList >> m_enumMacros;

};

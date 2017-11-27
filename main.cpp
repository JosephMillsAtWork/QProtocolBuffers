#include <QCoreApplication>
#include <google/protobuf/compiler/plugin.h>
#include "qobjectmacroplugin.h"

int main( int argc, char *argv[] )
{
    QObjectMacroGenerator gen;
    return google::protobuf::compiler::PluginMain( argc, argv, &gen );
}

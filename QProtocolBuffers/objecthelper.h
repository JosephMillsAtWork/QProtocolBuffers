#ifndef OBJECTHELPER_H
#define OBJECTHELPER_H

#include <QObject>
#include <QString>
#include <QChar>
#include <QDebug>
#include <QFileInfo>
#include <stdio.h>
class ObjectHelper : public QObject {
    Q_OBJECT

public:
    explicit ObjectHelper( QObject *parent = 0 );

    /*!
      if string is fooBar => FooBar
      if string is FooBar => FooBar
     */
    QString capFirstLetter( QString str )
    {
        // JIC spaces
        str.replace( " " , "" );
        QChar fLetter = str.at( 0 ).toUpper() ;
        return str.replace( 0, 1, fLetter ) ;
    }
    /*!
      if string is fooBar => fooBar
      if string is FooBar => fooBar
      if string is Foo Bar => fooBar
      if string is Foo a Bar => fooaBar
     */
    QString camelCase( QString str )
    {
        // just incase there is spaces
        str.replace( " " , "" );
        QChar fLetter = str.at( 0 ).toLower();
        return str.replace( 0, 1, fLetter );;
    }

    /*!
      if string is ClassName => CLASSNAME_H
      if string is Class Name => CLASSNAME_H
      if string is class Name => CLASSNAME_H
      if string is className => CLASSNAME_H
    */
    QString classToIfDef( QString str)
    {
        str.replace(" ","");
        return QString("%1_H")
                .arg(str).toUpper();
    }

    QString replaceDotCol( QString str )
    {
        QString ret = str;
        ret.replace(" ","");
        ret.replace(".", "::");

        return ret;
    }


    QString stripProto( QString fName )
    {
        return QFileInfo( fName ).baseName();
    }


    std::string stripProto( std::string fName )
    {
        return QFileInfo( QString::fromStdString( fName ) ).baseName().toStdString();
    }

    virtual void test() {}

};

#endif // OBJECTHELPER_H

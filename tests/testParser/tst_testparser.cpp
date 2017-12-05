#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "objecthelper.h"


class TestParser : public QObject
{
    Q_OBJECT

public:
    TestParser(){
        m_objHelper = new ObjectHelper( );
    }
    ~TestParser(){
        m_objHelper->deleteLater();
    }
private Q_SLOTS:
    void testObjecthelperCamelCase();
    void testObjecthelperUpper();
    void testObjecthelperIfDef();
    void testNameSpace();

private:
    ObjectHelper *m_objHelper;
};



void TestParser::testObjecthelperCamelCase()
{
    QCOMPARE( m_objHelper->camelCase("ThisIsATest"), QString("thisIsATest") );
}
void TestParser::testObjecthelperUpper()
{
    QCOMPARE( m_objHelper->capFirstLetter("this Is A Test"), QString("ThisIsATest") );
}
void TestParser::testObjecthelperIfDef()
{
    QString inDef = "ClassOne";
    QCOMPARE( m_objHelper->classToIfDef(inDef) , QString( "CLASSONE_H" ) );
}
void TestParser::testNameSpace()
{
    QString t = "var.car.nar";
    QCOMPARE( m_objHelper->replaceDotCol(t), QString("var::car::nar") );
}

QTEST_MAIN(TestParser)

#include "tst_testparser.moc"

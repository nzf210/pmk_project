#ifndef AUTOCONNECT_H
#define AUTOCONNECT_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QChildEvent>
#include <QMetaObject>
#include <QMetaMethod>

class autoconnectQ
{

public:
    static void connectQ(QObject *sender, QObject *receiver)
    {
        if(!sender) return;
        if(!receiver) return;

        QList<QByteArray> sendrSignals = scanType(sender,QMetaMethod::Signal);
        QList<QByteArray> receiverSlots = scanType(receiver,QMetaMethod::Slot);

        foreach(QByteArray signature, sendrSignals)
        {
            if(receiverSlots.contains(signature))
            {
                qInfo() << "Auto connecting:" << signature;
                //SIGNAL AND SLOT MACROS:
                //#define SLOT(a) "1"#a
                //#define SIGNAL(a) "2"#a
                //QObject::connect(sender,SIGNAL(readyRead()),receiver,SLOT(readyRead()));
                //qInfo() << "Matched: " << signature;
                //qInfo() << "SIGNAL: " << SIGNAL(readyRead());
                //qInfo() << "SLOT: " << SLOT(readyRead());

                //QObject::connect(sender,&QObject::signal,receiver,&Qobject::slot);
                //QObject::connect(sender,SIGNAL(test()),receiver, SLOT(test()));
                QObject::connect(sender,"2"+signature, receiver,"1"+signature);
            }
        }
    }

private:
    static QList<QByteArray> scanType(QObject *object, QMetaMethod::MethodType type)
    {
        QList<QByteArray> list;
        if(!object) return list;

        const QMetaObject *mo = object->metaObject();
        if(!mo) return list;
        for(int i = mo->methodOffset(); i < mo->methodCount(); i++)
        {
            QMetaMethod method = mo->method(i);
            if(method.methodType() == type)
            {
                list.append(method.methodSignature());
            }
        }

        return list;
    }

};

#endif // AUTOCONNECT_H

#include "ConnecterDesktop.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(Client)

CConnecterDesktop::CConnecterDesktop(CPluginClient *parent)
    : CConnecterDesktopThread(parent)
{
}

CConnecterDesktop::~CConnecterDesktop()
{
    qDebug(Client) << "CConnecterDesktop::~CConnecterDesktop";
}
int CConnecterDesktop::Connect()
{
    //qDebug(Client) << "CConnecterDesktop::Connect";
    emit sigConnect(this);
    return 0;
}

int CConnecterDesktop::DisConnect()
{
    //qDebug(Client) << "CConnecterDesktop::DisConnect";
    emit sigDisconnect(this);
    return 0;
}


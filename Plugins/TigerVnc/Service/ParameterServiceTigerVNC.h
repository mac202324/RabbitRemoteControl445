// Author: Kang Lin <kl222@126.com>

#ifndef CPARAMETERSERVICETIGERVNC_H
#define CPARAMETERSERVICETIGERVNC_H

#include "ParameterService.h"

class CParameterServiceTigerVNC : public CParameterService
{
    Q_OBJECT
    
public:
    explicit CParameterServiceTigerVNC(QObject *parent = nullptr);
    virtual ~CParameterServiceTigerVNC();
    
    // CParameterService interface
public:
    virtual int OnLoad(const QString& szFile = QString());
    virtual int OnSave(const QString& szFile = QString());
};

#endif // CPARAMETERSERVICETIGERVNC_H
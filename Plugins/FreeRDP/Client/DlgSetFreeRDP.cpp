// Author: Kang Lin <kl222@126.com>

#include "DlgSetFreeRDP.h"
#include "ui_DlgSetFreeRDP.h"
#include <QApplication>
#include <QScreen>
#include <QFileSystemModel>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QMediaDevices>
    #include <QAudioDevice>
#else
    #include <QAudioDeviceInfo>
#endif

#include "DlgDesktopSize.h"

CDlgSetFreeRDP::CDlgSetFreeRDP(CParameterFreeRDP *pSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgSetFreeRDP),
    m_pSettings(pSettings),
    m_pFileModel(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    
    ui->cbDesktopSize->addItems(m_pSettings->GetDesktopSizes());

    int width = GetScreenGeometry().width();
    int height = GetScreenGeometry().height();
    QString curSize = QString::number(width) + "×" + QString::number(height);
    ui->rbLocalScreen->setText(tr("Local screen:") + ": " + curSize);
    if(ui->cbDesktopSize->findText(curSize) == -1)
        InsertDesktopSize(width, height);
    
    ui->cbColorDepth->addItem(tr("8 bits"), 8);
    ui->cbColorDepth->addItem(tr("16 bits"), 16);
    ui->cbColorDepth->addItem(tr("24 bits"), 24);
    ui->cbColorDepth->addItem(tr("32 bits"), 32);

    QString szRdpSndParameters
                     = tr("- [sys:<sys>,][dev:<dev>,][format:<format>,][rate:<rate>,][channel:<channel>]\n"
                     #if defined (Q_OS_WINDOWS) || defined(Q_OS_WIN) || defined(Q_OS_WIN32) || defined(Q_OS_WINRT)
                          "- sys:winmm"
                     #elif defined(Q_OS_IOS)
                          "- sys:ios\n"
                          "- sys:mac"
                     #elif defined (Q_OS_ANDROID)
                          "- sys:opensles" 
                     #elif defined (Q_OS_LINUX) || defined (Q_OS_UNIX)
                          "- sys:alsa\n" 
                          "- sys:oss\n"
                          "- sys:oss,dev:1,format:1\n"
                          "- sys:sndio"
                     #endif
                         );
    QString szRdpSnd = tr("Options for redirection of audio output:\n") + szRdpSndParameters;
    ui->leRdpSnd->setToolTip(szRdpSnd);
    ui->leRdpSnd->setStatusTip(szRdpSnd);
    ui->leRdpSnd->setWhatsThis(szRdpSnd);
    
    QString szAudin = tr("Options for redirection of audio input:\n") + szRdpSndParameters;
    ui->leAudin->setToolTip(szAudin);
    ui->leAudin->setStatusTip(szAudin);
    ui->leAudin->setWhatsThis(szAudin);
    
    m_pFileModel= new QFileSystemModel();
    m_pFileModel->setNameFilterDisables(false);
    m_pFileModel->setFilter(QDir::Drives | QDir::Dirs | QDir::NoDotAndDotDot);
    m_pFileModel->setRootPath("");
    ui->tvDrive->setModel(m_pFileModel);
    ui->tvDrive->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int count = m_pFileModel->columnCount();
    for(int i = 1; i < count; i++)
        ui->tvDrive->hideColumn(i);
}

CDlgSetFreeRDP::~CDlgSetFreeRDP()
{
    delete ui;
}

void CDlgSetFreeRDP::on_pbOk_clicked()
{
    // Server
    m_pSettings->SetName(ui->leName->text());
    m_pSettings->SetHost(ui->leServer->text());
    m_pSettings->SetPort(ui->spPort->value());
    m_pSettings->SetUser(ui->leUserName->text());
    m_pSettings->SetPassword(ui->lePassword->text());
    m_pSettings->SetDomain(ui->leDomain->text());
    m_pSettings->SetSavePassword(ui->cbSavePassword->isChecked());
    m_pSettings->SetOnlyView(ui->cbOnlyView->isChecked());
    m_pSettings->SetClipboard(ui->cbClipboard->isChecked());
    m_pSettings->SetShowServerName(ui->cbShowServerName->isChecked());
    
    // Display
    QString szSize = ui->cbDesktopSize->currentText();
    int index = szSize.indexOf("×");
    if(-1 < index)
    {
        UINT32 width = szSize.left(index).toInt();
        UINT32 height = szSize.right(szSize.length() - index - 1).toInt();
        freerdp_settings_set_uint32(m_pSettings->m_pSettings,
                                    FreeRDP_DesktopWidth, width);
        freerdp_settings_set_uint32(m_pSettings->m_pSettings,
                                    FreeRDP_DesktopHeight, height);
    }

    freerdp_settings_set_bool(m_pSettings->m_pSettings, FreeRDP_UseMultimon, ui->cbAllMonitor->isChecked());
    if(ui->cbAllMonitor->isChecked())
    {
        //TODO: complete it
        freerdp_settings_set_uint32(m_pSettings->m_pSettings, FreeRDP_MonitorCount, QApplication::screens().length());
    }
    freerdp_settings_set_uint32(m_pSettings->m_pSettings, FreeRDP_ColorDepth,
                               ui->cbColorDepth->currentData().toInt());
    m_pSettings->SetReconnectInterval(ui->sbReconnect->value());

    // Redirection
    m_pSettings->SetRedirectionPrinter(ui->cbPrinter->isChecked());
    if(HasAudioOutput()) {
        CParameterFreeRDP::RedirecionSoundType tRdirectionSound
            = CParameterFreeRDP::RedirecionSoundType::Disable;
        if(ui->rbAudioDisable->isChecked())
            tRdirectionSound = CParameterFreeRDP::RedirecionSoundType::Disable;
        else if(ui->rbAudioLocal->isChecked())
            tRdirectionSound = CParameterFreeRDP::RedirecionSoundType::Local;
        else if(ui->rbAudioRemote->isChecked())
            tRdirectionSound = CParameterFreeRDP::RedirecionSoundType::Remote;
        m_pSettings->SetRedirectionSound(tRdirectionSound);
    } else {
        m_pSettings->SetRedirectionSound(
            CParameterFreeRDP::RedirecionSoundType::Disable);
    }
    m_pSettings->SetRedirectionSoundParameters(ui->leRdpSnd->text());
    if(HasAudioInput())
        m_pSettings->SetRedirectionMicrophone(ui->cbAudin->isChecked());
    else
        m_pSettings->SetRedirectionMicrophone(false);
    m_pSettings->SetRedirectionMicrophoneParameters(ui->leAudin->text());

    QStringList lstDrives;
    //获取选中的行，默认获取选中行的第一列数据（0），列的索引值和上面一样0、1、2、3......
    QModelIndexList selected = ui->tvDrive->selectionModel()->selectedRows(0);
    QList<QModelIndex>::iterator it;
    QModelIndex modelIndex;
    QString szPath;
    for (it = selected.begin(); it != selected.end(); ++it)
    {
        modelIndex = *it;
        szPath = m_pFileModel->filePath(modelIndex);
        if(!szPath.isEmpty())
            lstDrives.append(szPath);
    }
    m_pSettings->SetRedirectionDrives(lstDrives);

    accept();
}

void CDlgSetFreeRDP::on_pbCancel_clicked()
{
    reject();
}

void CDlgSetFreeRDP::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    Q_ASSERT(m_pSettings);

    // Server
    ui->leName->setText(m_pSettings->GetName());
    ui->leDomain->setText(m_pSettings->GetDomain());
    ui->leServer->setText(m_pSettings->GetHost());
    ui->spPort->setValue(m_pSettings->GetPort());
    ui->leUserName->setText(m_pSettings->GetUser());
    ui->lePassword->setText(m_pSettings->GetPassword());
    ui->pbShow->setEnabled(m_pSettings->GetParameterClient()->GetViewPassowrd());

    ui->cbSavePassword->setChecked(m_pSettings->GetSavePassword());
    ui->lePassword->setEnabled(ui->cbSavePassword->isChecked());
    if(ui->cbSavePassword->isChecked())
        ui->lePassword->setPlaceholderText(tr("Input password"));
    else
        ui->lePassword->setPlaceholderText(tr("Please checked save password to enable"));
    ui->cbOnlyView->setChecked(m_pSettings->GetOnlyView());
    ui->cbClipboard->setChecked(m_pSettings->GetClipboard());
    ui->cbShowServerName->setChecked(m_pSettings->GetShowServerName());

    // Display
    int width = GetScreenGeometry().width();
    int height = GetScreenGeometry().height();
    QString curSize = QString::number(width) + "×" + QString::number(height);
    InsertDesktopSize(width, height);
    UINT32 desktopWidth = 0, desktopHeight = 0;
    desktopWidth = freerdp_settings_get_uint32(m_pSettings->m_pSettings,
                                               FreeRDP_DesktopWidth);
    desktopHeight = freerdp_settings_get_uint32(m_pSettings->m_pSettings,
                                                FreeRDP_DesktopHeight);
    if(width == desktopWidth && height == desktopHeight)
    {
        ui->rbLocalScreen->setChecked(true);
        ui->cbDesktopSize->setCurrentText(curSize);
    } else {
        ui->rbSelect->setChecked(true);
        InsertDesktopSize(desktopWidth, desktopHeight);
        curSize = QString::number(desktopWidth) 
                + "×" + QString::number(desktopHeight);
        ui->cbDesktopSize->setCurrentText(curSize);
    }
    if(freerdp_settings_get_bool(m_pSettings->m_pSettings, FreeRDP_UseMultimon))
        ui->cbAllMonitor->setChecked(true);
    int nIndex = ui->cbColorDepth->findData(
                freerdp_settings_get_uint32(m_pSettings->m_pSettings,
                                            FreeRDP_ColorDepth));
    if(-1 != nIndex)
        ui->cbColorDepth->setCurrentIndex(nIndex);

    ui->sbReconnect->setValue(m_pSettings->GetReconnectInterval());

    // Redirection
    ui->cbPrinter->setChecked(m_pSettings->GetRedirectionPrinter());
    ui->leRdpSnd->setEnabled(HasAudioOutput());
    ui->gbAudio->setEnabled(HasAudioOutput());
    if(m_pSettings->GetRedirectionSound() == CParameterFreeRDP::RedirecionSoundType::Disable)
        ui->rbAudioDisable->setChecked(true);
    if(m_pSettings->GetRedirectionSound() == CParameterFreeRDP::RedirecionSoundType::Local)
        ui->rbAudioLocal->setChecked(true);
    if(m_pSettings->GetRedirectionSound() == CParameterFreeRDP::RedirecionSoundType::Remote)
        ui->rbAudioRemote->setChecked(true);
    ui->leRdpSnd->setText(m_pSettings->GetRedirectionSoundParameters());
    if(HasAudioInput()) {
        ui->cbAudin->setChecked(m_pSettings->GetRedirectionMicrophone());
    } else {
        ui->cbAudin->setChecked(false);
        ui->cbAudin->setEnabled(false);
        ui->leAudin->setEnabled(false);
    }
    ui->leAudin->setText(m_pSettings->GetRedirectionMicrophoneParameters());

    QStringList lstDrives = m_pSettings->GetRedirectionDrives();
    foreach(auto path, lstDrives)
    {
        QModelIndex index;
        if(!path.isEmpty()) {
            index = m_pFileModel->index(path);
            ui->tvDrive->setCurrentIndex(index);
        }
    }
}

void CDlgSetFreeRDP::on_rbLocalScreen_clicked(bool checked)
{
    if(!checked) return;
    QScreen* pScreen = QApplication::primaryScreen();
    int width = pScreen->availableGeometry().width();
    int height = pScreen->availableGeometry().height();
    QString curSize = QString::number(width) + "×" + QString::number(height);
    //ui->rbLocalScreen->setText(tr("Local screen:") + ": " + curSize);
    if(ui->cbDesktopSize->findText(curSize) == -1)
        ui->cbDesktopSize->addItem(curSize);
    ui->cbDesktopSize->setCurrentText(curSize);
}

void CDlgSetFreeRDP::on_leServer_editingFinished()
{
    auto s = ui->leServer->text().split(":");
    if(s.size() == 2)
    {
        ui->spPort->setValue(s[1].toUInt());
        ui->leServer->setText(s[0]);
    }
}

QRect CDlgSetFreeRDP::GetScreenGeometry()
{
    QRect r;
    if(freerdp_settings_get_bool(m_pSettings->m_pSettings, FreeRDP_UseMultimon))
    {
        auto lstScreen = QApplication::screens();
        foreach(auto pScreen, lstScreen)
        {
            r = r.united(pScreen->geometry());
        }
    } else {
        QScreen* pScreen = QApplication::primaryScreen();
        r = pScreen->geometry();
    }
    return r;
}
int CDlgSetFreeRDP::InsertDesktopSize(QString szSize)
{
    int w, h;
    int nIndex = szSize.indexOf("×");
    if(nIndex > -1)
    {
        bool ok = false;
        w = szSize.left(nIndex).toInt(&ok);
        if(ok)
            h = szSize.right(szSize.length() - nIndex - 1).toInt(&ok);
        if(!ok)
            return -1;
    }
    return InsertDesktopSize(w, h);
}

int CDlgSetFreeRDP::InsertDesktopSize(int width, int height)
{
    QString curSize = QString::number(width) + "×" + QString::number(height);
    if(ui->cbDesktopSize->findText(curSize) > -1)
        return 0;

    int nCount = ui->cbDesktopSize->count();
    for(int i = 0; i < nCount; i++)
    {
        QString curText = ui->cbDesktopSize->itemText(i);
        int nIndex = curText.indexOf("×");
        if(nIndex > -1)
        {
            int w = curText.left(nIndex).toInt();
            int h = curText.right(curText.length() - nIndex - 1).toInt();
            if(w > width)
            {
                ui->cbDesktopSize->insertItem(i, curSize);
                return 0;
            } else if(w == width) {
                if(h > height)
                {
                    ui->cbDesktopSize->insertItem(i, curSize);
                    return 0;
                } if(h == height) {
                    return 0;
                } else {
                    int j = i + 1;
                     while(j < nCount) {
                        QString curText = ui->cbDesktopSize->itemText(j);
                        int nIndex = curText.indexOf("×");
                        if(-1 >= nIndex) {
                            j++;
                        } else {
                            int w = curText.left(nIndex).toInt();
                            int h = curText.right(curText.length() - nIndex - 1).toInt();
                            if(w != width) {
                                ui->cbDesktopSize->insertItem(j, curSize);
                                return 0;
                            } else {
                                if(h > height)
                                {
                                    ui->cbDesktopSize->insertItem(j, curSize);
                                    return 0;
                                } else if(h == height)
                                    return 0;
                                else
                                    j++;
                            }
                        }
                    }
                }
            }
        }
    }

    if(ui->cbDesktopSize->findText(curSize) == -1)
        ui->cbDesktopSize->addItem(curSize);
    
    return 0;
}

void CDlgSetFreeRDP::on_pbShow_clicked()
{
    switch(ui->lePassword->echoMode())
    {
    case QLineEdit::Password:
        ui->lePassword->setEchoMode(QLineEdit::Normal);
        ui->pbShow->setIcon(QIcon::fromTheme("eye-off"));
        break;
    case QLineEdit::Normal:
        ui->lePassword->setEchoMode(QLineEdit::Password);
        ui->pbShow->setIcon(QIcon::fromTheme("eye-on"));
        break;
    default:
        ui->pbShow->setIcon(QIcon::fromTheme("eye-on"));
    }
}

void CDlgSetFreeRDP::on_rbAudioDisable_toggled(bool checked)
{
    ui->leRdpSnd->setEnabled(!checked);
}

void CDlgSetFreeRDP::on_rbAudioLocal_toggled(bool checked)
{
    ui->leRdpSnd->setEnabled(checked);
}

void CDlgSetFreeRDP::on_rbAudioRemote_toggled(bool checked)
{
    ui->leRdpSnd->setEnabled(!checked);
    ui->leAudin->setEnabled(!checked);
    ui->cbAudin->setEnabled(!checked);
}

void CDlgSetFreeRDP::on_cbSavePassword_stateChanged(int arg1)
{
    if(Qt::Checked == arg1) {
        ui->lePassword->setEnabled(true);
        ui->lePassword->setPlaceholderText(tr("Input password"));
    } else {
        ui->lePassword->setEnabled(false);
        ui->lePassword->setPlaceholderText(tr("Please checked save password to enable"));
    }
}

bool CDlgSetFreeRDP::HasAudioOutput()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto d = QMediaDevices::defaultAudioOutput();
    return !d.isNull();
#else
    return !QAudioDeviceInfo::defaultOutputDevice().isNull();
#endif
}

bool CDlgSetFreeRDP::HasAudioInput()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    auto d = QMediaDevices::defaultAudioInput();
    return !d.isNull();
#else
    return !QAudioDeviceInfo::defaultInputDevice().isNull();
#endif
}

void CDlgSetFreeRDP::on_pbSizeEdit_clicked()
{
    CDlgDesktopSize dlg;
    QStringList lstSize;
    for(int i = 0; i < ui->cbDesktopSize->count(); i++)
        lstSize << ui->cbDesktopSize->itemText(i);
    dlg.SetDesktopSizes(lstSize);
    if(QDialog::Accepted == dlg.exec())
    {
        ui->cbDesktopSize->clear();
        foreach(auto s, dlg.GetDesktopSize())
        {
            InsertDesktopSize(s);
        }
    }
}

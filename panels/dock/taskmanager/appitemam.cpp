#include "appitemam.h"
#include "appitem.h"
#include "global.h"
#include "dsglobal.h"
#include <QDBusConnection>

// AM static string
static const QString AM_DBUS_PATH = "org.desktopspec.ApplicationManager1";
static const QString DESKTOP_ENTRY_ICON_KEY = "Desktop Entry";
static const QString DEFAULT_KEY = "default";

// copy from dde-application-manager
inline QString escapeToObjectPath(const QString &str)
{
    if (str.isEmpty()) {
        return "_";
    }

    auto ret = str;
    QRegularExpression re{R"([^a-zA-Z0-9])"};
    auto matcher = re.globalMatch(ret);
    while (matcher.hasNext()) {
        auto replaceList = matcher.next().capturedTexts();
        replaceList.removeDuplicates();
        for (const auto &c : replaceList) {
            auto hexStr = QString::number(static_cast<uint>(c.front().toLatin1()), 16);
            ret.replace(c, QString{R"(_%1)"}.arg(hexStr));
        }
    }
    return ret;
}

inline QString unescapeFromObjectPath(const QString &str)
{
    auto ret = str;
    for (qsizetype i = 0; i < str.size(); ++i) {
        if (str[i] == '_' and i + 2 < str.size()) {
            auto hexStr = str.sliced(i + 1, 2);
            ret.replace(QString{"_%1"}.arg(hexStr), QChar::fromLatin1(hexStr.toUInt(nullptr, 16)));
            i += 2;
        }
    }
    return ret;
}

DS_BEGIN_NAMESPACE
namespace dock {
AppItemAm::AppItemAm(QDBusObjectPath path, QObject* parent)
    : AppItem(dbusPath2id(path.path()), parent)
{
    m_applicationInterface.reset(new Application(AM_DBUS_PATH, path.path(), QDBusConnection::sessionBus(), this));
}

AppItemAm::~AppItemAm()
{
    disconnect();
}

QString AppItemAm::id()
{
    if (m_id.isEmpty() && m_applicationInterface) {
        m_id = m_applicationInterface->iD();
    }
    return m_id;
}

QString AppItemAm::name()
{
    auto updateLocalName = [this]() {
        QString currentLanguageCode = QLocale::system().name();
        auto names = m_applicationInterface->name();
        auto localeName = names.value(currentLanguageCode);
        auto fallbackName = names.value(DEFAULT_KEY);
        m_name = localeName.isEmpty() ? fallbackName : localeName;
    };

    if (m_name.isEmpty() && m_applicationInterface) {
        QDBusConnection::sessionBus().connect(m_applicationInterface->service(), m_applicationInterface->path(), m_applicationInterface->interface(), "nameChanged", this, SLOT([this](){
            updateLocalName();
            Q_EMIT nameChanged();
        }));
        updateLocalName();
    }
    return m_name;
}

QString AppItemAm::desktopIcon()
{
    if(m_icon.isEmpty() && m_applicationInterface) {
        QDBusConnection::sessionBus().connect(m_applicationInterface->service(), m_applicationInterface->path(), m_applicationInterface->interface(), "iconsChanged", this, SLOT([this](){
            m_icon = m_applicationInterface->icon().value(DESKTOP_ENTRY_ICON_KEY);
            Q_EMIT iconChanged();
        }));
        m_icon = m_applicationInterface->icons().value(DESKTOP_ENTRY_ICON_KEY);
    }

    return m_icon;
}

QString AppItemAm::genericName()
{
    auto updateLocalGenericName = [this]() {
        QString currentLanguageCode = QLocale::system().name();
        auto genericNames = m_applicationInterface->genericName();
        auto localeGenericName = genericNames.value(currentLanguageCode);
        auto fallBackGenericName = genericNames.value(DEFAULT_KEY);
        m_genericName = localeGenericName.isEmpty() ? fallBackGenericName : localeGenericName;
    };

    if(m_genericName.isEmpty() && m_applicationInterface) {
        QDBusConnection::sessionBus().connect(m_applicationInterface->service(), m_applicationInterface->path(), m_applicationInterface->interface(), "genericNameChanged", this, SLOT([this](){
            updateLocalGenericName();
            Q_EMIT genericNameChanged();
        }));
        updateLocalGenericName();
    }

    return m_genericName;
}

QString AppItemAm::menus()
{
    if(m_menus.isEmpty() && m_applicationInterface) {
        auto actions = m_applicationInterface->actions();
    }

    return m_menus;
}

QString AppItemAm::dbusPath2id(const QString& dbusPath)
{
    return unescapeFromObjectPath(dbusPath);
}

QString AppItemAm::id2dbusPath(const QString& id)
{
    return escapeToObjectPath(id);
}
}
DS_END_NAMESPACE

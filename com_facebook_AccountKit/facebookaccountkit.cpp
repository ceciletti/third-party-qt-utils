#include "facebookaccountkit.h"

#include <QLoggingCategory>
#include <QTimer>

Q_LOGGING_CATEGORY(FB_ACCOUNTKIT, "facebook.accountkit", QtInfoMsg)

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidIntent>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidActivityResultReceiver>

class FacebookAccountKitPrivate : public QAndroidActivityResultReceiver
{
    Q_DECLARE_PUBLIC(FacebookAccountKit)
public:
    FacebookAccountKitPrivate(FacebookAccountKit *q) : q_ptr(q) { }
    virtual void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data) override;

    void callBuilderMethod(const char *methodName, const QString &data);
    void callBuilderMethod(const char *methodName, const QStringList &stringlist);
    void callBuilderMethod(const char *methodName, bool enable);
    void callBuilderMethod(const char *methodName, int value);

    QAndroidJniObject configurationBuilder;
    FacebookAccountKit *q_ptr;
    int m_receiverRequestCode;
};
#else
class FacebookAccountKitPrivate
{
    Q_DECLARE_PUBLIC(FacebookAccountKit)
public:
    FacebookAccountKitPrivate(FacebookAccountKit *q) : q_ptr(q) { }

    FacebookAccountKit *q_ptr;
};
#endif


FacebookAccountKit::FacebookAccountKit(FacebookAccountKit::LoginType loginType, FacebookAccountKit::ResponseType responseType, QObject *parent)
    : QObject(parent)
    , d_ptr(new FacebookAccountKitPrivate(this))
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);

    const QAndroidJniObject loginT = QAndroidJniObject::getStaticObjectField("com/facebook/accountkit/ui/LoginType",
                                                                             loginType == PHONE ? "PHONE" : "EMAIL",
                                                                             "Lcom/facebook/accountkit/ui/LoginType;");
    Q_ASSERT(loginT.isValid());

    const QAndroidJniObject responseT = QAndroidJniObject::getStaticObjectField("com/facebook/accountkit/ui/AccountKitActivity$ResponseType",
                                                                                 responseType == CODE ? "CODE" : "TOKEN",
                                                                                 "Lcom/facebook/accountkit/ui/AccountKitActivity$ResponseType;");
    Q_ASSERT(responseT.isValid());

    d->configurationBuilder = QAndroidJniObject("com/facebook/accountkit/ui/AccountKitConfiguration$AccountKitConfigurationBuilder",
                                                "(Lcom/facebook/accountkit/ui/LoginType;Lcom/facebook/accountkit/ui/AccountKitActivity$ResponseType;)V",
                                                loginT.object(), responseT.object());
    Q_ASSERT(d->configurationBuilder.isValid());
#endif
}

FacebookAccountKit::~FacebookAccountKit()
{
    delete d_ptr;
}

void FacebookAccountKit::setInitialAuthState(const QString &initialAuthState)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setInitialAuthState", initialAuthState);
#endif
}

void FacebookAccountKit::setInitialEmail(const QString &initialEmail)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setInitialEmail", initialEmail);
#endif
}

void FacebookAccountKit::setDefaultCountryCode(const QString &defaultCountryCode)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setDefaultCountryCode", defaultCountryCode);
#endif
}

void FacebookAccountKit::setInitialPhoneNumber(const QString &countryCode, const QString &phoneNumber, const QString &countryCodeIso)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    QAndroidJniObject phone("com/facebook/accountkit/PhoneNumber",
                            "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                            QAndroidJniObject::fromString(countryCode).object<jstring>(),
                            QAndroidJniObject::fromString(phoneNumber).object<jstring>(),
                            QAndroidJniObject::fromString(countryCodeIso).object<jstring>());
    Q_ASSERT(phone.isValid());

    d->configurationBuilder = d->configurationBuilder.callObjectMethod("setInitialPhoneNumber",
                                                                       "(Lcom/facebook/accountkit/PhoneNumber;)Lcom/facebook/accountkit/ui/AccountKitConfiguration$AccountKitConfigurationBuilder;",
                                                                       phone.object());
    Q_ASSERT(d->configurationBuilder.isValid());
#endif
}

void FacebookAccountKit::setFacebookNotificationsEnabled(bool facebookNotificationsEnabled)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setFacebookNotificationsEnabled", facebookNotificationsEnabled);
#endif
}

void FacebookAccountKit::setTitleType(FacebookAccountKit::TitleType titleType)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    const QAndroidJniObject titleT = QAndroidJniObject::getStaticObjectField("com/facebook/accountkit/ui/AccountKitActivity$TitleType",
                                                                             titleType == APP_NAME ? "APP_NAME" : "LOGIN",
                                                                             "Lcom/facebook/accountkit/ui/AccountKitActivity$TitleType;");
    Q_ASSERT(titleT.isValid());
    d->configurationBuilder = d->configurationBuilder.callObjectMethod("setTitleType",
                                                                       "(Lcom/facebook/accountkit/ui/AccountKitActivity$TitleType;)Lcom/facebook/accountkit/ui/AccountKitConfiguration$AccountKitConfigurationBuilder;",
                                                                       titleT.object());
    Q_ASSERT(d->configurationBuilder.isValid());
#endif
}

void FacebookAccountKit::setTheme(int theme)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setTheme", theme);
#endif
}

void FacebookAccountKit::setReadPhoneStateEnabled(bool readPhoneStateEnabled)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setReadPhoneStateEnabled", readPhoneStateEnabled);
#endif
}

void FacebookAccountKit::setReceiveSMS(bool receiveSMSEnabled)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setReceiveSMS", receiveSMSEnabled);
#endif
}

void FacebookAccountKit::setSMSWhitelist(const QStringList &smsWhitelist)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setSMSWhitelist", smsWhitelist);
#endif
}

void FacebookAccountKit::setSMSBlacklist(const QStringList &smsBlacklist)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);
    d->callBuilderMethod("setSMSBlacklist", smsBlacklist);
#endif
}

void FacebookAccountKit::show(int receiverRequestCode)
{
#ifdef Q_OS_ANDROID
    Q_D(FacebookAccountKit);

//    const QString configKey = QAndroidJniObject::getStaticObjectField<jstring>("com/facebook/accountkit/ui/AccountKitActivity",
//                                                                               "ACCOUNT_KIT_ACTIVITY_CONFIGURATION").toString();
    const QString configKey = QStringLiteral("AccountKitConfiguration");

    const QAndroidJniObject config = d->configurationBuilder.callObjectMethod("build", "()Lcom/facebook/accountkit/ui/AccountKitConfiguration;");
    Q_ASSERT(config.isValid());

    QAndroidIntent intent(QtAndroid::androidActivity(), "com.facebook.accountkit.ui.AccountKitActivity");
    Q_ASSERT(intent.handle().isValid());

    intent.handle().callObjectMethod("putExtra",
                                     "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;",
                                     QAndroidJniObject::fromString(configKey).object<jstring>(),
                                     config.object());
    Q_ASSERT(intent.handle().isValid());

    d->m_receiverRequestCode = receiverRequestCode;
    QtAndroid::startActivity(intent.handle(), receiverRequestCode, d);
#else
    QTimer::singleShot(0, this, [=] {
        Q_EMIT error(INTERNAL_ERROR, QStringLiteral("Not Android OS"));
    });
#endif
}

#ifdef Q_OS_ANDROID
void FacebookAccountKitPrivate::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    if (m_receiverRequestCode != receiverRequestCode) {
        return;
    }
    Q_Q(FacebookAccountKit);

    qCDebug(FB_ACCOUNTKIT) << receiverRequestCode << resultCode << data.isValid();

    const QString resultKey = QAndroidJniObject::getStaticObjectField<jstring>("com/facebook/accountkit/AccountKitLoginResult",
                                                                               "RESULT_KEY").toString();
    const QAndroidJniObject loginResult = data.callObjectMethod("getParcelableExtra",
                                                                "(Ljava/lang/String;)Landroid/os/Parcelable;",
                                                                QAndroidJniObject::fromString(resultKey).object<jstring>());
    Q_ASSERT(loginResult.isValid());

    const QAndroidJniObject error = loginResult.callObjectMethod("getError", "()Lcom/facebook/accountkit/AccountKitError;");
    if (error.isValid()) {
        const QAndroidJniObject type = error.callObjectMethod("getErrorType", "()Lcom/facebook/accountkit/AccountKitError$Type;");
        Q_ASSERT(type.isValid());

        const QString message = type.callObjectMethod<jstring>("getMessage").toString();
        const int code = type.callMethod<jint>("getCode");
        qCDebug(FB_ACCOUNTKIT) << "AccountKit error" << static_cast<FacebookAccountKit::ErrorCode>(code) << message;
        Q_EMIT q->error(code, message);
    } else if (loginResult.callMethod<jboolean>("wasCancelled")) {
        qCDebug(FB_ACCOUNTKIT) << "Cancelled";
        Q_EMIT q->cancelled();
    } else {
        QString code;
        const QAndroidJniObject token = loginResult.callObjectMethod("getAccessToken", "()Lcom/facebook/accountkit/AccessToken;");
        if (token.isValid()) {
            code = token.callObjectMethod<jstring>("getAccountId").toString();
            qCDebug(FB_ACCOUNTKIT) << "got token" << code;
        } else {
            code = loginResult.callObjectMethod<jstring>("getAuthorizationCode").toString();
            qCDebug(FB_ACCOUNTKIT) << "got code" << code;
        }
        Q_EMIT q->success(code);
    }
}

void FacebookAccountKitPrivate::callBuilderMethod(const char *methodName, const QString &data)
{
    configurationBuilder = configurationBuilder.callObjectMethod(methodName,
                                                                 "(Ljava/lang/String;)Lcom/facebook/accountkit/ui/AccountKitConfiguration$AccountKitConfigurationBuilder;",
                                                                 QAndroidJniObject::fromString(data).object<jstring>());
    Q_ASSERT(configurationBuilder.isValid());
}

void FacebookAccountKitPrivate::callBuilderMethod(const char *methodName, const QStringList &stringlist)
{
    QAndroidJniEnvironment env;
    jobjectArray jarray = env->NewObjectArray(stringlist.count(), env->FindClass("java/lang/String"), nullptr);
    for (int i = 0; i < stringlist.count(); ++i) {
         env->SetObjectArrayElement(jarray, i, env->NewStringUTF(stringlist.at(i).toUtf8().constData()));
    }

    configurationBuilder = configurationBuilder.callObjectMethod(methodName,
                                                                 "([Ljava/lang/String;)Lcom/facebook/accountkit/ui/AccountKitConfiguration$AccountKitConfigurationBuilder;",
                                                                 jarray);
    env->DeleteLocalRef(jarray);

    Q_ASSERT(configurationBuilder.isValid());
}

void FacebookAccountKitPrivate::callBuilderMethod(const char *methodName, bool enable)
{
    configurationBuilder = configurationBuilder.callObjectMethod(methodName,
                                                                 "(Z)Lcom/facebook/accountkit/ui/AccountKitConfiguration$AccountKitConfigurationBuilder;",
                                                                 enable);
    Q_ASSERT(configurationBuilder.isValid());
}

void FacebookAccountKitPrivate::callBuilderMethod(const char *methodName, int value)
{
    configurationBuilder = configurationBuilder.callObjectMethod(methodName,
                                                                 "(I)Lcom/facebook/accountkit/ui/AccountKitConfiguration$AccountKitConfigurationBuilder;",
                                                                 value);
    Q_ASSERT(configurationBuilder.isValid());
}
#endif

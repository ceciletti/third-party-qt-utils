# Usage
You still need to do the XML and gradle changes describibed on their integration page, but you can skip the java part,
a sample of usage code:

```
auto ak = new FacebookAccountKit(FacebookAccountKit::PHONE, FacebookAccountKit::CODE, this);
ak->setDefaultCountryCode(QStringLiteral("BR")); // Optional
ak->setSMSWhitelist({ QStringLiteral("BR") }); // Optional
ak->setInitialPhoneNumber(QStringLiteral("55"), QStringLiteral("11999999999"), QStringLiteral("BR")); // Optional
connect(ak, &FacebookAccountKit::cancelled, this, [=] {
    qDebug() << "got cancelled";
    ak->deleteLater();
});
connect(ak, &FacebookAccountKit::error, this, [=] (int errorCode, const QString &message) {
    qDebug() << "got error" << errorCode << message;
    ak->deleteLater();
});
connect(ak, &FacebookAccountKit::success, this, [=] (const QString &data) {
    qDebug() << "got success code/token" << data;
    ak->deleteLater();
});
ak->show(); // Call after the connects are done to get an error when not on Android
```

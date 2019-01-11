#ifndef FACEBOOKACCOUNTKIT_H
#define FACEBOOKACCOUNTKIT_H

#include <QObject>

class FacebookAccountKitPrivate;
class FacebookAccountKit : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FacebookAccountKit)
public:
    enum LoginType {
        PHONE,
        EMAIL,
    };
    Q_ENUM(LoginType)

    enum ResponseType {
        CODE,
        TOKEN,
    };
    Q_ENUM(ResponseType)

    enum TitleType {
        APP_NAME,
        LOGIN,
    };
    Q_ENUM(TitleType)

    enum ErrorCode {
        NETWORK_CONNECTION_ERROR,
        SERVER_ERROR,
        LOGIN_INVALIDATED,
        INTERNAL_ERROR,
        INITIALIZATION_ERROR,
        ARGUMENT_ERROR,
        UPDATE_INVALIDATED,
    };
    Q_ENUM(ErrorCode)

    explicit FacebookAccountKit(LoginType loginType, ResponseType responseType, QObject *parent = nullptr);
    virtual ~FacebookAccountKit();

    /**
     * (Optional) A developer-generated nonce used to verify that the received response matches the request.
     * Fill this with a random value at runtime; when the login call returns, check that the corresponding
     * param in the response matches the one you set in this method.
     */
    void setInitialAuthState(const QString &initialAuthState);

    /**
     * (Optional) Pre-fill the user's email address in the email login flow.
     *
     * @note: By default, the email field provides a dropdown list of the user's email addresses if the
     * GET_ACCOUNTS permission is granted.
     */
    void setInitialEmail(const QString &initialEmail);

    /**
     * (Optional) Set the default country code shown in the SMS login flow. e.g BR, US...
     */
    void setDefaultCountryCode(const QString &defaultCountryCode);

    /**
     * @param countryCode - The country code for the phone number
     * @param phoneNumber - The remaining portion of the phone number
     * @param countryCodeIso - The country code iso for the phone number While it is safe to provide any characters
     * for the 'countryCode' and 'phoneNumber', the values should only contain digits. All other characters
     * will be stripped from this value when it is used.
     */
    void setInitialPhoneNumber(const QString &countryCode, const QString &phoneNumber, const QString &countryCodeIso);

    /**
     * (Optional) If this flag is set, Account Kit offers the user the option to receive their confirmation
     * message via a Facebook notification in the event of an SMS failure, if their phone number is
     * associated with their Facebook account. The associated phone number must be the primary phone number
     * for that Facebook account.
     *
     * @default: true
     */
    void setFacebookNotificationsEnabled(bool facebookNotificationsEnabled);

    /**
     * (Optional) Set to TitleType::APP_NAME to use your application's name as the title for
     * the login screen, or TitleType::LOGIN to use a localized translation of "Login" as the title.
     *
     * @default: TitleType::LOGIN
     */
    Q_DECL_DEPRECATED void setTitleType(TitleType titleType);

    /**
     * (Optional) Pass in a resource identifier for a theme to have that theme used for the login screen.
     * See Customizing the UI for Android for more information.
     *
     * @note: If you specify both a theme and an Advanced UI Manager object (see below),
     * the Advanced UI Manager will take precedence and overwrite theme elements.
     */
    Q_DECL_DEPRECATED void setTheme(int theme);

    /**
     * (Optional) If the READ_PHONE_STATE permission is granted and this flag is true, the app will pre-fill
     * the user's phone number in the SMS login flow. Set to false if you wish to use the READ_PHONE_STATE
     * permission yourself, but you do not want the user's phone number pre-filled by Account Kit.
     *
     * @default: true
     */
    void setReadPhoneStateEnabled(bool readPhoneStateEnabled);

    /**
     * (Optional) If the RECEIVE_SMS permission is granted and this flag is true, the app will automatically
     * read the Account Kit confirmation SMS and pre-fill the confirmation code in the SMS login flow. Set to
     * false if you wish to use the RECEIVE_SMS permission yourself, but you do not want the SMS confirmation
     * code pre-filled by Account Kit.
     *
     * @default: true
     */
    void setReceiveSMS(bool receiveSMSEnabled);

    /**
     * (Optional) Use this to specify a list of permitted country codes for use in the SMS login flow. The value
     * is an array of short country codes as defined by ISO 3166-1 Alpha 2. To restrict availability to just
     * the US (+1) and The Netherlands (+31), pass in ["US", "NL"].
     */
    void setSMSWhitelist(const QStringList &smsWhitelist);

    /**
     * (Optional) Use this to specify a list of country codes to exclude during the SMS login flow. Only the
     * country codes in the blacklist are unavailable. People can still use the rest of Account Kit's supported
     * country codes. If a country code appears in both the whitelist and the blacklist, the blacklist takes
     * precedence and the country code is not available. Just like the whitelist, the value is an array of short
     * country codes as defined by ISO 3166-1 Alpha 2.
     */
    void setSMSBlacklist(const QStringList &smsBlacklist);

    /**
     * @brief show Facebook's AccountKit dialog
     * @param receiverRequestCode is an unique code that you don't use else where in your application
     */
    void show(int receiverRequestCode = 99);

Q_SIGNALS:
    void cancelled();
    void error(int code, const QString &message);
    void success(const QString &data);

private:
    FacebookAccountKitPrivate *d_ptr;
};

#endif // FACEBOOKACCOUNTKIT_H

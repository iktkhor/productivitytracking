#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    QString get_login();
    QString get_password();

    void set_login(QString login);
    void set_password(QString password);

private:
    QString login;
    QString password;
};

#endif // USER_H

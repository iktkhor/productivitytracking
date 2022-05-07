#include "user.h"

User::User()
{

}

QString User::get_login()
{
    return login;
}

QString User::get_password()
{
    return password;
}

void User::set_login(QString login)
{
    this->login = login;
}

void User::set_password(QString password)
{
    this->password = password;
}

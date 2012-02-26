/*
    libKGoogle - KGoogleAuth
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "auth.h"
#include "auth_p.h"

#include "common.h"
#include "services/accountinfo.h"

#include <kwallet.h>

#include <klocalizedstring.h>

#include <qmutex.h>

using namespace KWallet;
using namespace KGoogle;

Auth* AuthPrivate::instance = 0;

Auth *Auth::instance()
{
  static QMutex mutex;
  if (!AuthPrivate::instance) {
    mutex.lock();

    if (!AuthPrivate::instance)
      AuthPrivate::instance = new Auth();

    mutex.unlock();
  }

  return AuthPrivate::instance;
}

Auth::Auth():
  d_ptr(new AuthPrivate(this))
{
  Q_D(Auth);

  d->kwalletFolder = "libkgoogle";
  d->kwallet = 0;
}


Auth::~Auth()
{
  delete d_ptr;
}

void Auth::setKWalletFolder(const QString& folder)
{
  Q_D(Auth);

  d->kwalletFolder = folder;
}

KGoogle::Account *Auth::getAccount (const QString &account)
{
  Q_D(Auth);

  if (!d->initKWallet())
    return 0;

  if (!d->kwallet->hasFolder(d->kwalletFolder)) {
    throw Exception::UnknownAccount(account);
    return 0;
  }

  d->kwallet->setFolder(d->kwalletFolder);

  QMap< QString, QString > map;
  if (d->kwallet->readMap(account, map) != 0) {
    throw Exception::UnknownAccount(account);
    return 0;
  }

  QStringList scopes = map["scopes"].split(',');
  QList< QUrl > scopeUrls;
  foreach (const QString &scope, scopes)
    scopeUrls << QUrl(scope);

  Account *acc = new Account(account, map["accessToken"], map["refreshToken"], scopeUrls);
  return acc;
}

QList< KGoogle::Account * > Auth::getAccounts()
{
  Q_D(Auth);

  if (!d->initKWallet())
    return QList< Account* >();

  if (!d->kwallet->hasFolder(d->kwalletFolder))
    return QList< Account *>();


  d->kwallet->setFolder(d->kwalletFolder);
  QStringList list = d->kwallet->entryList();
  QList< Account * > accounts;
  foreach (QString accName, list) {
    QMap< QString, QString > map;
    d->kwallet->readMap(accName, map);

    QStringList scopes = map["scopes"].split(',');
    QList< QUrl > scopeUrls;
    foreach (const QString &scope, scopes)
      scopeUrls << QUrl(scope);
    accounts.append(new Account(accName, map["accessToken"], map["refreshToken"], scopeUrls));
  }

  return accounts;
}

void Auth::storeAccount (const KGoogle::Account *account)
{
  Q_D(Auth);

  if (!d->initKWallet())
     return;

  if (!account || account->accountName().isEmpty() ||
       account->accessToken().isEmpty() || account->refreshToken().isEmpty()) {
    throw Exception::InvalidAccount();
    return;
  }

  if (!d->kwallet->hasFolder(d->kwalletFolder))
    d->kwallet->createFolder(d->kwalletFolder);

  d->kwallet->setFolder(d->kwalletFolder);

  if (d->kwallet->hasEntry(account->accountName()))
    d->kwallet->removeEntry(account->accountName());

  QStringList scopes;
  foreach (const QUrl &scope, account->scopes())
    scopes << scope.toString();

  QMap< QString, QString > map;
  map["accessToken"] = account->accessToken();
  map["refreshToken"] = account->refreshToken();
  map["scopes"] = scopes.join(",");
  d->kwallet->writeMap(account->accountName(), map);
}

void Auth::authenticate (KGoogle::Account *account, bool autoSave)
{
  Q_D(Auth);

  if (!d->initKWallet())
    return;

  if (!account) {
    throw Exception::InvalidAccount();
    return;
  }

  if (account->refreshToken().isEmpty() || (account->m_scopesChanged == true)) {

    account->addScope(Services::AccountInfo::EmailScopeUrl);
    d->fullAuthentication(account, autoSave);

  } else {

    if (account->accountName().isEmpty()) {
      throw Exception::InvalidAccount();
      return;
    }
    d->refreshTokens(account, autoSave);

  }
}

bool Auth::revoke (Account *account)
{
  Q_D(Auth);

  if (!account || account->accountName().isEmpty()) {
    return false;
  }

  if (!d->initKWallet())
    return false;

  if (!d->kwallet->hasFolder(d->kwalletFolder))
    return false;

  if (d->kwallet->hasEntry(account->accountName())) {

    if (d->kwallet->removeEntry(account->accountName()) == 0) {

      account->setAccessToken("");
      account->setRefreshToken("");
      account->setScopes(QList< QUrl >());
      return true;

    } else {

      return false;

    }

  } else {

    return false;

  }
}

/*
 * This file is part of LibKGAPI library
 *
 * Copyright (C) 2013  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "authjob.h"
#include "account.h"
#include "debug.h"
#include "job_p.h"
#include "ui/authwidget.h"
#include "ui/authwidget_p.h"
#include "private/newtokensfetchjob_p.h"

#include <QtGui/QWidget>

#include <KDE/KDialog>
#include <KDE/KLocalizedString>
#include <KDE/KWindowSystem>

#include <qjson/parser.h>

using namespace KGAPI2;

class AuthJob::Private
{
  public:
    Private(AuthJob *parent);

    QWidget* fullAuthentication();
    void refreshTokens();

    void _k_fullAuthenticationFinished(const KGAPI2::AccountPtr& account);
    void _k_fullAuthenticationFailed(KGAPI2::Error errorCode, const QString &errorMessage);

    AccountPtr account;
    QString apiKey;
    QString secretKey;
    QWidget* widget;

    QString username;
    QString password;

  private:
    AuthJob *q;
};

AuthJob::Private::Private(AuthJob *parent):
    widget(0),
    q(parent)
{
}

QWidget* AuthJob::Private::fullAuthentication()
{
    AuthWidget* authWidget =  new AuthWidget(widget);
    // FIXME: Find a better way to pass the keys
    authWidget->d->apiKey = apiKey;
    authWidget->d->secretKey = secretKey;

    connect(authWidget, SIGNAL(error(KGAPI2::Error,QString)),
            q, SLOT(_k_fullAuthenticationFailed(KGAPI2::Error,QString)));
    connect(authWidget, SIGNAL(authenticated(KGAPI2::AccountPtr)),
            q, SLOT(_k_fullAuthenticationFinished(KGAPI2::AccountPtr)));

    authWidget->setUsername(username);
    authWidget->setPassword(password);
    authWidget->setAccount(account);

    return authWidget;
}

void AuthJob::Private::refreshTokens()
{
    static_cast<Job*>(q)->d->accessManager->setCookieJar(new QNetworkCookieJar);

    QNetworkRequest request;

    request.setUrl(QUrl(QLatin1String("https://accounts.google.com/o/oauth2/token")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));

    QUrl params;
    params.addQueryItem(QLatin1String("client_id"), apiKey);
    params.addQueryItem(QLatin1String("client_secret"), secretKey);
    params.addQueryItem(QLatin1String("refresh_token"), account->refreshToken());
    params.addQueryItem(QLatin1String("grant_type"), QLatin1String("refresh_token"));

    KGAPIDebugRawData() << "Requesting token refresh: " << params.encodedQuery();

    q->enqueueRequest(request, params.encodedQuery());
}

void AuthJob::Private::_k_fullAuthenticationFailed(Error errorCode, const QString &errorMessage)
{
    q->setError(errorCode);
    q->setErrorString(errorMessage);
    q->emitFinished();
}

void AuthJob::Private::_k_fullAuthenticationFinished( const AccountPtr &account_ )
{
    account = account_;
    q->emitFinished();
}

AuthJob::AuthJob(const AccountPtr& account, const QString &apiKey, const QString &secretKey, QWidget* parent):
    Job(parent),
    d(new Private(this))
{
    d->account = account;
    d->apiKey = apiKey;
    d->secretKey = secretKey;
    d->widget = parent;
}

AuthJob::AuthJob(const AccountPtr& account, const QString &apiKey, const QString &secretKey, QObject* parent):
    Job(parent),
    d(new Private(this))
{
    d->account = account;
    d->apiKey = apiKey;
    d->secretKey = secretKey;
}

AuthJob::~AuthJob()
{
    delete d;
}

AccountPtr AuthJob::account() const
{
    return d->account;
}

void AuthJob::handleReply(const QNetworkReply *reply, const QByteArray& rawData)
{
    Q_UNUSED(reply);

    QJson::Parser parser;
    bool ok = true;
    QVariantMap map = parser.parse(rawData, &ok).toMap();

    if (!ok) {
        setError(KGAPI2::InvalidResponse);
        setErrorString(i18n("Failed to parse newly fetched tokens"));
        emitFinished();
        return;
    }

    /* Expected structure:
     * {
     *  "access_token": "the_access_token",
     *  "token_type":"Bearer",
     *  "expires_in":3600
     * }
     */

    d->account->setAccessToken(map.value(QLatin1String("access_token")).toString());
    emitFinished();
}

void AuthJob::dispatchRequest(QNetworkAccessManager* accessManager, const QNetworkRequest& request, const QByteArray& data, const QString& contentType)
{
    Q_UNUSED(contentType);

    accessManager->post(request, data);
}

void AuthJob::start()
{
    AuthWidget *widget = 0;
    KDialog *dlg;

    if (d->account->refreshToken().isEmpty() || (d->account->m_scopesChanged == true)) {

        d->account->addScope(Account::accountInfoEmailScopeUrl());

        /* Pre-fill the username in the dialog so that user knows what account
         * (s)he is re-authenticating for */
        if (!d->account->accountName().isEmpty() && d->username.isEmpty()) {
            d->username = d->account->accountName();
        }

        widget = qobject_cast<AuthWidget*>(d->fullAuthentication());

    } else {

        if (d->account->accountName().isEmpty()) {
            setError(KGAPI2::InvalidAccount);
            setErrorString(i18n("Account name is empty"));
            emitFinished();
            return;
        }

        d->refreshTokens();
    }

    if (widget) {
        dlg = new KDialog();
        dlg->setModal(true);
        KWindowSystem::setMainWindow(dlg, KWindowSystem::activeWindow());

        dlg->setMainWidget(widget);
        connect(dlg, SIGNAL(cancelClicked()),
                dlg, SLOT(delayedDestruct()));
        connect(widget, SIGNAL(authenticated(KGAPI2::AccountPtr)),
                dlg, SLOT(delayedDestruct()));
        connect(widget, SIGNAL(error(KGAPI2::Error,QString)),
                dlg, SLOT(delayedDestruct()));

        dlg->show();
        dlg->setButtons(KDialog::Cancel);
        widget->authenticate();
    }
}

#include "authjob.moc"
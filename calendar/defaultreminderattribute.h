/*
    Calendar Resource - Default Reminder Attribute
    Copyright (C) 2012  Dan Vratil <dan@progdan.cz>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef DEFAULTREMINDERATTRIBUTE_H
#define DEFAULTREMINDERATTRIBUTE_H

#include <Akonadi/Attribute>

#include <libkgoogle/objects/calendar.h>

#ifdef WITH_KCAL
#include <KDE/KCal/Alarm>
#include <KDE/KCal/Incidence>
using namespace KCal;
#else
#include <KDE/KCalCore/Alarm>
#include <KDE/KCalCore/Incidence>
using namespace KCalCore;
#endif

using namespace KGoogle;

class DefaultReminderAttribute : public Akonadi::Attribute
{
  public:
    DefaultReminderAttribute(const Objects::Reminder::List &reminders = Objects::Reminder::List());

    Attribute* clone() const;
    void deserialize (const QByteArray& data);
    QByteArray serialized() const;
    QByteArray type() const;

    Alarm::List alarms(Incidence *incidence) const;

  private:
    Objects::Reminder::List m_reminders;
};

#endif // DEFAULTREMINDERATTRIBUTE_H

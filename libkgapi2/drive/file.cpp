/*
    Copyright 2012  Andrius da Costa Ribas <andriusmao@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "file.h"

using namespace KGAPI2;

///// DriveFile::Labels

class DriveFile::Labels::Private
{
  public:
    Private();
    Private(const Private &other);

    bool starred;
    bool hidden;
    bool trashed;
    bool restricted;
    bool viewed;
};

DriveFile::Labels::Private::Private():
    starred(false),
    hidden(false),
    trashed(false),
    restricted(false),
    viewed(false)
{
}

DriveFile::Labels::Private::Private(const Private &other):
    starred(other.starred),
    hidden(other.hidden),
    trashed(other.trashed),
    restricted(other.restricted),
    viewed(other.viewed)
{
}

DriveFile::Labels::Labels():
    d(new Private)
{
}

DriveFile::Labels::Labels(const Labels& other):
    d(new Private(*(other.d)))
{ }

DriveFile::Labels::~Labels()
{
    delete d;
}

bool DriveFile::Labels::starred() const
{
    return d->starred;
}

void DriveFile::Labels::setStarred(bool starred)
{
    d->starred = starred;
}

bool DriveFile::Labels::hidden() const
{
    return d->hidden;
}

void DriveFile::Labels::setHidden(bool hidden)
{
    d->hidden = hidden;
}

bool DriveFile::Labels::trashed() const
{
    return d->trashed;
}

void DriveFile::Labels::setTrashed(bool trashed)
{
    d->trashed = trashed;
}

bool DriveFile::Labels::restricted() const
{
    return d->restricted;
}

void DriveFile::Labels::setRestricted(bool restricted)
{
    d->restricted = restricted;
}

bool DriveFile::Labels::viewed() const
{
    return d->viewed;
}

void DriveFile::Labels::setViewed(bool viewed)
{
    d->viewed = viewed;
}

///// DriveFile::ImageMediaMetadata

class DriveFile::IndexableText::Private
{
  public:
    Private();
    Private(const Private &other);

    QString text;
};

DriveFile::IndexableText::Private::Private()
{
}

DriveFile::IndexableText::Private::Private(const Private &other):
    text(other.text)
{
}

DriveFile::IndexableText::IndexableText():
    d(new Private)
{
}

DriveFile::IndexableText::IndexableText(const IndexableText& other):
    d(new Private(*(other.d)))
{
}

DriveFile::IndexableText::~IndexableText()
{
    delete d;
}

QString DriveFile::IndexableText::text() const
{
    return d->text;
}

void DriveFile::IndexableText::setText(const QString& text)
{
    d->text = text;
}


///// DriveFile::ImageMediaMetadata::Location

class DriveFile::ImageMediaMetadata::Location::Private
{
  public:
    Private();
    Private(const Private &other);

    qreal latitude;
    qreal longitude;
    qreal altitude;
};

DriveFile::ImageMediaMetadata::Location::Private::Private():
    latitude(-1),
    longitude(-1),
    altitude(0)
{
}

DriveFile::ImageMediaMetadata::Location::Private::Private(const Private &other):
    latitude(other.latitude),
    longitude(other.longitude),
    altitude(other.altitude)
{
}


DriveFile::ImageMediaMetadata::Location::Location():
    d(new Private)
{
}

DriveFile::ImageMediaMetadata::Location::Location(const Location& other):
    d(new Private(*(other.d)))
{
}

DriveFile::ImageMediaMetadata::Location::~Location()
{
    delete d;
}

qreal DriveFile::ImageMediaMetadata::Location::latitude() const
{
    return d->latitude;
}

void DriveFile::ImageMediaMetadata::Location::setLatitude(qreal latitude)
{
    d->latitude = latitude;
}

qreal DriveFile::ImageMediaMetadata::Location::longitude() const
{
    return d->longitude;
}

void DriveFile::ImageMediaMetadata::Location::setLongitude(qreal longitude)
{
    d->longitude = longitude;
}

qreal DriveFile::ImageMediaMetadata::Location::altitude() const
{
    return d->altitude;
}

void DriveFile::ImageMediaMetadata::Location::setAltitude(qreal altitude)
{
    d->altitude = altitude;
}

///// DriveFile::ImageMediaMetadata

class DriveFile::ImageMediaMetadata::Private
{
  public:
    Private();
    Private(const Private &other);

    int width;
    int height;
    int rotation;
    LocationPtr location;
};

DriveFile::ImageMediaMetadata::Private::Private():
    width(-1),
    height(-1),
    rotation(-1)
{
}

DriveFile::ImageMediaMetadata::Private::Private(const Private &other):
    width(other.width),
    height(other.height),
    rotation(other.rotation),
    location(other.location)
{
}

DriveFile::ImageMediaMetadata::ImageMediaMetadata():
    d(new Private)
{
}

DriveFile::ImageMediaMetadata::ImageMediaMetadata(const ImageMediaMetadata& other):
    d(new Private(*(other.d)))
{
}

DriveFile::ImageMediaMetadata::~ImageMediaMetadata()
{
    delete d;
}

int DriveFile::ImageMediaMetadata::width() const
{
    return d->width;
}

void DriveFile::ImageMediaMetadata::setWidth(int width)
{
    d->width = width;
}

int DriveFile::ImageMediaMetadata::height() const
{
    return d->height;
}

void DriveFile::ImageMediaMetadata::setHeight(int height)
{
    d->height = height;
}

int DriveFile::ImageMediaMetadata::rotation() const
{
    return d->rotation;
}

void DriveFile::ImageMediaMetadata::setRotation(int rotation)
{
    d->rotation = rotation;
}

DriveFile::ImageMediaMetadata::LocationPtr DriveFile::ImageMediaMetadata::location() const
{
    return d->location;
}

void DriveFile::ImageMediaMetadata::setLocation(const LocationPtr &location)
{
    d->location = location;
}

////// DriveFile

class DriveFile::Private
{
  public:
    Private();
    Private(const Private &other);

    QString id;
    QUrl selfLink;
    QString title;
    QString mimeType;
    QString description;
    LabelsPtr labels;
    KDateTime createdDate;
    KDateTime modifiedDate;
    KDateTime modifiedByMeDate;
    QUrl downloadUrl;
    IndexableTextPtr indexableText;
    DrivePermissionPtr userPermission;
    QString fileExtension;
    QString md5Checksum;
    qlonglong fileSize;
    QUrl alternateLink;
    QUrl embedLink;
    KDateTime sharedWithMeDate;
    DriveParentReferencesList parents;
    QMap<QString,QUrl> exportLinks;
    QString originalFileName;
    qlonglong quotaBytesUsed;
    QList<QString> ownerNames;
    QString lastModifyingUserName;
    bool editable;
    bool writersCanShare;
    QUrl thumbnailLink;
    KDateTime lastViewedByMeDate;
    QUrl webContentLink;
    bool explicitlyTrashed;
    ImageMediaMetadataPtr imageMediaMetadata;
};

DriveFile::Private::Private():
    fileSize(-1),
    quotaBytesUsed(-1),
    editable(false),
    writersCanShare(false),
    explicitlyTrashed(false)
{
}

DriveFile::Private::Private(const Private& other):
    id(other.id),
    selfLink(other.selfLink),
    title(other.title),
    mimeType(other.mimeType),
    description(other.description),
    labels(other.labels),
    createdDate(other.createdDate),
    modifiedDate(other.modifiedDate),
    modifiedByMeDate(other.modifiedByMeDate),
    downloadUrl(other.downloadUrl),
    indexableText(other.indexableText),
    userPermission(other.userPermission),
    fileExtension(other.fileExtension),
    md5Checksum(other.md5Checksum),
    fileSize(other.fileSize),
    alternateLink(other.alternateLink),
    embedLink(other.embedLink),
    sharedWithMeDate(other.sharedWithMeDate),
    parents(other.parents),
    exportLinks(other.exportLinks),
    originalFileName(other.originalFileName),
    quotaBytesUsed(other.quotaBytesUsed),
    ownerNames(other.ownerNames),
    lastModifyingUserName(other.lastModifyingUserName),
    editable(other.editable),
    writersCanShare(other.writersCanShare),
    thumbnailLink(other.thumbnailLink),
    lastViewedByMeDate(other.lastViewedByMeDate),
    webContentLink(other.webContentLink),
    explicitlyTrashed(other.explicitlyTrashed),
    imageMediaMetadata(other.imageMediaMetadata)
{
}

DriveFile::DriveFile():
    KGAPI2::Object(),
    d(new Private)
{
}

DriveFile::DriveFile(const DriveFile& other):
    KGAPI2::Object(other),
    d(new Private(*(other.d)))
{ }

DriveFile::~DriveFile()
{
    delete d;
}

QString DriveFile::id() const
{
    return d->id;
}

void DriveFile::setId(const QString& id)
{
    d->id = id;
}

QUrl DriveFile::selfLink() const
{
    return d->selfLink;
}

void DriveFile::setSelfLink(const QUrl &selfLink)
{
    d->selfLink = selfLink;
}

QString DriveFile::title() const
{
    return d->title;
}

void DriveFile::setTitle(const QString& title)
{
    d->title = title;
}

QString DriveFile::mimeType() const
{
    return d->mimeType;
}

void DriveFile::setMimeType(const QString& mimeType)
{
    d->mimeType = mimeType;
}

QString DriveFile::description() const
{
    return d->description;
}

void DriveFile::setDescription(const QString& description)
{
    d->description = description;
}

DriveFile::LabelsPtr DriveFile::labels() const
{
    return d->labels;
}

void DriveFile::setLabels(const DriveFile::LabelsPtr &labels)
{
    d->labels = labels;
}

KDateTime DriveFile::createdDate() const
{
    return d->createdDate;
}

void DriveFile::setCreatedDate(const KDateTime& createdDate)
{
    d->createdDate = createdDate;
}

KDateTime DriveFile::modifiedDate() const
{
    return d->modifiedDate;
}

void DriveFile::setModifiedDate(const KDateTime& modifiedDate)
{
    d->modifiedDate = modifiedDate;
}

KDateTime DriveFile::modifiedByMeDate() const
{
    return d->modifiedByMeDate;
}

void DriveFile::setModifiedByMeDate(const KDateTime& modifiedByMeDate)
{
    d->modifiedByMeDate = modifiedByMeDate;
}

QUrl DriveFile::downloadUrl() const
{
    return d->downloadUrl;
}

void DriveFile::setDownloadUrl(const QUrl &downloadUrl)
{
    d->downloadUrl = downloadUrl;
}

DriveFile::IndexableTextPtr DriveFile::indexableText() const
{
    return d->indexableText;
}

void DriveFile::setIndexableText(const DriveFile::IndexableTextPtr &indexableText)
{
    d->indexableText = indexableText;
}

DrivePermissionPtr DriveFile::userPermission() const
{
    return d->userPermission;
}

void DriveFile::setUserPermission(const DrivePermissionPtr& userPermission)
{
    d->userPermission = userPermission;
}

QString DriveFile::fileExtension() const
{
    return d->fileExtension;
}

void DriveFile::setFileExtension(const QString& fileExtension)
{
    d->fileExtension = fileExtension;
}

QString DriveFile::md5Checksum() const
{
    return d->md5Checksum;
}

void DriveFile::setMd5Checksum(const QString& md5Checksum)
{
    d->md5Checksum = md5Checksum;
}

qlonglong DriveFile::fileSize() const
{
    return d->fileSize;
}

void DriveFile::setFileSize(qlonglong fileSize)
{
    d->fileSize = fileSize;
}

QUrl DriveFile::alternateLink() const
{
    return d->alternateLink;
}

void DriveFile::setAlternateLink(const QUrl &alternateLink)
{
    d->alternateLink = alternateLink;
}

QUrl DriveFile::embedLink() const
{
    return d->embedLink;
}

void DriveFile::setEmbedLink(const QUrl &embedLink)
{
    d->embedLink = embedLink;
}

KDateTime DriveFile::sharedWithMeDate() const
{
    return d->sharedWithMeDate;
}

void DriveFile::setSharedWithMeDate(const KDateTime& sharedWithMeDate)
{
    d->sharedWithMeDate = sharedWithMeDate;
}

DriveParentReferencesList DriveFile::parents() const
{
    return d->parents;
}

void DriveFile::setParents(const DriveParentReferencesList &parents)
{
    d->parents = parents;
}

QMap< QString, QUrl > DriveFile::exportLinks() const
{
    return d->exportLinks;
}

void DriveFile::setExportLinks(const QMap< QString, QUrl > &exportLinks)
{
    d->exportLinks = exportLinks;
}

QString DriveFile::originalFileName() const
{
    return d->originalFileName;
}

void DriveFile::setOriginalFileName(const QString& originalFileName)
{
    d->originalFileName = originalFileName;
}

qlonglong DriveFile::quotaBytesUsed() const
{
    return d->quotaBytesUsed;
}

void DriveFile::setQuotaBytesUsed(qlonglong quotaBytesUsed)
{
    d->quotaBytesUsed = quotaBytesUsed;
}

QStringList DriveFile::ownerNames() const
{
    return d->ownerNames;
}

void DriveFile::setOwnerNames(const QStringList& ownerNames)
{
    d->ownerNames = ownerNames;
}

QString DriveFile::lastModifyingUserName() const
{
    return d->lastModifyingUserName;
}

void DriveFile::setLastModifyingUserName(const QString& lastModifyingUserName)
{
    d->lastModifyingUserName = lastModifyingUserName;
}

bool DriveFile::editable() const
{
    return d->editable;
}

void DriveFile::setEditable(bool editable)
{
    d->editable = editable;
}

bool DriveFile::writersCanShare() const
{
    return d->writersCanShare;
}

void DriveFile::setWritersCanShare(bool writersCanShare)
{
    d->writersCanShare = writersCanShare;
}

QUrl DriveFile::thumbnailLink() const
{
    return d->thumbnailLink;
}

void DriveFile::setThumbnailLink(const QUrl &thumbnailLink)
{
    d->thumbnailLink = thumbnailLink;
}

KDateTime DriveFile::lastViewedByMeDate() const
{
    return d->lastViewedByMeDate;
}

void DriveFile::setLastViewedByMeDate(const KDateTime& lastViewedByMeDate)
{
    d->lastViewedByMeDate = lastViewedByMeDate;
}

QUrl DriveFile::webContentLink() const
{
    return d->webContentLink;
}

void DriveFile::setWebContentLink(const QUrl &webContentLink)
{
    d->webContentLink = webContentLink;
}

bool DriveFile::explicitlyTrashed() const
{
    return d->explicitlyTrashed;
}

void DriveFile::setExplicitlyTrashed(bool explicitlyTrashed)
{
    d->explicitlyTrashed = explicitlyTrashed;
}

DriveFile::ImageMediaMetadataPtr DriveFile::imageMediaMetadata() const
{
    return d->imageMediaMetadata;
}

void DriveFile::setImageMediaMetadata(const DriveFile::ImageMediaMetadataPtr &imageMediaMetadata)
{
    d->imageMediaMetadata = imageMediaMetadata;
}

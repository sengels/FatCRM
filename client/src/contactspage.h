#ifndef CONTACTSPAGE_H
#define CONTACTSPAGE_H

#include "ui_contactspage.h"
#include <contactdetails.h>

#include <akonadi/collection.h>

#include <QtGui/QWidget>

namespace Akonadi
{
    class ChangeRecorder;
    class Item;
}

class KJob;
class QModelIndex;

class ContactsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ContactsPage( QWidget *parent = 0 );

    ~ContactsPage();

Q_SIGNALS:
    void contactItemChanged();

private:
    void initialize();

    ContactDetails *mDetailsWidget;
    Akonadi::ChangeRecorder *mChangeRecorder;
    Akonadi::Collection mContactsCollection;
    Akonadi::Item mCurrentItem;
    Ui_ContactsPage mUi;

private Q_SLOTS:
    void slotResourceSelectionChanged( const QByteArray &identifier );
    void slotCollectionFetchResult( KJob *job );
    void slotContactChanged( const Akonadi::Item &item );
    void slotNewContactClicked();
    void slotFilterChanged( const QString& text );
    void slotAddContact( const Akonadi::Item &item );
    void slotSetCurrent( const QModelIndex&,int,int );
};

#endif

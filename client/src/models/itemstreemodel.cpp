#include "itemstreemodel.h"

#include <kdcrmdata/sugaraccount.h>
#include <kdcrmdata/sugarcampaign.h>
#include <kdcrmdata/sugarlead.h>
#include <kdcrmdata/sugaropportunity.h>

#include <kabc/addressee.h>
#include <kabc/phonenumber.h>

#include <kglobal.h>
#include <kicon.h>
#include <kiconloader.h>
#include <klocale.h>

using namespace Akonadi;

class ItemsTreeModel::Private
{
  public:
    Private()
      : mColumns( ItemsTreeModel::Columns() << ItemsTreeModel::Name ),
        mIconSize( KIconLoader::global()->currentSize( KIconLoader::Small ) )
    {
    }

    Columns mColumns;
    const int mIconSize;
};

ItemsTreeModel::ItemsTreeModel( DetailsType type, ChangeRecorder *monitor, QObject *parent )
  : EntityTreeModel( monitor, parent ), d( new Private ), mType( type )
{
    setColumns();
}

ItemsTreeModel::~ItemsTreeModel()
{
  delete d;
}

/**
 * Sets the columns that the model should show.
 */
void ItemsTreeModel::setColumns()
{
    emit layoutAboutToBeChanged();
    d->mColumns = columnsGroup( mType );
    emit layoutChanged();
}

/**
 * Returns the columns that the model currently shows.
 */
ItemsTreeModel::Columns ItemsTreeModel::columns() const
{
  return d->mColumns;
}

/**
 * Reimp: Returns the data displayed by the model
 */
QVariant ItemsTreeModel::entityData( const Item &item, int column, int role ) const
{
  if ( item.mimeType() == SugarAccount::mimeType() )
      return accountData( item, column, role );
  else if ( item.mimeType() == SugarCampaign::mimeType() )
      return campaignData( item, column, role );
  else if ( item.mimeType() == KABC::Addressee::mimeType() )
      return contactData( item, column, role );
  else if ( item.mimeType() == SugarLead::mimeType() )
      return leadData( item, column, role );
  else if ( item.mimeType() == SugarOpportunity::mimeType() )
      return opportunityData( item, column, role );

  return EntityTreeModel::entityData( item, column, role );
}

/**
 * Reimp
 */
QVariant ItemsTreeModel::entityData( const Collection &collection, int column, int role ) const
{
  if ( role == Qt::DisplayRole ) {
    switch ( column ) {
      case 0:
        return EntityTreeModel::entityData( collection, column, role );
      default:
        return QString(); // pass model test
    }
  }

  return EntityTreeModel::entityData( collection, column, role );
}

/**
 * Reimp
 */
int ItemsTreeModel::entityColumnCount( HeaderGroup headerGroup ) const
{
  if ( headerGroup == EntityTreeModel::CollectionTreeHeaders ) {
    return 1;
  } else if ( headerGroup == EntityTreeModel::ItemListHeaders ) {
    return d->mColumns.count();
  } else {
    return EntityTreeModel::entityColumnCount( headerGroup );
  }
}

/**
 * Reimp: Return the Header data to display
 */
QVariant ItemsTreeModel::entityHeaderData( int section, Qt::Orientation orientation, int role, HeaderGroup headerGroup ) const
{
  if ( role == Qt::DisplayRole ) {
    if ( orientation == Qt::Horizontal ) {
      if ( headerGroup == EntityTreeModel::CollectionTreeHeaders ) {

        if ( section >= 1 )
          return QVariant();

        switch ( section ) {
          case 0:
            return i18nc( "@title:Sugar items overview", "Items" );
            break;
        }
      } else if ( headerGroup == EntityTreeModel::ItemListHeaders ) {
        if ( section < 0 || section >= d->mColumns.count() )
          return QVariant();

        switch ( d->mColumns.at( section ) ) {
        case Name:
            return i18nc( "@title:column name", "Name" );
        case City:
            return i18nc( "@title:column city", "City" );
        case Country:
            return i18nc( "@title:column country ", "Country" );
        case Phone:
            return i18nc( "@title:column phone", "Phone" );
        case Email:
            return i18nc( "@title:column email", "Email" );
        case CreatedBy:
            return i18nc( "@title:column created by user", "Created By" );
        case CampaignName:
            return i18nc( "@title:column name of a campaign ", "Campaign" );
        case Status:
            return i18nc( "@title:column status - status", "Status" );
        case Type:
            return i18nc( "@title:column type - Campaign type", "Type" );
        case EndDate:
            return i18nc( "@title:column end date - End Date", "End Date" );
        case User:
            return i18nc( "@title:column Assigned User Name", "User" );
        case FullName:
            return i18nc( "@title:column full name of a contact ", "Name" );
        case Role:
            return i18nc( "@title:column role - role", "Role" );
        case Organization:
            return i18nc( "@title:column company", "Organization" );
        case PreferredEmail:
            return i18nc( "@title:column email", "Preferred Email" );
        case PhoneNumber:
            return i18nc( "@title:column phone work", "Phone Work" );
        case GivenName:
            return i18nc( "@title:column given name", "Given Name" );
        case LeadName:
            return i18nc( "@title:column Lead's Full Name", "Name" );
        case LeadStatus:
            return i18nc( "@title:column Lead's Status", "Status" );
        case LeadAccountName:
            return i18nc( "@title:column Account Name", "Account Name" );
        case LeadEmail:
            return i18nc( "@title:column Lead's Primary email", "Email" );
        case LeadUser:
            return i18nc( "@title:column Lead's Assigny name", "User" );
        case OpportunityName:
            return i18nc( "@title:column name for the Opportunity", "Name" );
        case OpportunityAccountName:
            return i18nc( "@title:column account name", "Account Name" );
        case SalesStage:
            return i18nc( "@title:column sales stage", "Sales Stage" );
        case Amount:
            return i18nc( "@title:column amount", "Amount" );
        case Close:
            return i18nc( "@title:column date closed", "Close" );
        case AssignedTo:
            return i18nc( "@title:column assigned to name", "Assigned To" );
        }
      }
    }
  }

  return EntityTreeModel::entityHeaderData( section, orientation, role, headerGroup );
}

/**
 * Return the data. SugarAccount type
 */
QVariant ItemsTreeModel::accountData( const Item &item, int column, int role ) const
{
    if ( !item.hasPayload<SugarAccount>() ) {
      // Pass modeltest
        if ( role == Qt::DisplayRole )
            return item.remoteId();

        return QVariant();
    }
    const SugarAccount account = item.payload<SugarAccount>();

    if ( (role == Qt::DisplayRole) || (role == Qt::EditRole) ) {
        switch ( columns().at( column ) ) {
        case Name:
            return account.name();
        case City:
            return account.billingAddressCity();
        case Country:
            return account.billingAddressCountry();
        case Phone:
            return account.phoneOffice();
        case Email:
            return account.email1();
        case CreatedBy:
          return account.createdByName();
        default:
            return QVariant();
      }
    }
    return QVariant();
}

/**
 * Return the data. SugarCampaign type
 */
QVariant ItemsTreeModel::campaignData( const Item &item, int column, int role ) const
{
    if ( !item.hasPayload<SugarCampaign>() ) {

        // Pass modeltest
        if ( role == Qt::DisplayRole )
            return item.remoteId();

        return QVariant();
    }

    const SugarCampaign campaign = item.payload<SugarCampaign>();

    if ( (role == Qt::DisplayRole) || (role == Qt::EditRole) ) {
        switch ( columns().at( column ) ) {
        case CampaignName:
            return campaign.name();
        case Status:
            return campaign.status();
        case Type:
            return campaign.campaignType();
        case EndDate:
            return campaign.endDate();
        case User:
            return campaign.assignedUserName();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

/**
 * Return the data. KABC::Addressee type - ref: Contacts
 */
QVariant ItemsTreeModel::contactData( const Item &item, int column, int role ) const
{
    if ( !item.hasPayload<KABC::Addressee>() ) {

      // Pass modeltest
        if ( role == Qt::DisplayRole )
            return item.remoteId();

        return QVariant();
    }

    const KABC::Addressee addressee = item.payload<KABC::Addressee>();

    if ( (role == Qt::DisplayRole) || (role == Qt::EditRole) ) {
      switch ( columns().at( column ) ) {
        case FullName:
            return addressee.assembledName();
        case Role:
            return addressee.role();
        case Organization:
            return addressee.organization();
        case PreferredEmail:
            return addressee.preferredEmail();
        case PhoneNumber:
            return addressee.phoneNumber(KABC::PhoneNumber::Work).number();
        case GivenName:
          return addressee.givenName();
      default:
          return QVariant();
      }
    }
    return QVariant();
}

/**
 * Return the data. SugarLead type
 */
QVariant ItemsTreeModel::leadData( const Item &item, int column, int role ) const
{
    if ( !item.hasPayload<SugarLead>() ) {

      // Pass modeltest
      if ( role == Qt::DisplayRole )
        return item.remoteId();

      return QVariant();
    }

    const SugarLead lead = item.payload<SugarLead>();

    if ( (role == Qt::DisplayRole) || (role == Qt::EditRole) ) {
      switch ( columns().at( column ) ) {
        case LeadName:
            return lead.lastName();
        case LeadStatus:
            return lead.status();
        case LeadAccountName:
            return lead.accountName();
        case LeadEmail:
            return lead.email1();
        case LeadUser:
            return lead.assignedUserName();
      default:
          return QVariant();
      }
    }
    return QVariant();
}

/**
 * Return the data. SugarOpportunity type
 */
QVariant ItemsTreeModel::opportunityData( const Item &item, int column, int role ) const
{
    if ( !item.hasPayload<SugarOpportunity>() ) {

      // Pass modeltest
      if ( role == Qt::DisplayRole )
        return item.remoteId();

      return QVariant();
    }

    const SugarOpportunity opportunity = item.payload<SugarOpportunity>();

    if ( (role == Qt::DisplayRole) || (role == Qt::EditRole) ) {
      switch ( columns().at( column ) ) {
        case OpportunityName:
            return opportunity.name();
        case OpportunityAccountName:
            return opportunity.accountName();
        case SalesStage:
            return opportunity.salesStage();
        case Amount:
            return opportunity.amount();
        case Close:
            return opportunity.dateClosed();
        case AssignedTo:
          return opportunity.assignedUserName();
      default:
          return QVariant();
      }
    }
  return QVariant();
}

ItemsTreeModel::Columns ItemsTreeModel::columnsGroup( DetailsType type ) const
{
    ItemsTreeModel::Columns columns;

    switch( type ) {
    case Account:
        columns << ItemsTreeModel::Name
                << ItemsTreeModel::City
                << ItemsTreeModel::Country
                << ItemsTreeModel::Phone
                << ItemsTreeModel::Email
                << ItemsTreeModel::CreatedBy;
        break;
    case Contact:
        columns << ItemsTreeModel::FullName
                << ItemsTreeModel::Role
                << ItemsTreeModel::Organization
                << ItemsTreeModel::PreferredEmail
                << ItemsTreeModel::PhoneNumber
                << ItemsTreeModel::GivenName;
        break;
    case Lead:
        columns << ItemsTreeModel::LeadName
                << ItemsTreeModel::LeadStatus
                << ItemsTreeModel::LeadAccountName
                << ItemsTreeModel::LeadEmail
                << ItemsTreeModel::LeadUser;
        break;
    case Opportunity:
        columns << ItemsTreeModel::OpportunityName
                << ItemsTreeModel::OpportunityAccountName
                << ItemsTreeModel::SalesStage
                << ItemsTreeModel::Amount
                << ItemsTreeModel::Close
                << ItemsTreeModel::AssignedTo;
        break;
    case Campaign:
        columns << ItemsTreeModel::CampaignName
                << ItemsTreeModel::Status
                << ItemsTreeModel::Type
                << ItemsTreeModel::EndDate
                << ItemsTreeModel::User;
    default:
        return columns;
    }

    return columns;
}

#include "itemstreemodel.moc"

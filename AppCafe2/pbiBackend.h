#ifndef _APPCAFE_PBI_BACKEND_H
#define _APPCAFE_PBI_BACKEND_H

/*  TERMINOLOGY NOTE:
	"PBI" refers to locally installed applications
	"APP" refers to available applications in the repo database
	"CAT" refers to available categories in the repo database
*/
#include <QFileSystemWatcher>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDate>

#include "containers.h"
#include "extras.h"
#include "pbiDBAccess.h"
#include "processManager.h"

class PBIBackend : public QObject{
	Q_OBJECT

public:
	//Initializations
	PBIBackend();
	~PBIBackend(){}
	// Clean Start/Stop functions
	void setWardenMode(QString dir, QString ip);
	void setDownloadDir(QString);
	void keepDownloadedFiles(bool);
	bool start();
	int numInstalled, numAvailable;
	//Search variables for public slot inputs
	QString searchTerm;
	QString searchSimilar;
	// Main Listing functions
	QStringList installedList(); //return unique ID's of all installed PBI's
	QStringList browserCategories(); //return list of available browser categories
	QStringList browserApps( QString catID ); //list all apps in the given category
	QStringList getRecentApps(); //list all recent applications/update
	// Local/Repo Interaction
	QString isInstalled(QString appID); //returns pbiID that is installed (if possible);
	QString upgradeAvailable(QString pbiID); //returns ID for PBI upgrade (if available)
	QString downgradeAvailable(QString pbiID); //returns ID for PBI downgrade (if available)
	void upgradePBI(QStringList pbiID); //start upgrade process for list of PBI's
	void removePBI(QStringList pbiID); //start the removal process
	void stopUpdate(QStringList pbiID); //stop upgrade/downgrade/removal process
	void installApp(QStringList appID); //[install/upgrade/downgrade] application from the repo (as appropriate)
	void addDesktopIcons(QStringList pbiID, bool allusers); // add XDG desktop icons
	void addMenuIcons(QStringList pbiID, bool allusers); // add XDG menu icons
	void addPaths(QStringList pbiID, bool allusers); // create path links
	void addMimeTypes(QStringList pbiID, bool allusers); //add XDG Mime type associations
	void rmDesktopIcons(QStringList pbiID, bool allusers); // remove XDG desktop icons
	void rmMenuIcons(QStringList pbiID, bool allusers); // remove XDG menu icons
	void rmPaths(QStringList pbiID, bool allusers); // remove path links
	void rmMimeTypes(QStringList pbiID, bool allusers); //remove XDG Mime type associations
	
	// Information Retrieval
	QStringList PBIInfo( QString pbiID, QStringList infoList);
	QStringList CatInfo( QString catID, QStringList infoList);
	QStringList AppInfo( QString appID, QStringList infoList);

public slots:
	void startAppSearch(); //get list of apps containing the search string (SIGNAL OUTPUT ONLY)
	void startSimilarSearch(); //get list of apps that are similar to the input app
	
private:
	//variables - warden mode
	QString wardenDir, wardenIP;
	bool wardenMode;
	//variables - filesystem watcher;
	QFileSystemWatcher *watcher;
	QString baseDBDir; //  This is always /var/db/pbi/
	QString repoNumber, DBDir;
	//variables - database
	PBIDBAccess *sysDB;
	QHash<QString, InstalledPBI> PBIHASH;
	QHash<QString, MetaCategory> CATHASH;
	QHash<QString, MetaPBI> APPHASH;
	bool noRepo;
	//variables - processes
	ProcessManager *PMAN;
	QString cDownload, cInstall, cRemove, cUpdate, cDownloadFile, cOther; //currently running command/pbi
	QStringList PENDINGDL, PENDINGINSTALL, PENDINGREMOVAL, PENDINGUPDATE, PENDINGOTHER;
	//variables - other
	QString baseDlDir, dlDir; // download/install directories
	QString sysArch; //system architecture
	//User Preferences
	bool keepDownloads, autoDesktop;	

	//functions
	QString addRootCMD(QString cmd, bool needRoot);
	QString generateUpdateCMD(QString pbiID);
	QString generateRemoveCMD(QString pbiID);
	QString generateAutoUpdateCMD(QString pbiID, bool enable);
	QString generateXDGCMD(QString pbiID, QStringList actions, bool allusers = FALSE);
	QString generateDownloadCMD(QString appID, QString version="");
	QString generateInstallCMD(QString pbiID);
	
private slots:
	//Process functions
	void checkProcesses();
	void slotProcessFinished(int);
	void slotProcessMessage(int, QString);
	void slotProcessError(int, QString);

	// Database watcher
	void slotSyncToDatabase(bool localChanges=TRUE);
	void syncPBI(QString pbiID, bool useDB=TRUE);
	void slotUpdateAllStatus();
	void updateStatus(QString pbiID);
	void syncCurrentRepo();

	
signals:
	void RepositoryInfoReady();
	void ErrorNoRepo();
	void LocalPBIChanges();
	void PBIStatusChange(QString pbiID);
	//Process Signals
	void Error(QString title, QString message);
	//Search results
	void SearchComplete(QStringList, QStringList);// "best" and "rest" results lists
	void SimilarFound(QStringList);
};

#endif

#ifndef _APPCAFE_MAINUI_H
#define _APPCAFE_MAINUI_H


#ifndef PREFIX
#define PREFIX QString("/usr/local")
#endif

/* QT4 */
#include <QDebug>
#include <QDialog>
#include <QMenu>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <QScrollBar>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>

/* LibPCBSD Includes */
#include <pcbsd-sysFlags.h>

/* Local Includes */
#include "pbiNgBackend.h"
#include "extras.h"
#include "largeItemWidget.h"
#include "smallItemWidget.h"

namespace Ui {
    class MainUI;
}

class MainUI : public QMainWindow
{
        Q_OBJECT

public:
   explicit MainUI(QWidget* parent = 0);
   void ProgramInit();
   //void setWardenMode(QString dir,QString ip); //call before ProgramInit

public slots:
    void slotSingleInstance();
    
protected:
    void closeEvent(QCloseEvent *event);

private slots:
  //MENU OPTIONS
  void on_actionImport_PBI_List_triggered();
  void on_actionExport_PBI_List_triggered();
  void on_actionQuit_triggered();
  void on_actionAppCafe_Settings_triggered();
  //void on_actionInstall_From_File_triggered();
  //INSTALLED TAB
  void slotRefreshInstallTab();
  void slotCheckSelectedItems();
  void slotPBIStatusUpdate(QString);
  void on_tool_install_details_clicked();
  void on_tool_install_back_clicked();
  void on_tool_install_gotobrowserpage_clicked();
  void on_tool_install_toggleall_clicked();
  void on_tree_install_apps_itemSelectionChanged();
  void on_tree_install_apps_itemDoubleClicked(QTreeWidgetItem *item);
  //void on_check_install_autoupdate_clicked();
  //void on_tool_install_update_clicked();
  void on_tool_install_remove_clicked();
  void on_tool_install_cancel_clicked();
  void on_tool_install_maintainer_clicked();
  void slotInstalledAppRightClicked(const QPoint &);
  void contextMenuFinished();
  void slotActionAddDesktop();
  void slotActionRemoveDesktop();
  //void slotActionAddPath();
  //void slotActionRemovePath();
  //void slotActionAddPathAll();
  //void slotActionAddMenu();
  //void slotActionRemoveMenu();
  //void slotActionAddMenuAll();
  //void slotActionAddMime();
  //void slotActionRemoveMime();
  //void slotActionAddMimeAll();
  //void slotActionUpdate();
  void slotActionRemove();
  void slotActionCancel();
  void slotStartApp(QAction*);
  void slotUpdateSelectedPBI();
  void updateInstallDetails(QString appID);
  //BROWSER TAB
  void slotDisableBrowser(bool shownotification = TRUE);
  void slotEnableBrowser();
  void slotUpdateBrowserHome();
  void slotGoToHome();
  void slotGoToCatBrowser();
  void slotGoToCategory(QString);
  void slotGoToApp(QString);
  void slotUpdateAppDownloadButton();
  void slotGoToSearch();
  void slotShowSimilarApps(QStringList);
  void slotShowSearchResults(QStringList, QStringList);
  void on_tabWidget_currentChanged();
  void on_tool_browse_home_clicked();
  void on_tool_browse_cat_clicked();
  void on_tool_browse_app_clicked();
  void on_line_browse_searchbar_textChanged();
  void on_tool_bapp_download_clicked();
  void on_group_br_home_newapps_toggled(bool);
  void on_group_bapp_similar_toggled(bool);
  //OTHER
  void slotDisplayError(QString,QString,QStringList);
  
private:
  Ui::MainUI *ui;
  QString defaultIcon;
  PBIBackend *PBI;
  //INSTALLED TAB
  QMenu *actionMenu, *appBinMenu, *shortcutMenu, *sDeskMenu, *contextActionMenu;
  QString cDetails;
  void initializeInstalledTab();
  void formatInstalledItemDisplay(QTreeWidgetItem *item);
  QStringList getCheckedItems();
  //BROWSER TAB
  QTimer *searchTimer;
  QString cCat, cApp;
  QString bCat, bApp; //current cat/app for the buttons
  void initializeBrowserTab();
  //OTHER
  QLabel *statusLabel;
  void clearScrollArea(QScrollArea*);
  void slotDisplayStats();
    
} ;

#endif


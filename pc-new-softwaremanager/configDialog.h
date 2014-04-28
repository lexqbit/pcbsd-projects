/***************************************************************************
 *   Copyright (C) 2011 - iXsystems				 	  *
 *   kris@pcbsd.org  *
 *   tim@pcbsd.org   *
 *   ken@pcbsd.org   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#ifndef _APPCAFE_CONFIG_DIALOG_H
#define _APPCAFE_CONFIG_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include "pbiDBAccess.h"

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog{
	Q_OBJECT
	
public:
	explicit ConfigDialog(QWidget* parent = 0);
	virtual ~ConfigDialog();
	
	void setupDone(); //make sure the public variables below are set before running this
	
	//Data setup/retrieval variables
	bool applyChanges;
	QStringList xdgOpts;
	bool keepDownloads;
	QString downloadDir;
	PBIDBAccess *DB;
	
private:
	Ui::ConfigDialog *ui;
	QString repoID;
	
private slots:
	//Repo Tab
	void refreshRepoTab();
	void on_combo_repo_currentIndexChanged();
	void on_tool_repo_add_clicked();
	void on_tool_repo_remove_clicked();
	void on_tool_repomirror_add_clicked();
	void on_tool_repomirror_remove_clicked();
	void on_tool_repomirror_up_clicked();
	void on_tool_repomirror_down_clicked();
	
	//Config Tab
	void on_group_download_toggled(bool);
	void on_tool_getDownloadDir_clicked();
	
	//ButtonBox
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
	
};

#endif

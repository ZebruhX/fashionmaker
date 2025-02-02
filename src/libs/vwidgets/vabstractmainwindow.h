/************************************************************************
 **
 **  @file
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   19 7, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VABSTRACTMAINWINDOW_H
#define VABSTRACTMAINWINDOW_H

#include <qcompilerdetection.h>
#include <QMainWindow>
#include <QMetaObject>
#include <QObject>
#include <QString>

struct VFinalMeasurement;

class VAbstractMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit VAbstractMainWindow(QWidget *parent = nullptr);
    virtual ~VAbstractMainWindow() Q_DECL_EQ_DEFAULT;

public slots:
    virtual void ShowToolTip(const QString &toolTip)=0;
    virtual void UpdateVisibilityGroups();
    virtual void UpdateDetailsList();
    virtual void ZoomFitBestCurrent();

protected slots:
    void WindowsLocale();
    void ExportDataToCSV();

protected:
    int     m_curFileFormatVersion;
    QString m_curFileFormatVersionStr;
    enum { MaxRecentFiles = 5 };
    QVector<QAction *> m_recentFileActs{QVector<QAction *>(MaxRecentFiles)};
    QAction *m_separatorAct{nullptr};

    bool ContinueFormatRewrite(const QString &currentFormatVersion, const QString &maxFormatVersion);
    void ToolBarStyle(QToolBar *bar);

    QString CSVFilePath();

    virtual void ExportToCSVData(const QString &fileName, bool withHeader, int mib, const QChar &separator)=0;

    virtual QStringList RecentFileList() const =0;
    void UpdateRecentFileActions();

private:
    Q_DISABLE_COPY(VAbstractMainWindow)
};

#endif // VABSTRACTMAINWINDOW_H

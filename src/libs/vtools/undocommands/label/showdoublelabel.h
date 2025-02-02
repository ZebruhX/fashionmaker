/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 9, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef SHOWDOUBLELABEL_H
#define SHOWDOUBLELABEL_H

#include "../vundocommand.h"

class QGraphicsScene;

enum class ShowDoublePoint: qint8 { FirstPoint, SecondPoint };

class ShowDoubleLabel : public VUndoCommand
{
    Q_OBJECT
public:
    ShowDoubleLabel(VAbstractPattern *doc, quint32 toolId, quint32 pointId, bool visible, ShowDoublePoint type,
                    QUndoCommand *parent = nullptr);
    virtual ~ShowDoubleLabel()=default;

    virtual void undo() override;
    virtual void redo() override;
private:
    Q_DISABLE_COPY(ShowDoubleLabel)
    bool m_visible;
    bool m_oldVisible;
    //Need for resizing scene rect
    QGraphicsScene *m_scene;
    ShowDoublePoint m_type;
    quint32         m_idTool;

    void Do(bool visible);
};

#endif // SHOWDOUBLELABEL_H

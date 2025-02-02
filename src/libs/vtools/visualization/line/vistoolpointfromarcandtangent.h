/************************************************************************
 **
 **  @file   vistoolpointfromarcandtangent.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VISTOOLPOINTFROMARCANDTANGENT_H
#define VISTOOLPOINTFROMARCANDTANGENT_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "visline.h"

class VArc;

class VisToolPointFromArcAndTangent : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolPointFromArcAndTangent(const VContainer *data, QGraphicsItem *parent = nullptr);
    virtual ~VisToolPointFromArcAndTangent() = default;

    virtual void RefreshGeometry() override;

    void         setArcId(const quint32 &value);
    void         setCrossPoint(const CrossCirclesPoint &value);

    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::ToolPointFromArcAndTangent)};
private:
    Q_DISABLE_COPY(VisToolPointFromArcAndTangent)
    quint32           arcId;
    CrossCirclesPoint crossPoint;
    VScaledEllipse   *point;
    VScaledEllipse   *tangent;
    VCurvePathItem   *arcPath;

    void FindRays(const QPointF &p, const VArc *arc);
};

#endif // VISTOOLPOINTFROMARCANDTANGENT_H

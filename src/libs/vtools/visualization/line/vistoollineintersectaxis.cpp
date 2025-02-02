/************************************************************************
 **
 **  @file   vislineintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 10, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vistoollineintersectaxis.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QLine>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>
#include <new>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollineintersectaxis.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "../visualization.h"
#include "visline.h"
#include "../vmisc/vmodifierkey.h"
#include "../vmisc/compatibility.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolLineIntersectAxis::VisToolLineIntersectAxis(const VContainer *data, QGraphicsItem *parent)
    : VisLine(data, parent), point2Id(NULL_ID), axisPointId(NULL_ID), angle(-1), point(nullptr), lineP1(nullptr),
      lineP2(nullptr), basePoint(nullptr), baseLine(nullptr), axisLine(nullptr), line_intersection(nullptr)
{
    this->mainColor = Qt::red;

    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this);
    basePoint = InitPoint(supportColor, this);
    baseLine = InitItem<VScaledLine>(supportColor, this);
    axisLine = InitItem<VScaledLine>(supportColor, this);
    line_intersection = InitItem<VScaledLine>(supportColor, this);
    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersectAxis::RefreshGeometry()
{
    if (object1Id > NULL_ID)
    {
        const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(lineP1, static_cast<QPointF>(*first), supportColor);

        if (point2Id <= NULL_ID)
        {
            DrawLine(baseLine, QLineF(static_cast<QPointF>(*first), Visualization::scenePos), supportColor);
        }
        else
        {
            const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(point2Id);
            DrawPoint(lineP2, static_cast<QPointF>(*second), supportColor);

            const QLineF base_line(static_cast<QPointF>(*first), static_cast<QPointF>(*second));
            DrawLine(baseLine, base_line, supportColor);

            if (axisPointId > NULL_ID)
            {
                QLineF axis;
                const QSharedPointer<VPointF> third = Visualization::data->GeometricObject<VPointF>(axisPointId);
                if (VFuzzyComparePossibleNulls(angle, -1))
                {
                    axis = Axis(static_cast<QPointF>(*third), Visualization::scenePos);
                }
                else
                {
                    axis = Axis(static_cast<QPointF>(*third), angle);
                }
                DrawPoint(basePoint, static_cast<QPointF>(*third), mainColor);
                DrawLine(axisLine, axis, supportColor, Qt::DashLine);

                QPointF p;
                VToolLineIntersectAxis::FindPoint(axis, base_line, &p);
                QLineF axis_line(static_cast<QPointF>(*third), p);
                DrawLine(this, axis_line, mainColor, lineStyle);

                DrawPoint(point, p, mainColor);
                ShowIntersection(axis_line, base_line);

                Visualization::toolTip = tr("<b>Intersection line and axis</b>: angle = %1°; <b>%2</b> - "
                                            "sticking angle, <b>%3</b> - finish creation")
                        .arg(this->line().angle())
                        .arg(VModifierKey::Shift(), VModifierKey::EnterKey());
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolLineIntersectAxis::Angle() const
{
    return QString("%1").arg(this->line().angle());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersectAxis::SetAngle(const QString &expression)
{
    angle = FindValFromUser(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersectAxis::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersectAxis::setAxisPointId(const quint32 &value)
{
    axisPointId = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLineIntersectAxis::ShowIntersection(const QLineF &axis_line, const QLineF &base_line)
{
    QPointF p;
    QLineF::IntersectType intersect = Intersects(axis_line, base_line, &p);

    if (intersect == QLineF::UnboundedIntersection)
    {
        line_intersection->setVisible(true);
        DrawLine(line_intersection, QLineF(base_line.p1(), axis_line.p2()), supportColor, Qt::DashLine);
    }
    else if (intersect == QLineF::BoundedIntersection || intersect == QLineF::NoIntersection)
    {
        line_intersection->setVisible(false);
    }
}

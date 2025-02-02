/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   3 11, 2016
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

#include "vpiecenode.h"
#include "vpiecenode_p.h"
#include "vcontainer.h"
#include "calculator.h"
#include "vformula.h"
#include "../vmisc/vabstractapplication.h"

#include <QDataStream>
#include <QtNumeric>

const quint32 VPieceNodeData::streamHeader = 0x2198CBC8; // CRC-32Q string "VPieceNodeData"
const quint16 VPieceNodeData::classVersion = 1;

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode()
    : d(new VPieceNodeData)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode(quint32 id, Tool typeTool, bool reverse)
    : d(new VPieceNodeData(id, typeTool, reverse))
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode(const VPieceNode &node)
    : d (node.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode &VPieceNode::operator=(const VPieceNode &node)
{
    if ( &node == this )
    {
        return *this;
    }
    d = node.d;
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS
//---------------------------------------------------------------------------------------------------------------------
VPieceNode::VPieceNode(const VPieceNode &&node) Q_DECL_NOTHROW
    : d (node.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceNode &VPieceNode::operator=(VPieceNode &&node) Q_DECL_NOTHROW
{
    std::swap(d, node.d);
    return *this;
}
#endif

//---------------------------------------------------------------------------------------------------------------------
VPieceNode::~VPieceNode()
{}

// Friend functions
//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const VPieceNode &p)
{
    out << *p.d;
    return out;
}

//---------------------------------------------------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, VPieceNode &p)
{
    in >> *p.d;
    return in;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VPieceNode::GetId() const
{
    return d->m_id;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetId(quint32 id)
{
    d->m_id = id;
}

//---------------------------------------------------------------------------------------------------------------------
Tool VPieceNode::GetTypeTool() const
{
    return d->m_typeTool;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetTypeTool(Tool value)
{
    d->m_typeTool = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::GetReverse() const
{
    return d->m_reverse;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetReverse(bool reverse)
{
    if (d->m_typeTool != Tool::NodePoint)
    {
        d->m_reverse = reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSABefore(const VContainer *data) const
{
    if (d->m_formulaWidthBefore == currentSeamAllowance)
    {
        return -1;
    }

    VFormula formula(d->m_formulaWidthBefore, data);
    formula.setCheckZero(false);
    formula.Eval();

    if (formula.error())
    {
        QString nodeName;
        try
        {
            nodeName = data->GetGObject(d->m_id)->name();
        }
        catch (const VExceptionBadId &)
        {}

        const QString errorMsg = QObject::tr("Cannot calculate seam allowance before for point '%1'. Reason: %2.")
                .arg(nodeName, formula.Reason());
        qApp->IsPedantic() ? throw VException(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
        return -1;
    }
    return formula.getDoubleValue();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSABefore(const VContainer *data, Unit unit) const
{
    if (d->m_formulaWidthBefore == currentSeamAllowance)
    {
        return -1;
    }

    VFormula formula(d->m_formulaWidthBefore, data);
    formula.setCheckZero(false);
    formula.Eval();

    if (formula.error())
    {
        QString nodeName;
        try
        {
            nodeName = data->GetGObject(d->m_id)->name();
        }
        catch (const VExceptionBadId &)
        {}

        const QString errorMsg = QObject::tr("Cannot calculate seam allowance before for point '%1'. Reason: %2.")
                .arg(nodeName, formula.Reason());
        qApp->IsPedantic() ? throw VException(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
        return -1;
    }

    qreal value = formula.getDoubleValue();
    if (value >= 0)
    {
        value = ToPixel(value, unit);
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPieceNode::GetFormulaSABefore() const
{
    return d->m_formulaWidthBefore;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetFormulaSABefore(const QString &formula)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_formulaWidthBefore = formula;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSAAfter(const VContainer *data) const
{
    if (d->m_formulaWidthAfter == currentSeamAllowance)
    {
        return -1;
    }

    VFormula formula(d->m_formulaWidthAfter, data);
    formula.setCheckZero(false);
    formula.Eval();

    if (formula.error())
    {
        QString nodeName;
        try
        {
            nodeName = data->GetGObject(d->m_id)->name();
        }
        catch (const VExceptionBadId &)
        {}

        const QString errorMsg = QObject::tr("Cannot calculate seam allowance after for point '%1'. Reason: %2.")
                .arg(nodeName, formula.Reason());
        qApp->IsPedantic() ? throw VException(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
        return -1;
    }

    return formula.getDoubleValue();
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetSAAfter(const VContainer *data, Unit unit) const
{
    if (d->m_formulaWidthAfter == currentSeamAllowance)
    {
        return -1;
    }

    VFormula formula(d->m_formulaWidthAfter, data);
    formula.setCheckZero(false);
    formula.Eval();

    if (formula.error())
    {
        QString nodeName;
        try
        {
            nodeName = data->GetGObject(d->m_id)->name();
        }
        catch (const VExceptionBadId &)
        {}

        const QString errorMsg = QObject::tr("Cannot calculate seam allowance after for point '%1'. Reason: ")
                .arg(nodeName, formula.Reason());
        qApp->IsPedantic() ? throw VException(errorMsg) :
                             qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
        return -1;
    }

    qreal value = formula.getDoubleValue();

    if (value >= 0)
    {
        value = ToPixel(value, unit);
    }
    return value;
}

//---------------------------------------------------------------------------------------------------------------------
QString VPieceNode::GetFormulaSAAfter() const
{
    return d->m_formulaWidthAfter;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetFormulaSAAfter(const QString &formula)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_formulaWidthAfter = formula;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VPieceNode::GetFormulaPassmarkLength() const
{
    return d->m_formulaPassmarkLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetFormulaPassmarkLength(const QString &formula)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_formulaPassmarkLength = formula;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPieceNode::GetPassmarkLength(const VContainer *data, Unit unit) const
{
    if (d->m_manualPassmarkLength)
    {
        VFormula formula(d->m_formulaPassmarkLength, data);
        formula.setCheckZero(false);
        formula.setCheckLessThanZero(false);
        formula.Eval();

        if (formula.error())
        {
            QString nodeName;
            try
            {
                nodeName = data->GetGObject(d->m_id)->name();
            }
            catch (const VExceptionBadId &)
            {}

            const QString errorMsg = QObject::tr("Cannot calculate passmark length for point '%1'. Reason: %2.")
                    .arg(nodeName, formula.Reason());
            qApp->IsPedantic() ? throw VException(errorMsg) :
                                 qWarning() << VAbstractApplication::patternMessageSignature + errorMsg;
            return VSAPoint::maxPassmarkLength;
        }

        return ToPixel(formula.getDoubleValue(), unit);
    }
    return -1;
}

//---------------------------------------------------------------------------------------------------------------------
PieceNodeAngle VPieceNode::GetAngleType() const
{
    return d->m_angleType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetAngleType(PieceNodeAngle type)
{
    if (d->m_typeTool == Tool::NodePoint)
    {
        d->m_angleType = type;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsPassmark() const
{
    return d->m_isPassmark;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetPassmark(bool passmark)
{
    if (GetTypeTool() == Tool::NodePoint)
    {
        d->m_isPassmark = passmark;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsMainPathNode() const
{
    return d->m_isMainPathNode;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetMainPathNode(bool value)
{
    d->m_isMainPathNode = value;
}

//---------------------------------------------------------------------------------------------------------------------
PassmarkLineType VPieceNode::GetPassmarkLineType() const
{
    return d->m_passmarkLineType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetPassmarkLineType(PassmarkLineType lineType)
{
    d->m_passmarkLineType = lineType;
}

//---------------------------------------------------------------------------------------------------------------------
PassmarkAngleType VPieceNode::GetPassmarkAngleType() const
{
    return d->m_passmarkAngleType;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetPassmarkAngleType(PassmarkAngleType angleType)
{
    d->m_passmarkAngleType = angleType;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsShowSecondPassmark() const
{
    return d->m_isShowSecondPassmark;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetShowSecondPassmark(bool value)
{
    d->m_isShowSecondPassmark = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsCheckUniqueness() const
{
    return d->m_checkUniqueness;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetCheckUniqueness(bool value)
{
    d->m_checkUniqueness = (d->m_typeTool == Tool::NodePoint ? value : true);
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsManualPassmarkLength() const
{
    return d->m_manualPassmarkLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetManualPassmarkLength(bool value)
{
    d->m_manualPassmarkLength = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceNode::IsExcluded() const
{
    return d->m_excluded;
}

//---------------------------------------------------------------------------------------------------------------------
void VPieceNode::SetExcluded(bool exclude)
{
    d->m_excluded = exclude;
}

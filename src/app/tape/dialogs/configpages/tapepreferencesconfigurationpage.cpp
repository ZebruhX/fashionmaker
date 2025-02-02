/************************************************************************
 **
 **  @file   tapepreferencesconfigurationpage.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2017
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

#include "tapepreferencesconfigurationpage.h"
#include "ui_tapepreferencesconfigurationpage.h"
#include "../../mapplication.h"
#include "../vmisc/vtapesettings.h"
#include "../vpatterndb/variables/vmeasurement.h"
#include "../vpatterndb/pmsystems.h"

//---------------------------------------------------------------------------------------------------------------------
TapePreferencesConfigurationPage::TapePreferencesConfigurationPage(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::TapePreferencesConfigurationPage),
      m_langChanged(false),
      m_systemChanged(false),
      m_defGradationChanged(false)
{
    ui->setupUi(this);
    RetranslateUi();

    InitLanguages(ui->langCombo);
    connect(ui->langCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_langChanged = true;
    });

    //-------------------- Decimal separator setup
    ui->osOptionCheck->setChecked(qApp->TapeSettings()->GetOsSeparator());

    // Theme
     ui->darkModeCheck->setChecked(qApp->TapeSettings()->GetDarkMode());

    //---------------------- Pattern making system
    ui->systemBookValueLabel->setFixedHeight(4 * QFontMetrics(ui->systemBookValueLabel->font()).lineSpacing());
    connect(ui->systemCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]()
    {
        m_systemChanged = true;
        QString text = qApp->TrVars()->PMSystemAuthor(ui->systemCombo->currentData().toString());
        ui->systemAuthorValueLabel->setText(text);
        ui->systemAuthorValueLabel->setToolTip(text);

        text = qApp->TrVars()->PMSystemBook(ui->systemCombo->currentData().toString());
        ui->systemBookValueLabel->setPlainText(text);
    });

    // set default pattern making system
    int index = ui->systemCombo->findData(qApp->TapeSettings()->GetPMSystemCode());
    if (index != -1)
    {
        ui->systemCombo->setCurrentIndex(index);
    }

    //----------------------------- Measurements Editing
    connect(ui->resetWarningsButton, &QPushButton::released, this, []()
    {
        qApp->TapeSettings()->SetConfirmFormatRewriting(true);
    });

    //----------------------- Toolbar
    ui->toolBarStyleCheck->setChecked(qApp->TapeSettings()->GetToolBarStyle());

    //---------------------------Default height and size
    // If change units don't forget about the label
    ui->defHeightCombo->addItems(VMeasurement::WholeListHeights(Unit::Cm));
    index = ui->defHeightCombo->findText(QString().setNum(qApp->TapeSettings()->GetDefHeight()));
    if (index != -1)
    {
        ui->defHeightCombo->setCurrentIndex(index);
    }

    auto DefGradationChanged = [this]()
    {
        m_defGradationChanged = true;
    };

    connect(ui->defHeightCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, DefGradationChanged);

    // If change units don't forget about the label
    ui->defSizeCombo->addItems(VMeasurement::WholeListSizes(Unit::Cm));
    index = ui->defSizeCombo->findText(QString().setNum(qApp->TapeSettings()->GetDefSize()));
    if (index != -1)
    {
        ui->defSizeCombo->setCurrentIndex(index);
    }
    connect(ui->defSizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, DefGradationChanged);
}

//---------------------------------------------------------------------------------------------------------------------
TapePreferencesConfigurationPage::~TapePreferencesConfigurationPage()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList TapePreferencesConfigurationPage::Apply()
{
    QStringList preferences;
    VTapeSettings *settings = qApp->TapeSettings();
    settings->SetOsSeparator(ui->osOptionCheck->isChecked());

    settings->SetToolBarStyle(ui->toolBarStyleCheck->isChecked());

    if (settings->GetDarkMode() != ui->darkModeCheck->isChecked())
    {
        settings->SetDarkMode(ui->darkModeCheck->isChecked());
        preferences.append(tr("dark mode"));
    }

    if (m_langChanged || m_systemChanged)
    {
        const QString locale = qvariant_cast<QString>(ui->langCombo->currentData());
        settings->SetLocale(locale);
        m_langChanged = false;

        const QString code = qvariant_cast<QString>(ui->systemCombo->currentData());
        settings->SetPMSystemCode(code);
        m_systemChanged = false;

        qApp->LoadTranslation(locale);
        qApp->processEvents();// force to call changeEvent

        // Part about measurments will not be updated automatically
        qApp->RetranslateTables();
        qApp->RetranslateGroups();
    }

    if (m_defGradationChanged)
    {
        settings->SetDefHeight(ui->defHeightCombo->currentText().toInt());
        settings->SetDefSize(ui->defSizeCombo->currentText().toInt());
        m_defGradationChanged = false;
    }

    return preferences;
}

//---------------------------------------------------------------------------------------------------------------------
void TapePreferencesConfigurationPage::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        // retranslate designer form (single inheritance approach)
        RetranslateUi();
        ui->retranslateUi(this);
    }
    // remember to call base class implementation
    QWidget::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void TapePreferencesConfigurationPage::RetranslateUi()
{
    ui->osOptionCheck->setText(tr("With OS options") + QStringLiteral(" (%1)").arg(QLocale().decimalPoint()));

    {
    const QString code = qvariant_cast<QString>(ui->systemCombo->currentData());
    ui->systemCombo->blockSignals(true);
    ui->systemCombo->clear();
    InitPMSystems(ui->systemCombo);
    ui->systemCombo->setCurrentIndex(-1);
    ui->systemCombo->blockSignals(false);
    ui->systemCombo->setCurrentIndex(ui->systemCombo->findData(code));
    }

    ui->labelHeightUnit->setText(UnitsToStr(Unit::Cm, true));
    ui->labelSizeUnit->setText(UnitsToStr(Unit::Cm, true));
}

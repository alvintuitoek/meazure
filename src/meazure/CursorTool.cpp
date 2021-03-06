/*
 * Copyright 2001, 2004, 2011 C Thing Software
 *
 * This file is part of Meazure.
 * 
 * Meazure is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Meazure is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Meazure.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "MeaAssert.h"
#include "CursorTool.h"
#include "Hooks/Hooks.h"
#include "Resource.h"
#include "ToolMgr.h"
#include "Colors.h"
#include <crtdbg.h>
#include <windowsx.h>


const CString MeaCursorTool::kToolName(_T("CursorTool"));


MeaCursorTool::MeaCursorTool(MeaToolMgr* mgr) :
    MeaRadioTool(mgr), m_cursorPos(0, 0), m_xDist(0), m_yDist(0)
{
}


MeaCursorTool::~MeaCursorTool()
{
    try {
        Disable();
    }
    catch(...) {
        MeaAssert(false);
    }
}


void MeaCursorTool::Create()
{
    // The data window is positioned at the cursor position
    // offset by half the size of the cursor.
    //
    m_xDist = GetSystemMetrics(SM_CXCURSOR) / 2;
    m_yDist = GetSystemMetrics(SM_CYCURSOR) / 2;

    // Create the data window.
    //
    m_dataWin.Create(MeaColors::GetA(MeaColors::CrossHairOpacity), IDS_MEA_X1, IDS_MEA_Y1);
}


void MeaCursorTool::Enable()
{
    if (IsEnabled()) {
        return;
    }

    MeaTool::Enable();

    // Tell the tool manager which data display fields
    // we will be using.
    //
    m_mgr->EnableRegionFields(MeaX1Field | MeaY1Field, 0);

    if (!IsWindow(m_dataWin)) {
        Create();
    }

    // Show one liner on how to use the tool.
    //
    m_mgr->SetStatus(IDS_MEA_CURSOR_STATUS);

    POINT point;            // Ensure pos initialized
    GetCursorPos(&point);
    m_cursorPos = point;

    // Turn on mouse pointer monitoring.
    //
    MeaEnableMouseHook();

    m_dataWin.Show();
}


void MeaCursorTool::Disable()
{
    if (!IsEnabled()) {
        return;
    }

    m_dataWin.Hide();

#ifdef _DEBUG
    bool ret =
#endif
    MeaDisableMouseHook();
    MeaAssert(ret);

    MeaTool::Disable();
}


void MeaCursorTool::Update(MeaUpdateReason reason)
{
    if (IsEnabled()) {
        MeaTool::Update(reason);

        // Convert the pixel position of the pointer
        // to the current units.
        //
        FPOINT cursorPos = MeaUnitsMgr::Instance().ConvertCoord(m_cursorPos);

        // Display the measurement.
        //
        m_mgr->ShowXY1(m_cursorPos, cursorPos);

        // The screen information depends on the pointer position.
        //
        m_mgr->UpdateScreenInfo(m_cursorPos);

        // If the units or origin changes, force an update of
        // the data window information.
        //
        if ((reason == UnitsChanged) || (reason == OriginChanged)) {
            m_dataWin.Hide();
            m_dataWin.Show();
        }
        if (reason == DataWinArming) {
            if (MeaDataWin::IsArmed()) {
                m_dataWin.Show();
            } else {
                m_dataWin.Hide();
            }
        }

        // The data window is displayed at the pointer position
        // offset by half the cursor size.
        //
        CRect rect(m_cursorPos, m_cursorPos);
        rect.InflateRect(m_xDist, m_yDist);
        m_dataWin.ShowXY(cursorPos);
        m_dataWin.Update(rect);
    }
}


void MeaCursorTool::Strobe()
{
    m_dataWin.Strobe();
}


void MeaCursorTool::ColorsChanged()
{
    m_dataWin.SetOpacity(MeaColors::GetA(MeaColors::CrossHairOpacity));
}


bool MeaCursorTool::HasCrosshairs() const
{
    return false;
}


const POINT& MeaCursorTool::GetPosition() const
{
    return m_cursorPos;
}


void MeaCursorTool::GetPosition(MeaPositionLogMgr::Position& position) const
{
    position.RecordXY1(MeaUnitsMgr::Instance().ConvertCoord(m_cursorPos));
}


CString MeaCursorTool::GetToolName() const
{
    return kToolName;
}


UINT MeaCursorTool::GetLabelId() const
{
    return IDS_MEA_CURSOR;
}


void MeaCursorTool::OnMouseHook(WPARAM /* wParam */, LPARAM lParam)
{
    m_cursorPos.x = GET_X_LPARAM(lParam);
    m_cursorPos.y = GET_Y_LPARAM(lParam);

    Update(NormalUpdate);
}

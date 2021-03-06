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

/// @file
/// @brief Header file for window layout utilities.

#pragma once

#include <math.h>
#include "Units.h"

/// Returns the absolute value of the the argument.
#define MEA_ABS(a)      (((a)<0) ? -(a) : (a))

/// Returns 1 if a >= 0, -1 if a < 0.
#define MEA_SGN(a)      (((a)<0) ? -1 : 1)


/// Utility methods for performing various window layout tasks.
///
class MeaLayout {
public:
    /// Aligns the specified windows such that their left edges all begin
    /// at the specified x location.
    ///
    /// @param leftX        [in] Align each window's left edge to this x position, in pixels.
    /// @param ...          [in] CWnd pointers terminated by NULL.
    ///
    static void AlignLeft(int leftX, ...);

    /// Aligns the specified windows such that their right edges are all
    /// aligned. The widest window is positioned at the specified x location.
    ///
    /// @param leftX        [in] Widest window starts at this x position, in pixels.
    /// @param ...          [in] CWnd pointers terminated by NULL.
    ///
    static void AlignRight(int leftX, ...);

    /// Aligns the specified windows such that they all share a common vertical
    /// center. The top of the tallest window begins at the specified y position.
    ///
    /// @param topY         [in] Tallest window begins at this y position, in pixels.
    /// @param ...          [in] CWnd pointers terminated by NULL.
    ///
    static void AlignCenter(int topY, ...);

    /// Performs a right alignment of the specified windows relative to the right
    /// edge of the specified base window.
    ///
    /// @param baseWnd      [in] Align windows relative to this base window.
    /// @param ...          [in] CWnd pointers terminated by NULL.
    ///
    static void AlignRightTo(const CWnd *baseWnd, ...);

    /// Places the back window after the specified front window and separates
    /// them by the specified spacing.
    ///
    /// @param frontWnd     [in] Window in front of backWnd.
    /// @param backWnd      [in] Window to place after frontWnd.
    /// @param spacing      [in] Separation between the two windows, in pixels.
    ///
    static void PlaceAfter(const CWnd& frontWnd, const CWnd& backWnd, int spacing);


    /// Sets the position of the specified window.
    ///
    /// @param wnd      [in] Window to position.
    /// @param x        [in] X position for the window, in pixels.
    /// @param y        [in] Y position for the window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool SetWindowPos(const CWnd& wnd, int x, int y) {
        return ::SetWindowPos(wnd.m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? true : false;
    }

    /// Sets the size of the specified window.
    ///
    /// @param wnd      [in] Window to size.
    /// @param cx       [in] Width for the window, in pixels.
    /// @param cy       [in] Height for the window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool SetWindowSize(const CWnd& wnd, int cx, int cy) {
        return ::SetWindowPos(wnd.m_hWnd, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER) ? true : false;
    }

    /// Returns the height of the specified window.
    ///
    /// @param wnd      [in] Window whose height is desired.
    ///
    /// @return Height of the window in pixels.
    ///
    static int GetWindowHeight(const CWnd& wnd) {
        CRect rect;
        wnd.GetWindowRect(rect);
        return rect.Height();
    }

    /// Sets the height of the window.
    ///
    /// @param wnd      [in] Window whose height is to be set.
    /// @param cy       [in] Height for the window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool SetWindowHeight(const CWnd& wnd, int cy) {
        CRect rect;
        wnd.GetWindowRect(rect);
        return ::SetWindowPos(wnd.m_hWnd, NULL, 0, 0, rect.Width(), cy, SWP_NOMOVE | SWP_NOZORDER) ? true : false;
    }

    /// Moves the specified window vertically.
    ///
    /// @param wnd      [in] Window to be moved.
    /// @param cy       [in] Vertical distance to move window, in pixels.
    ///
    /// @return <b>true</b> if the method succeeds.
    ///
    static bool MoveWindowHeight(const CWnd& wnd, int cy) {
        CRect rect;
        wnd.GetWindowRect(rect);
        CWnd *parentWnd = wnd.GetParent();
        parentWnd->ScreenToClient(rect);
        return ::SetWindowPos(wnd.m_hWnd, NULL, rect.left, rect.top + cy, 0, 0, SWP_NOSIZE | SWP_NOZORDER) ? true : false;
    }

    /// Calculates the bounding dimensions for all the children of
    /// the specified window.
    ///
    /// @param sizep        [out] Bounding dimensions encompassing all child windows.
    /// @param parentWnd    [in] All child windows of this window form the bounding dimensions.
    ///
    static void GetBoundingSize(SIZE *sizep, const CWnd* parentWnd);

    /// Calculates the length corresponding to the specified x and y distances
    /// using the formula:
    /// \f[
    ///     length=\sqrt{dx^2+dy^2}
    /// \f]
    /// @param dx   [in] X distance.
    /// @param dy   [in] Y distance.
    ///
    /// @return Length corresponding to the vertical and horizontal distances.
    ///
    static double CalcLength(double dx, double dy) {
        return sqrt(dx * dx + dy * dy);
    }

    /// Calculates the length between the two specified points using the
    /// formula:
    /// \f[
    ///     length=\sqrt{(p2_x-p1_x)^2+(p2_y-p1_y)^2}
    /// \f]
    /// @param p1   [in] Point 1.
    /// @param p2   [in] Point 2.
    ///
    /// @return Length between point 1 and point 2.
    ///
    static double CalcLength(const POINT& p1, const POINT& p2) {
        return CalcLength(static_cast<double>(p2.x - p1.x), static_cast<double>(p2.y - p1.y));
    }

    /// Calculates the circumference of the circle with the specified radius using
    /// the formula:
    /// \f[
    ///     circumference=2*\pi*radius
    /// \f]
    /// @param radius   [in] Radius of the circle.
    ///
    /// @return Circumference of the circle.
    ///
    static double CalcCircumference(double radius) {
        return 2.0 * radius * MeaUnits::kPI;
    }

    /// Returns the angular sector containing the line specified by
    /// the two points.
    ///
    /// @param start        [in] Starting point for the line.
    /// @param end          [in] Ending point for the line.
    ///
    /// @return The circular sector corresponding to the line. The
    ///         value is an integer between -4 and 4
    ///
    static int  GetSector(const POINT& start, const POINT& end) {
        int deltax = end.x - start.x;
        int deltay = end.y - start.y;

        if (deltax == 0 && deltay == 0) {
            return 0;
        }
        return static_cast<int>(atan2(static_cast<double>(deltay), static_cast<double>(deltax)) / 0.785398163);
    }

    /// Returns the angle of the line from the specified start point
    /// to the specified end point relative to the horizontal (x-axis),
    /// measured in radians. Using the default coordinate system (i.e.
    /// positive y-axis pointed down from the origin), positive angles
    /// are measured clockwise from the horizontal to the line from
    /// start point to end point. The following formula is used:
    /// \f[
    ///     angle=atan2(end_y-start_y, end_x-start_x)
    /// \f]
    /// @param start    [in] Starting point for the line whose angle is
    ///                 to be determined.
    /// @param end      [in] Ending point for the line whose angle is to
    ///                 be determined.
    ///
    /// @return Angle between the x-axis and the line, in radians.
    ///         Degenerate cases return 0.0.
    ///
    static double   GetAngle(const FPOINT& start, const FPOINT& end) {
        double deltax = end.x - start.x;
        double deltay = end.y - start.y;

        if (fabs(deltax) < 0.0000001 && fabs(deltay) < 0.0000001) {
            return 0.0;
        }
        return atan2(deltay, deltax);
    }


    /// Returns the angle, in radians, between the line from the vertex
    /// to point p1 and the line from the vertex to point p2. Using the
    /// default coordinate system (i.e. positive y-axis pointed down from
    /// the origin), positive angles are measured clockwise from the
    /// vertex-p1 line to the vertex-p2 line.
    ///
    /// @param vertex   [in] Intersection point for the two lines.
    /// @param p1       [in] Forms the end point for the first line from the
    ///                 vertex.
    /// @param p2       [in] Forms the end point for the second line from the
    ///                 vertex.
    ///
    /// @return Angle between the the lines vertex-p1 and vertex-p2.
    ///
    static double   GetAngle(const FPOINT& vertex, const FPOINT& p1, const FPOINT& p2) {
        double deltax1 = p1.x - vertex.x;
        double deltax2 = p2.x - vertex.x;
        double deltay1 = p1.y - vertex.y;
        double deltay2 = p2.y - vertex.y;

        double numer = deltay2 * deltax1 - deltay1 * deltax2;
        double denom = deltax2 * deltax1 + deltay1 * deltay2;

        if (fabs(numer) < 0.0000001 && fabs(denom) < 0.0000001) {
            return 0.0;
        }
        return atan2(numer, denom);
    }


    /// Converts the x position in screen coordinates to
    /// an x position in the specified window's coordinate
    /// system.
    ///
    /// @param wnd      [in] The x coordinate is converts to this
    ///                 window's coordinate system.
    /// @param x        [in, out] The x coordinate to convert.
    ///
    /// @return <b>true</b> if the conversion was successful.
    ///
    static bool ScreenToClientX(const CWnd& wnd, int& x) {
        CPoint pt(x, 0);
        bool ret = ScreenToClient(wnd.m_hWnd, &pt) == TRUE;
        x = pt.x;
        return ret;
    }

    /// Converts the y position in screen coordinates to
    /// a y position in the specified window's coordinate
    /// system.
    ///
    /// @param wnd      [in] The y coordinate is converts to this
    ///                 window's coordinate system.
    /// @param y        [in, out] The y coordinate to convert.
    ///
    /// @return <b>true</b> if the conversion was successful.
    ///
    static bool ScreenToClientY(const CWnd& wnd, int& y) {
        CPoint pt(0, y);
        bool ret = ScreenToClient(wnd.m_hWnd, &pt) == TRUE;
        y = pt.y;
        return ret;
    }


    /// Performs and alpha blend on the two specified device contexts.
    /// The resulting blend is stored in the destination device context.
    /// The device contexts must be the same size.
    ///
    /// @param dstDC    [in] Destination device context.
    /// @param srcDC    [in] Source device context.
    /// @param width    [in] Width of the device contexts, in pixels.
    /// @param height   [in] Height of the device contexts, in pixels.
    /// @param alpha    [in] Opacity value between 0 (transparent) and 255 (opaque).
    ///
    static void AlphaBlend(CDC& dstDC, const CDC& srcDC, int width, int height, BYTE alpha);

    /// Draws an image to use as a background for use in setting
    /// the a control's opacity.
    ///
    /// @param wnd      [in] Window representing the control.
    /// @param dc       [in] Device context into which to draw the background.
    ///
    static void DrawOpacityBackground(const CWnd& wnd, CDC& dc);
};

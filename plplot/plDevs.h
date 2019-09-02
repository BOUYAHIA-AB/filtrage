// -*-C-*-
//
//  Maurice LeBrun
//  IFS, University of Texas at Austin
//  18-Jul-1994
//
//  Contains macro definitions that determine what device drivers are
//  compiled into the PLplot library.  On a Unix system, typically the
//  configure script builds plDevs.h from plDevs.h.in.  Elsewhere, it's
//  best to hand-configure a plDevs.h file and keep it with the
//  system-specific files.
//
//  Copyright (C) 2004  Andrew Roach
//  Copyright (C) 2005  Thomas J. Duck
//  Copyright (C) 2006  Andrew Ross
//  Copyright (C) 2006  Alan W. Irwin
//
//  This file is part of PLplot.
//
//  PLplot is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Library General Public License as published
//  by the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  PLplot is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public License
//  along with PLplot; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//

#ifndef __PLDEVS_H__
#define __PLDEVS_H__

// Same order (by source filename and by device) as in drivers.h.
// ps and psttf are special cases where two devices are handled with
// one PLD macro.
// xwin is special case where the macro name is PLD_xwin but the
// function name in drivers.h is plD_dispatch_init_xw

/* #undef PLD_aqt */
#define PLD_epscairo
#define PLD_extcairo
#define PLD_memcairo
#define PLD_pdfcairo
#define PLD_pngcairo
#define PLD_pscairo
#define PLD_svgcairo
/* #undef PLD_wincairo */
#define PLD_xcairo
/* #undef PLD_cgm */
/* #undef PLD_gif */
/* #undef PLD_jpeg */
/* #undef PLD_png */
#define PLD_mem
/* #undef PLD_ntk */
#define PLD_null
/* #undef PLD_pdf */
/* #undef PLD_plm */
#define PLD_ps
/* #undef PLD_pstex */
/* #undef PLD_psttf */
#define PLD_bmpqt
#define PLD_epsqt
#define PLD_extqt
#define PLD_jpgqt
#define PLD_memqt
#define PLD_pdfqt
#define PLD_pngqt
#define PLD_ppmqt
#define PLD_qtwidget
#define PLD_svgqt
#define PLD_tiffqt
#define PLD_svg
/* #undef PLD_tk */
/* #undef PLD_tkwin */
/* #undef PLD_wingcc */
/* #undef PLD_wxpng */
/* #undef PLD_wxwidgets */
#define PLD_xfig
#define PLD_xwin

#endif  // __PLDEVS_H__

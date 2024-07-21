/*
 * Copyright 2023 Nikolay Sivov for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>
#include <stdlib.h>

#include "windef.h"
#include "winbase.h"

#include "wldp.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(wldp);

/***********************************************************************
 *      WldpIsDynamicCodePolicyEnabled (wldp.@)
 */
HRESULT WINAPI WldpIsDynamicCodePolicyEnabled(BOOL *is_enabled)
{
    FIXME("%p\n", is_enabled);

    *is_enabled = FALSE;
    return S_OK;
}

/***********************************************************************
 *      WldpGetLockdownPolicy (wldp.@)
 */
HRESULT WINAPI WldpGetLockdownPolicy(WLDP_HOST_INFORMATION *info, DWORD *state, DWORD flags)
{
    FIXME("%p %p %lu\n", info, state, flags);

    *state = 0;
    return S_OK;
}

/***********************************************************************
 *      WldpQueryWindowsLockdownMode (wldp.@)
 */
HRESULT WINAPI WldpQueryWindowsLockdownMode(WLDP_WINDOWS_LOCKDOWN_MODE *lockdown_mode)
{
    FIXME("%p\n", lockdown_mode);

    *lockdown_mode = WLDP_WINDOWS_LOCKDOWN_MODE_UNLOCKED;
    return S_OK;
}
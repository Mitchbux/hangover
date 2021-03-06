/*
 * Copyright 2017 André Hentschel
 * Copyright 2018 Stefan Dösinger for CodeWeavers
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

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#define COBJMACROS

#include <windows.h>
#include <stdio.h>

#include "thunk/qemu_windows.h"

#include "windows-user-services.h"
#include "dll_list.h"

#ifdef QEMU_DLL_GUEST

#include <d3d11.h>
#include <debug.h>

#include <initguid.h>

#else

#include <d3d11_2.h>
#include <wine/debug.h>

#endif

#include "thunk/qemu_d3d11.h"

#include "qemudxgi.h"
#include "qemu_d3d11.h"

WINE_DEFAULT_DEBUG_CHANNEL(qemu_d3d11);

struct qemu_d3d11_blend_state_QueryInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t riid;
    uint64_t object;
};

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_state *impl_from_ID3D11BlendState(ID3D11BlendState *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D11BlendState_iface);
}

static inline struct qemu_d3d11_state *impl_from_ID3D10BlendState1(ID3D10BlendState1 *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D10BlendState1_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_blend_state_QueryInterface(ID3D11BlendState *iface, REFIID riid, void **object)
{
    struct qemu_d3d11_blend_state_QueryInterface call;
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    if (IsEqualGUID(riid, &IID_ID3D11BlendState)
            || IsEqualGUID(riid, &IID_ID3D11DeviceChild)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        ID3D11BlendState_AddRef(iface);
        *object = iface;
        return S_OK;
    }

    if (IsEqualGUID(riid, &IID_ID3D10BlendState1)
            || IsEqualGUID(riid, &IID_ID3D10BlendState)
            || IsEqualGUID(riid, &IID_ID3D10DeviceChild))
    {
        ID3D10BlendState1_AddRef(&state->ID3D10BlendState1_iface);
        *object = &state->ID3D10BlendState1_iface;
        return S_OK;
    }

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_BLEND_STATE_QUERYINTERFACE);
    call.iface = (ULONG_PTR)state;
    call.riid = (ULONG_PTR)riid;

    qemu_syscall(&call.super);

    *object = NULL;
    return E_NOINTERFACE;
}

#else

void qemu_d3d11_blend_state_QueryInterface(struct qemu_syscall *call)
{
    struct qemu_d3d11_blend_state_QueryInterface *c = (struct qemu_d3d11_blend_state_QueryInterface *)call;
    struct qemu_d3d11_state *state;
    IUnknown *obj;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11BlendState_QueryInterface(state->host_bs11, QEMU_G2H(c->riid), (void **)obj);
    if (SUCCEEDED(c->super.iret))
    {
        WINE_FIXME("Host returned an interface for %s which this wrapper does not know about.\n",
                wine_dbgstr_guid(QEMU_G2H(c->riid)));
        IUnknown_Release(obj);
    }
}

#endif

struct qemu_d3d11_blend_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_blend_state_AddRef(ID3D11BlendState *iface)
{
    struct qemu_d3d11_blend_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_BLEND_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_blend_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d11_blend_state_AddRef *c = (struct qemu_d3d11_blend_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11BlendState_AddRef(state->host_bs11);
}

#endif

struct qemu_d3d11_blend_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_blend_state_Release(ID3D11BlendState *iface)
{
    struct qemu_d3d11_blend_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_BLEND_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_blend_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d11_blend_state_Release *c = (struct qemu_d3d11_blend_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11BlendState_Release(state->host_bs11);
}

#endif

struct qemu_d3d11_blend_state_GetDevice
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t device;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_blend_state_GetDevice(ID3D11BlendState *iface, ID3D11Device **device)
{
    struct qemu_d3d11_blend_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_BLEND_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D11Device *)&dev_impl->ID3D11Device2_iface;
}

static void STDMETHODCALLTYPE d3d10_blend_state_GetDevice(ID3D10BlendState1 *iface, ID3D10Device **device)
{
    struct qemu_d3d11_blend_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_BLEND_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D10Device *)&dev_impl->ID3D10Device1_iface;
}

#else

void qemu_d3d11_blend_state_GetDevice(struct qemu_syscall *call)
{
    struct qemu_d3d11_blend_state_GetDevice *c = (struct qemu_d3d11_blend_state_GetDevice *)call;
    struct qemu_d3d11_state *state;
    ID3D11Device2 *host;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11BlendState_GetDevice(state->host_bs11, (ID3D11Device **)&host);
    c->device = QEMU_H2G(device_from_host(host));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_blend_state_GetPrivateData(ID3D11BlendState *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_blend_state_SetPrivateData(ID3D11BlendState *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_blend_state_SetPrivateDataInterface(ID3D11BlendState *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d11_blend_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_blend_state_GetDesc(ID3D11BlendState *iface, D3D11_BLEND_DESC *desc)
{
    struct qemu_d3d11_blend_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D11BlendState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_BLEND_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_blend_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d11_blend_state_GetDesc *c = (struct qemu_d3d11_blend_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11BlendState_GetDesc(state->host_bs11, QEMU_G2H(c->desc));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_blend_state_QueryInterface(ID3D10BlendState1 *iface, REFIID riid, void **object)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    return d3d11_blend_state_QueryInterface(&state->ID3D11BlendState_iface, riid, object);
}

#endif

struct qemu_d3d10_blend_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_blend_state_AddRef(ID3D10BlendState1 *iface)
{
    struct qemu_d3d10_blend_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_BLEND_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_blend_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d10_blend_state_AddRef *c = (struct qemu_d3d10_blend_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10BlendState1_AddRef(state->host_bs10);
}

#endif

struct qemu_d3d10_blend_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_blend_state_Release(ID3D10BlendState1 *iface)
{
    struct qemu_d3d10_blend_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_BLEND_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_blend_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d10_blend_state_Release *c = (struct qemu_d3d10_blend_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10BlendState1_Release(state->host_bs10);
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_blend_state_GetPrivateData(ID3D10BlendState1 *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_blend_state_SetPrivateData(ID3D10BlendState1 *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_blend_state_SetPrivateDataInterface(ID3D10BlendState1 *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d10_blend_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_blend_state_GetDesc(ID3D10BlendState1 *iface, D3D10_BLEND_DESC *desc)
{
    struct qemu_d3d10_blend_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_BLEND_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_blend_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d10_blend_state_GetDesc *c = (struct qemu_d3d10_blend_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D10BlendState1_GetDesc(state->host_bs10, QEMU_G2H(c->desc));
}

#endif

struct qemu_d3d10_blend_state_GetDesc1
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_blend_state_GetDesc1(ID3D10BlendState1 *iface, D3D10_BLEND_DESC1 *desc)
{
    struct qemu_d3d10_blend_state_GetDesc1 call;
    struct qemu_d3d11_state *state = impl_from_ID3D10BlendState1(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_BLEND_STATE_GETDESC1);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_blend_state_GetDesc1(struct qemu_syscall *call)
{
    struct qemu_d3d10_blend_state_GetDesc1 *c = (struct qemu_d3d10_blend_state_GetDesc1 *)call;
    struct qemu_d3d11_state *state;

    WINE_FIXME("Unverified!\n");
    state = QEMU_G2H(c->iface);

    ID3D10BlendState1_GetDesc1(state->host_bs10, QEMU_G2H(c->desc));
}

#endif

struct qemu_d3d11_depthstencil_state_QueryInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t riid;
    uint64_t object;
};

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_state *impl_from_ID3D11DepthStencilState(ID3D11DepthStencilState *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D11DepthStencilState_iface);
}

static inline struct qemu_d3d11_state *impl_from_ID3D10DepthStencilState(ID3D10DepthStencilState *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D10DepthStencilState_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_depthstencil_state_QueryInterface(ID3D11DepthStencilState *iface, REFIID riid, void **object)
{
    struct qemu_d3d11_depthstencil_state_QueryInterface call;
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    if (IsEqualGUID(riid, &IID_ID3D11DepthStencilState)
            || IsEqualGUID(riid, &IID_ID3D11DeviceChild)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        ID3D11DepthStencilState_AddRef(iface);
        *object = iface;
        return S_OK;
    }

    if (IsEqualGUID(riid, &IID_ID3D10DepthStencilState)
            || IsEqualGUID(riid, &IID_ID3D10DeviceChild))
    {
        ID3D10DepthStencilState_AddRef(&state->ID3D10DepthStencilState_iface);
        *object = &state->ID3D10DepthStencilState_iface;
        return S_OK;
    }

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEPTHSTENCIL_STATE_QUERYINTERFACE);
    call.iface = (ULONG_PTR)state;
    call.riid = (ULONG_PTR)riid;

    qemu_syscall(&call.super);

    *object = NULL;
    return E_NOINTERFACE;
}

#else

void qemu_d3d11_depthstencil_state_QueryInterface(struct qemu_syscall *call)
{
    struct qemu_d3d11_depthstencil_state_QueryInterface *c = (struct qemu_d3d11_depthstencil_state_QueryInterface *)call;
    struct qemu_d3d11_state *state;
    IUnknown *obj;

    WINE_FIXME("Unverified!\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DepthStencilState_QueryInterface(state->host_ds11, QEMU_G2H(c->riid), (void **)&obj);
    if (SUCCEEDED(c->super.iret))
    {
        WINE_FIXME("Host returned an interface for %s which this wrapper does not know about.\n",
                wine_dbgstr_guid(QEMU_G2H(c->riid)));
        IUnknown_Release(obj);
    }
}

#endif

struct qemu_d3d11_depthstencil_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_depthstencil_state_AddRef(ID3D11DepthStencilState *iface)
{
    struct qemu_d3d11_depthstencil_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEPTHSTENCIL_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_depthstencil_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d11_depthstencil_state_AddRef *c = (struct qemu_d3d11_depthstencil_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DepthStencilState_AddRef(state->host_ds11);
}

#endif

struct qemu_d3d11_depthstencil_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_depthstencil_state_Release(ID3D11DepthStencilState *iface)
{
    struct qemu_d3d11_depthstencil_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEPTHSTENCIL_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_depthstencil_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d11_depthstencil_state_Release *c = (struct qemu_d3d11_depthstencil_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11DepthStencilState_Release(state->host_ds11);
}

#endif

struct qemu_d3d11_depthstencil_state_GetDevice
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t device;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_depthstencil_state_GetDevice(ID3D11DepthStencilState *iface, ID3D11Device **device)
{
    struct qemu_d3d11_depthstencil_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEPTHSTENCIL_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;
    call.device = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D11Device *)&dev_impl->ID3D11Device2_iface;
}

static void STDMETHODCALLTYPE d3d10_depthstencil_state_GetDevice(ID3D10DepthStencilState *iface, ID3D10Device **device)
{
    struct qemu_d3d11_depthstencil_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEPTHSTENCIL_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;
    call.device = (ULONG_PTR)device;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D10Device *)&dev_impl->ID3D10Device1_iface;
}

#else

void qemu_d3d11_depthstencil_state_GetDevice(struct qemu_syscall *call)
{
    struct qemu_d3d11_depthstencil_state_GetDevice *c = (struct qemu_d3d11_depthstencil_state_GetDevice *)call;
    struct qemu_d3d11_state *state;
    ID3D11Device2 *host;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11DepthStencilState_GetDevice(state->host_ds11, (ID3D11Device **)&host);
    c->device = QEMU_H2G(device_from_host(host));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_depthstencil_state_GetPrivateData(ID3D11DepthStencilState *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_depthstencil_state_SetPrivateData(ID3D11DepthStencilState *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_depthstencil_state_SetPrivateDataInterface(ID3D11DepthStencilState *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d11_depthstencil_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_depthstencil_state_GetDesc(ID3D11DepthStencilState *iface, D3D11_DEPTH_STENCIL_DESC *desc)
{
    struct qemu_d3d11_depthstencil_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D11DepthStencilState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_DEPTHSTENCIL_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_depthstencil_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d11_depthstencil_state_GetDesc *c = (struct qemu_d3d11_depthstencil_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11DepthStencilState_GetDesc(state->host_ds11, QEMU_G2H(c->desc));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_depthstencil_state_QueryInterface(ID3D10DepthStencilState *iface,
        REFIID riid, void **object)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    return d3d11_depthstencil_state_QueryInterface(&state->ID3D11DepthStencilState_iface, riid, object);
}

#endif

struct qemu_d3d10_depthstencil_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_depthstencil_state_AddRef(ID3D10DepthStencilState *iface)
{
    struct qemu_d3d10_depthstencil_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEPTHSTENCIL_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_depthstencil_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d10_depthstencil_state_AddRef *c = (struct qemu_d3d10_depthstencil_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10DepthStencilState_AddRef(state->host_ds10);
}

#endif

struct qemu_d3d10_depthstencil_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_depthstencil_state_Release(ID3D10DepthStencilState *iface)
{
    struct qemu_d3d10_depthstencil_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEPTHSTENCIL_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_depthstencil_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d10_depthstencil_state_Release *c = (struct qemu_d3d10_depthstencil_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10DepthStencilState_Release(state->host_ds10);
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_depthstencil_state_GetPrivateData(ID3D10DepthStencilState *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_depthstencil_state_SetPrivateData(ID3D10DepthStencilState *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_depthstencil_state_SetPrivateDataInterface(ID3D10DepthStencilState *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d10_depthstencil_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_depthstencil_state_GetDesc(ID3D10DepthStencilState *iface, D3D10_DEPTH_STENCIL_DESC *desc)
{
    struct qemu_d3d10_depthstencil_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D10DepthStencilState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_DEPTHSTENCIL_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_depthstencil_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d10_depthstencil_state_GetDesc *c = (struct qemu_d3d10_depthstencil_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D10DepthStencilState_GetDesc(state->host_ds10, QEMU_G2H(c->desc));
}

#endif

struct qemu_d3d11_rasterizer_state_QueryInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t riid;
    uint64_t object;
};

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_state *impl_from_ID3D11RasterizerState(ID3D11RasterizerState *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D11RasterizerState_iface);
}

static inline struct qemu_d3d11_state *impl_from_ID3D10RasterizerState(ID3D10RasterizerState *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D10RasterizerState_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_rasterizer_state_QueryInterface(ID3D11RasterizerState *iface, REFIID riid, void **object)
{
    struct qemu_d3d11_rasterizer_state_QueryInterface call;
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    if (IsEqualGUID(riid, &IID_ID3D11RasterizerState)
            || IsEqualGUID(riid, &IID_ID3D11DeviceChild)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        ID3D11RasterizerState_AddRef(iface);
        *object = iface;
        return S_OK;
    }

    if (IsEqualGUID(riid, &IID_ID3D10RasterizerState)
            || IsEqualGUID(riid, &IID_ID3D10DeviceChild))
    {
        ID3D10RasterizerState_AddRef(&state->ID3D10RasterizerState_iface);
        *object = &state->ID3D10RasterizerState_iface;
        return S_OK;
    }

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_RASTERIZER_STATE_QUERYINTERFACE);
    call.iface = (ULONG_PTR)state;
    call.riid = (ULONG_PTR)riid;

    qemu_syscall(&call.super);

    *object = NULL;
    return E_NOINTERFACE;
}

#else

void qemu_d3d11_rasterizer_state_QueryInterface(struct qemu_syscall *call)
{
    struct qemu_d3d11_rasterizer_state_QueryInterface *c = (struct qemu_d3d11_rasterizer_state_QueryInterface *)call;
    struct qemu_d3d11_state *state;
    IUnknown *obj;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11RasterizerState_QueryInterface(state->host_rs11, QEMU_G2H(c->riid), (void **)&obj);
    if (SUCCEEDED(c->super.iret))
    {
        WINE_FIXME("Host returned an interface for %s which this wrapper does not know about.\n",
                wine_dbgstr_guid(QEMU_G2H(c->riid)));
        IUnknown_Release(obj);
    }
}

#endif

struct qemu_d3d11_rasterizer_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_rasterizer_state_AddRef(ID3D11RasterizerState *iface)
{
    struct qemu_d3d11_rasterizer_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_RASTERIZER_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_rasterizer_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d11_rasterizer_state_AddRef *c = (struct qemu_d3d11_rasterizer_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11RasterizerState_AddRef(state->host_rs11);
}

#endif

struct qemu_d3d11_rasterizer_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_rasterizer_state_Release(ID3D11RasterizerState *iface)
{
    struct qemu_d3d11_rasterizer_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_RASTERIZER_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_rasterizer_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d11_rasterizer_state_Release *c = (struct qemu_d3d11_rasterizer_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11RasterizerState_Release(state->host_rs11);
}

#endif

struct qemu_d3d11_rasterizer_state_GetDevice
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t device;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_rasterizer_state_GetDevice(ID3D11RasterizerState *iface, ID3D11Device **device)
{
    struct qemu_d3d11_rasterizer_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_RASTERIZER_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D11Device *)&dev_impl->ID3D11Device2_iface;
}

static void STDMETHODCALLTYPE d3d10_rasterizer_state_GetDevice(ID3D10RasterizerState *iface, ID3D10Device **device)
{
    struct qemu_d3d11_rasterizer_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_RASTERIZER_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D10Device *)&dev_impl->ID3D10Device1_iface;
}

#else

void qemu_d3d11_rasterizer_state_GetDevice(struct qemu_syscall *call)
{
    struct qemu_d3d11_rasterizer_state_GetDevice *c = (struct qemu_d3d11_rasterizer_state_GetDevice *)call;
    struct qemu_d3d11_state *state;
    ID3D11Device2 *host;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11RasterizerState_GetDevice(state->host_rs11, (ID3D11Device **)&host);
    c->device = QEMU_H2G(device_from_host(host));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_rasterizer_state_GetPrivateData(ID3D11RasterizerState *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_rasterizer_state_SetPrivateData(ID3D11RasterizerState *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_rasterizer_state_SetPrivateDataInterface(ID3D11RasterizerState *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d11_rasterizer_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_rasterizer_state_GetDesc(ID3D11RasterizerState *iface, D3D11_RASTERIZER_DESC *desc)
{
    struct qemu_d3d11_rasterizer_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D11RasterizerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_RASTERIZER_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_rasterizer_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d11_rasterizer_state_GetDesc *c = (struct qemu_d3d11_rasterizer_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11RasterizerState_GetDesc(state->host_rs11, QEMU_G2H(c->desc));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_rasterizer_state_QueryInterface(ID3D10RasterizerState *iface,
        REFIID riid, void **object)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    return d3d11_rasterizer_state_QueryInterface(&state->ID3D11RasterizerState_iface, riid, object);
}

#endif

struct qemu_d3d10_rasterizer_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_rasterizer_state_AddRef(ID3D10RasterizerState *iface)
{
    struct qemu_d3d10_rasterizer_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_RASTERIZER_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_rasterizer_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d10_rasterizer_state_AddRef *c = (struct qemu_d3d10_rasterizer_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10RasterizerState_AddRef(state->host_rs10);
}

#endif

struct qemu_d3d10_rasterizer_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_rasterizer_state_Release(ID3D10RasterizerState *iface)
{
    struct qemu_d3d10_rasterizer_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_RASTERIZER_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_rasterizer_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d10_rasterizer_state_Release *c = (struct qemu_d3d10_rasterizer_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10RasterizerState_Release(state->host_rs10);
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_rasterizer_state_GetPrivateData(ID3D10RasterizerState *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_rasterizer_state_SetPrivateData(ID3D10RasterizerState *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_rasterizer_state_SetPrivateDataInterface(ID3D10RasterizerState *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d10_rasterizer_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_rasterizer_state_GetDesc(ID3D10RasterizerState *iface, D3D10_RASTERIZER_DESC *desc)
{
    struct qemu_d3d10_rasterizer_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D10RasterizerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_RASTERIZER_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_rasterizer_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d10_rasterizer_state_GetDesc *c = (struct qemu_d3d10_rasterizer_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D10RasterizerState_GetDesc(state->host_rs10, QEMU_G2H(c->desc));
}

#endif

struct qemu_d3d11_sampler_state_QueryInterface
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t riid;
    uint64_t object;
};

#ifdef QEMU_DLL_GUEST

static inline struct qemu_d3d11_state *impl_from_ID3D11SamplerState(ID3D11SamplerState *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D11SamplerState_iface);
}

static inline struct qemu_d3d11_state *impl_from_ID3D10SamplerState(ID3D10SamplerState *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, ID3D10SamplerState_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_sampler_state_QueryInterface(ID3D11SamplerState *iface, REFIID riid, void **object)
{
    struct qemu_d3d11_sampler_state_QueryInterface call;
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    if (IsEqualGUID(riid, &IID_ID3D11SamplerState)
            || IsEqualGUID(riid, &IID_ID3D11DeviceChild)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        ID3D11SamplerState_AddRef(iface);
        *object = iface;
        return S_OK;
    }

    if (IsEqualGUID(riid, &IID_ID3D10SamplerState)
            || IsEqualGUID(riid, &IID_ID3D10DeviceChild))
    {
        ID3D10SamplerState_AddRef(&state->ID3D10SamplerState_iface);
        *object = &state->ID3D10SamplerState_iface;
        return S_OK;
    }

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_SAMPLER_STATE_QUERYINTERFACE);
    call.iface = (ULONG_PTR)state;
    call.riid = (ULONG_PTR)riid;

    qemu_syscall(&call.super);

    *object = NULL;
    return E_NOINTERFACE;
}

#else

void qemu_d3d11_sampler_state_QueryInterface(struct qemu_syscall *call)
{
    struct qemu_d3d11_sampler_state_QueryInterface *c = (struct qemu_d3d11_sampler_state_QueryInterface *)call;
    struct qemu_d3d11_state *state;
    IUnknown *obj;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11SamplerState_QueryInterface(state->host_ss11, QEMU_G2H(c->riid), (void **)&obj);
    if (SUCCEEDED(c->super.iret))
    {
        WINE_FIXME("Host returned an interface for %s which this wrapper does not know about.\n",
                wine_dbgstr_guid(QEMU_G2H(c->riid)));
        IUnknown_Release(obj);
    }
}

#endif

struct qemu_d3d11_sampler_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_sampler_state_AddRef(ID3D11SamplerState *iface)
{
    struct qemu_d3d11_sampler_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_SAMPLER_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_sampler_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d11_sampler_state_AddRef *c = (struct qemu_d3d11_sampler_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11SamplerState_AddRef(state->host_ss11);
}

#endif

struct qemu_d3d11_sampler_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d11_sampler_state_Release(ID3D11SamplerState *iface)
{
    struct qemu_d3d11_sampler_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_SAMPLER_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d11_sampler_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d11_sampler_state_Release *c = (struct qemu_d3d11_sampler_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D11SamplerState_Release(state->host_ss11);
}

#endif

struct qemu_d3d11_sampler_state_GetDevice
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t device;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_sampler_state_GetDevice(ID3D11SamplerState *iface, ID3D11Device **device)
{
    struct qemu_d3d11_sampler_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_SAMPLER_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D11Device *)&dev_impl->ID3D11Device2_iface;
}

static void STDMETHODCALLTYPE d3d10_sampler_state_GetDevice(ID3D10SamplerState *iface, ID3D10Device **device)
{
    struct qemu_d3d11_sampler_state_GetDevice call;
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);
    struct qemu_d3d11_device *dev_impl;

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_SAMPLER_STATE_GETDEVICE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    dev_impl = (struct qemu_d3d11_device *)(ULONG_PTR)call.device;
    *device = (ID3D10Device *)&dev_impl->ID3D10Device1_iface;
}

#else

void qemu_d3d11_sampler_state_GetDevice(struct qemu_syscall *call)
{
    struct qemu_d3d11_sampler_state_GetDevice *c = (struct qemu_d3d11_sampler_state_GetDevice *)call;
    struct qemu_d3d11_state *state;
    ID3D11Device2 *host;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11SamplerState_GetDevice(state->host_ss11, (ID3D11Device **)&host);
    c->device = QEMU_H2G(device_from_host(host));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d11_sampler_state_GetPrivateData(ID3D11SamplerState *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_sampler_state_SetPrivateData(ID3D11SamplerState *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_sampler_state_SetPrivateDataInterface(ID3D11SamplerState *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d11_sampler_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d11_sampler_state_GetDesc(ID3D11SamplerState *iface, D3D11_SAMPLER_DESC *desc)
{
    struct qemu_d3d11_sampler_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D11SamplerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D11_SAMPLER_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d11_sampler_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d11_sampler_state_GetDesc *c = (struct qemu_d3d11_sampler_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D11SamplerState_GetDesc(state->host_ss11, QEMU_G2H(c->desc));
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_sampler_state_QueryInterface(ID3D10SamplerState *iface, REFIID riid,
        void **object)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);

    WINE_TRACE("iface %p, riid %s, object %p.\n", iface, wine_dbgstr_guid(riid), object);

    return d3d11_sampler_state_QueryInterface(&state->ID3D11SamplerState_iface, riid, object);
}

#endif

struct qemu_d3d10_sampler_state_AddRef
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_sampler_state_AddRef(ID3D10SamplerState *iface)
{
    struct qemu_d3d10_sampler_state_AddRef call;
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_SAMPLER_STATE_ADDREF);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_sampler_state_AddRef(struct qemu_syscall *call)
{
    struct qemu_d3d10_sampler_state_AddRef *c = (struct qemu_d3d10_sampler_state_AddRef *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10SamplerState_AddRef(state->host_ss10);
}

#endif

struct qemu_d3d10_sampler_state_Release
{
    struct qemu_syscall super;
    uint64_t iface;
};

#ifdef QEMU_DLL_GUEST

static ULONG STDMETHODCALLTYPE d3d10_sampler_state_Release(ID3D10SamplerState *iface)
{
    struct qemu_d3d10_sampler_state_Release call;
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_SAMPLER_STATE_RELEASE);
    call.iface = (ULONG_PTR)state;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_d3d10_sampler_state_Release(struct qemu_syscall *call)
{
    struct qemu_d3d10_sampler_state_Release *c = (struct qemu_d3d10_sampler_state_Release *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    c->super.iret = ID3D10SamplerState_Release(state->host_ss10);
}

#endif

#ifdef QEMU_DLL_GUEST

static HRESULT STDMETHODCALLTYPE d3d10_sampler_state_GetPrivateData(ID3D10SamplerState *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %p, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_get_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_sampler_state_SetPrivateData(ID3D10SamplerState *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);

    WINE_TRACE("iface %p, guid %s, data_size %u, data %p.\n",
            iface, wine_dbgstr_guid(guid), data_size, data);

    return d3d_set_private_data(&state->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_sampler_state_SetPrivateDataInterface(ID3D10SamplerState *iface,
        REFGUID guid, const IUnknown *data)
{
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);

    WINE_TRACE("iface %p, guid %s, data %p.\n", iface, wine_dbgstr_guid(guid), data);

    return d3d_set_private_data_interface(&state->private_store, guid, data);
}

#endif

struct qemu_d3d10_sampler_state_GetDesc
{
    struct qemu_syscall super;
    uint64_t iface;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

static void STDMETHODCALLTYPE d3d10_sampler_state_GetDesc(ID3D10SamplerState *iface, D3D10_SAMPLER_DESC *desc)
{
    struct qemu_d3d10_sampler_state_GetDesc call;
    struct qemu_d3d11_state *state = impl_from_ID3D10SamplerState(iface);

    call.super.id = QEMU_SYSCALL_ID(CALL_D3D10_SAMPLER_STATE_GETDESC);
    call.iface = (ULONG_PTR)state;
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);
}

#else

void qemu_d3d10_sampler_state_GetDesc(struct qemu_syscall *call)
{
    struct qemu_d3d10_sampler_state_GetDesc *c = (struct qemu_d3d10_sampler_state_GetDesc *)call;
    struct qemu_d3d11_state *state;

    WINE_TRACE("\n");
    state = QEMU_G2H(c->iface);

    ID3D10SamplerState_GetDesc(state->host_ss10, QEMU_G2H(c->desc));
}

#endif

#ifdef QEMU_DLL_GUEST

static struct ID3D11BlendStateVtbl d3d11_blend_state_vtbl =
{
    /* IUnknown methods */
    d3d11_blend_state_QueryInterface,
    d3d11_blend_state_AddRef,
    d3d11_blend_state_Release,
    /* ID3D11DeviceChild methods */
    d3d11_blend_state_GetDevice,
    d3d11_blend_state_GetPrivateData,
    d3d11_blend_state_SetPrivateData,
    d3d11_blend_state_SetPrivateDataInterface,
    /* ID3D11BlendState methods */
    d3d11_blend_state_GetDesc,
};

static struct ID3D10BlendState1Vtbl d3d10_blend_state_vtbl =
{
    /* IUnknown methods */
    d3d10_blend_state_QueryInterface,
    d3d10_blend_state_AddRef,
    d3d10_blend_state_Release,
    /* ID3D10DeviceChild methods */
    d3d10_blend_state_GetDevice,
    d3d10_blend_state_GetPrivateData,
    d3d10_blend_state_SetPrivateData,
    d3d10_blend_state_SetPrivateDataInterface,
    /* ID3D10BlendState methods */
    d3d10_blend_state_GetDesc,
    /* ID3D10BlendState1 methods */
    d3d10_blend_state_GetDesc1,
};

static struct ID3D11DepthStencilStateVtbl d3d11_depthstencil_state_vtbl =
{
    /* IUnknown methods */
    d3d11_depthstencil_state_QueryInterface,
    d3d11_depthstencil_state_AddRef,
    d3d11_depthstencil_state_Release,
    /* ID3D11DeviceChild methods */
    d3d11_depthstencil_state_GetDevice,
    d3d11_depthstencil_state_GetPrivateData,
    d3d11_depthstencil_state_SetPrivateData,
    d3d11_depthstencil_state_SetPrivateDataInterface,
    /* ID3D11DepthStencilState methods */
    d3d11_depthstencil_state_GetDesc,
};

static struct ID3D10DepthStencilStateVtbl d3d10_depthstencil_state_vtbl =
{
    /* IUnknown methods */
    d3d10_depthstencil_state_QueryInterface,
    d3d10_depthstencil_state_AddRef,
    d3d10_depthstencil_state_Release,
    /* ID3D10DeviceChild methods */
    d3d10_depthstencil_state_GetDevice,
    d3d10_depthstencil_state_GetPrivateData,
    d3d10_depthstencil_state_SetPrivateData,
    d3d10_depthstencil_state_SetPrivateDataInterface,
    /* ID3D10DepthStencilState methods */
    d3d10_depthstencil_state_GetDesc,
};

static struct ID3D11RasterizerStateVtbl d3d11_rasterizer_state_vtbl =
{
    /* IUnknown methods */
    d3d11_rasterizer_state_QueryInterface,
    d3d11_rasterizer_state_AddRef,
    d3d11_rasterizer_state_Release,
    /* ID3D11DeviceChild methods */
    d3d11_rasterizer_state_GetDevice,
    d3d11_rasterizer_state_GetPrivateData,
    d3d11_rasterizer_state_SetPrivateData,
    d3d11_rasterizer_state_SetPrivateDataInterface,
    /* ID3D11RasterizerState methods */
    d3d11_rasterizer_state_GetDesc,
};

static struct ID3D10RasterizerStateVtbl d3d10_rasterizer_state_vtbl =
{
    /* IUnknown methods */
    d3d10_rasterizer_state_QueryInterface,
    d3d10_rasterizer_state_AddRef,
    d3d10_rasterizer_state_Release,
    /* ID3D10DeviceChild methods */
    d3d10_rasterizer_state_GetDevice,
    d3d10_rasterizer_state_GetPrivateData,
    d3d10_rasterizer_state_SetPrivateData,
    d3d10_rasterizer_state_SetPrivateDataInterface,
    /* ID3D10RasterizerState methods */
    d3d10_rasterizer_state_GetDesc,
};

static struct ID3D11SamplerStateVtbl d3d11_sampler_state_vtbl =
{
    /* IUnknown methods */
    d3d11_sampler_state_QueryInterface,
    d3d11_sampler_state_AddRef,
    d3d11_sampler_state_Release,
    /* ID3D11DeviceChild methods */
    d3d11_sampler_state_GetDevice,
    d3d11_sampler_state_GetPrivateData,
    d3d11_sampler_state_SetPrivateData,
    d3d11_sampler_state_SetPrivateDataInterface,
    /* ID3D11SamplerState methods */
    d3d11_sampler_state_GetDesc,
};

static struct ID3D10SamplerStateVtbl d3d10_sampler_state_vtbl =
{
    /* IUnknown methods */
    d3d10_sampler_state_QueryInterface,
    d3d10_sampler_state_AddRef,
    d3d10_sampler_state_Release,
    /* ID3D10DeviceChild methods */
    d3d10_sampler_state_GetDevice,
    d3d10_sampler_state_GetPrivateData,
    d3d10_sampler_state_SetPrivateData,
    d3d10_sampler_state_SetPrivateDataInterface,
    /* ID3D10SamplerState methods */
    d3d10_sampler_state_GetDesc,
};

void qemu_d3d11_sampler_state_guest_init(struct qemu_d3d11_state *state)
{
    state->ID3D11SamplerState_iface.lpVtbl = &d3d11_sampler_state_vtbl;
    state->ID3D10SamplerState_iface.lpVtbl = &d3d10_sampler_state_vtbl;
    wined3d_private_store_init(&state->private_store);
}

void qemu_d3d11_blend_state_guest_init(struct qemu_d3d11_state *state)
{
    state->ID3D11BlendState_iface.lpVtbl = &d3d11_blend_state_vtbl;
    state->ID3D10BlendState1_iface.lpVtbl = &d3d10_blend_state_vtbl;
    wined3d_private_store_init(&state->private_store);
}

void qemu_d3d11_depthstencil_state_guest_init(struct qemu_d3d11_state *state)
{
    state->ID3D11DepthStencilState_iface.lpVtbl = &d3d11_depthstencil_state_vtbl;
    state->ID3D10DepthStencilState_iface.lpVtbl = &d3d10_depthstencil_state_vtbl;
    wined3d_private_store_init(&state->private_store);
}

void qemu_d3d11_rasterizer_state_guest_init(struct qemu_d3d11_state *state)
{
    state->ID3D11RasterizerState_iface.lpVtbl = &d3d11_rasterizer_state_vtbl;
    state->ID3D10RasterizerState_iface.lpVtbl = &d3d10_rasterizer_state_vtbl;
    wined3d_private_store_init(&state->private_store);
}

void __fastcall d3d11_state_guest_destroy(struct qemu_d3d11_state *state)
{
    wined3d_private_store_cleanup(&state->private_store);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D11BlendState(ID3D11BlendState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == &d3d11_blend_state_vtbl);*/

    return impl_from_ID3D11BlendState(iface);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D10BlendState(ID3D10BlendState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == (ID3D10BlendStateVtbl *)&d3d10_blend_state_vtbl);*/

    return impl_from_ID3D10BlendState1((ID3D10BlendState1 *)iface);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D11DepthStencilState(ID3D11DepthStencilState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == &d3d11_depthstencil_state_vtbl);*/

    return impl_from_ID3D11DepthStencilState(iface);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D10DepthStencilState(ID3D10DepthStencilState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == &d3d10_depthstencil_state_vtbl);*/

    return impl_from_ID3D10DepthStencilState(iface);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D11RasterizerState(ID3D11RasterizerState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == &d3d11_rasterizer_state_vtbl);*/

    return impl_from_ID3D11RasterizerState(iface);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D10RasterizerState(ID3D10RasterizerState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == &d3d10_rasterizer_state_vtbl);*/

    return impl_from_ID3D10RasterizerState(iface);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D11SamplerState(ID3D11SamplerState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == &d3d11_sampler_state_vtbl);*/

    return impl_from_ID3D11SamplerState(iface);
}

struct qemu_d3d11_state *unsafe_impl_from_ID3D10SamplerState(ID3D10SamplerState *iface)
{
    if (!iface)
        return NULL;
    /*assert(iface->lpVtbl == &d3d10_sampler_state_vtbl);*/

    return impl_from_ID3D10SamplerState(iface);
}

#else

static inline struct qemu_d3d11_state *impl_from_priv_data(IUnknown *iface)
{
    return CONTAINING_RECORD(iface, struct qemu_d3d11_state, priv_data_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_state_priv_data_QueryInterface(IUnknown *iface, REFIID riid, void **out)
{
    WINE_ERR("Unexpected call\n");
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE d3d11_state_priv_data_AddRef(IUnknown *iface)
{
    struct qemu_d3d11_state *state = impl_from_priv_data(iface);
    ULONG refcount = InterlockedIncrement(&state->refcount);

    WINE_TRACE("%p increasing refcount to %u.\n", state, refcount);

    return refcount;
}

static ULONG STDMETHODCALLTYPE d3d11_state_priv_data_Release(IUnknown *iface)
{
    struct qemu_d3d11_state *state = impl_from_priv_data(iface);
    ULONG refcount = InterlockedDecrement(&state->refcount);

    WINE_TRACE("%p decreasing refcount to %u.\n", state, refcount);

    if (!refcount)
    {
        WINE_TRACE("Destroying state wrapper %p for host state %p.\n", state, state->host_ds11);
        qemu_ops->qemu_execute(QEMU_G2H(d3d11_state_guest_destroy), QEMU_H2G(state));
        HeapFree(GetProcessHeap(), 0, state);
    }

    return refcount;
}

static struct IUnknownVtbl priv_data_vtbl =
{
    /* IUnknown methods */
    d3d11_state_priv_data_QueryInterface,
    d3d11_state_priv_data_AddRef,
    d3d11_state_priv_data_Release,
};

HRESULT qemu_d3d11_state_create(ID3D11DeviceChild *host, const IID *d3d10iface, struct qemu_d3d11_state **state)
{
    struct qemu_d3d11_state *obj;
    HRESULT hr;

    obj = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*obj));
    if (!obj)
    {
        WINE_WARN("Out of memory\n");
        return E_OUTOFMEMORY;
    }

    obj->host_bs11 = (void *)host;
    if (d3d10iface)
    {
        hr = ID3D11View_QueryInterface(host, d3d10iface, (void **)&obj->host_bs10);
        if (FAILED(hr))
            WINE_ERR("Failed to QI %s.\n", wine_dbgstr_guid(d3d10iface));
        ID3D10View_Release(obj->host_bs10);
    }

    obj->priv_data_iface.lpVtbl = &priv_data_vtbl;
    /* Leave the ref at 0, we want the host obj to own the only / final reference. */
    ID3D11View_SetPrivateDataInterface(host, &IID_d3d11_priv_data, &obj->priv_data_iface);

    *state = obj;
    return S_OK;
}

struct qemu_d3d11_state *state_from_host(ID3D11DeviceChild *host)
{
    IUnknown *priv;
    DWORD size = sizeof(priv);
    HRESULT hr;

    if (!host)
        return NULL;

    hr = ID3D11DeviceChild_GetPrivateData(host, &IID_d3d11_priv_data, &size, &priv);
    if (FAILED(hr))
    {
        WINE_TRACE("Failed to get private data from host state %p.\n", host);
        return NULL;
    }

    IUnknown_Release(priv);
    return impl_from_priv_data(priv);
}

#endif

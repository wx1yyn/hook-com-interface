#include "stdafx.h"
#include "VtableHooks.h"
namespace Hook
{
    //STDMETHODIMP get_ObjectName(IUnknown* This, BSTR* pVal);
    //STDMETHODIMP DoWork(IUnknown* This, LONG arg1, LONG arg2, LONG* result);
    //STDMETHODIMP QueryInterface(IUnknown* This, REFIID riid, void **ppvObject);
	STDMETHODIMP GetBuffer(IUnknown* This, UINT32  NumFramesRequested, BYTE **ppData);
	STDMETHODIMP Initialize(
		IUnknown* This,
		AUDCLNT_SHAREMODE ShareMode,
		DWORD StreamFlags,
		REFERENCE_TIME hnsBufferDuration,
		REFERENCE_TIME hnsPeriodicity,
		const WAVEFORMATEX *pFormat,
		LPCGUID AudioSessionGuid
		);
	STDMETHODIMP Activate( 
		IUnknown* This,
		REFIID iid,
		IMMDevice *pDevice,
		PROPVARIANT *pActivationParams,
		void **ppInterface);
	STDMETHODIMP CreateInstance(
		IUnknown* This,
		IUnknown *pUnkOuter,
		REFIID riid,
		void **ppvObject
		);
}

struct Context
{
    Context()
        : m_Name("Hooked object")
    {
    }

    //PVOID m_OriginalQueryInterface;
    //PVOID g_OriginalRelease;
    //PVOID m_OriginalGetObjectName;
    //PVOID m_OriginalDoWork;
	PVOID m_OriginalGetBuffer;
	PVOID m_OriginalInitialize;
	PVOID m_OriginalActivate;
	PVOID m_OriginalCreateInstance;
    ATL::CComBSTR m_Name;
};

//////////////////////////////////////////////////////////////////////////

std::auto_ptr<Context> g_Context;

//////////////////////////////////////////////////////////////////////////

const IID ID_IID_IAudioRenderClient = __uuidof(IAudioRenderClient);//{0xF294ACFC,0x3146,0x4483,{0xA7,0xBF,0xAD,0xDC,0xA7,0xC2,0x60,0xE2}};
const IID ID_IID_IAudioClient = __uuidof(IAudioClient);
const IID ID_IID_IMMDeviceActivator = __uuidof(IMMDeviceActivator);

HRESULT HookMethod(IUnknown* original, PVOID proxyMethod, PVOID* originalMethod, DWORD vtableOffset)
{
    PVOID* originalVtable = *(PVOID**)original;

    if (originalVtable[vtableOffset] == proxyMethod)
        return S_OK;

    *originalMethod = originalVtable[vtableOffset];
    originalVtable[vtableOffset] = proxyMethod;

    return S_OK;
}

//HRESULT InstallComInterfaceHooks(IUnknown* originalInterface)
//{
//    // Only single instance of a target object is supported in the sample
//    if (g_Context.get())
//        return E_FAIL;
//
//    ATL::CComPtr<ISampleObject> so;
//    HRESULT hr = originalInterface->QueryInterface(IID_ISampleObject, (void**)&so);
//    if (FAILED(hr))
//        return hr; // we need this interface to be present
//
//    // remove protection from the vtable
//    DWORD dwOld = 0;
//    if(!::VirtualProtect(*(PVOID**)(originalInterface), sizeof(LONG_PTR), PAGE_EXECUTE_READWRITE, &dwOld))
//        return E_FAIL;
//
//    // hook interface methods
//    g_Context.reset(new Context);
//    HookMethod(so, (PVOID)Hook::QueryInterface, &g_Context->m_OriginalQueryInterface, 0);
//    HookMethod(so, (PVOID)Hook::get_ObjectName, &g_Context->m_OriginalGetObjectName, 7);
//    HookMethod(so, (PVOID)Hook::DoWork, &g_Context->m_OriginalDoWork, 9);
//
//    return S_OK;
//}

HRESULT InstallComInterfaceHooks(IUnknown* originalInterface, REFIID riid)
{
	// Only single instance of a target object is supported in the sample
	if (g_Context.get())
		//return E_FAIL;
		return S_OK;

	ATL::CComPtr<IClassFactory> so;
	HRESULT hr = originalInterface->QueryInterface(riid, (void**)&so);
	if (FAILED(hr))
	    return hr; // we need this interface to be present

	// remove protection from the vtable
	DWORD dwOld = 0;
	if(!::VirtualProtect(*(PVOID**)(originalInterface), sizeof(LONG_PTR), PAGE_EXECUTE_READWRITE, &dwOld))
		return E_FAIL;

	// hook interface methods
	g_Context.reset(new Context);
	HookMethod(so, (PVOID)Hook::CreateInstance, &g_Context->m_OriginalCreateInstance, 3);//watch head file
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//typedef HRESULT (WINAPI *QueryInterface_T)(IUnknown* This, REFIID riid, void **ppvObject);
//
//STDMETHODIMP Hook::QueryInterface(IUnknown* This, REFIID riid, void **ppvObject)
//{
//    QueryInterface_T qi = (QueryInterface_T)g_Context->m_OriginalQueryInterface;
//    HRESULT hr = qi(This, riid, ppvObject);
//    return hr;
//}
//
//STDMETHODIMP Hook::get_ObjectName(IUnknown* This, BSTR* pVal)
//{
//    return g_Context->m_Name.CopyTo(pVal);
//}
//
//STDMETHODIMP Hook::DoWork(IUnknown* This, LONG arg1, LONG arg2, LONG* result)
//{
//    *result = 42;
//    return S_OK;
//}

typedef HRESULT (WINAPI *GetBuffer_T)(IUnknown* This, UINT32  NumFramesRequested, BYTE **ppData);

STDMETHODIMP Hook::GetBuffer(IUnknown* This, UINT32  NumFramesRequested, BYTE **ppData)
{
	GetBuffer_T gt = (GetBuffer_T)g_Context->m_OriginalGetBuffer;
	HRESULT hr = gt(This, NumFramesRequested, ppData);
	return S_OK;
}

typedef HRESULT (WINAPI *Initialize_T)(
	IUnknown* This,
	AUDCLNT_SHAREMODE ShareMode,
	DWORD StreamFlags,
	REFERENCE_TIME hnsBufferDuration,
	REFERENCE_TIME hnsPeriodicity,
	const WAVEFORMATEX *pFormat,
	LPCGUID AudioSessionGuid
	);

STDMETHODIMP Hook::Initialize(
	IUnknown* This,
	AUDCLNT_SHAREMODE ShareMode,
	DWORD StreamFlags,
	REFERENCE_TIME hnsBufferDuration,
	REFERENCE_TIME hnsPeriodicity,
	const WAVEFORMATEX *pFormat,
	LPCGUID AudioSessionGuid
	)
{
	Initialize_T gt = (Initialize_T)g_Context->m_OriginalInitialize;
	HRESULT hr = gt(This, ShareMode, StreamFlags, hnsBufferDuration, hnsPeriodicity, pFormat, AudioSessionGuid);

	if (FAILED(hr))
		return hr;

	ATL::CComPtr<IAudioRenderClient> so;
	HRESULT hr_ = ((IAudioClient*)This)->GetService(ID_IID_IAudioRenderClient, (void**)&so);
	if (FAILED(hr_))
		return hr_; // we need this interface to be present

	// remove protection from the vtable
	DWORD dwOld = 0;
	if(!::VirtualProtect(*(PVOID**)((IUnknown*)so), sizeof(LONG_PTR), PAGE_EXECUTE_READWRITE, &dwOld))
		return E_FAIL;
	HookMethod(so, (PVOID)Hook::GetBuffer, &g_Context->m_OriginalGetBuffer, 3);

	return S_OK;
}

typedef HRESULT (WINAPI *CreateInstance_T)(
	IUnknown* This,
	IUnknown *pUnkOuter,
	REFIID riid,
	void **ppvObject
	);

STDMETHODIMP Hook::CreateInstance(
	IUnknown* This,
	IUnknown *pUnkOuter,
	REFIID riid,
	void **ppvObject
	)
{
	CreateInstance_T gt = (CreateInstance_T)g_Context->m_OriginalCreateInstance;
	HRESULT hr = gt(This, pUnkOuter, riid, ppvObject);
	if (FAILED(hr))
		return hr;

	ATL::CComPtr<IMMDeviceActivator> so;
	HRESULT hr_ = ((IUnknown*)*ppvObject)->QueryInterface(ID_IID_IMMDeviceActivator, (void**)&so);
	if (FAILED(hr_))
		return hr_; // we need this interface to be present

	// remove protection from the vtable
	DWORD dwOld = 0;
	if(!::VirtualProtect(*(PVOID**)((IUnknown*)*ppvObject), sizeof(LONG_PTR), PAGE_EXECUTE_READWRITE, &dwOld))
		return E_FAIL;
	HookMethod(so, (PVOID)Hook::Activate, &g_Context->m_OriginalActivate, 3);
	return S_OK;
}

typedef HRESULT (WINAPI *Activate_T)( 
	IUnknown* This,
	REFIID iid,
	IMMDevice *pDevice,
	PROPVARIANT *pActivationParams,
	void **ppInterface);

STDMETHODIMP Hook::Activate(
	IUnknown* This,
	REFIID iid,
	IMMDevice *pDevice,
	PROPVARIANT *pActivationParams,
	void **ppInterface)
{
	Activate_T gt = (Activate_T)g_Context->m_OriginalActivate;
	HRESULT hr = gt(This, iid, pDevice, pActivationParams, ppInterface);

	if (FAILED(hr))
		return hr;

	ATL::CComPtr<IAudioClient> so;
	HRESULT hr_ = ((IUnknown*)*ppInterface)->QueryInterface(ID_IID_IAudioClient, (void**)&so);
	if (FAILED(hr_))
		return hr_; // we need this interface to be present

	// remove protection from the vtable
	DWORD dwOld = 0;
	if(!::VirtualProtect(*(PVOID**)((IUnknown*)*ppInterface), sizeof(LONG_PTR), PAGE_EXECUTE_READWRITE, &dwOld))
		return E_FAIL;
	HookMethod(so, (PVOID)Hook::Initialize, &g_Context->m_OriginalInitialize, 3);

	return S_OK;
}
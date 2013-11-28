#include "stdafx.h"
#include "SampleObjectProxy.h"

HRESULT CSampleObjectProxy::CreateInstance(IUnknown* original, REFIID riid, void **ppvObject)
{
    ATL::CComObjectNoLock<CSampleObjectProxy>* self = new ATL::CComObjectNoLock<CSampleObjectProxy>;

    self->SetVoid(NULL);

    self->InternalFinalConstructAddRef();
    HRESULT hr = self->_AtlInitialConstruct();
    if (SUCCEEDED(hr))
        hr = self->FinalConstruct();
    if (SUCCEEDED(hr))
        hr = self->_AtlFinalConstruct();
    self->InternalFinalConstructRelease();

    if (SUCCEEDED(hr))
        hr = original->QueryInterface(IID_ISampleObject, (void**)&self->m_OriginalObject);

    if (SUCCEEDED(hr))
        hr = self->QueryInterface(riid, ppvObject);

    if (hr != S_OK)
        delete self;

    return hr;
}

STDMETHODIMP CSampleObjectProxy::get_ObjectName(BSTR* pVal)
{
    return m_Name.CopyTo(pVal);
}

STDMETHODIMP CSampleObjectProxy::put_ObjectName(BSTR newVal)
{
    return m_OriginalObject->put_ObjectName(newVal);
}

STDMETHODIMP CSampleObjectProxy::DoWork(LONG arg1, LONG arg2, LONG* result)
{
    *result = 42;
    return S_OK;
}

HRESULT CSampleObjectProxy::FinalConstruct()
{
    return S_OK;
}

void CSampleObjectProxy::FinalRelease()
{

}

CSampleObjectProxy::CSampleObjectProxy()
: m_Name("Hooked object")
{

}
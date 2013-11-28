#pragma once

class ATL_NO_VTABLE CSampleObjectProxy :
    public ATL::CComObjectRootEx<ATL::CComMultiThreadModel>,
	public ATL::CComCoClass<CSampleObjectProxy, &CLSID_SampleObject>,
	public ATL::IDispatchImpl<ISampleObject, &IID_ISampleObject, &LIBID_ComSampleLib, 1, 0>
{
public:
    CSampleObjectProxy();

    DECLARE_NO_REGISTRY()

    BEGIN_COM_MAP(CSampleObjectProxy)
	    COM_INTERFACE_ENTRY(ISampleObject)
	    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
	HRESULT FinalConstruct();
	void FinalRelease();

public:
    HRESULT static CreateInstance(IUnknown* original, REFIID riid, void **ppvObject);

public:
    STDMETHOD(get_ObjectName)(BSTR* pVal);
    STDMETHOD(put_ObjectName)(BSTR newVal);
    STDMETHOD(DoWork)(LONG arg1, LONG arg2, LONG* result);

private:
    ATL::CComPtr<ISampleObject> m_OriginalObject;
    ATL::CComBSTR m_Name;
};

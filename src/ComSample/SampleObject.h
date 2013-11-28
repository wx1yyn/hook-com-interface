// SampleObject.h : Declaration of the CSampleObject

#pragma once
#include "resource.h"       // main symbols

#include "ComSample_i.h"




// CSampleObject

class ATL_NO_VTABLE CSampleObject :
    public ATL::CComObjectRootEx<ATL::CComMultiThreadModel>,
	public ATL::CComCoClass<CSampleObject, &CLSID_SampleObject>,
	public ATL::IDispatchImpl<ISampleObject, &IID_ISampleObject, &LIBID_ComSampleLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CSampleObject();

    DECLARE_REGISTRY_RESOURCEID(IDR_SAMPLEOBJECT)

    BEGIN_COM_MAP(CSampleObject)
	    COM_INTERFACE_ENTRY(ISampleObject)
	    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
	HRESULT FinalConstruct();
	void FinalRelease();

public:
    STDMETHOD(get_ObjectName)(BSTR* pVal);
    STDMETHOD(put_ObjectName)(BSTR newVal);
    STDMETHOD(DoWork)(LONG arg1, LONG arg2, LONG* result);

private:
    ATL::CComBSTR m_Name;
};

OBJECT_ENTRY_AUTO(__uuidof(SampleObject), CSampleObject)

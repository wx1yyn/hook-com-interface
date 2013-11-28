// SampleObject.cpp : Implementation of CSampleObject

#include "stdafx.h"
#include "SampleObject.h"


// CSampleObject



STDMETHODIMP CSampleObject::get_ObjectName(BSTR* pVal)
{
    return m_Name.CopyTo(pVal);
}

STDMETHODIMP CSampleObject::put_ObjectName(BSTR newVal)
{
    return m_Name.AssignBSTR(newVal);
}

STDMETHODIMP CSampleObject::DoWork(LONG arg1, LONG arg2, LONG* result)
{
    *result = arg1 + arg2;    

    return S_OK;
}

HRESULT CSampleObject::FinalConstruct()
{
    return S_OK;
}

void CSampleObject::FinalRelease()
{

}

CSampleObject::CSampleObject()
: m_Name("Original object")
{

}
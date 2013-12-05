

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Dec 04 11:12:31 2013
 */
/* Compiler settings for ComSample.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ComSample_i_h__
#define __ComSample_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISampleObject_FWD_DEFINED__
#define __ISampleObject_FWD_DEFINED__
typedef interface ISampleObject ISampleObject;
#endif 	/* __ISampleObject_FWD_DEFINED__ */


#ifndef __SampleObject_FWD_DEFINED__
#define __SampleObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class SampleObject SampleObject;
#else
typedef struct SampleObject SampleObject;
#endif /* __cplusplus */

#endif 	/* __SampleObject_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISampleObject_INTERFACE_DEFINED__
#define __ISampleObject_INTERFACE_DEFINED__

/* interface ISampleObject */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISampleObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("515754AD-E9EC-460A-B88E-02359839F0D8")
    ISampleObject : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ObjectName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ObjectName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoWork( 
            LONG arg1,
            LONG arg2,
            LONG *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISampleObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISampleObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISampleObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISampleObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISampleObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISampleObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISampleObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISampleObject * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ObjectName )( 
            ISampleObject * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ObjectName )( 
            ISampleObject * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DoWork )( 
            ISampleObject * This,
            LONG arg1,
            LONG arg2,
            LONG *result);
        
        END_INTERFACE
    } ISampleObjectVtbl;

    interface ISampleObject
    {
        CONST_VTBL struct ISampleObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISampleObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISampleObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISampleObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISampleObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISampleObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISampleObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISampleObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISampleObject_get_ObjectName(This,pVal)	\
    ( (This)->lpVtbl -> get_ObjectName(This,pVal) ) 

#define ISampleObject_put_ObjectName(This,newVal)	\
    ( (This)->lpVtbl -> put_ObjectName(This,newVal) ) 

#define ISampleObject_DoWork(This,arg1,arg2,result)	\
    ( (This)->lpVtbl -> DoWork(This,arg1,arg2,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISampleObject_INTERFACE_DEFINED__ */



#ifndef __ComSampleLib_LIBRARY_DEFINED__
#define __ComSampleLib_LIBRARY_DEFINED__

/* library ComSampleLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ComSampleLib;

EXTERN_C const CLSID CLSID_SampleObject;

#ifdef __cplusplus

class DECLSPEC_UUID("6F3A4816-5F14-42F0-BFF7-65F0664A9BC3")
SampleObject;
#endif
#endif /* __ComSampleLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



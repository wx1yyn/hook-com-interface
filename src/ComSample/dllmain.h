// dllmain.h : Declaration of module class.

class CComSampleModule : public ATL::CAtlDllModuleT< CComSampleModule >
{
public :
	DECLARE_LIBID(LIBID_ComSampleLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMSAMPLE, "{1A6E3ACF-E81D-4E0A-B30F-DF2CF0EE4154}")
};

extern class CComSampleModule _AtlModule;

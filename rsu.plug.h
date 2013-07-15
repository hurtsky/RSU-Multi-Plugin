// -----------------------------------------------------------------
// RagnarokOnline Patcher Lite (codename: RSU)
// (c) 2009-2010 Ai4rei/AN
//
// -----------------------------------------------------------------

#ifndef _RSU_PLUG_H_
#define _RSU_PLUG_H_

#ifdef __cplusplus
extern "C"
{
#else  /* __cplusplus */
    /* for people, who do not have those for C on their own */
    #ifndef __bool_true_false_are_defined
        typedef char bool;
        #define false ((bool)(1==0))
        #define true  ((bool)(1==1))
        #define __bool_true_false_are_defined
    #endif
#endif  /* __cplusplus */

#define PLUGDEF(x) (__stdcall *##x)

/**
 * Plug::InterfaceQuery
 * @brief   Retrieves a plugin interface version and type to
 *          communicate with and alter the main application.
 * @param   luVersion
 *          Specifies the interface version to retrieve.
 * @param   luType
 *          Specifies the interface type to retrieve.
 * @param   lpReserved
 *          Reserved for future use, must be set to NULL.
 * @param   lppInterface
 *          Pointer to a pointer variable, that receives the
 *          interface pointer.
 * @return  If the interface of given type and version exists true,
 *          otherwise false.
 ******************************************************************/
typedef bool PLUGDEF(PlugInterfaceQueryFunc)(unsigned long luVersion, unsigned long luType, void* lpReserved, void** lppInterface);

/**
 * Plug::OnEvent
 * @brief   Allows plugins to process events, that are posted by the
 *          main application.
 * @param   luEvent
 *          ID of the event, that either is about to occur, or has
 *          occured.
 * @param   luData
 *          Variable associated with the event.
 * @param   lpData
 *          Pointer associated with the event.
 * @return  If the event was processed, return true otherwise false,
 *          unless event description states otherwise.
 ******************************************************************/
typedef bool PLUGDEF(PlugInterfaceOnEventFunc)(unsigned long luEvent, unsigned long luData, void* lpData);

/**
 * Plugin::Load
 * @brief   Called upon loading of the plugin in question.
 * @param   luAppVersion
 *          Contains the version of the calling application.
 * @param   luPlugVersion
 *          Pointer to a variable, that receives the plugin's
 *          version. Must be set to RSU_PLUG_VERSION_CURRENT.
 * @return  Return true, of the plugin want to be processed further,
 *          otherwise return false, that causes the plugin to be
 *          immediately unloaded.
 * @note    Do not perform any initialization inside this function,
 *          only check luAppversion and set luPlugversion. If you
 *          want to check further conditions, Plugin_Init is the
 *          right function to do so.
 ******************************************************************/
typedef bool PLUGDEF(PluginLoadFunc)(unsigned long luAppVersion, unsigned long* luPlugVersion);

/**
 * Plugin::Init
 * @brief   Allows the plugin perform all necessary initialization,
 *          such as allocating memory or registering own interfaces.
 * @param   lpInterfaceQuery
 *          Pointer to the Plug::InterfaceQuery function.
 * @param   lppOnEvent
 *          Pointer to a function pointer, which receives you plugin
 *          event handling function. Can be NULL, if the plugin does
 *          not want to receive event notifications.
 * @param   lpReserved
 *          Not used, must be NULL.
 * @return  Return true, if the initialization was successful,
 *          otherwise false to unload the plugin.
 ******************************************************************/
typedef bool PLUGDEF(PluginInitFunc)(PlugInterfaceQueryFunc lpInterfaceQuery, PlugInterfaceOnEventFunc* lppOnEvent, void* lpReserved);

/**
 * Plugin::Quit
 * @brief   Notifies the plugin, that it is about being unloaded
 *          and should release all acquired resources.
 ******************************************************************/
typedef void PLUGDEF(PluginQuitFunc)(void);

/* Constants */
enum
{
    /* plugin interface versions (31-16: major, 15-0: minor) */
    RSU_PLUG_VERSION1           = 0x00010000,
    RSU_PLUG_VERSION2           = 0x00020000,
    RSU_PLUG_VERSION_CURRENT    = 0x00020000,

    /* plugin interface types */
    RSU_PLUG_TYPE_COMMON        = 0,
    RSU_PLUG_TYPE_UI            = 1,
    RSU_PLUG_TYPE_GRF           = 2,
    RSU_PLUG_TYPE_RGZ           = 3,
    RSU_PLUG_TYPE_NETWORK       = 4,
    RSU_PLUG_TYPE_INTL          = 5,
    RSU_PLUG_TYPE_SETTING       = 6,

    /* plugin events */
    RSU_PLUG_EVENT_NULL         = 0,
    RSU_PLUG_EVENT_INIT         = 1,
    RSU_PLUG_EVENT_INITDONE     = 2,
    RSU_PLUG_EVENT_HANDLEOBJECT = 3,
    RSU_PLUG_EVENT_PATCHALLOW   = 4,
    RSU_PLUG_EVENT_SYSMENUINIT  = 5,
    RSU_PLUG_EVENT_UI_REGISTER  = 6,
    RSU_PLUG_EVENT_UI_INIT      = 7,
    RSU_PLUG_EVENT_UI_BASE      = 8,
    RSU_PLUG_EVENT_UI_FONT      = 9,
    RSU_PLUG_EVENT_UI_INITDONE  = 10,
    RSU_PLUG_EVENT_UI_DESTROY   = 11,
    RSU_PLUG_EVENT_QUERYTASK    = 12,
    RSU_PLUG_EVENT_WEBCONNFAULT = 13,
    RSU_PLUG_EVENT_FTPCONNFAULT = 14,

    /* patch information types */
    RSU_IPATCH_INFFILE = 0,
    RSU_IPATCH_GRFFILE,
    RSU_IPATCH_EXEFILE,
    RSU_IPATCH_EXECMDL,
    RSU_IPATCH_WEBSITE,
    RSU_IPATCH_WEBPATH,
    RSU_IPATCH_WEBDENY,
    RSU_IPATCH_WEBLIST,
    RSU_IPATCH_FTPSITE,
    RSU_IPATCH_FTPUSER,
    RSU_IPATCH_FTPPASS,
    RSU_IPATCH_FTPPATH,
    RSU_IPATCH_WEBPORT,
    RSU_IPATCH_FTPPORT,

    /* task failure actions */
    RSU_TASK_FAIL_ABORT     = 0x00,
    RSU_TASK_FAIL_CLEAR     = 0x01,
    RSU_TASK_FAIL_CONTINUE  = 0x02,
    RSU_TASK_FAIL_RETRYONCE = 0x10,
};

/* Structs */
struct PlugEventHandleObjectInfo
{
    /* Name of the object in question */
    const char* lpszFileName;
    /* Must be set to true, if object was handled */
    bool bHandled;
};

/* Interfaces */
typedef struct IPlugUnknown_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);
}
IPlugUnknown;

/* IVersion 1 */
typedef struct IPlugCommon1_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);

    bool PLUGDEF(InterfaceRegister)(const unsigned long* lpInterfaceId, unsigned long luVersion, void* lpReserved, void* lpInterface);
    bool PLUGDEF(InterfaceQuery)(const unsigned long* lpInterfaceId, unsigned long luVersion, void* lpReserved, void** lppInterface);
    bool PLUGDEF(GetPatchInfo)(unsigned long luType, const char** lppszString);
    bool PLUGDEF(SetPatchInfo)(unsigned long luType, const char* lpszString);
    bool PLUGDEF(PushTask)(bool PLUGDEF(lpTaskFunc)(void* lpContext), void* lpContext, int nFailType);
}
IPlugCommon1;

typedef struct IPlugUI1_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);

    void PLUGDEF(SetProgress)(unsigned long luDone, unsigned long luTotal);
    bool PLUGDEF(GetStatus)(char* lpszBuf, unsigned long luSize);
    void PLUGDEF(SetStatus)(const char* lpszText);
    void PLUGDEF(SetTitle)(const char* lpszTitle);
    void* PLUGDEF(GetHandle)(void);
}
IPlugUI1;

typedef struct IPlugGRF1_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);

    void* PLUGDEF(Open)(const char* lpszFileName, bool bReadOnly);
    bool PLUGDEF(Commit)(void* hGrf);
    void PLUGDEF(Revert)(void* hGrf);
    void PLUGDEF(Close)(void* hGrf);
    bool PLUGDEF(Exists)(void* hGrf, const char* lpszFileName);
    bool PLUGDEF(Delete)(void* hGrf, const char* lpszFileName);
    bool PLUGDEF(Rename)(void* hGrf, const char* lpszOldName, const char* lpszNewName);
    bool PLUGDEF(Put)(void* hGrf, const char* lpszFileName, const unsigned char* lpPtr, unsigned long luSize);
    bool PLUGDEF(Get)(void* hGrf, const char* lpszFileName, unsigned char* lpPtr, unsigned long* luSize);
    bool PLUGDEF(Merge)(void* hGrf, void* hPatchGrf);
}
IPlugGRF1;

typedef struct IPlugRGZ1_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);

    void* PLUGDEF(Open)(const char* lpszFileName);
    bool PLUGDEF(Extract)(void* hRGZ);
    void PLUGDEF(Close)(void* hRGZ);
}
IPlugRGZ1;

typedef struct IPlugIntl1_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);

    const char* PLUGDEF(GetMsgStr)(unsigned long luId);
    bool PLUGDEF(SetMsgStr)(unsigned long luId, const char* lpszString);
}
IPlugIntl1;

typedef struct IPlugSetting1_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);

    bool PLUGDEF(Get)(const char* lpszSection, const char* lpszKey, const char* lpszDefault, char* lpszBuffer, unsigned long luBufferSize);
}
IPlugSetting1;

/* IVersion 2 */
typedef struct IPlugCommon2_
{
    void PLUGDEF(Acquire)(void* lpSelf);
    void PLUGDEF(Release)(void* lpSelf);

    bool PLUGDEF(InterfaceRegister)(const unsigned long* lpInterfaceId, unsigned long luVersion, void* lpReserved, void* lpInterface);
    bool PLUGDEF(InterfaceQuery)(const unsigned long* lpInterfaceId, unsigned long luVersion, void* lpReserved, void** lppInterface);
    bool PLUGDEF(GetPatchInfo)(unsigned long luType, const char** lppszString);
    bool PLUGDEF(SetPatchInfo)(unsigned long luType, const char* lpszString);
    bool PLUGDEF(PushTask)(bool PLUGDEF(lpTaskFunc)(void* lpContext), void* lpContext, int nFailType);
    bool PLUGDEF(GetPatchInfoNum)(unsigned long luType, unsigned long* luNumber);
    bool PLUGDEF(SetPatchInfoNum)(unsigned long luType, unsigned long luNumber);
}
IPlugCommon2;

/* Version Independent Definitions */
#define IPlugCommon     IPlugCommon2
#define IPlugUI         IPlugUI1
#define IPlugGRF        IPlugGRF1
#define IPlugRGZ        IPlugRGZ1
#define IPlugIntl       IPlugIntl1
#define IPlugSetting    IPlugSetting1

#ifdef __cplusplus
};
#endif  /* __cplusplus */

#endif /* _RSU_PLUG_H_ */

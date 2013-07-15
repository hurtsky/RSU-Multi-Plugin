// -----------------------------------------------------------------
// Multi Plug-in for Ragnarok Online Patcher Lite (codename: RSU)
// (c) 2009-2010 Ai4rei/AN
// Plugin by: Kisuka
// Allows for patching multiple RO GRF files + private server GRF using 1 patch client.
// -----------------------------------------------------------------

#include "rsu.plug.h"  // Plug SDK
#include "multi.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static PlugInterfaceQueryFunc Plug_InterfaceQuery = NULL;
static IPlugCommon* lpCommon = NULL;
static const unsigned long luMultiInterfaceId = 0x00000001;
static unsigned char PatchStep = 0;

// Does not require a reference count
static void __stdcall Plugin_P_Acquire(void* lpSelf)
{
	return;

	lpSelf;	// Unused
}

static void __stdcall Plugin_P_Release(void* lpSelf)
{
	return;

	lpSelf;	// Unused
}

// Set Patch Info for each patch step / GRF.
static bool __stdcall Set_Patch_Info(void* lpContext)
{
	switch((unsigned char)lpContext)
	{
		// Set kRO Patch Settings
		case PATCH_KRO:
			lpCommon->SetPatchInfo(RSU_IPATCH_INFFILE, "patch.inf");
			lpCommon->SetPatchInfo(RSU_IPATCH_GRFFILE, "data.grf");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBSITE, "webpatch.ragnarok.co.kr");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBPATH, "/patch");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBDENY, "patch_allow.txt");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBLIST, "patch8.txt");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPSITE, "ragnarok.nowcdn.co.kr");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPUSER, "Anonymous");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPASS, "");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPATH, "/Patch");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBPORT, "80");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPORT, "20021");
			break;
		// Set kRO:RE Patch Settings
		case PATCH_RE:
			lpCommon->SetPatchInfo(RSU_IPATCH_INFFILE, "patchRE.inf");
			lpCommon->SetPatchInfo(RSU_IPATCH_GRFFILE, "rdata.grf");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBSITE, "webpatch.ragnarok.co.kr");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBPATH, "/patch");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBDENY, "patch_allow_RE.txt");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBLIST, "patchRE3.txt");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPSITE, "ragnarok.nowcdn.co.kr");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPUSER, "Anonymous");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPASS, "");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPATH, "/Patch");
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBPORT, "80");
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPORT, "20021");
			break;
		// Set Private Server Patch Settings
		case PATCH_CUSTOM:
			// INF File to keep track of current patch (ex: PatchRRO.inf)
			lpCommon->SetPatchInfo(RSU_IPATCH_INFFILE, "");

			// GRF File where the data for this server's client is stored (ex: adata.grf)
			lpCommon->SetPatchInfo(RSU_IPATCH_GRFFILE, "");

			// HTTP Website where Patch list / allow is located
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBSITE, "");

			// Port the HTTP Website is on, usually is 80.
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBPORT, "80");

			// Directory the patch list / allow is located. Set to blank if in home. (ex: /patch)
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBPATH, "");

			// Patch allow / deny txt file (ex: patch_allow.txt)
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBDENY, "");

			// Patch list that lists of what patches to download (ex: patch.txt)
			lpCommon->SetPatchInfo(RSU_IPATCH_WEBLIST, "");

			// FTP Server where the patches are located.
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPSITE, "");

			// Port the FTP server is on, usually is 21.
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPORT, "21");

			// Directory where the patches are stored on the FTP. leave blank if in home. (ex: /patch)
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPATH, "");

			// FTP user, may need to set as Anonymous if no user is set.
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPUSER, "");

			// Password for FTP user.
			lpCommon->SetPatchInfo(RSU_IPATCH_FTPPASS, "");			
			break;

		// You had ONE job! What Happened?
		default:
			return false;
	}

	// All GREEN!
	return true;
}
static const IPlugMulti1 TblMulti1 =
{
	Plugin_P_Acquire,
	Plugin_P_Release,
};

static bool __stdcall Plugin_OnEvent(unsigned long luEvent, unsigned long luData, void* lpData)
{
	IPlugUI* lpUI;

	switch(luEvent)
	{
		// Plug-in Initialized
		case RSU_PLUG_EVENT_INITDONE:
				// Set Client EXE & Command.
				lpCommon->SetPatchInfo(RSU_IPATCH_EXEFILE, "");	// (ex: ragexe.exe)
				lpCommon->SetPatchInfo(RSU_IPATCH_EXECMDL, "");	// (ex: 1rag1)
			break;

		// Interface is Ready
		case RSU_PLUG_EVENT_UI_INITDONE:
			// query for UI interface functions
			if(Plug_InterfaceQuery(RSU_PLUG_VERSION1, RSU_PLUG_TYPE_UI, NULL, (void**)&lpUI))
			{
				// Set Status Text
				lpUI->SetStatus("Press \"Start\" to Update.");

				// Release Interface
				lpUI->Release(lpUI);
			}
			break;

		// Add Tasks to Queue
		case RSU_PLUG_EVENT_QUERYTASK:
			// Set kRO Settings
			if(PatchStep == PATCH_KRO)
			{
				if(lpCommon->PushTask(&Set_Patch_Info, (unsigned char*)PATCH_KRO, RSU_TASK_FAIL_RETRYONCE|RSU_TASK_FAIL_CONTINUE))
					PatchStep++;
			}
			// Set kRO:RE Settings
			else if(PatchStep == PATCH_RE)
			{
				if(lpCommon->PushTask(&Set_Patch_Info, (unsigned char*)PATCH_RE, RSU_TASK_FAIL_RETRYONCE|RSU_TASK_FAIL_CONTINUE))
					PatchStep++;
			}
			// Set Private Server Settings
			else if(PatchStep == PATCH_CUSTOM)
			{
				if(lpCommon->PushTask(&Set_Patch_Info, (unsigned char*)PATCH_CUSTOM, RSU_TASK_FAIL_RETRYONCE|RSU_TASK_FAIL_ABORT))
					PatchStep++;
			}
			// All Finished!
			else
			{
				break;
			}

			// Perform Patching
			lpCommon->PushTask(NULL, NULL, RSU_TASK_FAIL_RETRYONCE|RSU_TASK_FAIL_ABORT);
			break;

		// Not Processed
		default:
			return false;
	}

	return true;	// Processed

	// Unused
	lpData;
	luData;
}

void __stdcall Plugin_Quit(void)
{
	lpCommon->Release(lpCommon);
}

bool __stdcall Plugin_Init(PlugInterfaceQueryFunc lpInterfaceQuery, PlugInterfaceOnEventFunc* lppOnEvent, void* lpReserved)
{
	// store interface query function
	Plug_InterfaceQuery = lpInterfaceQuery;

	// pass notification function
	*lppOnEvent = &Plugin_OnEvent;

	// Register our own interface
	if(Plug_InterfaceQuery(RSU_PLUG_VERSION1, RSU_PLUG_TYPE_COMMON, NULL, (void**)&lpCommon))
	{
		if(!lpCommon->InterfaceRegister(&luMultiInterfaceId, 1, NULL, (void*)&TblMulti1))
		{
			;
		}
	}

	return true;

	lpReserved;	// Unused
}

bool __stdcall Plugin_Load(unsigned long luAppVersion, unsigned long* luPlugVersion)
{
	// pass plugin's version
	*luPlugVersion = RSU_PLUG_VERSION_CURRENT;

	return true;

	luAppVersion;	// Unused
}

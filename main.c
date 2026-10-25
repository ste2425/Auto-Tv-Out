/*
 * main.c - PopsAnalog
 */
#include <pspsdk.h>
#include <pspkernel.h>
#include <psploadcore.h>
#include <pspsysmem_kernel.h>
#include <string.h>
#include <systemctrl.h>
#include <cfwmacros.h>
#include <pspctrl.h>
#include "debug/debug.h"
#include "sceImpose_imports.h"
#include "mips.h"

PSP_MODULE_INFO("auto_tvout", PSP_MODULE_KERNEL, 1, 0);

int thid;
int running;

// find the methdd by offset that actually enables TVOut
// ths is what is called after the screen is held for a specific ammount of time
static u32 *find_TVEnable(SceModule *mod) 
{
    u32 *fn = (u32 *)(mod->text_addr + 0x5ac);	

	// Check the first few instructions are what we expect to be sure 
	// we have correctly found it
	if (fn[0] != MIPS_ADDIU(REG_SP, REG_SP, -16) 
		|| fn[1] != MIPS_SW(REG_RA, REG_SP, 0)
		|| fn[5] != MIPS_ORI(REG_A0, REG_ZERO, 0x8000))
		return NULL;

	return fn;
}

// Check the internal state of sceImpose to see if TVOut already enabled
// 0 is off, 2 switching and 1 enabled.
static int tv_state(SceModule *mod)
{
    u32 base  = mod->text_addr;

	// find pointer to struct that holds state from offset of data region
    u32 *pptr = (u32 *)(base + 0xA1D0);
	// cast to a byte so when we add offsets we move one byte per step
    u8  *st   = (u8 *)*pptr;

    if ((u32)st != base + 0xA220)
        return -1;

	// find the actual byte within the struct that holds the state
    return *(volatile u32 *)(st + 0x324);
}

int main_thread(SceSize args, void *argp)
{	
	// give a bit of time. 
	// tvOut wont be enabled on cold boot untill the wave animation completes anyway.
	sceKernelDelayThread(2 * 1000 * 1000);

	int model = sceKernelGetModel();
	if (model == 0)
	{
		debugLog("Not supported on a 1000");
		
		return 0;
	}

    SceModule *mod = sceKernelFindModuleByName("sceImpose_Driver");

	u32 *fn = find_TVEnable(mod);
	if (fn == NULL)
	{
		debugLog("Unable to find TVOut fn");
		return 0;
	}
	
	while (running)
	{
	    sceKernelDelayThread(1 * 1000 * 1000);

		// if 0 returned there is a AV cable connected
        int connected = sceImpose_driver_E9B0E008(NULL);

		// don't enable if there is no cable or we are already switching or enabled.
        if (connected == 0 && tv_state(mod) == 0)
		{
            ((void (*)(int))fn)(2);
        }
	}

	return sceKernelExitDeleteThread(0);
}

int module_start(SceSize args, void *argp)
{
    debugInit();

	running = 0;

	// i dont fully understand all these arguments
	// one day i may learn
	thid = sceKernelCreateThread("tvout", main_thread, 0x10, 4*1024, 0, NULL);
	if (thid >= 0)
	{
		running = 1;

		sceKernelStartThread(thid, args, argp);
	}
	
	return 0;
}

int module_stop(SceSize args, void *argp)
{
	if (running)
	{
		running = 0;
		SceUInt time = 200*1000;

		int ret = sceKernelWaitThreadEnd(thid, &time);

		if (ret < 0)
			sceKernelTerminateDeleteThread(thid);
	}
	
	return 0;
}
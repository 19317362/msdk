/** @file msplugin.h
 *  @brief ֨ҥҥܾݓߚ
 *  @author magj
 *  @version 0.01 
 *  @date 2015/01/09 
 */

#ifndef _MSPLUGIN_INTERFACE_H_
#define _MSPLUGIN_INTERFACE_H_

#include <mscom\msbase.h>

namespace msdk {;
namespace mscom {;

interface IMsPlugin : public IMSBase
{
	
	STDMETHOD(Init)(void*) = 0;
	
	
	STDMETHOD(Uninit)() = 0;
};
MS_DEFINE_IID(IMsPlugin, "{74252668-870B-4c27-B69D-C6E199C6D3D5}");



interface IMsPluginRun : public IMSBase
{
	STDMETHOD(Start)() = 0;
	
	STDMETHOD(Stop)() = 0;
};
MS_DEFINE_IID(IMsPluginRun, "{24AE3325-4FE1-4c03-8765-6C3C967D9D44}");

};
};

#endif //_MZSPLUGIN_INTERFACE_H_

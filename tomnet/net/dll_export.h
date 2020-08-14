#ifndef  TOM_NET_DLL_EXPORT_H
#define  TOM_NET_DLL_EXPORT_H

#ifdef _WIN32
    #ifdef TOM_ TOM_NET_DLL_EXPORT
        #define TOM_ TOM_NET_CLASS_API __declspec(dllexport)
    #else
        #define TOM_ TOM_NET_CLASS_API __declspec(dllimport)
    #endif 
#else
    #define TOM_ TOM_NET_CLASS_API
#endif

#ifdef __cplusplus
    #define TOM_ TOM_NET_DLL_DECL extern  TOM_ TOM_NET_CLASS_API
    #define TOM_ TOM_NET_DLL_C_DECL extern "C"  TOM_ TOM_NET_CLASS_API
#else
    #define TOM_ TOM_NET_DLL_DECL extern  TOM_ TOM_NET_CLASS_API
    #define TOM_ TOM_NET_DLL_C_DECL  TOM_ TOM_NET_DLL_DECL
#endif
 
#endif
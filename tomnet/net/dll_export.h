#ifndef  NET_DLL_EXPORT_H
#define  NET_DLL_EXPORT_H

#ifdef _WIN32
    #ifdef TOM_TOM_NET_DLL_EXPORT
        #define TOM_TOM_NET_CLASS_API __declspec(dllexport)
    #else
        #define TOM_TOM_NET_CLASS_API __declspec(dllimport)
    #endif 
#else
    #define TOM_TOM_NET_CLASS_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
    #define TOM_TOM_NET_DLL_DECL extern  TOM_TOM_NET_CLASS_API
    #define TOM_TOM_NET_DLL_C_DECL extern "C"  TOM_TOM_NET_CLASS_API
#else
    #define TOM_TOM_NET_DLL_DECL extern  TOM_TOM_NET_CLASS_API
    #define TOM_TOM_NET_DLL_C_DECL  TOM_TOM_NET_DLL_DECL
#endif
 
#endif

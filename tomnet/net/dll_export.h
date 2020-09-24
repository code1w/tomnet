#ifndef  NET_DLL_EXPORT_H
#define  NET_DLL_EXPORT_H

#ifdef _WIN32
    #ifdef TOM_TOM_NET_DLL_EXPORT
        #define TOM_TOM_NET_CLASS_API __declspec(dllexport)
    #else
        #define TOM_TOM_NET_CLASS_API __declspec(dllimport)
    #endif 
#else
    #define TOM_TOM_NET_CLASS_API
#endif

#ifdef __cplusplus
    #define TOM_TOM_NET_DLL_DECL extern  TOM_TOM_NET_CLASS_API
    #define TOM_TOM_NET_DLL_C_DECL extern "C"  TOM_TOM_NET_CLASS_API
#else
    #define TOM_TOM_NET_DLL_DECL extern  TOM_TOM_NET_CLASS_API
    #define TOM_TOM_NET_DLL_C_DECL  TOM_TOM_NET_DLL_DECL
#endif
 
#endif



#if defined(TOMNET_USE_DLLS)
#if defined(_MSC_VER)
#ifdef LIBTOMNET_EXPORTS
#define TOMNET_EXPORT __declspec(dllexport)
#else
#define TOMNET_EXPORT __declspec(dllimport)
#endif
#else  // defined(_MSC_VER)
#ifdef LIBTOMNET_EXPORTS
#define TOMNET_EXPORT __attribute__((visibility("default")))
#else
#define TOMNET_EXPORT
#endif
#endif
#else  // defined(PROTOBUF_USE_DLLS)
#define TOMNET_EXPORT
#endif
/////////////////////////////////////////////////////////////////////////////
// Name:        StorageServer.h
// Purpose:     
// Author:      Center
// Modified by: 
// Created:     
// RCS-ID:      
// Copyright:   CodeCenter
// Licence:     
/////////////////////////////////////////////////////////////////////////////
#pragma once
#ifdef StorageServer_OP_API
#else
#ifdef _DEBUG
#pragma comment(lib,"StorageServerd.lib")
#else
#pragma comment(lib,"StorageServer.lib")
#endif
#define StorageServer_OP_API extern "C" _declspec(dllimport)
#endif

// ¿ªÊ¼
StorageServer_OP_API int Stor_Start();
// Í£Ö¹
StorageServer_OP_API int Stor_Stop();

StorageServer_OP_API int Stor_Status();

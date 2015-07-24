#pragma once

#include "Memory.h"

#include <queue>

/*
	StoC Packet Handler

	Ripped from the GWLP Dumper
	Credits: ACB, _rusty, et al.
	
	This .h file created by 4D 1
	
	
	Usage:
	struct Pak : public Packet{
		// Struct def here
	};
	
	class PakClass : public PacketHandler{
	public:
		PakClass(Packet* pack){
			m_pak = *(Pak*)pack;
		}
	
		virtual bool HandlePacket();
		virtual DWORD GetHeader(){ return m_pak.Header; }
	private:
		Pak m_pak;
	};
	
	bool PakClass::HandlePacket(){
		// Handler here.
	}
*/

namespace GWAPI{

#pragma pack(1)
	struct Packet
	{
		DWORD Header;
	};
#pragma pack()

	typedef bool(__fastcall *handler)(Packet*, DWORD);

	class PacketHandler
	{
	public:
		virtual bool HandlePacket(){ return false; }
		virtual DWORD GetHeader(){ return -1; }
	};

	struct StoCPacketMetadata
	{
		DWORD* PacketTemplate;
		int TemplateSize;
		handler HandlerFunc;
	};

	class AutoMutex
	{
	public:
		AutoMutex(HANDLE mutex)
		{
			m_Mutex = mutex;
			if (WaitForSingleObject(m_Mutex, INFINITE) != WAIT_OBJECT_0)
			{
				MessageBoxA(NULL, "Unable to retrieve mutex!", "Auto Mutex", MB_ICONERROR | MB_OK);
			}
		}
		~AutoMutex()
		{
			ReleaseMutex(m_Mutex);
		}
	private:
		HANDLE m_Mutex;
	};
	class CStoCHandler{
	public:
		class Exception{
			char* msg;
			va_list vl;
			int va_len;
		public:
			const char* getMsg(){
				return msg;
			}
			Exception(const char* message, ...){
				va_start(vl, message);
				va_len = _vscprintf(message, vl);
				msg = new char[va_len + 1];
				vsprintf_s(msg, (va_len + 1) * sizeof(char), message, vl);
				va_end(vl);
			}
			void clear(){
				delete[] msg;
			}
		};
	private:
		class StoChandler{
		public:
			static HANDLE m_PacketQueueMutex;

			StoChandler();
			~StoChandler();

			StoCPacketMetadata* GetLSMetaData();
			StoCPacketMetadata* GetGSMetaData();

			handler SetGSPacket(DWORD Header, handler function);
			handler SetLSPacket(DWORD Header, handler function);

			void RestoreGSPacket(DWORD Header);
			void RestoreLSPacket(DWORD Header);

			handler GetGSHandlerFunc(DWORD Header){
				return m_OrigGSHandler[Header];
			}

			handler GetLSHandlerFunc(DWORD Header){
				return m_OrigLSHandler[Header];
			}

			DWORD GetLSCount();
			DWORD GetGSCount();

			template<class T> void AddHandler(DWORD Header, bool Gameserver);

		private:
			template<class T> static bool __fastcall LSEnqueuePacket(Packet* pack, DWORD unk);
			template<class T> static bool __fastcall GSEnqueuePacket(Packet* pack, DWORD unk);
			static void DisplayError(const char* Format, ...);
			static DWORD WINAPI ProcessPacketThread(LPVOID);

		private:
			DWORD LSPacketMetadataBase = NULL, GSPacketMetadataBase = NULL;
			StoCPacketMetadata* m_LSPacketMetadata;
			StoCPacketMetadata* m_GSPacketMetadata;
			static handler* m_OrigLSHandler;
			static handler* m_OrigGSHandler;
			static std::queue<PacketHandler*> m_PacketQueue;
			int m_LSPacketCount, m_GSPacketCount;
			//static HANDLE m_PacketQueueMutex;
			HANDLE m_PacketQueueThread;
		};

		CStoCHandler(){}
		void operator=(CStoCHandler const&);
		CStoCHandler(CStoCHandler const&);

	public:

		static StoChandler* GetInstance(){
			static StoChandler* m_stocobj = new StoChandler();
			return m_stocobj;
		}

	};

}
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
//offsets
DWORD dwGlowObjectManager = 0x529A250;
DWORD dwEntityList = 0x4D523EC;
DWORD m_iGlowIndex = 0xA438;
//entity data
DWORD entity;
DWORD glowIndex;
//get module base address by the module's name
DWORD GetModuleBaseExtern(const DWORD dwProcessId, const char* szModuleName) 
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (!hSnap) return 0;
	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);
	DWORD dwReturn = 0;
	if (Module32First(hSnap, &me)) { //iterate through every module(dll) until a matching one is found
		while (Module32Next(hSnap, &me)) {
			if (lstrcmpi(me.szModule, szModuleName) == 0) {
				dwReturn = (DWORD)me.modBaseAddr;
				break;
			}
		}
	}
	CloseHandle(hSnap);
	return dwReturn;
}
//Glow Object Struct that will be read and written
struct GlowObjectDefinition_t
{
	void* pEntity; //entity pointer
	float r; //red
	float g; //green
	float b; //blue
	float a; //alpha
	uint8_t unk1[16]; //unknown, padding
	bool m_bRenderWhenOccluded; //should render?
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom; //full bloom glow
	uint8_t unk2[10]; //padding
};
//a template to read structs from memory
template <typename var>
var Read(DWORD Address, HANDLE processHandle)
{
	var value{};
	__try
	{
		ReadProcessMemory(processHandle, (LPVOID)Address, &value, sizeof(var), 0);
	}
	__except (EXCEPTION_ACCESS_VIOLATION) {}
	return value;
}

int main()
{
	DWORD csgoProcessId = 0;

	HWND hwnd = FindWindowA(0, "Counter-Strike: Global Offensive"); //get hwnd of csgo 

	GetWindowThreadProcessId(hwnd, &csgoProcessId); //get procid of csgo
	std::cout << "CSGO PID: " << csgoProcessId << std::endl;

	DWORD clientDllBase = GetModuleBaseExtern(csgoProcessId, "client.dll"); //get base address of client.dll in memory
	std::cout << "CLIENT BASE: " << clientDllBase << std::endl;

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, csgoProcessId); //open handle 

	for (;;) { //infinite loop
		DWORD glowManager; //glowobjectmanager 
		ReadProcessMemory(processHandle, (LPCVOID)(clientDllBase + dwGlowObjectManager), &glowManager, sizeof(glowManager), NULL);

		for (int i = 1; i <= 64; i++) { //a loop to loop through every entity (max 64 players).
			ReadProcessMemory(processHandle, (LPCVOID)(clientDllBase + dwEntityList + (i * 0x10)), &entity, sizeof(entity), NULL); //read in the entity

			ReadProcessMemory(processHandle, (LPCVOID)(entity + m_iGlowIndex), &glowIndex, sizeof(glowIndex), NULL); //read the glowIndex of the entity

			GlowObjectDefinition_t glowData = Read<GlowObjectDefinition_t>(glowManager + (glowIndex * 0x38), processHandle); //read Glow object data

			//modify glow object data to set a desired color and alpha
			glowData.a = 0.7f;
			glowData.r = 0.5f;
			glowData.g = 0.5f;
			glowData.b = 1.f;

			//modify glow object data to enable glow
			glowData.m_bRenderWhenOccluded = true;
			glowData.m_bRenderWhenUnoccluded = false;
			//write glow object data back to memory
			WriteProcessMemory(processHandle, (LPVOID)(glowManager + (glowIndex * 0x38)), &glowData, sizeof(glowData), NULL); 
			Sleep(1);//sleep to save CPU power
		}
	}
}

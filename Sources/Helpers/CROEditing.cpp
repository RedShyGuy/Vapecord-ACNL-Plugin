#include "Helpers/CROEditing.hpp"
#include "Address/Address.hpp"

namespace CTRPluginFramework {  
    bool CRO::GetMemAddress(const char* croFileName, u32& buffer) {
        static const Address CroData(0x95269C);
        u32 data = *(u32 *)CroData.addr;

        u32 iVar1 = *(u32 *)(data + 0x14);

        std::string output = "";

        while((iVar1 - 0x124) != (data - 0x114)) {
            if(*(u8 *)(iVar1 - 4) == 2) {   
            //Reads .cro file name
                output.clear();
                if(Process::ReadString(*(u32 *)(iVar1 - 0x34), output, 13, StringFormat::Utf8)) {
                    if(output == croFileName) {
                        buffer = *(u32 *)(iVar1 - 0xC); //.cro mem region
                        return true;
                    }
                }
            }
            iVar1 = *(u32 *)(iVar1 + 4);
        }
        return false;
    }

    bool CRO::WritePermToggle(u32 address, bool ON) {
		Handle processHandle;
		u32 pID;
		Result res;
		bool out = false;
		MemInfo mInfo;
		PageInfo pInfo;

		svcGetProcessId(&pID, CUR_PROCESS_HANDLE);

		res = svcOpenProcess(&processHandle, pID);
		if(R_SUCCEEDED(res)) {
			res = svcQueryMemory(&mInfo, &pInfo, address);
			if(R_SUCCEEDED(res)) {
				u32 perm = ON ? MemPerm(MEMPERM_READ | MEMPERM_EXECUTE | MEMPERM_WRITE) : MemPerm(MEMPERM_READ | MEMPERM_EXECUTE);
				res = svcControlProcessMemory(processHandle, mInfo.base_addr, 0, mInfo.size, MemOp(MEMOP_PROT), perm);		

				if(R_SUCCEEDED(res)) {
                    out = true;
                }
			}
			svcCloseHandle(processHandle);
		}

		return out;
	}
}
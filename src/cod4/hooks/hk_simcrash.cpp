#include "hk_decl.h"

#include "../simulation.h"

auto global1 = (DWORD*)0x0185C2F8; // dword_185C2F8
auto global2 = (DWORD*)0x0185C2FC; // dword_185C2FC

void __fastcall hooks::simcrash1(DWORD* a1, int a2)
{
    if (simulation::simulating || a1 == nullptr)
    {
        return;
    }

    unsigned __int16 v2; // ax

    *a1 = a2;
    if (a2 == 1023 || (v2 = *(WORD*)((*global1) + a2 * (*global2) + 340)) == 0)
    {
        a1[1] = -1;
        *((BYTE*)a1 + 8) = 1;
        *((BYTE*)a1 + 11) = 1;
        *((BYTE*)a1 + 10) = 1;
        *((BYTE*)a1 + 9) = 0;
    }
    else
    {
        a1[1] = v2 - 1;
        *((BYTE*)a1 + 8) = 1;
        *((BYTE*)a1 + 11) = 1;
        *((BYTE*)a1 + 10) = 1;
        *((BYTE*)a1 + 9) = 0;
    }
}


auto global3 = (DWORD*)0xD5EC410;// dword_D5EC410
bool callOrig = true;
__declspec(noinline) void __fastcall logic2(int* a1, int a2)
{
    callOrig = !simulation::simulating;
}
__declspec(noinline) void __fastcall logic3(int retval)
{

}

int* a1; int a2;
int retval;
//int __usercall simcrash2@<eax>(int *a1@<eax>, int a2@<ecx>)
__declspec(naked) int hooks::simcrash2()
{
    __asm {
        mov a1, eax
        mov a2, ecx
    }

    LOGICCALL(logic2(a1, a2);)

    __asm push eax

    if (!!callOrig) // test eax,eax sets eax
    {
        __asm {
            pop eax
            call orig_simcrash2
        }
    }
    else
    {
        __asm {
            pop eax
            mov eax, 0
            ret
        }
    }

    //__asm mov retval, eax
    //LOGICCALL(logic3(retval);)
    __asm ret
}
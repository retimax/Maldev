## Process Injection

The most basic type of injection. Allocates a RWX memory page, writes the shellcode onto it, and executes it via a new thread.
```
  [ Current Process ]

  1. VirtualAlloc          2. WriteProcessMemory
  +-------------+          +---------------------+
  |  Allocate   |          |  Write shellcode     |
  |  RWX page   | -------> |  onto allocated      |
  |  in memory  |          |  memory page         |
  +-------------+          +----------+----------+
                                      |
                                      v
                           +---------------------+
                           |    Memory Page      |
                           |  +---------------+  |
                           |  | \xfc\xe8...   |  | <-- shellcode
                           |  | (shellcode)   |  |
                           |  +---------------+  |
                           |   RWX permissions   |
                           +----------+----------+
                                      |
                                      v
  5. CloseHandle           3. CreateThread
  +-------------+          +---------------------+
  |   Close     |          |  New thread points   |
  |   hThread   | <------- |  to shellcode addr   |
  +-------------+          +----------+----------+
         ^                            |
         |                            v
  4. WaitForSingleObject   +---------------------+
  +-------------+          |  Thread executes     |
  |  Wait until | <------- |  shellcode           |
  |  thread ends|          |  [!] PAYLOAD RUNS    |
  +-------------+          +---------------------+

  Detection Indicators (IoCs)
  ---------------------------
  [!] RWX memory allocation        (very noisy)
  [!] WriteProcessMemory on self    (suspicious)
  [!] CreateThread on alloc'd mem   (flagged by EDRs)
```

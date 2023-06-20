# DLL-HookLoader
A simple hookloader, that you can use, if you edit the code to suit your needs.

# Usage
The HookDLLToWindow function loads the DLL, retrieves the address of the hook procedure, and installs the hook using the SetWindowsHookEx function. It then enters a message loop until the window is closed, processing the window messages and calling the hook procedure. Once the window is closed, it unhooks the hook procedure and releases the DLL.

# Quick Note
<ul>
  <li>Please note that you need to replace "Window Title" with the actual title of the window you want to hook and "path/to/your/dll.dll" with the path to your DLL.</li>
  <li>Make sure to compile the code as a DLL, and ensure that the hook procedure is exported from the DLL.</li>
  <li>If you want to compile the code as an EXE, then you must add the 'main()' function.</li>
</ul>

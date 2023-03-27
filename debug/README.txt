RemedyBG : a user-mode 64-bit Windows debugger

Contact Information
-=-=-=-=-=-=-=-=-=-
George Menhorn
remedybg@protonmail.com
https://remedybg.handmade.network/
https://remedybg.itch.io/remedybg
https://github.com/x13pixels/remedybg-issues/issues
https://discord.gg/hxWxDee : handmade.network Discord server (channel #remedybg)

Acknowledgments
-=-=-=-=-=-=-=-=
Thank you for purchasing a copy of RemedyBG and supporting its continued development.

Also, thanks to Visual Studio for providing the motivation, in the form of aggravation and want, for
making we want to write a debugger from scratch in the first place.

A big thanks to Casey Muratori for his inspiration, suggestions, and the rekindling of the handmade
movement!

Thanks to Simon Anciaux for the extraordinarily detailed feedback on pretty much every release that
I've put out to date along with many, many others that have provided excellent feedback and feature
suggestions.

I welcome any and all bug reports, feature requests, praise, and/or curses (though please refrain
from using any voodoo). Also, feel free to use the Github issue tracker, listed above, to report
any problems you might bump into.

Attribution
-=-=-=-=-=-
RemedyBG would not be possible without the following pieces of software:

 * Dear ImGui: https://github.com/ocornut/imgui, docking branch 7b53551032
 * Omar's mini memory editor for ImGui: https://github.com/ocornut/imgui_club/tree/master/imgui_memory_editor
 * Intel XED: https://github.com/intelxed
 * Heavily modified ImGuiColorTextEdit from https://github.com/BalazsJako/ImGuiColorTextEdit used as
 a starting point for the text view.

Getting started
-=-=-=-=-=-=-=-
Once RemedyBG is launched, you can setup a target for debugging in the field named
"Command" under the Application and Parameters section in the Session menu. Type in the
name of the executable or click the "..." button to select an executable from the file
browser. Once selected, you can begin debugging by choosing Step Over (F10) or Step In
(F11) from the Control menu.

If, at this point, the disassembly window opens rather than source code (and you weren't expecting
this), then the binary may not have been built with debug information. Check with your compiler's
manual on how to produce necessary debug information, or symbol files.

Note that RemedyBG does not support loading symbols from binaries created with MSVC's
/DEBUG:fastlink switch which stored debug information in individual object files rather than placing
them in a single PDB file. In the compiler shipped in Visual Studio 2017 (15.0) and later specifying
/DEBUG will default to /DEBUG:fastlink. This will need to be changed to /DEBUG:FULL.

RemedyBG can unexpectedly switch to the debugger pane when stepping through code if the /JMC
compiler switch is used. When /JMC is enabled, the compiler inserts calls to a helper function in
function prologs. This will cause a step over operation to halt in the prolog. Since the prolog
doesn't have any associated line information, RemedyBG will switch to the disassembly pane. Use of
this compiler option will be supported or at least handled more gracefully in a future release.

Note that RemedyBG will first try to load the PDB that is specified in the binary's header (PE32+).
If this cannot be found, then RemedyBG will make a second attempt to load the PDB file in same
directory as the binary.

Some example invocations for generating symbols with MSVC:

   cl /Z7 test.c
   cl /Zi additional-compiler-switches /link /DEBUG:FULL additional-link-switches

Sessions
-=-=-=-=
   A session in RemedyBG is the collection of one or more configurations along with any opened
   source files, breakpoints, and watch window expressions. A session can be stored and loaded from
   disk (stored with the .rdbg extension).

   The target process, or simply target, is the process being debugged by RemedyBG.

   A configuration is defined to be the set of arguments required for creating the target process
   and for the defining how the initial step will behave. This includes,

      Command: the executable to debug

      Command Arguments: the arguments passed to the executable to debug

      Working Directory: the working, or current, directory to use for the target process. If this
      is not specified, then the current directory in which RemedyBG was launched will be used
      instead.

      Step halts new instance at: this is used to change the behavior of where the debugger will
      halt when starting the application with Step Over (F10) or Step In (F11). In most cases,
      you'll want to set this to "Nominal entry point". This will halt execution at WinMain, main,
      wWinMain, wmain, WinMainCRTStartup, or wWinMainCRTStartup, if one of these functions exists in
      the executable. If not, or the other option "Entry point function" is selected, execution will
      halt at the entry point that is specified by the binary (in the PE32+ header).

      Environment: set environment variables used by the process. Variables are parsed
      from the given input as VARNAME=VARVALUE, one per line. Lines that are not in this
      form are ignored. If "Inherit from parent" is selected, the parent's environment variables
      will be used by target process. Note that the parent process will be remedybg.exe in this
      case.

      (An aside: the debug heap)
      --------------------------
      Sadly, the Windows operating system will enable the debug heap by default when
      creating a process for debugging. The debug heap can significantly slow down an
      application (e.g., seen during OpenGL initialization). Because of this, RemedyBG
      will disable the debug heap automatically for you. This is done by setting
      _NO_DEBUG_HEAP=1 in the initial environment used for the target process if it isn't
      already specified somewhere.

      To opt in to using the debug heap when using RemedyBG, add the environment variable
      _NO_DEBUG_HEAP=0 to the system environment or by adding an environment variable in this
      section.

      Prior to 0.3.7.0, if you found that your application was crashing only when running under
      RemedyBG, and not when running standalone or under Visual Studio, then the debug heap being
      automatically enabled by Windows might explain the reason why. Now that this behavior will be
      opt-in, RemedyBG, too, will hide any potential issues with heap usage.

   Multiple configurations can be created that share the same breakpoints and watch expressions but
   with different commands, command arguments, working directory, and environment. This can be used
   to quickly switch between a debug and release build of an application, for instance, without
   having to set up separate sessions.

   To create a new configuration, click on the '+' icon under Application and Parameters in the
   Session menu. This will create a copy of the existing configuration that can be used as the basis
   for a new configuration. You can then cycle between configurations using the '<' or '>' buttons
   in the menu or by using the hotkey Ctrl+Shift+1, ..., Ctrl+Shift+9.  Configurations can be also
   be deleted from this menu using the 'X' button.

Text Windows
-=-=-=-=-=-=
   Ctrl+F will initiate a search in the current file. If the entered text contains all lower-case
   characters, then the search is case-insensitive. Otherwise, the search is case-sensitive. F3 can
   be used to repeat the last search (forward) or Shift+F3 (reverse). Any text selected in the
   active window will be used to initialize the search pattern. Regular expression support is not
   currently implemented.

Control Menu
-=-=-=-=-=-=
   Source Mode: If a Step Into operation enters a function that does not contain symbols and Source
   Mode is enabled, then RemedyBG will automatically step back out to the calling function.  Without
   this enabled, stepping into a function without symbols will transition to the disassembly window.

Watch Window
-=-=-=-=-=-=
   Registers
   ---------
   The watch window can parse register names such as RAX, RIP, EAX, AL, XMM0, YMM4, and so forth.
   The register names are case insensitive (e.g., Rax or rax work as well). Note that variables in
   scope take precedence over a register name. So, for example, if you had a local variable name
   rsp, then the watch window will show that variable's value and not the register name. To override
   any variable shadowing, you can prefix the variable name with a '$' (e.g., $RAX).

   Relative Scopes
   ---------------
   A watch window expression can indicate the relative scope for which to perform a variable lookup.
   The syntax for this uses a prefix of the form "{n}" or "{^n}" where n >= 0. For n = 0, this is
   the normal lookup at the current scope. For n > 0, the resolution of an identifier is done 'n'
   frames above the current frame. In addition, you can use "{*}" or "{^*}" to search up the stack
   to the first frame that properly resolves an identifier. Some examples:

       {1}foo          # Evaluate 'foo' in the parent frame
       {*}(char*)&foo  # Evaluate expression in first ancestor containing the identifer 'foo'

   Locking Scope
   -------------
   For an expression in the watch window you can toggle the lock icon. Locking an expression will
   maintain the scope of expression.

   Format Specifiers
   -----------------
   A format specifier, like the name implies, can alter the formatting, or display, of the value of
   an expression in the watch window. The syntax for format specifiers starts with a comma after the
   end of an expression followed by the format specifier itself.

   Note that the context menu for an expression includes a "Format As" menu item which shows a list
   of format specifiers valid for the current selection. This can be helpful for remembering a
   particular format specifier.

   As expressions using the "str", "wstr", inline-memory, or table format specifiers can contain
   large amounts of data, these can be detached from the watch window into their own, separate
   window by clicking on the zoom icon that will appear when one of these expressions is selected.

   Following is a list of valid format specifiers:

   -- b : Using the 'b' format specifier will cause the output of any values to be in binary,
   rather than decimal.

       var, b           # Display 'var' and any children in binary

   -- d: Using the 'd' format specifier will cause the output of any values to be in decimal (the
   default). This can be used as an override in the case that a watch window is set to display all
   values in hexadecimal, for instance.

   -- hr: The 'hr' format specifier will append an HRESULT or Win32 error code message

       0x80004003, hr   # Displays "0x80004003: Invalid pointer"
       $err, hr         # Displays the HRESULT or Win32 error code of the last error

   -- x or X : Using the 'x' format specifier will cause the output of any values to be in hexadecimal,
   rather than decimal. Note that this format specifier takes precedence over the specifier for
   number of elements. Similarly, using 'X' will display upper-case hexadecimal values where
   appropriate.

       var, x           # Display 'var' and any children in lower-case hexadecimal
       var, X           # Display 'var' and any children in upper-case hexadecimal

   -- <num elements>: This is for specifying the number of elements to display in an array or pointer
   type. The value can be an integer literal or an integer/enum value that can be resolved in the
   current scope. The <num elements> expression can be used in conjunction with another format
   specifier.

       arr, 16            # Display 16 values of the expression 'arr'. 
       arr, count         # Display 'count' values of the expression 'arr'. 
       arr, X count       # Display 'count' values of the expression 'arr' in upper-case hexadecimal

   -- str or wstr : Using the 'str' or 'wstr' format specifier will show an inline, multiline string
   view in the watch window. This format specifier can be followed with a value to indicate the
   number of characters to display. The value can be an integer literal or an integer/enum value
   that can be resolved in the current scope. Use 'wstr' for wide character strings.

      ShaderText, str             # Displays the contents of 'ShaderText' in an inline string view
      text.Data, str text.Count   # Displays 'text.Count' characters of 'text.Data' in an inline string view

   -- _ : A no-op format specifier that can be used to disambiguate between a format specifier and a
   local variable used as a <num elements> expression.

      arr, x              # Display array 'arr' in lower-case hexadecimal
      arr, _ x            # Display 'x' values of the expression 'arr'
      arr, x x            # Display 'x' values of the expression 'arr' in lower-case hexadecimal

   -- vector register/type format specifiers: these apply to any vector register (xmm1, ymm2, etc.)
   as well as any variable that is 16, 32, or 64 bytes in size.

      (xmmN|ymmN|zmmN), ps                                # Display vector register/type as floats
      (xmmN|ymmN|zmmN), pd                                # Display vector register/type as doubles

      (xmmN|ymmN|zmmN), (epi8|epi16|epi32|epi64)          # Display vector register/type as signed values
      (xmmN|ymmN|zmmN), (epu8|epu16|epu32|epu64)          # Display vector register/type as unsigned values
      (xmmN|ymmN|zmmN), (epu8x|epu16x|epu32x|epu64x)      # Display vector register/type as unsigned
                                                            values in lower-case hexadecimal
      (xmmN|ymmN|zmmN), (epu8X|epu16X|epu32X|epu64X)      # Display vector register/type as unsigned
                                                            values in upper-case hexadecimal

   -- inline-memory format specifiers. These take the form:

         address_expr, <format specifier> [/cColumnCount] [(/nNumObjects|ending_address_expr)]

      format specifiers: (note that the names of these specifiers were borrowed from WinDBG)

         db  : display byte values along with ASCII characters
         dc  : double-word values and ASCII characters
         dd  : double-word values
         dD  : double-precision floating-point numbers
         df  : single-precision floating-point numbers
         dq  : quad-word values
         dW  : word values and ASCII characters
         dw  : word values
         dyb : binary values and byte values
         dyd : binary values and double-word values

         /c  : used to specify the number of columns (e.g., /c16). If not specified, then a suitable
               default for the format specifier is used.

         /n  : used to specify the number of objects (bytes, words, double-words, quad-words, floats, or
               doubles); e.g, /n12. Cannot be used if ending_address_expr is given.

         ending_address_expr : any expression that can be evaluated to an address. Cannot be used at
               the same time as the '/n' option.


   -- matrix format specifiers. Note that you can change between row major (the default) and column
   major by right-clicking in the watch window and choosing "Column Major Display". These take the form:

          address_expr, <format specifier>

      format specifiers:

         mtx        : display 4 x 4 matrix
         mtx{m}x{n} : display m x n matrix, where m, n \in [1, 32]

   -- table format specifier for arrays. This specifier can be used to construct a table
   of values, with one or more columns, where the values in each column are described by
   an arbitrary expression typically ranging over the elements of the array. The formatter
   can be used after any array expression in a watch window. The format specifier uses the
   following structure:

      { [@column-name] expression (; [@column-name] expression)* }

   with a list of one or more expressions, wrapped in braces and semicolon separated, one
   expression per column. An optional column name can be used before each expression. If
   not specified, the column name displayed will be the expression itself.

   Within this context there are two additional pseudo-variables that can be used: $, which
   evaluates to whatever the current array element is for a particular row and __index, which
   evaluates to the current row in the table. So on row 5, say, the pseudo variable $ will evaluate
   to the sixth array element and __index evaluates to, simply, 5.

      # Create a three-column table display the width and height of each box in an array of boxes
      boxes, box_count { @idx __index; @width $.max.x - $.min.x; @height $.max.y - $.min.y }

   Expressions can be drawn from any variable in scope (not just variable that is being
   iterated over). Also, the array elements themselves are not required to be referenced.
   As an example,

      # Create a table of squares, displayed in hexadecimal with the second column name
      # explicitly specified as "sq".
      (int *)0, 10 { __index; @sq __index * __index, x }

   -- disassembly format specifier. This specifier will disassemble 'num_bytes_expr' bytes of
   instructions starting at 'address_expr'. The 'address_expr' can be any expression that resolves
   to an address. The 'num_bytes_expr' expression is any expression that resolves to an integral
   value. It is optional and defaults to 128 bytes if not specified.

          address_expr, disasm [num_bytes_expr]

   Anonymous Namespaces
   --------------------
   You can use the prefix `anonymous namespace':: to watch values within an anonymous namespace.

   Pseudo Variables
   ----------------
   -- $err (unsigned int): the per-thread value of the last error set by SetLastError
   -- $handles (unsigned int): the number of handles allocated by the process
   -- $tid (unsigned int): the thread ID of the current thread
   -- $pid (unsigned int): the process ID

   Synthetic Structures
   --------------------
   Synthetic structures are identifiers that can be used in the watch window that aren't actual
   types in the target being debugged per se but are, instead, synthetized from the current state of
   the target. Valid synthetic structures are as follows:

   -- __locals : Displays the list of local variables starting from the innermost scope out to
   function scope.  Note that __locals can be combined with a relative scope operator to view local
   variables in parent scopes.

      __locals       # Displays local variables in the current scope
      {^1}__locals   # Display local variables in the parent scope

   -- __params : Displays the list of function parameters. Note that __params can be combined with
   a relative scope operator to view function parameters in parent scopes.

      __params       # Displays the current function's parameters
      {^1}__params   # Displays the parent function's parameters

   Global Variables
   ----------------
   If global variables in two or more translation units have the same name, RemedyBG will display
   all of them using the synthetic fields __g0, __g1, and so on.

Breakpoints
-=-=-=-=-=-=
   Processor Breakpoints (or Data Breakpoints as they are known in some circles)
   ---------------------
   A new processor breakpoint can be added in the breakpoints windows. Right-click anywhere in the
   window and choose "New Processor Breakpoint..." (Ctrl+P). Processor breakpoints cannot be added
   while the target is halted. Also, any processor breakpoints added will automatically be disabled
   when the target is restarted.

   Note that Intel processors support up to 4 processor breakpoints per thread. At the moment,
   RemedyBG does not take advantage of per-thread processor breakpoints and limits the total number
   of enabled processor breakpoints to 4.
   
   Conditional Breakpoints 
   -----------------------
   Conditions on any type of breakpoint can be added by right-clicking on a breakpoint (or
   right-clicking a breakpoint icon in a text window) and choosing "Edit". The condition can be any
   valid expression that can be evaluated to a boolean.


Keyboard Shortcuts
-=-=-=-=--=--=-=-=

   Ctrl+[0-9]           Switch to a saved layout

   Ctrl+Tab             Switch active text window
   Ctrl+L, Ctrl+B       Display the breakpoints window
   Ctrl+L, Ctrl+C       Display the call stack window
   Ctrl+L, Ctrl+D       Display the disassembly window
   Ctrl+L, Ctrl+M       Display the first memory window
   Ctrl+L, Ctrl+O       Display the output window
   Ctrl+L, Ctrl+R       Display the registers window
   Ctrl+L, Ctrl+T       Display the threads window
   Ctrl+L, Ctrl+U       Display the modules window; cycles through open modules windows
   Ctrl+L, Ctrl+K       Display the first watch window; cycles through open watch windows.
   Esc                  If disassembly window is in focus, then Esc returns focus to topmost text window.
                        Otherwise, returns focus to either disassembly or text window, whichever one
                        was in focus last.

   Ctrl+B               Add function breakpoint
   Ctrl+Shift+X         Break execution.
   F12                  Break execution when the application being debugged is in focus. This hotkey
                        allows you to break execution without having to change back to RemedyBG
                        first. Note that this is a Windows system-wide hotkey and can be changed by
                        modifying the value of the following registry key:
                           HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug; UserDebuggerHotKey (REG_DWORD)
                        See Microsoft's documentation for further details.
   F5                   Start debugging / continue execution
   Shift+F5             Stop debugging
   Ctrl+Shift+F5        Restart
   Shift+F6             Attach to process (when no target)
   Shift+F6             Detach (if target is executing)
   Ctrl+Shift+F6        Reattach to process
   F9                   Toggle breakpoint in text window
   Ctrl+F9              Toggle breakpoint enabled in text window
   F10                  Step Over (Line); when disassembly window is active, this behaves as Step Over (Instruction)
   Ctrl+F10             Run to Cursor (can be used even when the target has not been launched)
   Ctrl+Shift+F10       Set Next Statement
   F11                  Step Into (Line); when disassembly window is active, this behaves as Step In (Instruction)

   Ctrl+G               Brings up the Go To Line popup for the active text window
   Ctrl+F               Brings up the Find in File popup for active text window
   F3                   Repeat last search (forward)
   Shift+F3             Repeat last search (reverse)
   Ctrl+W               Close current source file

   Ctrl+O               Open source file
   Ctrl+Shift+O         Open session
   Ctrl+S               Save session
   Ctrl+Shift+[1-9]     Select active configuration

   Ctrl+A               Select All

   PgDown               Scroll down
   PgUp                 Scroll up
   Home                 Scroll to top
   End                  Scroll to bottom

   -- Breakpoints window
   Ctrl+C               Copy information on selected row(s) to clipboard
   Del                  Delete currently selected breakpoint(s)
   Enter                Go to location of breakpoint in text window
   Alt+Enter            Edit the selected breakpoint
   Space                Toggle breakpoint(s) enabled

   -- Callstack window
   Enter                Set the current frame
   Ctrl+Up              Change to the next frame
   Ctrl+Down            Change to the previous frame

   -- Output window
   Ctrl+C               Copy contents of output window to clipboard

   -- Threads window
   Ctrl+C               Copy information on selected row(s) to clipboard
   Enter                Set the active thread

   -- Watch window
   Ctrl+Up              Move watch up
   Ctrl+Down            Move watch down
   F2,Shift+Enter       Edit watch expression
   Enter                Toggle fold state between expanded and collapsed

Command line Usage
-=-=-=-=-=-=-=-=-=
 remedybg.exe [-g] [-q] exe-filename [exe-arguments...]
 remedybg.exe [-g] [-q] rdbg-filename

 Executing remedybg.exe without any command-line parameters will launch RemedyBG with an unnamed
 session. See Sessions section below to learn how to fill in the necessary details for
 launching an executable.

 exe-filename: Specify an executable to debug. This will auto-populate the session's
 "Command". RemedyBG will automatically append the ".exe" extension, if not specified.

 rdbg-filename: Specify an session file (.rdbg) to load. See Sessions section below for
 details on what is stored in this file.

 -g: Launch the target application on startup (equivalent to Control > Start Debugging)
 -q: Exit RemedyBG after the target application terminates given that the target was
 never suspended or restarted (breakpoint hit, exception raised, etc.) and the target application
 exits with exit code zero.

 Storage
 -------
 Files for storing UI state, and so forth, are located in the current user's application data
 directory.  You can get to this directory in Windows explorer by typing "%APPDATA%\remedybg". In
 the case that the smoke hits the fan and you want to reset RemedyBG to its default state, you can
 delete the files in that directory (or simply delete the entire remedybg folder).

 If you prefer your settings to be stored in an alternate location, you can start RemedyBG with
 the "--settings" parameter and specify the alternate folder. The option should come before any
 session file or exe To be loaded. For example:

 remedybg --settings C:\your\settings\folder
 remedybg --settings C:\your\settings\folder [-g] [-q] exe-filename [exe-arguments...]/exename/
 remedybg --settings C:\your\settings\folder [-g] [-q] rdbg-filename

 Driving an existing instance
 ----------------------------
 An existing instance of RemedyBG can be driven via the command-line. Following are the
 currently supported commands and their arguments. Optional arguments are enclosed in
 brackets '[]'.

 Note that filenames passed in this fashion should, at the moment anyway, be absolute and
 not relative.

   remedybg.exe open-session session-filename
   remedybg.exe open-file filename [line-number]
   remedybg.exe close-file filename
   remedybg.exe start-debugging [1] ; break at main if '1' specified (defaults to 0)
   remedybg.exe stop-debugging
   remedybg.exe attach-to-process-by-id process-id [--continue]
   remedybg.exe continue-execution
   remedybg.exe add-breakpoint-at-file filename line-number
   remedybg.exe remove-breakpoint-at-file filename line-number
   remedybg.exe add-breakpoint-at-function function-name [condition expression]
   remedybg.exe remove-breakpoint-at-function function-name
   remedybg.exe remove-all-breakpoints
   remedybg.exe run-to-cursor filename line-number

 Alternatively, RemedyBG can be driven via a named pipes implementation. The API and sample code for
 doing so can be found at https://gist.github.com/x13pixels/d1f0f99b108f34b6c461be9f9249e8e8

 JIT debugging
 -------------
 RemedyBG can be setup as the just-in-time (JIT) debugger to be invoked whenever an unhandled
 exception occurs in the system. The following registry keys will need to be set under
 HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug

   Auto (REG_SZ): either "0" or "1"
   Debugger (REG_SZ): {full-path-to}\remedybg.exe attach-to-process-by-id %ld --jit-event-handle %ld

 If Auto is set to 0, a confirmation message box will be displayed. Otherwise, RemedyBG will be
 launched immediately.

Changelog
-=-=-=--=

Version 0.3.8.7
---------------
SHA256: 6b64bd4b7b5d785d9c6c4405d94a9dcae8f550b0b30dfa9654a4614cf18c60a7
New Features
* Adds "Relative Address" column to call stack window which displays the address relative to the
  module's base address.

Bug Fixes
* Fixed a case where new session expressions were not being added when starting from the
  command line.
* Variables displayed using vector register formatters had their 128-bit lanes in reverse order.
  This has been fixed.
* Fixed a bug where function parameters stored relative to RBP and greater than 8 bytes were not
  being properly dereferenced.
* Fixed implementation of RDBG_DEBUG_EVENT_KIND_TARGET_{DETACH,CONTINUED} to match documentation
  (was not sending process_id).
* Fixed handling of call stack parameters emitted by JAI.

Version 0.3.8.6
---------------
SHA256: acc76bb5775be8f4798f07d25ddefcb6b12a9b634fe1538bfea89a889d1c3c2d
New Features
* Added the MXCSR control and status register to the registers pane.
* Added a new setting to automatically include a list of expressions in a new debugging session.
* Added new events to the named pipe interface for detecting when a debug session starts, is
  attached, or detached.
* Added commands to the named pipe interface for getting and setting the RemedyBG window position.
* Added the "--jit-event-handle" switch to the "attach-to-process-by-id" command, which allows
  setting up RemedyBG as the Just-in-Time debugger.

Improvements
* Updated the register values in the watch window to use the appropriate data type (e.g., EAX is now
  a 32-bit unsigned integer rather than a 64-bit unsigned integer).
* Modified the border color of popped out string/disassembly watch expressions in the user interface.

Bug Fixes
* Fixed selection of case-sensitive or case-insensitive search when special characters are included
  in the pattern.
* Corrected an issue where using the keyboard to navigate a menu would also affect the selection in
  the underlying window.
* Corrected an issue where the hit-rect for items in the call stack was not being calculated
  correctly if only the "Function" column was displayed.

Version 0.3.8.5
---------------
SHA256: 600f75ace6f520e81fdcc84b0d32ee7ecdd4776368e118707e0ff9316ab78d7a
New Feature
* Added a Control | Source Mode option to avoid changing to disassembly view when no symbols for a
function are available.

Improvements
* Fixed consistency issues with placement of trailing "..." in menus.
* Moves Hexadecimal / Column Major into their own group in watch window context-menu to clarify that
  these options are applied globally to the window.
* Update menu item hotkeys to reflect state of disassembly window override.
* Search (Ctrl+F, F3, and Shift+F3) enabled for disassembly window.

Bug Fixes
* Fixed a bug reading PDB files larger than 2 GB.
* Fixed a problem where a watch expression couldn't be immediately edited after using hotkey Ctrl+L,
  Ctrl+K to change to the watch window.

Version 0.3.8.4
---------------
SHA256: 0333ef8c2daad40842f0ebb1953c842912bd7e8613ff591860f8cf6d10b68a1f
New Features
 * Added the ",disasm" format specifier for disassembling instructions given an address and number
 of bytes to disassemble.
 * Added RDBG_DEBUG_EVENT_KIND_SOURCE_LOCATION_CHANGED named pipe event. This is triggered whenever
 the source location is changed in RemedyBG: via command-line, the named pipe, breakpoint
 selected, current frame changed, active thread changed, breakpoint hit, exception hit, step
 in/out/over, or a debug break.

Improvements
 * Added RDBG_COMMAND_GET_BREAKPOINT to retrieve information about a breakpoint given its ID.
   Previously, one would have to use RDBG_COMMAND_GET_BREAKPOINTS and walk the list of breakpoints.
 * Added common controls manifest for 21st century native dialog boxes.

Bug Fixes
 * Fixed potential crash handling PDBs with large page sizes.
 * Driver command RDBG_COMMAND_ADD_WATCH now returns 4, rather than 2, bytes as was documented.
 * Removed inadvertent 'enable' in RDBG_DEBUG_EVENT_KIND_BREAKPOINT_MODIFIED documentation.
 * Fixed a case where detaching from a running process could fail.
 * Fixed unintentional display of the byte order mark in text window.

Version 0.3.8.3
---------------
SHA256: d72f6fa313c5cde9f9eaeba563bb1eda1f4ecdc1391c8641ae33bf8690abc632
Bug Fix
 * Fixed crash introduced in 0.3.8.2 mainly affecting code compiled with
   clang-cl.

Version 0.3.8.2
---------------
SHA256: 132a1be0816692386544fe9f6ba9969385c3db92ee7fe5f3c9390a7a1d3a5802
New Feature
 * Added "Break When Value Changes" context-menu entry for watch expressions.

Improvements
 * Exposed named piped events for breakpoints (added, modified, and removed).

Bug Fixes
 * Fixed bug where the verbosity setting of "Detailed" in callstack would not
   get restored upon restarting.
 * Fixed a reported crash when column expressions are maximized.

Version 0.3.8.1
---------------
SHA256: 5c5cbb4e0db8db1b28176e839bfaa8b9748b64d578cc719e97969ed9d99210b8
Improvements
 * Adds support for binary constants in expression parser (e.g., "0b10010001")
 * Now render binary values with leading "0b" so they can be copied directly into a new watch
   expression.
 * Now allow optional digit separators (', _, or `) for constants in watch window expressions (e.g.,
   "0b1111_1010" or "0xABCD'1234").
 * Matrix formatter now allows arbitrary dimensions (e.g., ",mtx3x5").
 * Adds "Name" to a session configuration. This name is now displayed in the title bar.

Bug Fixes
 * Fixed a problem where maximized window weren't being restored at startup.
 * Fixed crash on startup when running on Windows 7.
 * Fixed regression in waking the msg loop when receiving a Windows message from a second RemedyBG
   instance.
 * Fixed problems constructing subexpressions for "Add Watch", including the case where an
   expression is dereferenced and requires parenthesis to properly access fields.
 * Fixed a problem when the ",hr" formatter is used on a character array.

Version 0.3.8.0
---------------
SHA256: b7674b4e37d29126c851f68144cec56df2cd4c6825fb2c0e239e27143c254265
New features
 * Added a way to drive RemedyBG and receive notifications via named pipes.
 * Added support for 8K, 16K, and 32K PDB page sizes.
 * Thread names set programmatically via an exception or with SetThreadDescription are now displayed.
 * Added an explicit autoscroll option to Output window.

Improvements
 * The watch window input box now spans the width of the watch window rather than the name column.
 * Increased the maximum length of watch window expressions.
 * YMM registers are now rendered in a single row of a watch window.
 * Focus is now preserved on the text pane after a text window is closed.
 * The working directory of a configuration when started from the command line is now the current
   directory rather than the executable's directory.
 * The window is now restored whenever a breakpoint is hit, or any event that would bring RemedyBG
   to the foreground, and the window is minimized.
 * A breakpoint's line number can now be edited.

Bug Fixes
 * The most derived field is now used when looking up a field in expression. Fixes the expectation
   for using the most derived structure when there are two or more fields with identical names.
 * Fixed problem where two or more column expressions could render into to the same window.
 * Fixed reported problem with JAI's Context type where the type could be resolved as a Win32 type
   instead due to a case insensitive lookup.
 * Fixed crash caused when using a watch expression with relative scope when the stack overflows
   (e.g., `{*}game_state`).
 * Fixed a bug where stale, cached pages of memory were being read before being refreshed in certain
   cases.
 * Better fix for off-screen window at startup.
 * Fixed case where line info subsections were not properly aligned at 4 bytes boundaries.
 * Now look past any modifiers of an enum's underlying type (const/volatile).
 * Fixed a bug that could cause invalid expressions in the memory window when using "Send to Memory
   Window".
 * The Callstack pane would not respond to mouse events if the Module column was not displayed. This
   has been fixed.
 * Fixed bug where "--settings" couldn't be used with other options.
 * Fixed a case where detaching from process could leave the debugger in a bad state.
 * Fixed handling of Copy/Copy Value in watch window for summaries in struct/union types.

Version 0.3.7.1
---------------
SHA256: 1bf70c7b787042e24c4e5826bf63888a948a3bc1810e5eb62f053f935146ba55
Improvements
 * Added context menu items for enabling/disabling breakpoints in a text window
 * Now specify initial size of zoomed watch expressions to avoid super small/large windows
 * Moved icon for zoomable watch expressions to the applicable row. Also, the expression and value
 will now remain in the watch window while zoomed.

Bug Fixes
 * Fixed evaluation of sizeof(T *) when T is a non-primitive type
 * Fixed array evaluation that was inadvertently broke in 0.3.7.0
 * Fixed case where _NO_DEBUG_HEAP environment variable wasn't being added when expected

Version 0.3.7.0
---------------
SHA256: 0464f100edef188483be13f407c7c2d1e86b7492ddca30ba915a41a06abe07e8
New Features
 * Added a new type of format specifier for formatting arrays of structures
 * Allow lifting string, memory, and table expressions into their own, independent
 windows. A zoom button on the "Value" header will appear in a watch window when one of
 these types of expressions is selected. Closing the expression's window will result in
 that expression being returned to the watch window (rather than deleting it altogether).

Improvements
 * Added Ctrl+C / Copy menu item to allow copying the call stack to the clipboard
 * Automatically disable Windows debug heap.
 * Normally, RemedyBG will automatically reload a file when a change is detected. However,
 there are reported cases where this detection does not work as expected (e.g., a mounted
 WSL filesystem). Added a Source File | Reload menu item to force reloading a source file
 from disk.

Bug Fixes
 * Now ensure the restored window is visible on screen (e.g., shutting down on one monitor
 and reopening on a smaller monitor with smaller dimensions)
 * Fixed computation of array element addresses for non-int sized enums.
 * Fixes detection of asan shadow memory when exception occurs in a DLL.

Version 0.3.6.4
---------------
SHA256: e974c25aae44887f3276342238f54e8e163c4cd9583af02e4647632ea76c7373 
Bug Fix
 * Fixed a timing issue that caused a failure to terminate the target application in certain cases

Version 0.3.6.3
---------------
SHA256: c24ead4c1142f437bc559c877b6876d9cdd19f97ceffd01f451a86aaa934fd90
New Features
 * Added "-g" and "-q" command-line arguments which can be used to automatically launch target application and terminate
 RemedyBG. @matias^
 * Added remove-all-breakpoints to command-line driver. @cancel

Improvements
 * Replaced Ctrl+Alt+* keyboard shortcuts to eliminate problems with international keyboard layouts.
 Ctrl+Alt+[0-9], used to switch saved layouts, is now simply Ctrl+[0-9]. Replaced keyboard shortcuts
 for changing window focus to use chorded "Ctrl+L, Ctrl+*" combos, instead.
 * User mode DEP violation at 0x0 will now show location of exception in text window. @greensleevless

Bug Fixes
 * Fixed a case where consecutive line numbers in symbol information, as emitted by Clang but not
MSVC, required multiple step-over operations. @EladCiuraru
 * Better handling for invalid/corrupt line information in PDBs. @Zilarrezko
 * Fixes case where output window is not scrolled properly when the window is not active.
 * Fixed hidden windows showing up in tab bars after reloading a layout in certain cases.

Version 0.3.6.2
---------------
MD5: 9560103d7c1147f651f3b3955e623e3f
SHA1: 20c27dfb3b5ee97ea0eafd2006b3b1d969130b4d
Improvements
 * Allow toggling the display of module load/unload, thread exit, and process exit messages

Version 0.3.6.1
---------------
MD5: d8e44cb18ff3aea47425e63c62b8591b
SHA1: c4b118205dfbfd4ab96b9a9c4eb79f39c7ca6b78
Bug Fixes
 * Fixes potential crash in stack walk code introduced in 3.6.0 (The Sandvich Maker)
 * Evaluate on hover will now consider array indexing when picking the expression to evaluate
 (datamite)
 * Fixes conditional breakpoints added at program startup (datamite)
 * AltGR+[^A-Z] can now be used to initiate an edit in the watch window (seventh-chord)
 * Fixed expression generated for Add Watch / Send to Memory Window for dereferenced pointers
 (JensWallgren)

Version 0.3.6.0
---------------
MD5: 3087556dc2eb80e45560d82bae5a3689
SHA1: 07d7d809da03aa6ca9bae7a84e9837e7615e79f0
New Features
 * Evaluate on hover. Can set the tooltip delay in Windows>Style and can optionally disable this
 feature altogether.
 * A stack-overflow exception will now collapse repeated frames and show "Collapsed N frames" in the
 Call Stack pane. Can double-click collapsed frame or toggle this behavior via "Collapse Repeated
 Frames" context menu item.
 * Adds "Format As" menu item showing relevant format specifiers for the selected watch(s).
 * Added $err pseudo-variable for the last error value set by SetLastError for the current thread
 * Added $handles pseudo-variable which shows the number of handles allocated by the process
 * Added $tid and $pid pseudo-variables for thread and process IDs
 * Added ",d" decimal format specifier
 * Added ",hr" format specifier for displaying an HRESULT or Win32 error code
 * Adds process ID (optionally) to status text. Can be toggle via status text's context menu.
 * Information about module load/unload events and thread exit codes are now written to the Output window
 * Added "Read Only" checkbox to memory window's options
 * Added "Copy Address" to memory window's context menu
 * Allow breakpoint on either nominal entry point (WinMain, main, etc...) or entry point as defined
 in the PE header.
 * Can now run to a filename/line via the command line driver (see run-to-cursor in documentation)

Improvements
 * Lazily load function table entries. This helps improve performance when stepping over calls that
 load many DLLs (e.g., Direct3D initialization).
 * Use high-contrast color for collapsed panes to make them easier to find.
 * Adds error message when trying to index non-array/pointer type.
 * Properly handle device removed scenarios (Direct3D).
 * Line numbers have been added back to the call stack display.
 * Render the frame rather than show black while resizing.
 * Implements PgUp/PgDown in panes (previous these keys mapped to what is Home/End now)
 * Added binary formatting of enum values
 * We now alphabetically sort text file names in the drop-down menu.
 * Adds Ctrl+C hotkey to Output Window to copy contents to clipboard
 * Rearranged menu in Output Window so "Clear" is less likely to be accidently clicked.
 * Adjusted vertical position of lock icon in embedded font
 * Added more descriptive error message in the case CreateProcess fails
 * No longer count disable processor BPs against total number use
 * Adds "Move Up / Move Down" menu items in watch window to aid discoverability of this existing
 feature.
 * Show arrow next to processor breakpoint that was hit.
 * Run-to-cursor operation will now show yellow breakpoint symbol in text/disassembly
 * No longer initiate editing of address in memory window if a cell is being edited.
 * Double-click, not single-click, now used to initiate memory window editing.
 * An error in a breakpoint's condition is now more clearly presented.
 * Allow adding a breakpoint at an address without having to open/navigate to the address in the
 disassembly window.

Bug Fixes
 * Fixed lookups of static, global TLS variables.
 * Fixes the case in which a breakpoint is added on the same line as the run-to-cursor location
 before the run-to-cursor operation completes.
 * String summaries are now culled to 64-chars for non-nested previews and 32-chars otherwise.
 * Addressed problems where double-clicks were not registering on certain touchpads.
 * Fixed a problem where the corresponding text window would not be brought to the front when
 stepping through disassembly.
 * Fixed a case where the session would be considered modified immediately after opening.
 * Fixes a problem where keyboard input was ignored in the auto-created watch expression in certain
 situations.
 * Now properly sign-extend bitfield values.
 * Fixed the editing of bitfield values in watch.
 * An exception is now considered "handled" after using set-next-statement
 * Fixed so that Ctrl+Alt+W no longer deletes a watch expression when changing focus to another
 watch window.
 * Modifying condition on an existing processor breakpoint was broke in previous release. This has
 been fixed.
 * Fixed dereferencing union type when using '.' rather than '->'
 * Left arrow was unintentionally changing to focus to last row in some cases. This has been fixed.
 * Fixed reported case where cursor position on ASM source goes to PROC declaration rather than
 first instruction.
 * Fixed problem terminating app when breakpoint present in multithread code.
 * Fixed bug when run-to-cursor is executed on the same line
 * Fixed hang adding a function breakpoint to a module containing only a single function.
 * Fix for memory window so navigate to an address occurs even when reevaluation of an expression is
 identical to the previous address.


Version 0.3.5.4
---------------
MD5: 8672afff37a5ff944a57d4b59098d0fa 
SHA1: 15056eff0dd52204f975132525587bb84d075cd7
Improvements
 * Slightly better error message for pointer subtraction when the element types are of two different
 sizes.
Bug fixes
 * Remove debug code left in that could cause a crash
 * Fix subtle bug in allocator that could cause a crash in rare cases
 * Fixed crash when using "wstr" formatter with zero length
 * Fixed issue where changing font size would reset dark/light mode setting.

Version 0.3.5.3
---------------
MD5: 45bcc2deffecd5ed80a37c5d10c64cbf
SHA1: aee805b10c224cc9cec436aa73f57c8a58ed71a6
Improvements
 * Ctrl+Up/Ctrl+Down in callstack window will now set the frame relative to the current frame rather
 than relative to the focused row.
 * Expression parser now accepts the less verbose {n} or {*}, as opposed to {^n} or {^*} (caret no
 longer required), when specifying a relative scope.

Bug fixes
 * Fixes a crash triggered when adding or removing a conditional or processor breakpoint while the
 target is running. 
 * An explicit binary format specifier will now take precedence over the global hex display setting.
 * Current breakpoint indicator (arrow) now properly cleared when continuing execution.
 * Set modified bit on session if active configuration is changed.
 * Make sure breakpoint hit "animation" is triggered in disassembly window.

Version 0.3.5.2
---------------
MD5: c73bc1b5e725aa68292d3cfadb8cb8e3
SHA1: 4090d7537fc9475e5fce900745fd60425460dcee
Bug fixes
 * Fixes one off in handling of OutputDebugStringW
 * Fixes infinite loop when obtaining field information for a type that contains a virtual base
 class.

Version 0.3.5.1
---------------
MD5: bc0f39a7ee4b29d52fb33679e95fb5a2 
SHA1: 2559e28b75176098e5abc9275bc584bae4356ea4 
Improvements
 * Support Unicode strings output with OutputDebugStringW.
 * Now show string summary for char[0] as if it were of type char*.
 * The "attach to process" flow is now possible without a mouse.
 * Can now select multiple source files in file open dialog.
 * Now allow subtraction of pointers of differing types as long as the underlying type has the same
 size.
 * Longer string preview shown when not part of an aggregate summary.

Bug fixes
 * Fixed formatting of address value in address breakpoint dialog box
 * Fixed a potential hang when inspecting a global variable which occurs in two or more translation
 units. In this case, we render a synthetic var with the fields name __g0, __g1, and so forth,
 corresponding to each instance of the global variable.
 * Fixed problem with Reattach to Process wouldn't work in some cases.
 * Using space in search term no longer causes a case-sensitive search.
 * Properly format negative signed character literals.
 * Fixed problem where enums couldn't be used as array indices.
 * Fix to allow manipulating diassembly view settings (show address/code bytes) after target process
 is stopped.
 * Fixed evaluation of struct addresses passed by value that were less than eight bytes and not a
 power of two.

Version 0.3.5.0
---------------
MD5: 712b9f95196b2071bee96b3a43d7e097
SHA1: 929902f0a5a8c43923f506c57f2e488e2720d534
New features
 * Added an optional toolbar for debug control. This can be toggled in Window > Style > Show Toolbar
 * Local and global implicit thread local storage variables can now be watched.
 * Adds support for AddressSanitizer. Access violations that attempt to read/write into
 AddressSanitizer's address space (e.g., used to expand virtual address space) are now skipped
 (i.e., allowed to be handled without halting execution). There is no UI required as this is
 detected and handled automatically by RemedyBG. 
 * Added "Reevaluate Automatically" option to memory window to allow reevaluating and updating the
 address in a memory window.

Improvements
 * Allow making forward progress on first-chance, continuable exceptions
 * Properly unwind stack in the case that a null function pointer is called.
 * Bake in information about stdint types (uint8_t, etc.) as a temporary workaround until typedef
 information is properly supported.
 * UI will now be automatically scaled based on the DPI of the monitor.

Bug fixes
 * Fix to properly render unprintable escape characters (e.g., '\n') in watch
 * Fixed evaluation of variables cast to structures in certain cases
 * Fixed output of process ID in the output window.
 * Selection of rows in breakpoints pane using mouse has been fixed
 * Fixed ambiguity of format expression when variable name matches one of the format specifiers.
 * Fixed Edit Breakpoint, Add Watch, and Set Next context-menu handling in the disassembly window.
 * Copying a multiline string value (eg., "var, str") will now be copied to the clipboard as
 expected.

Version 0.3.4.1
---------------
MD5: 407b8ca5e4b3e04fda14c10c02757a19 
SHA1: 399774ffe57faaed7f5618df379c48cc24d6eb4b 
 * Fixes potential crash when displaying long summaries (L. Reyes)

Version 0.3.4.0
---------------
MD5: 74b003e6143a7e8d5db726534371fce7
SHA1: 83cd0e0a90d47eeb36b9f713710dab115f417667
New features
 * Adds column major option to watch windows (context menu toggle)
 * Adds an "Add Watch" context menu item for text windows
 * Allow writing member access operator as '.' instead of '->' even for pointer types.
 * Adds optional Comment column in watch window to allow entering arbitrary text (single line at the
 moment)

Improvements
 * We now save hexadecimal setting for watch windows to settings file
 * Allow repeatedly using Run to Cursor on the same line
 * UI tweaks: reduced size of arrow column in tables, removed unnecessary window padding around
 tables, removes rounding from selection rect in text window
 * Adds numpad keys to list of keys that initiate watch editing
 * Better formatting of array elements (removes redundant expression, align index)
 * We now do automatic type conversion when modifying values in watch.

Bug fixes
 * Prevent potential crash when muting the only active thread.
 * Fix so that clicking scrollbar in table does not try to select row in table.
 * Fixes bug updating current breakpoint when another breakpoint is deleted
 * Fixed potential crash from non-escaped characters in structure summary.
 * Fix to ensure window is updated/redrawn in response to a OutputDebugString debug event
 * Removed hack that disabled modifier keys for a short period of time. This fixes problems with
 repeat-rate and eaten keys on certain keyboard layouts.
 * Fixed bug where size of ",str" format specifier would get ignored

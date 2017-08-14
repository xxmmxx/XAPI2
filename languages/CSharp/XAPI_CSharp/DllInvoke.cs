﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace XAPI
{
    [ComVisible(false)]
    [CLSCompliant(false)]
    public class DllInvoke:InvokeBase
    {
        [ComVisible(false)]
        [System.Flags]
        public enum LoadLibraryFlags : uint
        {
            DONT_RESOLVE_DLL_REFERENCES = 0x00000001,
            LOAD_IGNORE_CODE_AUTHZ_LEVEL = 0x00000010,
            LOAD_LIBRARY_AS_DATAFILE = 0x00000002,
            LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE = 0x00000040,
            LOAD_LIBRARY_AS_IMAGE_RESOURCE = 0x00000020,
            LOAD_WITH_ALTERED_SEARCH_PATH = 0x00000008
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        private extern static IntPtr LoadLibrary(string lpFileName);
        [DllImport("kernel32.dll", SetLastError = true)]
        private extern static IntPtr GetProcAddress(IntPtr hModule, string lpProcName);
        [DllImport("kernel32.dll")]
        private extern static bool FreeLibrary(IntPtr hModule);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern int GetDllDirectory(int bufSize, StringBuilder buf);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool SetDllDirectory(string lpPathName);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LoadLibraryEx(string lpFileName, IntPtr hReservedNull, LoadLibraryFlags dwFlags);

        [DllImport("kernel32.dll")]
        public extern static int FormatMessage(int flag, ref IntPtr source, int msgid, int langid, ref string buf, int size, ref IntPtr args);

        public static string GetSysErrMsg(int errCode)
        {
            IntPtr tempptr = IntPtr.Zero;
            string msg = null;
            FormatMessage(0x1300, ref tempptr, errCode, 0, ref msg, 255, ref tempptr);
            return msg;
        }

        public DllInvoke(string DLLPath)
        {
            hLib = LoadLibraryEx(DLLPath,IntPtr.Zero,LoadLibraryFlags.LOAD_WITH_ALTERED_SEARCH_PATH);
            if (hLib == IntPtr.Zero)
            {
                int errCode = Marshal.GetLastWin32Error();
                string errMsg = GetSysErrMsg(errCode);
                throw new Exception(string.Format("GetLastError:{0},FormatMessage:{1} {2}", errCode, errMsg, DLLPath));
            }
        }
        ~DllInvoke()
        {
            Dispose();
        }
        //将要执行的函数转换为委托
        public override Delegate Invoke(string APIName, Type t)
        {
            if (hLib == IntPtr.Zero)
                return (Delegate)null;

            IntPtr api = GetProcAddress(hLib, APIName);
            return (Delegate)Marshal.GetDelegateForFunctionPointer(api, t);
        }

        public override void Dispose()
        {
            // Kingstar交易接口在这里为何会导致软件当?
            // 由于接口的问题，在这里只好先这样，以后没有这个问题后再恢复

            //if (hLib != IntPtr.Zero)
            //    FreeLibrary(hLib);
            //hLib = IntPtr.Zero;
        }
    }
}

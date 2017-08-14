﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;


namespace XAPI
{
    [ComVisible(false)]
    public class PInvokeUtility
    {
        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern void OutputDebugString(string message);

        static Encoding encodingGB2312 = Encoding.GetEncoding(936);

        public static string GetUnicodeString(byte[] str)
        {
            if(str == null)
            {
                return string.Empty;
            }
            int bytecount = 0;
            foreach(byte b in str)
            {
                if (0 == b)
                    break;
                ++bytecount;
            }
            if (0 == bytecount)
                return string.Empty;
            //比TrimEnd('\0');快,减少了内存的复制
            return encodingGB2312.GetString(str, 0, bytecount);
        }

        public static T GetObjectFromIntPtr<T>(IntPtr handler)
        {
            if (handler == IntPtr.Zero)
            {
              return default(T);
            }
            else
            {
              return (T)Marshal.PtrToStructure(handler, typeof(T));
            }
        }

        public static DepthMarketDataNClass GetDepthMarketDataNClass(IntPtr ptr)
        {
            DepthMarketDataNField obj = (DepthMarketDataNField)Marshal.PtrToStructure(ptr, typeof(DepthMarketDataNField));

            DepthMarketDataNClass cls = new DepthMarketDataNClass();
            
            //obj.Size;
            cls.TradingDay = obj.TradingDay;
            cls.ActionDay = obj.ActionDay;
            cls.UpdateTime = obj.UpdateTime;
            cls.UpdateMillisec = obj.UpdateMillisec;
            cls.Exchange = obj.Exchange;
            cls.Symbol = obj.Symbol;
            cls.InstrumentID = obj.InstrumentID;
            cls.LastPrice = obj.LastPrice;
            cls.Volume = obj.Volume;
            cls.Turnover = obj.Turnover;
            cls.OpenInterest = obj.OpenInterest;
            cls.AveragePrice = obj.AveragePrice;
            cls.OpenPrice = obj.OpenPrice;
            cls.HighestPrice = obj.HighestPrice;
            cls.LowestPrice = obj.LowestPrice;
            cls.ClosePrice = obj.ClosePrice;
            cls.SettlementPrice = obj.SettlementPrice;
            cls.UpperLimitPrice = obj.UpperLimitPrice;
            cls.LowerLimitPrice = obj.LowerLimitPrice;
            cls.PreClosePrice = obj.PreClosePrice;
            cls.PreSettlementPrice = obj.PreSettlementPrice;
            cls.PreOpenInterest = obj.PreOpenInterest;
            cls.TradingPhase = obj.TradingPhase;
            //obj.BidCount;

            int size = Marshal.SizeOf(typeof(DepthField));
            IntPtr pBid = new IntPtr(ptr.ToInt64() + Marshal.SizeOf(typeof(DepthMarketDataNField)));
            int AskCount = (obj.Size - Marshal.SizeOf(typeof(DepthMarketDataNField))) / size - obj.BidCount;
            IntPtr pAsk = new IntPtr(ptr.ToInt64() + Marshal.SizeOf(typeof(DepthMarketDataNField)) + obj.BidCount * size);

            cls.Bids = new DepthField[obj.BidCount];
            cls.Asks = new DepthField[AskCount];
            
            for (int i = 0; i < obj.BidCount; ++i)
            {
                cls.Bids[i] = (DepthField)Marshal.PtrToStructure(new IntPtr(pBid.ToInt64() + i * size), typeof(DepthField));
            }

            for (int i = 0; i < AskCount; ++i)
            {
                cls.Asks[i] = (DepthField)Marshal.PtrToStructure(new IntPtr(pAsk.ToInt64() + i * size), typeof(DepthField));
            }

            return cls;
        }

        public static SettlementInfoClass GetSettlementInfoClass(IntPtr ptr)
        {
            SettlementInfoClass cls = new SettlementInfoClass();
            if (ptr == IntPtr.Zero)
                return cls;

            SettlementInfoField obj = (SettlementInfoField)Marshal.PtrToStructure(ptr, typeof(SettlementInfoField));

            int size = Marshal.SizeOf(typeof (SettlementInfoField));
            IntPtr pContent = new IntPtr(ptr.ToInt64() + size);

            cls.TradingDay = obj.TradingDay;

            unsafe
            {
                cls.Content = new string((sbyte*)pContent, 0, obj.Size, encodingGB2312);
            }

            return cls;
        }
    }
}

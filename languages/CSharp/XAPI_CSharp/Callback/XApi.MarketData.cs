﻿using XAPI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace XAPI.Callback
{
    public partial class XApi
    {
        public DelegateOnRtnDepthMarketData OnRtnDepthMarketData
        {
            get { return OnRtnDepthMarketData_; }
            set { OnRtnDepthMarketData_ = value; }
        }
        // 这种写法的主要目的是求快
        private DelegateOnRtnDepthMarketData OnRtnDepthMarketData_;

        public DelegateOnFilterSubscribe OnFilterSubscribe
        {
            get { return OnFilterSubscribe_; }
            set { OnFilterSubscribe_ = value; }
        }
        // 这种写法的主要目的是求快
        private DelegateOnFilterSubscribe OnFilterSubscribe_;

        public DelegateOnRtnInstrumentStatus OnRtnInstrumentStatus
        {
            get { return OnRtnInstrumentStatus_; }
            set { OnRtnInstrumentStatus_ = value; }
        }

        private DelegateOnRtnInstrumentStatus OnRtnInstrumentStatus_;


        #region 已经订阅的行情
        public int MaxSubscribedInstrumentsCount;
        public int SubscribedInstrumentsCount
        {
            get
            {
                lock (locker)
                {
                    int cnt = 0;
                    foreach (var kv in _SubscribedInstruments)
                    {
                        cnt += kv.Value.Count;
                    }
                    return cnt;
                }
            }
        }

        public bool SubscribedInstrumentsContains(string szInstrument, string szExchange)
        {
            lock (locker)
            {
                HashSet<string> instruments;
                if(_SubscribedInstruments.TryGetValue(szExchange, out instruments))
                {
                    return instruments.Contains(szInstrument);
                }
                return false;
            }
        }

        private Dictionary<string, HashSet<string>> _SubscribedInstruments = new Dictionary<string, HashSet<string>>();
        public Dictionary<string, HashSet<string>> SubscribedInstruments
        {
            get
            {
                lock (locker)
                {
                    return _SubscribedInstruments;
                }
            }
        }
        #endregion

        #region 订阅行情
        public virtual void Subscribe(string szInstrument, string szExchange)
        {
            lock (locker)
            {
                IntPtr szInstrumentPtr = Marshal.StringToHGlobalAnsi(szInstrument);
                IntPtr szExchangePtr = Marshal.StringToHGlobalAnsi(szExchange);

                proxy.XRequest((byte)RequestType.Subscribe, Handle, IntPtr.Zero, 0, 0,
                    szInstrumentPtr, 0, szExchangePtr, 0, IntPtr.Zero, 0);

                HashSet<string> instruments;
                if(!_SubscribedInstruments.TryGetValue(szExchange, out instruments))
                {
                    instruments = new HashSet<string>();
                    _SubscribedInstruments[szExchange] = instruments;
                }                

                szInstrument.Split(new char[2] { ';', ',' }).ToList().ForEach(x =>
                {
                    instruments.Add(x);
                });

                Marshal.FreeHGlobal(szInstrumentPtr);
                Marshal.FreeHGlobal(szExchangePtr);
            }
        }

        public virtual void Unsubscribe(string szInstrument, string szExchange)
        {
            lock (locker)
            {
                IntPtr szInstrumentPtr = Marshal.StringToHGlobalAnsi(szInstrument);
                IntPtr szExchangePtr = Marshal.StringToHGlobalAnsi(szExchange);

                proxy.XRequest((byte)RequestType.Unsubscribe, Handle, IntPtr.Zero, 0, 0,
                    szInstrumentPtr, 0, szExchangePtr, 0, IntPtr.Zero, 0);

                HashSet<string> instruments;
                if (!_SubscribedInstruments.TryGetValue(szExchange, out instruments))
                {
                    instruments = new HashSet<string>();
                    _SubscribedInstruments[szExchange] = instruments;
                }

                szInstrument.Split(new char[2] { ';', ',' }).ToList().ForEach(x =>
                {
                    instruments.Remove(x);
                });

                Marshal.FreeHGlobal(szInstrumentPtr);
                Marshal.FreeHGlobal(szExchangePtr);
            }
        }
        #endregion

        private void _OnRtnDepthMarketData(IntPtr ptr1,int size1,double double1)
        {
            // 求快，这个地方不判断
            //if (OnRtnDepthMarketData_ == null)
            //    return;
            DepthMarketDataNClass cls = PInvokeUtility.GetDepthMarketDataNClass(ptr1);
            OnRtnDepthMarketData_(this, ref cls);
        }

        private void _OnRtnInstrumentStatus(IntPtr ptr1, int size1)
        {
            // 求快，不加
            if (OnRtnInstrumentStatus_ == null)
                return;

            InstrumentStatusField obj = (InstrumentStatusField)Marshal.PtrToStructure(ptr1, typeof(InstrumentStatusField));

            OnRtnInstrumentStatus_(this, ref obj);
        }

        private bool _OnFilterSubscribe(double double1, int size1, int size2, int size3, IntPtr ptr1)
        {
            if (OnFilterSubscribe_ == null)
                return true;

            return OnFilterSubscribe_(this, (ExchangeType)double1, size1, size2, size3, ptr1);
        }
    }
}

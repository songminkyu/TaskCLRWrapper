using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using VecDictTests;

namespace SampleWrapperTest
{
    class NativeMethods
    {

        [DllImport("MFCWrapperUtility.dll", EntryPoint = nameof(GetNativeStructs))]
        private static extern void GetNativeStructsInternal(out CppVectorInternal<NativeStruct> result);

        public static CppVector<NativeStruct> GetNativeStructs()
        {
            GetNativeStructsInternal(out var result);
            return result;
        }

        [DllImport("MFCWrapperUtility.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void ShowMessageBox(string pszPath);

        //[DllImport("WrapperUtility.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        [DllImport("MFCWrapperUtility.dll", EntryPoint = nameof(GetSum))]
        private static extern int GetSum(int a, int b);

        [DllImport("MFCWrapperUtility.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = nameof(AsyncThreadTest))]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string AsyncThreadTest();

        [DllImport("MFCWrapperUtility.dll", EntryPoint = nameof(AsyncThreadTest_2))]       
        private static extern uint AsyncThreadTest_2();

        [DllImport("MFCWrapperUtility.dll", EntryPoint = nameof(AsyncThreadTest_3))]
        private static extern void AsyncThreadTest_3();

        public static void NativeShowMessageBox(string str)
        {
            ShowMessageBox(str);
        }

        public static int NativeGetSum(int a, int b)
        {
            return GetSum(a, b);
        }

        public static string GetTest()
        {            
            return AsyncThreadTest();
        }

        public static uint GetReverseSum()
        {
            return AsyncThreadTest_2();
        }

        public static async Task AsyncAwaitTest()
        {            
            await Task.Delay(1000).ConfigureAwait(false);
            AsyncThreadTest_3();
        }
    }
    [StructLayout(LayoutKind.Sequential)]
    struct NativeStruct
    {
        public int Value1;
        public double Value2;
        public float Value3;
    }


    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

        }

        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            await Task.Run(async () =>
            {
                await NativeMethods.AsyncAwaitTest();
            }).ConfigureAwait(false);        
            
            NativeMethods.NativeShowMessageBox("AsyncAwaitTest Completed");

            var tv = NativeMethods.GetReverseSum();

            NativeMethods.NativeShowMessageBox(tv.ToString());

            var tv1 = NativeMethods.GetTest();

            NativeMethods.NativeShowMessageBox(tv1);

            var v = NativeMethods.NativeGetSum(1, 2);

            NativeMethods.NativeShowMessageBox(v.ToString());
            
            //var structs = NativeMethods.GetNativeStructs();
            //foreach (var @struct in structs)
            //{
            //    Console.WriteLine($"Value1 = {@struct.Value1},\tValue2 = {@struct.Value2},\tValue3 = {@struct.Value3}");
            //}
        }
    }
}

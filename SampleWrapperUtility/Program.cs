// See https://aka.ms/new-console-template for more information



using System.Runtime.InteropServices;
using VecDictTests;
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
    private static extern string AsyncThreadTest(ref string pszPath);


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
        string str = "123123";
        var rr = AsyncThreadTest(ref str);
        Console.WriteLine("GetText {0}", rr);
        return str;

    }
}
[StructLayout(LayoutKind.Sequential)]
struct NativeStruct
{
    public int Value1;
    public double Value2;
    public float Value3;
}

class Program
{
    static unsafe void Main(string[] args)
    {
        var tv = NativeMethods.GetTest();
        
        Console.WriteLine(tv.ToString());

        var v = NativeMethods.NativeGetSum(1, 2);
        NativeMethods.NativeShowMessageBox(v.ToString());
        Console.WriteLine("Hello World!!");

        var structs = NativeMethods.GetNativeStructs();
        foreach (var @struct in structs)
        {
            Console.WriteLine($"Value1 = {@struct.Value1},\tValue2 = {@struct.Value2},\tValue3 = {@struct.Value3}");
        }
    }
}



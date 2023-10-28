using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace VecDictTests;

public unsafe struct CppVectorInternal<T>
    where T : struct
{
    public void* Vector;
    public delegate* unmanaged<void*, int> Count;
    public delegate* unmanaged<void*, int, void*, void> GetItemAt;
    public delegate* unmanaged<void*, void> Dispose;
}

public class CppVector<T> : IEnumerable<T>, IDisposable
    where T : struct
{
    private readonly CppVectorInternal<T> _internal;
    private bool _disposed = false;

    public CppVector(CppVectorInternal<T> @internal)
    {
        _internal = @internal;
    }

    public static implicit operator CppVector<T>(CppVectorInternal<T> @internal)
    {
        return new(@internal);
    }

    private void EnsureNotDisposed()
    {
        if (_disposed)
            throw new InvalidOperationException("already disposed");
    }

    public int Count
    {
        get
        {
            EnsureNotDisposed();
            unsafe
            {
                return _internal.Count(_internal.Vector);
            }
        }
    }

    public T this[int index]
    {
        get
        {
            EnsureNotDisposed();
            if (index < 0 || Count <= index)
                throw new IndexOutOfRangeException();

            T result = default;
            unsafe
            {
                _internal.GetItemAt(_internal.Vector, index, Unsafe.AsPointer(ref result));
            }

            return result;
        }
    }

    public void Dispose()
    {
        if (_disposed)
            return;

        unsafe
        {
            _disposed = true;
            _internal.Dispose(_internal.Vector);
        }
    }

    public IEnumerator<T> GetEnumerator()
    {
        int count = Count;
        for (int i = 0; i < count; ++i)
        {
            yield return this[i];
        }
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }
}
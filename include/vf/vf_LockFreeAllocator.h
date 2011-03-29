// Copyright (C) 2008-2011 by Vincent Falco, All rights reserved worldwide.
// This file is released under the MIT License:
// http://www.opensource.org/licenses/mit-license.php

#ifndef __VF_LOCKFREEALLOCATOR_VFHEADER__
#define __VF_LOCKFREEALLOCATOR_VFHEADER__

#include "vf/vf_Atomic.h"
#include "vf/vf_LockFreeReadWriteMutex.h"
#include "vf/vf_LockFreeStack.h"
#include "vf/vf_OncePerSecond.h"
#include "vf/vf_Type.h"

#define ALLOCATOR_COUNT_SWAPS 0
//#define ALLOCATOR_CHECK_LEAKS VF_CHECK_LEAKS
#define ALLOCATOR_CHECK_LEAKS 0

namespace LockFree {

enum
{
  //
  // This number should be as small as possible
  // without generating compile time assertions.
  //
  globalAllocatorBlockSize = 96
};

// Wait-free allocator for fixed size memory blocks
//
// - Any thread may allocate and free concurrently.
//
// - Freed blocks are re-used in future allocations after a certain amount
//   of time has passed, in order to prevent the ABA problem for callers.
//
// - When there are no free blocks available, the algorithm becomes a blocking
//   algorithm since we request memory from the standard library.
//
template <size_t BlockSize>
class FixedAllocator : private OncePerSecond
{
public:
  enum
  {
    // This allocator will time its garbage collections to
    // try to keep the memory utilization at or below the byteLimit.
    //
    // This is something of a soft limit but we should be close.
    //
    defaultKiloBytes = 64 * 1024,

    // If we exceed this limit on hard allocations
    // then an exception will be thrown.
    //
    // Typically this means that consumers cannot keep up
    // with producers and the app would be non-functional.
    //
    hardLimitMegaBytes = 1 * 1024
  };

  enum
  {
    blockSize = BlockSize
  };

  FixedAllocator (int byteLimit = defaultKiloBytes * 1024)
    : m_interval (byteLimit / (4 * (BlockSize + sizeof(Node))))
    , m_count (m_interval)
    , m_hard ((hardLimitMegaBytes * 1024 * 1024) / (BlockSize + sizeof(Node)))
#if ALLOCATOR_COUNT_SWAPS
    , m_swaps (0)
#endif
  {
    m_hot = &m_pool[0];
    m_cold = &m_pool[1];

    startOncePerSecond ();
  }

  ~FixedAllocator ()
  {
    endOncePerSecond ();

#if ALLOCATOR_CHECK_LEAKS
    vfassert (m_used.is_reset ());
#endif

    free (m_pool[0]);
    free (m_pool[1]);

#if ALLOCATOR_CHECK_LEAKS
    vfassert (m_total.is_reset ());
#endif
  }

  template <size_t Bytes>
  void* allocate ()
  {
    static_vfassert (Bytes <= BlockSize);
    return recycle_block ();
  }

  void deallocate (void* p)
  {
    Node* node = toNode (p);

    m_hot->garbage.push_front (node);

#if ALLOCATOR_CHECK_LEAKS
    m_used.release ();
#endif

    // See if we need to perform garbage collection
#if 1
    if (m_collect.isSet () && m_collect.tryClear ())
    {
      // Multiple threads may get here but only one will do the gc.
#if 0
      // Append the garbage to the recycle list.
      m_cold->recycle.push_front (m_cold->garbage);
#else
      // Perform the deferred swap of reused
      // and garbage in the cold pool.
      m_cold->recycle.swap (m_cold->garbage);
#endif

      // Now swap hot and cold.
      // This is atomic with respect to m_hot.
      Pool* temp = m_hot;
      m_hot = m_cold; // atomic
      m_cold = temp;

#if ALLOCATOR_COUNT_SWAPS
      String s;
      s << "swap " << String (++m_swaps);
#if ALLOCATOR_CHECK_LEAKS
      s << " (" << String (m_used.get()) << "/"
        << String (m_total.get()) << ")";
#endif
      Logger::outputDebugString (s);
#endif
    }

#else
    if (m_count.release ())
    {
#if 0
      // Append the garbage to the recycle list.
      m_cold->recycle.push_front (m_cold->garbage);
#else
      // Perform the deferred swap of reused
      // and garbage in the cold pool.
      m_cold->recycle.swap (m_cold->garbage);
#endif

      // Now swap hot and cold.
      // This is atomic with respect to m_hot.
      Pool* temp = m_hot;
      m_hot = m_cold; // atomic
      m_cold = temp;

      // Reset the swap countdown.
      m_count.set (m_interval);

#if ALLOCATOR_COUNT_SWAPS
      String s;
      s << "swap " << String (++m_swaps);
#if ALLOCATOR_CHECK_LEAKS
      s << " (" << String (m_used.get()) << "/"
        << String (m_total.get()) << ")";
#endif
      Logger::outputDebugString (s);
#endif
    }
#endif

  }

private:
  void* hard_alloc ()
  {
    // implicit global mutex
    void* p = ::operator new (BlockSize + sizeof (Node));
    if (!p)
      Throw (Error().fail (__FILE__, __LINE__, TRANS("the FixedAllocator receivd 0 on ::new")));

    const bool exhausted = m_hard.release ();

    if (exhausted)
      Throw (Error().fail (__FILE__, __LINE__, TRANS("the FixedAllocator exhausted it's allocations")));

#if ALLOCATOR_CHECK_LEAKS
    m_total.addref ();
#endif

    return p;
  }

  void* recycle_block ()
  {
    Node* node = m_hot->recycle.pop_front ();

    void* p;

    if (!node)
    {
      p = hard_alloc ();

#if 0
      // two for one special
      void* extra = hard_alloc ();

#if ALLOCATOR_CHECK_LEAKS
      m_used.addref ();
#endif

      deallocate (extra); // huh?
#endif
    }
    else
    {
      p = fromNode (node);
    }

#if ALLOCATOR_CHECK_LEAKS
    m_used.addref ();
#endif

    return p;
  }

  void doOncePerSecond ()
  {
    m_collect.trySet ();
  }

private:
  // In order to assure that allocated elements have the
  // same alignment as what we would get from a straight
  // new, the List::Node is placed just after the allocated
  // storage, rather than before. However this might cause
  // alignment issues for the List::Node itslf (which contains
  // an Atomic::Pointer that may need more strict alignment)
  //
  struct Node;
  typedef Stack <Node> List;
  struct Node : List::Node
  {
  };

  // A pool contains the recycle list, from which we allocate,
  // and the garbage list, into which we free. This avoids the
  // ABA problem in the underlying Stack object.
  //
  struct Pool
  {
    List recycle;   // list of nodes to re-use
    List garbage;   // list of freed nodes to cool down
  };

  static inline void* fromNode (Node* node)
  {
    return reinterpret_cast <char *> (node) - BlockSize;
  }

  static inline Node* toNode (void* p)
  {
    return reinterpret_cast <Node*> (reinterpret_cast <char *> (p) + BlockSize);
  }

  void free (List& list)
  {
    for (;;)
    {
      Node* node = list.pop_front ();

      if (node)
      {
        ::operator delete (fromNode (node)); // implicit global mutex

#if ALLOCATOR_CHECK_LEAKS
        m_total.release ();
#endif
      }
      else
      {
        break;
      }
    }
  }

  void free (Pool& pool)
  {
    free (pool.recycle);
    free (pool.garbage);
  }

private:
  // There are two pools, one hot and one cold. All alloc and
  // free go through the hot pool. Every so often, we will atomically
  // swap these two pools. Before the swap takes place, the lists
  // in the cold pool are themselves swapped. This operation is deferred
  // as long as possible to reduce the effective chance of encountering
  // the ABA problem down to zero. This depends on a sufficiently large
  // value of "defaultKiloBytes" for the problem domain.
  //
  Pool m_pool[2];                 // pair of pools
  Atomic::Pointer <Pool> m_hot;   // pool we are currently using
  Pool* m_cold;                   // pool which is cooling down
  int m_interval;                 // how often to swap
  Atomic::Counter m_count;        // countdown to swap
  Atomic::Flag m_collect;         // flag telling us to do gc
  Atomic::Counter m_hard;         // limit of system allocations

#if ALLOCATOR_COUNT_SWAPS
  int m_swaps;
#endif

#if ALLOCATOR_CHECK_LEAKS
  Atomic::Counter m_total;
  Atomic::Counter m_used;
#endif
};

typedef FixedAllocator <globalAllocatorBlockSize> GlobalAllocatorOld; 
extern GlobalAllocatorOld globalAllocator;

class GlobalAllocator
{
public:
  template <int Bytes>
  void* allocate ()
  {
    return globalAllocator.allocate <Bytes> ();
  }

  void deallocate (void* p)
  {
    globalAllocator.deallocate (p);
  }
};

// Sugar hack since the compiler cannot infer
// template arguments based on the return type.
//
// Usage:
//
//  C* c = globalAlloc<C>::New (arg1, arg2,...);
//
template <class C>
struct globalAlloc
{
  static C* New ()
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C;
  }

  template <class T1>
  static C* New (const T1& t1)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1);
  }

  template <class T1, class T2>
  static C* New (const T1& t1, const T2& t2)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1, t2);
  }

  template <class T1, class T2, class T3>
  static C* New (const T1& t1, const T2& t2, const T3& t3)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1, t2, t3);
  }

  template <class T1, class T2, class T3, class T4>
  static C* New (const T1& t1, const T2& t2, const T3& t3, const T4& t4)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1, t2, t3, t4);
  }

  template <class T1, class T2, class T3, class T4, class T5>
  static C* New (const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1, t2, t3, t4, t5);
  }

  template <class T1, class T2, class T3, class T4, class T5, class T6>
  static C* New (const T1& t1, const T2& t2, const T3& t3, const T4& t4, 
                 const T5& t5, const T6& t6)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1, t2, t3, t4, t5, t6);
  }

  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
  static C* New (const T1& t1, const T2& t2, const T3& t3, const T4& t4, 
                 const T5& t5, const T6& t6, const T7& t7)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1, t2, t3, t4, t5, t6, t7);
  }

  template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
  static C* New (const T1& t1, const T2& t2, const T3& t3, const T4& t4, 
                 const T5& t5, const T6& t6, const T7& t7, const T8& t8)
  {
    static_vfassert (sizeof (C) <= GlobalAllocatorOld::blockSize);
    return new (globalAllocator.allocate <sizeof (C)> ()) C (t1, t2, t3, t4, t5, t6, t7, t8);
  }
};

template <class C>
void globalDelete (C* c)
{
  c->~C();
  globalAllocator.deallocate (c);
}

}

#endif

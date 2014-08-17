// Copyright (c) 2007, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// ---
// Author: Geoff Pike
//
// This file provides a minimal cache that can hold a <key, value> pair
// with little if any wasted space.  The types of the key and value
// must be unsigned integral types or at least have unsigned semantics
// for >>, casting, and similar operations.
//
// Synchronization is not provided.  However, the cache is implemented
// as an array of cache entries whose type is chosen at compile time.
// If a[i] is atomic on your hardware for the chosen array type then
// raciness will not necessarily lead to bugginess.  The cache entries
// must be large enough to hold a partial key and a value packed
// together.  The partial keys are bit strings of length
// kKeybits - kHashbits, and the values are bit strings of length kValuebits.
//
// In an effort to use minimal space, every cache entry represents
// some <key, value> pair; the class provides no way to mark a cache
// entry as empty or uninitialized.  In practice, you may want to have
// reserved keys or values to get around this limitation.  For example, in
// tcmalloc's PageID-to-sizeclass cache, a value of 0 is used as
// "unknown sizeclass."
//
// Usage Considerations
// --------------------
//
// kHashbits controls the size of the cache.  The best value for
// kHashbits will of course depend on the application.  Perhaps try
// tuning the value of kHashbits by measuring different values on your
// favorite benchmark.  Also remember not to be a pig; other
// programs that need resources may suffer if you are.
//
// The main uses for this class will be when performance is
// critical and there's a convenient type to hold the cache's
// entries.  As described above, the number of bits required
// for a cache entry is (kKeybits - kHashbits) + kValuebits.  Suppose
// kKeybits + kValuebits is 43.  Then it probably makes sense to
// chose kHashbits >= 11 so that cache entries fit in a uint32.
//
// On the other hand, suppose kKeybits = kValuebits = 64.  Then
// using this class may be less worthwhile.  You'll probably
// be using 128 bits for each entry anyway, so maybe just pick
// a hash function, H, and use an array indexed by H(key):
//    void Put(K key, V value) { a_[H(key)] = pair<K, V>(key, value); }
//    V GetOrDefault(K key, V default) { const pair<K, V> &p = a_[H(key)]; ... }
//    etc.
//
// Further Details
// ---------------
//
// For caches used only by one thread, the following is true:
// 1. For a cache c,
//      (c.Put(key, value), c.GetOrDefault(key, 0)) == value
//    and
//      (c.Put(key, value), <...>, c.GetOrDefault(key, 0)) == value
//    if the elided code contains no c.Put calls.
//
// 2. Has(key) will return false if no <key, value> pair with that key
//    has ever been Put.  However, a newly initialized cache will have
//    some <key, value> pairs already present.  When you create a new
//    cache, you must specify an "initial value."  The initialization
//    procedure is equivalent to Clear(initial_value), which is
//    equivalent to Put(k, initial_value) for all keys k from 0 to
//    2^kHashbits - 1.
//
// 3. If key and key' differ then the only way Put(key, value) may
//    cause Has(key') to change is that Has(key') may change from true to
//    false. Furthermore, a Put() call that doesn't change Has(key')
//    doesn't change GetOrDefault(key', ...) either.
//
// Implementation details:
//
// This is a direct-mapped cache with 2^kHashbits entries;
// the hash function simply takes the low bits of the key.
// So, we don't have to store the low bits of the key in the entries.
// Instead, an entry is the high bits of a key and a value, packed
// together.  E.g., a 20 bit key and a 7 bit value only require
// a uint16 for each entry if kHashbits >= 11.
//
// Alternatives to this scheme will be added as needed.

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "../../../BAL/OWBAL/Concretizations/Types/WK/BCTCPackedCacheWK.h"


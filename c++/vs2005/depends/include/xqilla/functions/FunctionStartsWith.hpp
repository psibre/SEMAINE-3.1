/*
 * Copyright (c) 2001-2008
 *     DecisionSoft Limited. All rights reserved.
 * Copyright (c) 2004-2008
 *     Oracle. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * $Id$
 */

/*

  StartsWith function

*/

#ifndef _FUNCTIONSTARTSWITH_HPP
#define _FUNCTIONSTARTSWITH_HPP

#include <xqilla/ast/ConstantFoldingFunction.hpp>

/** StartsWith function. */
class XQILLA_API FunctionStartsWith : public ConstantFoldingFunction
{
public:
  static const XMLCh name[];
  static const unsigned int minArgs;
  static const unsigned int maxArgs;

  FunctionStartsWith(const VectorOfASTNodes &args, XPath2MemoryManager* memMgr);
  
  /** XPath function. returns true if string1 starts with string2 **/
  Sequence createSequence(DynamicContext* context, int flags=0) const;

};

#endif // _FUNCTIONSTARTSWITH_HPP

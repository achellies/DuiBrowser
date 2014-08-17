// Automatically generated from D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/MathObject.cpp using D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/create_hash_table. DO NOT EDIT!

#include "lookup.h"

namespace KJS {

static const struct HashTableValue mathTableValues[27] = {
   { "E", (intptr_t)MathObject::Euler, DontEnum|DontDelete|ReadOnly, 0 },
   { "LN2", (intptr_t)MathObject::Ln2, DontEnum|DontDelete|ReadOnly, 0 },
   { "LN10", (intptr_t)MathObject::Ln10, DontEnum|DontDelete|ReadOnly, 0 },
   { "LOG2E", (intptr_t)MathObject::Log2E, DontEnum|DontDelete|ReadOnly, 0 },
   { "LOG10E", (intptr_t)MathObject::Log10E, DontEnum|DontDelete|ReadOnly, 0 },
   { "PI", (intptr_t)MathObject::Pi, DontEnum|DontDelete|ReadOnly, 0 },
   { "SQRT1_2", (intptr_t)MathObject::Sqrt1_2, DontEnum|DontDelete|ReadOnly, 0 },
   { "SQRT2", (intptr_t)MathObject::Sqrt2, DontEnum|DontDelete|ReadOnly, 0 },
   { "abs", (intptr_t)mathProtoFuncAbs, DontEnum|Function, 1 },
   { "acos", (intptr_t)mathProtoFuncACos, DontEnum|Function, 1 },
   { "asin", (intptr_t)mathProtoFuncASin, DontEnum|Function, 1 },
   { "atan", (intptr_t)mathProtoFuncATan, DontEnum|Function, 1 },
   { "atan2", (intptr_t)mathProtoFuncATan2, DontEnum|Function, 2 },
   { "ceil", (intptr_t)mathProtoFuncCeil, DontEnum|Function, 1 },
   { "cos", (intptr_t)mathProtoFuncCos, DontEnum|Function, 1 },
   { "exp", (intptr_t)mathProtoFuncExp, DontEnum|Function, 1 },
   { "floor", (intptr_t)mathProtoFuncFloor, DontEnum|Function, 1 },
   { "log", (intptr_t)mathProtoFuncLog, DontEnum|Function, 1 },
   { "max", (intptr_t)mathProtoFuncMax, DontEnum|Function, 2 },
   { "min", (intptr_t)mathProtoFuncMin, DontEnum|Function, 2 },
   { "pow", (intptr_t)mathProtoFuncPow, DontEnum|Function, 2 },
   { "random", (intptr_t)mathProtoFuncRandom, DontEnum|Function, 0 },
   { "round", (intptr_t)mathProtoFuncRound, DontEnum|Function, 1 },
   { "sin", (intptr_t)mathProtoFuncSin, DontEnum|Function, 1 },
   { "sqrt", (intptr_t)mathProtoFuncSqrt, DontEnum|Function, 1 },
   { "tan", (intptr_t)mathProtoFuncTan, DontEnum|Function, 1 },
   { 0, 0, 0, 0 }
};

extern const struct HashTable mathTable = { 511, mathTableValues, 0 };

} // namespace
